#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "bullet.h"
#include "rock.h"
#include "explosion.h"

#define PI 3.141592654

// gcc sdl01.c -o sdl01 $(sdl2-config --cflags --libs)
// for github use  ssh-keygen -o -t rsa -C "windows-ssh@mcnz.com"

// Fix gamepad conficting with mouse on debian base linux:
//   sudo apt remove xserver-xorg-input-joystick
//   sudo apt remove xserver-xorg-input-joystick-dev


#define WIN_WIDTH  640
#define WIN_HEIGHT 480

const SDL_Point ShipShape[] = {{0,15},{10,-10},{-10,-10}};
const SDL_Point miniShipShape[] = {{0,-10},{7,7},{-7,7}};

typedef struct Segment {
    SDL_Point start;
    SDL_Point end;
} Segment;

typedef struct Symbol {
    int     nbSegments;
    Segment segments[];
} Symbol;


Symbol num0 = { 4, {{{0,0},{8,0}},{{8,0},{8,12}},{{8,12},{0,12}},{{0,12},{0,0}}}};
Symbol num1 = { 1, {{{4,0},{4,12}}}};
Symbol num2 = { 5, {{{0,0},{8,0}}, {{8,0},{8,5}}, {{8,5},{0,5}}, {{0,5},{0,12}}, {{0,12},{8,12}}} };
Symbol num3 = { 4, {{{0,0},{8,0}}, {{8,0},{8,12}}, {{8,12},{0,12}}, {{0,5},{8,5}}} };
Symbol num4 = { 3, {{{0,0},{0,5}}, {{0,5},{8,5}}, {{8,0},{8,12}}} };
Symbol num5 = { 5, {{{0,0},{8,0}}, {{0,0},{0,5}}, {{0,5},{8,5}}, {{8,5},{8,12}}, {{8,12},{0,12}}} };
Symbol num6 = { 5, {{{0,0},{8,0}}, {{0,0},{0,12}}, {{0,5},{8,5}}, {{8,5},{8,12}}, {{8,12},{0,12}}} };
Symbol num7 = { 2, {{{0,0},{8,0}}, {{8,0},{8,12}}} };
Symbol num8 = { 5, {{{0,0},{0,12}}, {{8,0},{8,12}}, {{0,0},{8,0}}, {{0,5},{8,5}}, {{0,12},{8,12}}} };
Symbol num9 = { 5, {{{0,0},{8,0}}, {{0,5},{8,5}}, {{0,0},{0,5}}, {{8,0},{8,12}}, {{0,12},{8,12}}} };

Symbol *nums[] = {&num0, &num1, &num2, &num3, &num4, &num5, &num6, &num7, &num8, &num9};

int myLifes = 3;

typedef struct Ship{

    float x;
    float y;
    float angle;
    int   iThrust;
    int   iRotate;
    SDL_FPoint u;
    SDL_FPoint n;
    SDL_FPoint v;

} Ship;




//-------------------------------------------------------------------
//-------------------------------------------------------------------
void Symbol_Draw(Symbol *ptrSymbol,SDL_Renderer *renderer,float x,float y)
{
    float   x1,y1,x2,y2;
    SDL_Color light_grey = {200, 200, 200, 255};
    //---------------------------------------------
    
    SDL_SetRenderDrawColor(renderer, light_grey.r, light_grey.g, light_grey.b, light_grey.a);

    for (int i=0;i<ptrSymbol->nbSegments;++i){
        x1 = x + ptrSymbol->segments[i].start.x;
        y1 = y + ptrSymbol->segments[i].start.y;
        x2 = x + ptrSymbol->segments[i].end.x;
        y2 = y + ptrSymbol->segments[i].end.y;
        SDL_RenderDrawLineF(renderer,x1,y1,x2,y2);
    }


}



//-------------------------------------------------------------------
//-------------------------------------------------------------------

