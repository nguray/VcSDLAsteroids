
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "game.h"

#include "bullet.h"
#include "rock.h"
#include "explosion.h"
#include "ship.h"


// gcc sdl01.c -o sdl01 $(sdl2-config --cflags --libs)
// for github use  ssh-keygen -o -t rsa -C "windows-ssh@mcnz.com"

// Fix gamepad conficting with mouse on debian base linux:
//   sudo apt remove xserver-xorg-input-joystick
//   sudo apt remove xserver-xorg-input-joystick-dev


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

Symbol charA = { 5, {{{0,12},{0,4}},{{0,4},{4,0}},{{4,0},{8,4}},{{8,4},{8,12}},{{0,7},{8,7}}}};
Symbol charB = { 10, {{{0,0},{6,0}},{{6,0},{8,2}},{{8,2},{8,4}},{{8,4},{6,6}},{{6,6},{8,8}},
                      {{8,8},{8,10}},{{8,10},{6,12}},{{6,12},{0,12}},{{0,6},{8,6}},{{0,0},{0,12}}}};
Symbol charC = { 3, {{{0,0},{8,0}}, {{0,0},{0,12}}, {{0,12},{8,12}}} };
Symbol charD = { 6, {{{0,0},{5,0}},{{5,0},{8,3}},{{8,3},{8,9}},{{8,9},{5,12}},{{5,12},{0,12}},{{0,12},{0,0}}} };
Symbol charE = { 4, {{{0,0},{8,0}}, {{0,0},{0,12}}, {{0,12},{8,12}}, {{0,6},{5,6}}} };
Symbol charF = { 3, {{{0,0},{8,0}}, {{0,0},{0,12}}, {{0,6},{5,6}}} };
Symbol charG = { 5, {{{8,0},{0,0}},{{0,0},{0,12}},{{0,12},{8,12}},{{8,12},{8,7}},{{8,7},{5,7}}}};
Symbol charH = { 3, {{{0,0},{0,12}},{{8,0},{8,12}},{{0,6},{8,6}}} };
Symbol charI = { 3, {{{3,0},{5,0}},{{4,0},{4,12}},{{3,12},{5,12}}} };
Symbol charJ = { 3, {{{8,0},{8,12}},{{8,12},{0,12}},{{0,12},{0,8}}} };
Symbol charK = { 3, {{{0,0},{0,12}},{{0,6},{8,0}},{{0,6},{8,12}}} };
Symbol charL = { 2, {{{0,0},{0,12}},{{0,12},{8,12}}} };
Symbol charM = { 4, {{{0,12},{0,0}}, {{0,0},{4,4}}, {{4,4},{8,0}}, {{8,0},{8,12}}} };
Symbol charN = { 3, {{{0,12},{0,0}}, {{0,0},{8,12}}, {{8,12},{8,0}}} };
Symbol charO = { 8, {{{0,2},{2,0}},{{2,0},{6,0}},{{6,0},{8,2}},{{8,2},{8,10}},{{8,10},{6,12}},
                      {{6,12},{2,12}},{{2,12},{0,10}},{{0,10},{0,2}}}};
Symbol charP = { 4, {{{0,0},{8,0}}, {{8,0},{8,6}}, {{8,6},{0,6}}, {{0,0},{0,12}}} };
Symbol charQ = { 9, {{{0,2},{2,0}},{{2,0},{6,0}},{{6,0},{8,2}},{{8,2},{8,10}},{{8,10},{6,12}},
                      {{6,12},{2,12}},{{2,12},{0,10}},{{0,10},{0,2}},{{8,12},{5,9}}}};
Symbol charR = { 5, {{{0,0},{8,0}},{{8,0},{8,6}},{{8,6},{0,6}},{{0,0},{0,12}},{{2,7},{8,12}}}};
Symbol charS = { 9, {{{8,0},{2,0}},{{2,0},{0,2}},{{0,2},{0,4}},{{0,4},{2,6}},{{2,6},{6,6}},
                      {{6,6},{8,8}},{{8,8},{8,10}},{{8,10},{6,12}},{{6,12},{0,12}}}};
