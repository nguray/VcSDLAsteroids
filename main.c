#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.141592654

// gcc sdl01.c -o sdl01 $(sdl2-config --cflags --libs)
// for github use  ssh-keygen -o -t rsa -C "windows-ssh@mcnz.com"

// Fix gamepad conficting with mouse on debian base linux:
//   sudo apt remove xserver-xorg-input-joystick
//   sudo apt remove xserver-xorg-input-joystick-dev


#define WIN_WIDTH  640
#define WIN_HEIGHT 480

const SDL_Point ShipShape[3] = {{0,15},{10,-10},{-10,-10}};

typedef struct Ship{

    float x;
    float y;
    float angle;
    SDL_FPoint u;
    SDL_FPoint n;
    SDL_FPoint v;

} Ship;


typedef struct Bullet {

    float           x;
    float           y;
    SDL_FPoint      v;
    SDL_bool        fDeleted;
    struct Bullet   *next;

} Bullet;


typedef struct Rock {
    float x;
    float y;
    SDL_FPoint      v;
    float           rayMin;
    float           rayMax;
    int             nbVertices;
    SDL_FPoint      vertices[12];
    SDL_bool        fDeleted;
    struct Rock     *next;
} Rock;

typedef struct Explosion {
    int                 iState;
    int                 nbVertices;
    SDL_FPoint          vertices[8];
    SDL_FPoint          velocities[8];
    SDL_bool            fDeleted;
    struct Explosion    *next;
} Explosion;

void AddNewExplosion(Explosion **listExplosions,float x,float y,float vx,float vy)
{
    float ra,e_vx,e_vy;
    Explosion *ptrExplosion,*ptrCur;
    //------------------------------------------------
    float coeffRa = PI/180.0f;
    if (ptrExplosion = (Explosion *) calloc(1,sizeof(Explosion))){
        ra = 0.0f;
        ptrExplosion->iState = 0;
        ptrExplosion->nbVertices = 8;
        for (int i=0; i<8; ++i){
            e_vx = 2.5f*cos(i*45*coeffRa) + vx; 
            e_vy = 2.5f*sin(i*45*coeffRa) + vy;
            ptrExplosion->velocities[i].x = e_vx; 
            ptrExplosion->velocities[i].y = e_vy;
            ptrExplosion->vertices[i].x = x; 
            ptrExplosion->vertices[i].y = y;
        }

        ptrExplosion->fDeleted = SDL_FALSE;

        if (ptrCur = *listExplosions){
            // Find last element
            while(ptrCur->next){
                ptrCur = ptrCur->next;
            }
            ptrCur->next = ptrExplosion;
        }else{
            *listExplosions = ptrExplosion;
        }


    }

}

void UpdateExplosions(Explosion *listExplosions)
/*-------------------------------------------------------------*\
    Compute new explosions states

    Raymond NGUYEN THANH                        22-08-2025
\*-------------------------------------------------------------*/
{
    Explosion *ptrExplosion;
    //----------------------------------------
    if (ptrExplosion=listExplosions){
        do{
            if (ptrExplosion->fDeleted==SDL_FALSE){
                if (ptrExplosion->iState<10){
                    for(int i=0;i<ptrExplosion->nbVertices;++i){
                        ptrExplosion->vertices[i].x += ptrExplosion->velocities[i].x;
                        ptrExplosion->vertices[i].y += ptrExplosion->velocities[i].y;
                    }
                    ptrExplosion->iState++;
                }else{
                    ptrExplosion->fDeleted = SDL_TRUE;
                }
            }
            ptrExplosion = ptrExplosion->next;
        }while(ptrExplosion);
    }
}