void Ship_Draw( Ship *ptrShip, SDL_Renderer *renderer)
{
    SDL_Point pts[5];
    SDL_Color white = {220, 220, 220, 128};
    SDL_Color orange = {255, 127, 40, 255};
    float x0,y0,x1,y1,x2,y2,x3,y3,x4,y4;

    // Draw space ship
    SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);

    x1 = ptrShip->x + ShipShape[0].y * ptrShip->u.x + ShipShape[0].x * ptrShip->n.x;
    y1 = ptrShip->y + ShipShape[0].y * ptrShip->u.y + ShipShape[0].x * ptrShip->n.y;
    SDL_RenderDrawLineF(renderer,ptrShip->x,ptrShip->y,x1,y1);


    x2 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x;
    y2 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y;
    SDL_RenderDrawLineF(renderer,ptrShip->x,ptrShip->y,x2,y2);

    x3 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x;
    y3 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y;
    SDL_RenderDrawLineF(renderer,ptrShip->x,ptrShip->y,x3,y3);

    pts[0].x = x1;
    pts[0].y = y1;
    pts[1].x = x2;
    pts[1].y = y2;
    pts[2].x = x3;
    pts[2].y = y3;
    pts[3].x = x1;
    pts[3].y = y1;
    SDL_RenderDrawLines(renderer, pts, 4);

    //-- Draw remain life
    x0 = WIN_WIDTH - 100.0f;
    y0 = 14.0;
    for (int i=0; i<myLifes; ++i){

        x1 = x0 + miniShipShape[0].x;
        y1 = y0 + miniShipShape[0].y;
        SDL_RenderDrawLineF(renderer, x0, y0, x1, y1);


        x2 = x0 + miniShipShape[1].x;
        y2 = y0 + miniShipShape[1].y;
        SDL_RenderDrawLineF(renderer, x0, y0, x2, y2);

        x3 = x0 + miniShipShape[2].x;
        y3 = y0 + miniShipShape[2].y;
        SDL_RenderDrawLineF(renderer, x0, y0, x3, y3);

        pts[0].x = x1;
        pts[0].y = y1;
        pts[1].x = x2;
        pts[1].y = y2;
        pts[2].x = x3;
        pts[2].y = y3;
        pts[3].x = x1;
        pts[3].y = y1;
        SDL_RenderDrawLines(renderer, pts, 4);

        x0 += 20.0;
    }

    void Draw_ExhaustGas(SDL_Renderer *renderer, float x1, float y1, float x2, float y2)
    {
        SDL_Point pts[5];
        SDL_Color white = {220, 220, 220, 128};
        //---------------------------------------------------------------
        float vx = x2-x1;
        float vy = y2-y1; 
        float norm = sqrt(vx*vx+vy*vy);
        SDL_FPoint u = {vx/norm,vy/norm};
        SDL_FPoint n = {-u.y,u.x};
        pts[0].x = x1 + u.x;
        pts[0].y = y1 + u.y;
        pts[1].x = x1 + 1.5*n.x + 1.5*u.x;
        pts[1].y = y1 + 1.5*n.y + 1.5*u.y;
        pts[2].x = x2;
        pts[2].y = y2;
        pts[3].x = x1 - 1.5*n.x + 1.5*u.x;
        pts[3].y = y1 - 1.5*n.y + 1.5*u.y;
        pts[4] = pts[0];
        SDL_RenderDrawLines(renderer, pts, 5);

    }

    if (ptrShip->iThrust==1){
        // Draw Exhaust gas during accelerationinner
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
        
        //_Draw right acceleratio exhaust gas
        x1 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x - 4.0*ptrShip->n.x;
        y1 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y - 4.0*ptrShip->n.y;
        x2 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x - 4.0*ptrShip->n.x - 10.0*ptrShip->u.x;
        y2 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y - 4.0*ptrShip->n.y - 10.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

        //_Draw left acceleratio exhaust gas
        x1 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x + 4.0*ptrShip->n.x;
        y1 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y + 4.0*ptrShip->n.y;
        x2 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x + 4.0*ptrShip->n.x - 10.0*ptrShip->u.x;
        y2 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y + 4.0*ptrShip->n.y - 10.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

    }else if (ptrShip->iThrust==-1){
        // Draw Exhaust gas during deceleration
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

        // Draw right deceleratio exhaust gas
        x1 = ptrShip->x - 6.0*ptrShip->n.x;
        y1 = ptrShip->y - 6.0*ptrShip->n.y;
        x2 = ptrShip->x - 8.0*ptrShip->n.x + 10.0*ptrShip->u.x;
        y2 = ptrShip->y - 8.0*ptrShip->n.y + 10.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

        // Draw left deceleratio exhaust gas
        x1 = ptrShip->x + 6.0*ptrShip->n.x;
        y1 = ptrShip->y + 6.0*ptrShip->n.y;
        x2 = ptrShip->x + 8.0*ptrShip->n.x + 10.0*ptrShip->u.x;
        y2 = ptrShip->y + 8.0*ptrShip->n.y + 10.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

    }

    if (ptrShip->iRotate>0){
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

        // Draw left decceleration exhaust gas
        x1 = ptrShip->x - 6.0*ptrShip->n.x;
        y1 = ptrShip->y - 6.0*ptrShip->n.y;
        x2 = ptrShip->x - 8.0*ptrShip->n.x + 8.0*ptrShip->u.x;
        y2 = ptrShip->y - 8.0*ptrShip->n.y + 8.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

        //_Draw right acceleratio exhaust gas
        x1 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x - 4.0*ptrShip->n.x;
        y1 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y - 4.0*ptrShip->n.y;
        x2 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x - 4.0*ptrShip->n.x - 8.0*ptrShip->u.x;
        y2 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y - 4.0*ptrShip->n.y - 8.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

    }else if (ptrShip->iRotate<0){
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

        // Draw right decceleration exhaust gas
        x1 = ptrShip->x + 6.0*ptrShip->n.x;
        y1 = ptrShip->y + 6.0*ptrShip->n.y;
        x2 = ptrShip->x + 8.0*ptrShip->n.x + 8.0*ptrShip->u.x;
        y2 = ptrShip->y + 8.0*ptrShip->n.y + 8.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

        //_Draw left acceleratio exhaust gas
        x1 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x + 4.0*ptrShip->n.x;
        y1 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y + 4.0*ptrShip->n.y;
        x2 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x + 4.0*ptrShip->n.x - 8.0*ptrShip->u.x;
        y2 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y + 4.0*ptrShip->n.y - 8.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

    }
}