Symbol charT = { 2, {{{0,0},{8,0}},{{4,0},{4,12}}} };
Symbol charU = { 3, {{{0,0},{0,12}},{{0,12},{8,12}},{{8,12},{8,0}}} };
Symbol charV = { 2, {{{0,0},{4,12}},{{4,12},{8,0}}} };
Symbol charW = { 4, {{{0,0},{0,12}}, {{0,12},{4,7}}, {{4,7},{8,12}}, {{8,12},{8,0}}} };
Symbol charX = { 2, {{{0,0},{8,12}},{{8,0},{0,12}}} };
Symbol charY = { 3, {{{0,0},{4,5}},{{4,5},{8,0}},{{4,5},{4,12}}} };
Symbol charZ = { 3, {{{0,0},{8,0}},{{8,0},{0,12}},{{0,12},{8,12}}} };

Symbol *tabChars[] = { &charA, &charB, &charC, &charD, &charE, &charF, &charG, &charH, &charI, &charJ, &charK, &charL, &charM,
                      &charN, &charO, &charP, &charQ, &charR, &charS, &charT, &charU, &charV, &charW, &charX, &charY, &charZ };

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

void DisplayMessage(SDL_Renderer *renderer, float xt, float yt, char *msg)
{
    int i,ic;
    float xc;
    char c;
    //--------------------------------------------------------------
    if (strlen(msg)){
        ic = 0;
        while(c=msg[ic++]){
            i = c - 'A';
            if ((i>=0)&&(i<26)){
                xc = ic*14.0 + xt;
                Symbol_Draw( tabChars[i], renderer, xc, yt);
            } 
        }
    }
}