void DrawExplosions(Explosion *listExplosions, SDL_Renderer *renderer)
{
    float x1,y1,x2,y2;
    float n,vx,vy;
    Explosion *ptrExplosion;
    SDL_Color light_grey = {200, 200, 200, 255};
    //----------------------------------------
    if (ptrExplosion=listExplosions){
        do{
            SDL_SetRenderDrawColor(renderer, light_grey.r, light_grey.g, light_grey.b, light_grey.a);
            if (ptrExplosion->fDeleted==SDL_FALSE){
                for (int i=0;i<ptrExplosion->nbVertices;++i){
                    vx = ptrExplosion->velocities[i].x;
                    vy = ptrExplosion->velocities[i].y;
                    n = sqrt(vx*vx+vy*vy);
                    vx /= n;
                    vy /= n;
                    x1 = ptrExplosion->vertices[i].x + 2.0f*vx;
                    y1 = ptrExplosion->vertices[i].y + 2.0f*vy;
                    x2 = ptrExplosion->vertices[i].x - 2.0f*vx;
                    y2 = ptrExplosion->vertices[i].y - 2.0f*vy;
                    SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
                }
            }
            ptrExplosion = ptrExplosion->next;
        }while(ptrExplosion);
        
    }

}

void UpdateExplosionssList(Explosion **listExplosions)
/*-------------------------------------------------------------*\
    Remove deleted objects from the explosions list

    Raymond NGUYEN THANH                        22-08-2025
\*-------------------------------------------------------------*/
{
    Explosion *ptrFirst=NULL; // For the case of there no active Rock
    Explosion *ptrPrev=NULL;
    Explosion *ptrCur,*ptrNext;
    //---------------------------------------------------------
    if (ptrCur=*listExplosions){
        do{
            ptrNext = ptrCur->next;
            if (ptrCur->fDeleted==SDL_TRUE){
                free(ptrCur);
            }else{
                ptrCur->next = NULL;
                if (ptrPrev==NULL){    // No previous active rock
                    ptrFirst = ptrCur; // Must be the head of the list 
                    ptrPrev=ptrCur;
                }else{
                    ptrPrev->next = ptrCur;
                    ptrPrev = ptrCur;
                }
            }
            ptrCur = ptrNext;
        }while(ptrCur);
        *listExplosions = ptrFirst;
    }

}


void Ship_Draw( Ship *ptrShip, SDL_Renderer *renderer)
{
    SDL_Point pts[4];
    SDL_Color orange = {255, 127, 40, 255};

    SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);

    float x1 = ptrShip->x + ShipShape[0].y * ptrShip->u.x + ShipShape[0].x * ptrShip->n.x;
    float y1 = ptrShip->y + ShipShape[0].y * ptrShip->u.y + ShipShape[0].x * ptrShip->n.y;
    SDL_RenderDrawLineF(renderer,ptrShip->x,ptrShip->y,x1,y1);


    float x2 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x;
    float y2 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y;
    SDL_RenderDrawLineF(renderer,ptrShip->x,ptrShip->y,x2,y2);

    float x3 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x;
    float y3 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y;
    SDL_RenderDrawLineF(renderer,ptrShip->x,ptrShip->y,x3,y3);

    pts[0].x = x1;
    pts[0].y = y1;

    pts[1].x = x2;
    pts[1].y = y2;

    pts[2].x = x3;

    pts[2].y = y3;

    pts[3].x = x1;
    pts[3].y = y1;

    //SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);
    SDL_RenderDrawLines(renderer, pts, 4);

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

void AddNewBullet(Bullet **listBullets, float x, float y, SDL_FPoint u)
/*-------------------------------------------------------------*\
    Create new bullet and append to the end of the list

    Raymond NGUYEN THANH                        18-08-2025
\*-------------------------------------------------------------*/
{
    Bullet *ptrBullet,*ptrCur;
    //--
    if (ptrBullet = (Bullet *) calloc(1,sizeof(Bullet))){
        ptrBullet->x = x;
        ptrBullet->y = y;
        ptrBullet->v.x = 2.0f*u.x;
        ptrBullet->v.y = 2.0f*u.y;
        ptrBullet->fDeleted = SDL_FALSE;

        if (ptrCur = *listBullets){
            // Find last element
            while(ptrCur->next){
                ptrCur = ptrCur->next;
            }
            ptrCur->next = ptrBullet;
        }else{
            *listBullets = ptrBullet;
        }


    }

}