void Ship_SetAngle( Ship *ptrShip, float a)
{
    //--------------------------------------
    ptrShip->angle = a;

    ptrShip->u.x = cos(a*PI/180.0);
    ptrShip->u.y = sin(a*PI/180.0);

    ptrShip->n.x = -ptrShip->u.y;
    ptrShip->n.y = ptrShip->u.x;

}

void Ship_SetLocation(Ship *ptrShip, int x, int y)
{
    //--------------------------------------
    ptrShip->x = x;
    ptrShip->y = y;

}

void Ship_SetVelocity(Ship *ptrShip, float vx, float vy)
{
    //--------------------------------------
    ptrShip->v.x = vx;
    ptrShip->v.y = vy;

}

void Ship_UpdatePosition(Ship *ptrShip)
{
    //--------------------------------------
    float x = ptrShip->x + ptrShip->v.x;
    float y = ptrShip->y + ptrShip->v.y;

    if (x<0.0f){
        ptrShip->v.x = -ptrShip->v.x;
        x = ptrShip->v.x;
    }else if (x>=WIN_WIDTH){
        ptrShip->v.x = -ptrShip->v.x;
        x = WIN_WIDTH -1.0f - ptrShip->v.x;
    }

    if (y<0.0f){
        ptrShip->v.y = -ptrShip->v.y;
        y = ptrShip->v.y;
    }else if (y>=WIN_HEIGHT){
        ptrShip->v.y = -ptrShip->v.y;
        y = WIN_HEIGHT - 1.0f - ptrShip->v.y;
    }

    ptrShip->x = x;
    ptrShip->y = y;

    //printf("x=%f y=%f\n",ptrShip->x,ptrShip->y);

}