float MessageWidth(char *msg)
{
    //--------------------------------------------------------------
    return strlen(msg)*14.0;
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

void NewLevel( Rock **listRocks, Game *game)
{
    int nbRocks = game->iLevel * 4 + 7;
    //-----------------------------------------------------
    for(int i = 0; i<nbRocks;++i){
        AddNewRock(listRocks);

    }
    if (myShip){
        Ship_SetLocation( myShip, 320, 240);
        Ship_SetVelocity( myShip, 0.0, 0.0);
    }
    game->fPause = SDL_FALSE;

}

int PlayModeProcessEvent(SDL_Event event, SDL_Joystick *joystick,Game *game,Ship *ship);

int (*ProcessEvent) (SDL_Event event, SDL_Joystick *joystick, struct Game *game,Ship *ship);

int IdleModeProcessEvent(SDL_Event event, SDL_Joystick *joystick,Game *game,Ship *ship)
{
    //-----------------------------------------------------
    if (event.type == SDL_QUIT){
        return SDL_TRUE;

    }else if ((event.type == SDL_KEYDOWN) && (!event.key.repeat)){
        //printf("Keydown\n");
        if(event.key.keysym.sym == SDLK_ESCAPE){
            return SDL_TRUE;
            //printf("Escape Key\n");
        }else if(event.key.keysym.sym == SDLK_SPACE){
            game->mode = PLAY_MODE;
            ProcessEvent = &PlayModeProcessEvent;
            if (myShip=(Ship *) calloc(1,sizeof(Ship))){
                myShip->iTrigger = 0;
                Ship_SetThrust(myShip, 0);
                Ship_SetRotate(myShip, 0);
                Ship_SetLocation(myShip, 320, 240);
                Ship_SetAngle( myShip, -90.0f);
            }

        }else if(event.key.keysym.sym == SDLK_p){
            game->fPause ^= SDL_TRUE;
        }
    }else if ((event.type == SDL_KEYUP)){
        //printf("KeyUp\n");
        if(event.key.keysym.sym == SDLK_SPACE){
        //    myShip.iTrigger = 0;
        }
    }
    return SDL_FALSE;

}

int PlayModeProcessEvent(SDL_Event event, SDL_Joystick *joystick,Game *game,Ship *ship)
{
    //-----------------------------------------------------
    if (ship==NULL) return SDL_FALSE;
    // Read gamepad
    if (joystick){

        if (event.type == SDL_JOYAXISMOTION){
            printf("Joystick event axis whick %d\n",event.jaxis.which); // Joystick number
            printf("Joystick event axis  %d --> %d\n",event.jaxis.axis,event.jaxis.value); // axis number
            if (event.jaxis.axis==4){
                if (event.jaxis.value>100){
                    Ship_SetRotate(ship, 1);
                }else if (event.jaxis.value<-100){
                    Ship_SetRotate(ship, -1);
                }else{
                    Ship_SetRotate(ship, 0);
                }
            }
            if (event.jaxis.axis==1){
                if (event.jaxis.value>100){
                    Ship_SetThrust(ship, -1);
                }else if (event.jaxis.value<-100){
                    Ship_SetThrust(ship, 1);
                }else{
                    Ship_SetThrust(ship, 0);
                }
            }

        }else if (event.type == SDL_JOYBUTTONDOWN){
            printf("Joystick event button down %d\n",event.jbutton.button);
            if (event.jbutton.button==5){
                ship->iTrigger = 1;
            }

        }else if (event.type == SDL_JOYBUTTONUP){
            printf("Joystick event button up %d\n",event.jbutton.button);
            if (event.jbutton.button==5){
                ship->iTrigger = 0;
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
        return SDL_TRUE;

    }else if ((event.type == SDL_KEYDOWN) && (!event.key.repeat)){
        //printf("Keydown\n");
        if(event.key.keysym.sym == SDLK_ESCAPE){
            return SDL_TRUE;
            //printf("Escape Key\n");
        }else if(event.key.keysym.sym == SDLK_LEFT){
            Ship_SetRotate(ship,1);
        }else if(event.key.keysym.sym == SDLK_RIGHT){
            Ship_SetRotate(ship,-1);
        }else if(event.key.keysym.sym == SDLK_UP){
            Ship_SetThrust(ship, 1);
        }else if(event.key.keysym.sym == SDLK_DOWN){
            Ship_SetThrust(ship, -1);
        }else if(event.key.keysym.sym == SDLK_SPACE){
            //SDL_CaptureMouse(SDL_TRUE);
            ship->iTrigger = 1;
            myGame.lastBulletTicks = SDL_GetTicks();
            //AddNewBullet( &myGame.listBullets, myShip.x, myShip.y, myShip.u);
            //Mix_PlayChannel( -1, laserSound, 0 );
        }else if(event.key.keysym.sym == SDLK_p){
            game->fPause ^= SDL_TRUE;
        }
    }else if ((event.type == SDL_KEYUP)){
        //printf("KeyUp\n");
        if (event.key.keysym.sym == SDLK_LEFT){
            Ship_SetRotate(ship, 0);
        }else if(event.key.keysym.sym == SDLK_RIGHT){
            Ship_SetRotate(ship, 0);
        }else if(event.key.keysym.sym == SDLK_UP){
            Ship_SetThrust(ship, 0);
        }else if(event.key.keysym.sym == SDLK_DOWN){
            Ship_SetThrust(ship, 0);
        }else if(event.key.keysym.sym == SDLK_SPACE){
            ship->iTrigger = 0;
        }
    }
    return SDL_FALSE;
}

int GameOverModeProcessEvent(SDL_Event event, SDL_Joystick *joystick,Game *game,Ship *ship)
{
    //-----------------------------------------------------

}

int HighScoresModeProcessEvent(SDL_Event event, SDL_Joystick *joystick,Game *game,Ship *ship)
{
    //-----------------------------------------------------

}


int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Mix_Chunk *laserSound = NULL;
    Mix_Chunk *explosionSound = NULL;

    SDL_Color orange = {255, 127, 40, 255};
    SDL_Color dark_blue = {10, 10, 150, 255};

    Rock    *listRocks = NULL;
    Explosion *listExplosions = NULL;


    srand(time(NULL));   // Initialization, should only be called once.

    myGame.score = 0;
    myGame.nbLifes = 3;
    myGame.listBullets = NULL;
    NewLevel( &listRocks, &myGame);

    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK))
    {
        fprintf(stderr, "SDL_Init Error : %s", SDL_GetError());
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
        fprintf(stderr, "SDL_CreateWindow Error: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_bool quit = SDL_FALSE;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer Error : %s", SDL_GetError());
        SDL_bool quit = SDL_TRUE;
    }



    Uint32 curTicks = SDL_GetTicks();
    Uint32 lastBulletTicks = curTicks;
    Uint32 lastUpdateExplosionTicks = curTicks;

    myGame.mode = IDLE_MODE;
    ProcessEvent = &IdleModeProcessEvent;

    SDL_Event event;
    while(!quit)
    {
        while(SDL_PollEvent(&event)){
            quit = ProcessEvent(event, joystick, &myGame, myShip);
        }

        if (myGame.fPause==SDL_FALSE){

            // Update Game states
            if (myShip){
                if (myShip->iRotate==1){
                    Ship_SetAngle(myShip, myShip->angle - 1.0f);
                }else if (myShip->iRotate==-1){
                    Ship_SetAngle(myShip, myShip->angle + 1.0f);
                }


                if (myShip->iThrust>0){
                    Ship_Accelerate(myShip, 0.015f);
                }else if (myShip->iThrust<0){
                    Ship_Decelerate(myShip, 0.015f);
                }

                if (myShip->iTrigger){
                    Uint32 curTicks = SDL_GetTicks();
                    if ((curTicks-lastBulletTicks)>200){
                        lastBulletTicks = curTicks;
                        AddNewBullet( &myGame.listBullets, myShip->x, myShip->y, myShip->u);
                        Mix_PlayChannel( -1, laserSound, 0 );
                    }
                }

                Ship_UpdatePosition(myShip);
            }

            UpdateRockPositions(listRocks);
            UpdateBulletPositions(myGame.listBullets);

            UpdateBulletsList(&myGame.listBullets);

            curTicks = SDL_GetTicks();
            if ((curTicks-lastUpdateExplosionTicks)>80){
                lastUpdateExplosionTicks = curTicks;
                UpdateExplosions(listExplosions);
                UpdateExplosionssList(&listExplosions);
            }

            Bullet *ptrBullet;
            if (ptrBullet=myGame.listBullets){
                Rock *ptrRock;
                do{
                    //
                    if (ptrRock=CheckBulletHitRocks(ptrBullet, listRocks)){
                        AddNewExplosion(&listExplosions,ptrRock->x,ptrRock->y, ptrRock->v.x, ptrRock->v.y);
                        Mix_PlayChannel( -1, explosionSound, 0 );
                        myGame.score += 30;
                        
                    }
                    //
                    ptrBullet = ptrBullet->next;
                }while(ptrBullet);
            }

            UpdateRocksList(&listRocks);
            if (listRocks==NULL){
                myGame.iLevel++;
                NewLevel(&listRocks, &myGame);
            }

            //-- Check Ship <-> Rock collision
            if (myShip){
                SDL_bool fLost = SDL_FALSE;
                Rock *ptrRock;
                if (ptrRock=listRocks){
                    float vx,vy,d;
                    do{
                        for (int i=0;i<3;++i){
                            vx = ptrRock->x - myShip->vertices[i].x;
                            vy = ptrRock->y - myShip->vertices[i].y;
                            d = sqrt(vx*vx+vy*vy);
                            if (d<ptrRock->rayMax){
                                fLost = SDL_TRUE;
                                ptrRock->fDeleted = SDL_TRUE;
                                AddNewExplosion(&listExplosions,ptrRock->x,ptrRock->y, ptrRock->v.x, ptrRock->v.y);
                                AddNewExplosion(&listExplosions,myShip->x, myShip->y, myShip->v.x, myShip->v.y);
                                Ship_SetLocation( myShip, WIN_WIDTH/2, WIN_HEIGHT/2);
                                Ship_SetVelocity( myShip, 0.0, 0.0);
                                Mix_PlayChannel( -1, explosionSound, 0 );                
                                //fPause = SDL_TRUE;
                                break;
                            }
                        }

                    }while((fLost==SDL_FALSE) && (ptrRock=ptrRock->next));
                }

                if (fLost){
                    myGame.nbLifes--;
                    if (myGame.nbLifes==0){
                        //fPause = SDL_TRUE;
                        free(myShip);
                        myShip = NULL;
                    // }else{
                    //     myShip.x = WIN_WIDTH/2;
                    //     myShip.y = WIN_HEIGHT/2;
                    //     myShip.v = (SDL_FPoint) {0.0,0.0};
                    }
                }
            }


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
        if (myShip) Ship_Draw( myShip, renderer);

        DrawBullets( myGame.listBullets, renderer);
        DrawRocks(listRocks, renderer);
        DrawExplosions(listExplosions, renderer);

        Draw_Score(renderer, myGame.score);

        // Symbol_Draw(&charA,renderer, 50.0, 100.0);
        // Symbol_Draw(&charB,renderer, 50.0+20.0, 100.0);
        // Symbol_Draw(&charC,renderer, 50.0+2*20.0, 100.0);
        // Symbol_Draw(&charD,renderer, 50.0+3*20.0, 100.0);
        // Symbol_Draw(&charE,renderer, 50.0+4*20.0, 100.0);
        // Symbol_Draw(&charF,renderer, 50.0+5*20.0, 100.0);
        // Symbol_Draw(&charG,renderer, 50.0+6*20.0, 100.0);
        // Symbol_Draw(&charH,renderer, 50.0+7*20.0, 100.0);
        // Symbol_Draw(&charI,renderer, 50.0+8*20.0, 100.0);
        // Symbol_Draw(&charJ,renderer, 50.0+9*20.0, 100.0);
        // Symbol_Draw(&charK,renderer, 50.0+10*20.0, 100.0);
        // Symbol_Draw(&charL,renderer, 50.0+11*20.0, 100.0);
        // Symbol_Draw(&charM,renderer, 50.0+12*20.0, 100.0);
        // Symbol_Draw(&charN,renderer, 50.0+13*20.0, 100.0);
        // Symbol_Draw(&charO,renderer, 50.0+14*20.0, 100.0);
        // Symbol_Draw(&charP,renderer, 50.0+15*20.0, 100.0);
        // Symbol_Draw(&charQ,renderer, 50.0+16*20.0, 100.0);
        // Symbol_Draw(&charR,renderer, 50.0+17*20.0, 100.0);
        // Symbol_Draw(&charS,renderer, 50.0+18*20.0, 100.0);
        // Symbol_Draw(&charT,renderer, 50.0+19*20.0, 100.0);
        // Symbol_Draw(&charU,renderer, 50.0+20*20.0, 100.0);
        // Symbol_Draw(&charV,renderer, 50.0+21*20.0, 100.0);
        // Symbol_Draw(&charW,renderer, 50.0+22*20.0, 100.0);
        // Symbol_Draw(&charX,renderer, 50.0+23*20.0, 100.0);
        // Symbol_Draw(&charY,renderer, 50.0+24*20.0, 100.0);
        // Symbol_Draw(&charZ,renderer, 50.0+25*20.0, 100.0);

        char strStart[]="PRESS TRIGGER TO START";
        char strGameOver[]="GAME OVER";

        switch (myGame.mode){
            case IDLE_MODE:
                DisplayMessage(renderer, (WIN_WIDTH-MessageWidth(strStart))/2.0, WIN_HEIGHT/2, strStart);
                break;
            case PLAY_MODE:
                break;
            case HIGH_SCORES_MODE:
                break;
            case GAME_OVER_MODE:
                DisplayMessage(renderer, (WIN_WIDTH-MessageWidth(strGameOver))/2.0, WIN_HEIGHT/2, strGameOver);
                break;
        }



        //
        SDL_RenderPresent(renderer);

        //
        SDL_Delay(10);

    }

    // Free Game Objects
    FreeBullets(&myGame.listBullets);
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