void UpdateBulletPositions(Bullet *listBullets)
/*-------------------------------------------------------------*\
    Compute new positions for active bullets

    Raymond NGUYEN THANH                        18-08-2025
\*-------------------------------------------------------------*/
{
    float x,y;
    Bullet *ptrBullet;
    //----------------------------------------
    if (ptrBullet=listBullets){
        do{
            if (ptrBullet->fDeleted==SDL_FALSE){
                x = ptrBullet->x;
                y = ptrBullet->y;
                if ((x<0.0f)||(x>=WIN_WIDTH)){
                    ptrBullet->fDeleted = SDL_TRUE;
                }else if ((y<0.0f)||(y>=WIN_HEIGHT)){
                    ptrBullet->fDeleted = SDL_TRUE;
                }
                if (ptrBullet->fDeleted==SDL_FALSE){
                    ptrBullet->x += ptrBullet->v.x;
                    ptrBullet->y += ptrBullet->v.y;
                }
            }
            ptrBullet = ptrBullet->next;
        }while(ptrBullet);
    }
}

void DrawBullets(Bullet *listBullets, SDL_Renderer *renderer)
{
    float   x1,y1,x2,y2;
    Bullet *ptrBullet;
    SDL_Color red = {255, 0, 0, 255};
    //----------------------------------------
    if (ptrBullet=listBullets){
        SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
        do{

            if (ptrBullet->fDeleted==SDL_FALSE){
                x1 = ptrBullet->x - 2.0f * ptrBullet->v.x;
                y1 = ptrBullet->y - 2.0f * ptrBullet->v.y;
                x2 = ptrBullet->x + 2.0f * ptrBullet->v.x;
                y2 = ptrBullet->y + 2.0f * ptrBullet->v.y;
                SDL_RenderDrawLineF( renderer, x1, y1, x2, y2);

            }

            ptrBullet = ptrBullet->next;

        }while(ptrBullet);
    }

}

void UpdateBulletsList(Bullet **listBullets)
/*-------------------------------------------------------------*\
    Remove deleted bullets from the list of bullets

    Raymond NGUYEN THANH                        19-08-2025
\*-------------------------------------------------------------*/
{
    Bullet *ptrBullet,*ptrNext=NULL;
    Bullet *ptrNotDeleted = NULL;
    //---------------------------------------
    if (ptrBullet=(*listBullets)){

        do{

            ptrNext = ptrBullet->next;
            if (ptrBullet->fDeleted==SDL_TRUE){
                free(ptrBullet);

            }else{
                ptrBullet->next = NULL;
                if (ptrNotDeleted==NULL){
                    // Firts not deleted
                    ptrNotDeleted = ptrBullet;
                    *listBullets = ptrNotDeleted;

                }else{

                    // Append to not deleted list
                    ptrNotDeleted->next = ptrBullet;
                    ptrNotDeleted = ptrBullet;
                }
            }

            ptrBullet = ptrNext;

        }while(ptrBullet);

        if (ptrNotDeleted==NULL){
            // list is empty
            *listBullets = NULL;
        }


    }
}