void Ship_Accelerate(Ship *ptrShip, float pac)
{
    //
    float vx = pac * ptrShip->u.x;
    float vy = pac * ptrShip->u.y;

    ptrShip->v.x += vx;
    ptrShip->v.y += vy;


}

void Ship_Decelerate(Ship *ptrShip, float pac)
{
    //
    float vx = pac * ptrShip->u.x;
    float vy = pac * ptrShip->u.y;

    ptrShip->v.x -= vx;
    ptrShip->v.y -= vy;

}

void Ship_SetThrust(Ship *ptrShip, int iThrust)
{
    //
    ptrShip->iThrust = iThrust;
}

void Ship_SetRotate(Ship *ptrShip, int iRotate)
{
    //
    ptrShip->iRotate = iRotate;
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------

Rock *CheckBulletHitRocks(Bullet *ptrBullet, Rock *listRocks)
{
    float   r,vx,vy;
    Rock *ptrRock;
    //----------------------------------------------------------
    if (ptrRock=listRocks){
        do{
            //--
            if (ptrRock->fDeleted==SDL_FALSE){
                vx = ptrRock->x - ptrBullet->x;
                vy = ptrRock->y - ptrBullet->y;
                r = sqrtf(vx*vx + vy*vy);
                if (r<ptrRock->rayMin){
                    ptrBullet->fDeleted = SDL_TRUE;
                    ptrRock->fDeleted = SDL_TRUE;
                    return ptrRock;
                }
            }
            //--
            ptrRock = ptrRock->next;
        }while(ptrRock);
    }
    return NULL;
}

void Draw_Score(SDL_Renderer *renderer,int score){
    //------------------------------------------------------
    int base,n,ic=0;
    for(int i = 5;i>=0;--i){
        base = pow(10,i);
        n = (int) (score / base);
        Symbol_Draw(nums[n], renderer, 50.0+ic*14.0, 5.0);
        score -= n*base;
        ic++;
    }
}

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Mix_Chunk *laserSound = NULL;
    Mix_Chunk *explosionSound = NULL;

    SDL_Color orange = {255, 127, 40, 255};
    SDL_Color dark_blue = {10, 10, 150, 255};

    int myScore = 0;

    Ship myShip;

    Bullet  *listBullets = NULL;
    Rock    *listRocks = NULL;
    Explosion *listExplosions = NULL;

    int iTrigger = 0;

    Ship_SetThrust(&myShip, 0);
    Ship_SetRotate(&myShip, 0);
    Ship_SetLocation( &myShip, 320, 240);

    Ship_SetAngle( &myShip, -90.0f);


    float ra = myShip.angle*PI/180.0;
    Ship_SetVelocity(&myShip, 0.2f*cos(ra), 0.2f*sin(ra));



    printf("nbVertices=%d\n",sizeof(ShipShape)/sizeof(SDL_Point));

    srand(time(NULL));   // Initialization, should only be called once.

    for(int i = 0; i<15;++i){
        AddNewRock(&listRocks);

    }

    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
    printf("The names of the joysticks are:\n");
		
    SDL_Joystick *joystick;
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);


    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        printf("%s", Mix_GetError());
    }

    if (laserSound = Mix_LoadWAV( "res/344276__nsstudios__laser3.wav" )){
        Mix_VolumeChunk(laserSound, MIX_MAX_VOLUME/4);
    }

    if (explosionSound = Mix_LoadWAV( "res/387857__runningmind__explosion_asteroid2.wav" )){
        Mix_VolumeChunk(explosionSound, MIX_MAX_VOLUME/4);
    }

    window = SDL_CreateWindow("SDL2 Asteroids", 100, 100,
                              WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_bool quit = SDL_FALSE;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        SDL_bool quit = SDL_TRUE;
    }


    SDL_bool fPause = SDL_FALSE;

    Uint32 curTicks = SDL_GetTicks();
    Uint32 lastBulletTicks = curTicks;
    Uint32 lastUpdateExplosionTicks = curTicks;

    SDL_Event event;
    while(!quit)
    {
        while(SDL_PollEvent(&event)){

            // Read gamepad
            if (joystick){

                if (event.type == SDL_JOYAXISMOTION){
                    printf("Joystick event axis whick %d\n",event.jaxis.which); // Joystick number
                    printf("Joystick event axis  %d --> %d\n",event.jaxis.axis,event.jaxis.value); // axis number
                    if (event.jaxis.axis==4){
                        if (event.jaxis.value>100){
                            Ship_SetRotate(&myShip, 1);
                        }else if (event.jaxis.value<-100){
                            Ship_SetRotate(&myShip, -1);
                        }else{
                            Ship_SetRotate(&myShip, 0);
                        }
                    }
                    if (event.jaxis.axis==1){
                        if (event.jaxis.value>100){
                            Ship_SetThrust(&myShip, -1);
                        }else if (event.jaxis.value<-100){
                            Ship_SetThrust(&myShip, 1);
                        }else{
                            Ship_SetThrust(&myShip, 0);
                        }
                    }

                }else if (event.type == SDL_JOYBUTTONDOWN){
                    printf("Joystick event button down %d\n",event.jbutton.button);
                    if (event.jbutton.button==5){
                        iTrigger = 1;
                    }

                }else if (event.type == SDL_JOYBUTTONUP){
                    printf("Joystick event button up %d\n",event.jbutton.button);
                    if (event.jbutton.button==5){
                        iTrigger = 0;
                    }

                }else if (event.type == SDL_JOYHATMOTION){
                    printf("Joystick event hat \n");
                    if ( event.jhat.value & SDL_HAT_UP )
                    {
                        /* Do up stuff here */
                        printf("HAT UP\n");
                    }
                    if ( event.jhat.value & SDL_HAT_DOWN )
                    {
                        /* Do up stuff here */
                        printf("HAT DOWN\n");
                    }
                    if ( event.jhat.value & SDL_HAT_LEFT )
                    {
                        /* Do left stuff here */
                        printf("HAT LEFT\n");
                    }
                    if ( event.jhat.value & SDL_HAT_RIGHT )
                    {
                        /* Do left stuff here */
                        printf("HAT RIGHT\n");
                    }

                    if ( event.jhat.value & SDL_HAT_RIGHTDOWN )
                    {
                        /* Do right and down together stuff here */
                        printf("HAT RIGHTDOWN\n");
                    }
                }
            }

            if(event.type == SDL_QUIT){
                quit = SDL_TRUE;

            }else if ((event.type == SDL_KEYDOWN) && (!event.key.repeat)){
                //printf("Keydown\n");
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    quit = SDL_TRUE;
                    //printf("Escape Key\n");
                }else if(event.key.keysym.sym == SDLK_LEFT){
                    Ship_SetRotate(&myShip,1);
                }else if(event.key.keysym.sym == SDLK_RIGHT){
                    Ship_SetRotate(&myShip,-1);
                }else if(event.key.keysym.sym == SDLK_UP){
                    Ship_SetThrust(&myShip, 1);
                }else if(event.key.keysym.sym == SDLK_DOWN){
                    Ship_SetThrust(&myShip, -1);
                }else if(event.key.keysym.sym == SDLK_SPACE){
                    //SDL_CaptureMouse(SDL_TRUE);
                    iTrigger = 1;
                    lastBulletTicks = SDL_GetTicks();
                    AddNewBullet( &listBullets, myShip.x, myShip.y, myShip.u);
                    Mix_PlayChannel( -1, laserSound, 0 );
                }else if(event.key.keysym.sym == SDLK_p){
                    fPause ^= SDL_TRUE;
                }else if(event.key.keysym.sym == SDLK_e){
                    AddNewExplosion(&listExplosions,100.0f,100.0f,1.0f,1.0f);
                    AddNewExplosion(&listExplosions,150.0f,100.0f,1.0f,1.0f);
                    AddNewExplosion(&listExplosions,200.0f,100.0f,1.0f,1.0f);
                }
            }else if ((event.type == SDL_KEYUP)){
                //printf("KeyUp\n");
                if (event.key.keysym.sym == SDLK_LEFT){
                    Ship_SetRotate(&myShip, 0);
                }else if(event.key.keysym.sym == SDLK_RIGHT){
                    Ship_SetRotate(&myShip, 0);
                }else if(event.key.keysym.sym == SDLK_UP){
                    Ship_SetThrust(&myShip, 0);
                }else if(event.key.keysym.sym == SDLK_DOWN){
                    Ship_SetThrust(&myShip, 0);
                }else if(event.key.keysym.sym == SDLK_SPACE){
                    iTrigger = 0;
                }
            }
        }

        if (fPause==SDL_FALSE){

            if (myShip.iRotate==1){
                Ship_SetAngle(&myShip, myShip.angle - 1.0f);
            }else if (myShip.iRotate==-1){
                Ship_SetAngle(&myShip, myShip.angle + 1.0f);
            }


            if (myShip.iThrust>0){
                Ship_Accelerate(&myShip, 0.015f);
            }else if (myShip.iThrust<0){
                Ship_Decelerate(&myShip, 0.015f);
            }

            if (iTrigger){
                Uint32 curTicks = SDL_GetTicks();
                if ((curTicks-lastBulletTicks)>200){
                    lastBulletTicks = curTicks;
                    AddNewBullet( &listBullets, myShip.x, myShip.y, myShip.u);
                    Mix_PlayChannel( -1, laserSound, 0 );
                }
            }

            // Update Game states
            Ship_UpdatePosition(&myShip);

            UpdateRockPositions(listRocks);
            UpdateBulletPositions(listBullets);

            UpdateBulletsList(&listBullets);

            curTicks = SDL_GetTicks();
            if ((curTicks-lastUpdateExplosionTicks)>80){
                lastUpdateExplosionTicks = curTicks;
                UpdateExplosions(listExplosions);
                UpdateExplosionssList(&listExplosions);
            }


            Bullet *ptrBullet;
            if (ptrBullet=listBullets){
                Rock *ptrRock;
                do{
                    //
                    if (ptrRock=CheckBulletHitRocks(ptrBullet, listRocks)){
                        AddNewExplosion(&listExplosions,ptrRock->x,ptrRock->y, ptrRock->v.x, ptrRock->v.y);
                        Mix_PlayChannel( -1, explosionSound, 0 );
                        myScore += 30;
                        //printf("SCORE = %d\n",myScore);
                    }
                    //
                    ptrBullet = ptrBullet->next;
                }while(ptrBullet);
            }

            UpdateRocksList(&listRocks);


            //
            // int nb = 0;
            // Bullet *ptrBullet;
            // if (ptrBullet=listBullets){

            //     do{
            //         nb++;
            //         ptrBullet = ptrBullet->next;
            //     }while(ptrBullet);
            // }
            // printf("nbre bullets = %d\n",nb);

            // int nb = 0;
            // Rock *ptrRock;
            // if (ptrRock=listRocks){

            //     do{
            //         nb++;
            //         ptrRock = ptrRock->next;
            //     }while(ptrRock);
            // }
            // printf("nbre Rocks = %d\n",nb);

        }


        //
        SDL_SetRenderDrawColor(renderer, dark_blue.r, dark_blue.g, dark_blue.b, dark_blue.a);
        SDL_RenderClear(renderer);

        //
        Ship_Draw( &myShip, renderer);

        DrawBullets( listBullets, renderer);
        DrawRocks(listRocks, renderer);
        DrawExplosions(listExplosions, renderer);


        Draw_Score(renderer, myScore);

        //
        SDL_RenderPresent(renderer);

        //
        SDL_Delay(10);

    }

    // Free Game Objects
    FreeBullets(&listBullets);
    FreeRocks(&listRocks);
    FreeExplosions(&listExplosions);

    //SDL_Delay(3000);

    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);

    if (laserSound!=NULL)
        Mix_FreeChunk( laserSound );

    if (explosionSound!=NULL)
        Mix_FreeChunk( explosionSound );

    if(NULL != window)
        SDL_DestroyWindow(window);

    if (joystick)
        SDL_JoystickClose(joystick);
 
    Mix_CloseAudio();

    //
    Mix_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