void AddNewRock(Rock **listRocks)
/*-------------------------------------------------------------*\
    Create new rock and append to the end of the list

    Raymond NGUYEN THANH                        19-08-2025
\*-------------------------------------------------------------*/
{
    float   a, da, r;
    Rock    *ptrRock,*ptrCur;
    //---------------------------------------
    if (ptrRock=(Rock *) calloc(1,sizeof(Rock))){
        
        //--
        ptrRock->x = rand() % WIN_WIDTH;
        ptrRock->y = rand() % WIN_HEIGHT;
        ptrRock->fDeleted = SDL_FALSE;
        a = ((float) (rand() % 360))*PI/180.0f;
        ptrRock->v.x = 0.3f * cos(a);
        ptrRock->v.y = 0.3f * sin(a);

        a = 0.0f;
        ptrRock->nbVertices = 0;
        ptrRock->rayMax = 0.0f;
        ptrRock->rayMin = 1000.0;
        for(int i=0;i<10;++i){
            a += ((float) (rand() % 40 + 20))*PI/180.0f;
            if (a>2*PI) break;
            r = ((float) (rand() % 5)) + 10.0;
            if (r < ptrRock->rayMin){
                ptrRock->rayMin = r;
            }
            if (r > ptrRock->rayMax){
                ptrRock->rayMax = r;
            }
            ptrRock->vertices[ptrRock->nbVertices].x = r * cos(a);
            ptrRock->vertices[ptrRock->nbVertices].y = r * sin(a);
            ptrRock->nbVertices++;
        }

        if (ptrCur=*listRocks){
            // Find last element
            while(ptrCur->next){
                ptrCur = ptrCur->next;
            }
            ptrCur->next = ptrRock;
        }else{
            *listRocks = ptrRock;
        }

    }

}

void UpdateRockPositions(Rock *listRocks)
/*-------------------------------------------------------------*\
    Compute new positions for active rocks

    Raymond NGUYEN THANH                        19-08-2025
\*-------------------------------------------------------------*/
{
    float x,y;
    Rock *ptrRock;
    //----------------------------------------
    if (ptrRock=listRocks){
        do{
            if (ptrRock->fDeleted==SDL_FALSE){
                x = ptrRock->x;
                y = ptrRock->y;

                if (x<0.0f) {
                    ptrRock->x = 1.0;
                    ptrRock->v.x = fabs(ptrRock->v.x);
                }else if (x>=WIN_WIDTH){
                    ptrRock->x = WIN_WIDTH - 1;
                    ptrRock->v.x = -fabs(ptrRock->v.x);
                }

                if (y<0.0f){
                    ptrRock->y = 1.0;
                    ptrRock->v.y = fabs(ptrRock->v.y);
                }else if (y>=WIN_HEIGHT){
                    ptrRock->y = WIN_HEIGHT - 1;
                    ptrRock->v.y = -fabs(ptrRock->v.y);
                }

                ptrRock->x += ptrRock->v.x;
                ptrRock->y += ptrRock->v.y;

            }

            ptrRock = ptrRock->next;

        }while(ptrRock);
    }
}

void DrawRocks( Rock *listRocks, SDL_Renderer *renderer)
{
    float x0,y0,x1,y1,x2,y2;
    Rock *ptrRock;
    SDL_Color light_grey = {200, 200, 200, 255};
    //-------------------------------------------------
    if (ptrRock=listRocks){

        SDL_SetRenderDrawColor(renderer, light_grey.r, light_grey.g, light_grey.b, light_grey.a);

        do{

            if (ptrRock->fDeleted==SDL_FALSE){
                int i=0;
                x1 = ptrRock->x + ptrRock->vertices[i].x;
                y1 = ptrRock->y + ptrRock->vertices[i].y;
                x0 = x1;
                y0 = y1;
                i++;
                while(i<10){
                    x2 = ptrRock->x + ptrRock->vertices[i].x;
                    y2 = ptrRock->y + ptrRock->vertices[i].y;
                    SDL_RenderDrawLineF( renderer, x1, y1, x2, y2);
                    x1 = x2;
                    y1 = y2;
                    i++;
                }
                SDL_RenderDrawLineF( renderer, x1, y1, x0, y0);
            }
            //
            ptrRock = ptrRock->next;
        
        }while(ptrRock);

    }

}

void UpdateRocksList(Rock **listRocks)
/*-------------------------------------------------------------*\
    Remove deleted rocks from the list of rocks

    Raymond NGUYEN THANH                        20-08-2025
\*-------------------------------------------------------------*/
{
    Rock *ptrFirst=NULL; // For the case of there no active Rock
    Rock *ptrPrev=NULL;
    Rock *ptrCur,*ptrNext;
    //---------------------------------------------------------
    if (ptrCur=*listRocks){
        do{
            ptrNext = ptrCur->next;
            if (ptrCur->fDeleted==SDL_TRUE){
                free(ptrCur);
            }else{
                ptrCur->next = NULL;
                if (ptrPrev==NULL){    // No previous active rock
                    ptrFirst = ptrCur; // Must be the head of the list 
                    ptrPrev=ptrCur;
                }else{
                    ptrPrev->next = ptrCur;
                    ptrPrev = ptrCur;
                }
            }
            ptrCur = ptrNext;
        }while(ptrCur);
        *listRocks = ptrFirst;
    }

}

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

void FreeRocks(Rock **listRocks)
{
    Rock *ptrCur,*ptrNext;
    //--------------------------------------------------------
    if (ptrCur=*listRocks){
        while(ptrCur){
            ptrNext = ptrCur->next;
            free(ptrCur);
            ptrCur = ptrNext;
        }
        *listRocks = NULL;
    }

}

void FreeBullets(Bullet **listBullets)
{
    Bullet *ptrCur,*ptrNext;
    //--------------------------------------------------------
    if (ptrCur=*listBullets){
        while(ptrCur){
            ptrNext = ptrCur->next;
            free(ptrCur);
            ptrCur = ptrNext;
        }
        *listBullets = NULL;
    }

}

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Mix_Chunk *laserSound = NULL;
    Mix_Chunk *explosionSound = NULL;

    SDL_Color orange = {255, 127, 40, 255};
    SDL_Color dark_blue = {10, 10, 255, 255};

    Ship myShip;

    Bullet  *listBullets = NULL;
    Rock    *listRocks = NULL;
    Explosion *listExplosions = NULL;

    int iRotate = 0;
    int iAccelerate = 0;
    int iTrigger = 0;

    Ship_SetLocation( &myShip, 320, 240);

    Ship_SetAngle( &myShip, -90.0f);


    float ra = myShip.angle*PI/180.0;
    Ship_SetVelocity(&myShip, 0.2f*cos(ra), 0.2f*sin(ra));


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

    if (explosionSound = Mix_LoadWAV( "res/huge-explosion-in-distance-100604.wav" )){
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
                            iRotate = 1;
                        }else if (event.jaxis.value<-100){
                            iRotate = -1;                        
                        }else{
                            iRotate = 0;
                        }
                    }
                    if (event.jaxis.axis==1){
                        if (event.jaxis.value>100){
                            iAccelerate = -1;
                        }else if (event.jaxis.value<-100){
                            iAccelerate = 1;                        
                        }else{
                            iAccelerate = 0;
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
                    iRotate = 1;
                }else if(event.key.keysym.sym == SDLK_RIGHT){
                    iRotate = -1;
                }else if(event.key.keysym.sym == SDLK_UP){
                    iAccelerate = 1;
                }else if(event.key.keysym.sym == SDLK_DOWN){
                    iAccelerate = -1;
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
                    iRotate = 0;
                }else if(event.key.keysym.sym == SDLK_RIGHT){
                    iRotate = 0;
                }else if(event.key.keysym.sym == SDLK_UP){
                    iAccelerate = 0;
                }else if(event.key.keysym.sym == SDLK_DOWN){
                    iAccelerate = 0;
                }else if(event.key.keysym.sym == SDLK_SPACE){
                    iTrigger = 0;
                }
            }
        }

        if (fPause==SDL_FALSE){

            if (iRotate==1){
                Ship_SetAngle(&myShip, myShip.angle - 1.0f);
            }else if (iRotate==-1){
                Ship_SetAngle(&myShip, myShip.angle + 1.0f);
            }


            if (iAccelerate>0){
                Ship_Accelerate(&myShip, 0.015f);
            }else if (iAccelerate<0){
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

            curTicks = SDL_GetTicks();;
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

        //
        SDL_RenderPresent(renderer);

        //
        SDL_Delay(10);

    }

    // Free Game Objects
    FreeBullets(&listBullets);
    FreeRocks(&listRocks);

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

