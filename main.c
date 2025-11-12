
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
#include "saucer.h"


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
    char    c;
    int     nbSegments;
    Segment segments[];
} Symbol;


Symbol num0 = { '0', 4, {{{0,0},{8,0}},{{8,0},{8,12}},{{8,12},{0,12}},{{0,12},{0,0}}}};
Symbol num1 = { '1',1, {{{4,0},{4,12}}}};
Symbol num2 = { '2',5, {{{0,0},{8,0}}, {{8,0},{8,5}}, {{8,5},{0,5}}, {{0,5},{0,12}}, {{0,12},{8,12}}} };
Symbol num3 = { '3',4, {{{0,0},{8,0}}, {{8,0},{8,12}}, {{8,12},{0,12}}, {{0,5},{8,5}}} };
Symbol num4 = { '4',3, {{{0,0},{0,5}}, {{0,5},{8,5}}, {{8,0},{8,12}}} };
Symbol num5 = { '5',5, {{{0,0},{8,0}}, {{0,0},{0,5}}, {{0,5},{8,5}}, {{8,5},{8,12}}, {{8,12},{0,12}}} };
Symbol num6 = { '6',5, {{{0,0},{8,0}}, {{0,0},{0,12}}, {{0,5},{8,5}}, {{8,5},{8,12}}, {{8,12},{0,12}}} };
Symbol num7 = { '7',2, {{{0,0},{8,0}}, {{8,0},{8,12}}} };
Symbol num8 = { '8',5, {{{0,0},{0,12}}, {{8,0},{8,12}}, {{0,0},{8,0}}, {{0,5},{8,5}}, {{0,12},{8,12}}} };
Symbol num9 = { '9',5, {{{0,0},{8,0}}, {{0,5},{8,5}}, {{0,0},{0,5}}, {{8,0},{8,12}}, {{0,12},{8,12}}} };

Symbol *nums[] = {&num0, &num1, &num2, &num3, &num4, &num5, &num6, &num7, &num8, &num9};

Symbol charA = { 'A', 5, {{{0,12},{0,4}},{{0,4},{4,0}},{{4,0},{8,4}},{{8,4},{8,12}},{{0,7},{8,7}}}};
Symbol charB = { 'B',10, {{{0,0},{6,0}},{{6,0},{8,2}},{{8,2},{8,4}},{{8,4},{6,6}},{{6,6},{8,8}},
                      {{8,8},{8,10}},{{8,10},{6,12}},{{6,12},{0,12}},{{0,6},{8,6}},{{0,0},{0,12}}}};
Symbol charC = { 'C',3, {{{0,0},{8,0}}, {{0,0},{0,12}}, {{0,12},{8,12}}} };
Symbol charD = { 'D',6, {{{0,0},{5,0}},{{5,0},{8,3}},{{8,3},{8,9}},{{8,9},{5,12}},{{5,12},{0,12}},{{0,12},{0,0}}} };
Symbol charE = { 'E',4, {{{0,0},{8,0}}, {{0,0},{0,12}}, {{0,12},{8,12}}, {{0,6},{5,6}}} };
Symbol charF = { 'F',3, {{{0,0},{8,0}}, {{0,0},{0,12}}, {{0,6},{5,6}}} };
Symbol charG = { 'G',5, {{{8,0},{0,0}},{{0,0},{0,12}},{{0,12},{8,12}},{{8,12},{8,7}},{{8,7},{5,7}}}};
Symbol charH = { 'H',3, {{{0,0},{0,12}},{{8,0},{8,12}},{{0,6},{8,6}}} };
Symbol charI = { 'I',3, {{{3,0},{5,0}},{{4,0},{4,12}},{{3,12},{5,12}}} };
Symbol charJ = { 'J',3, {{{8,0},{8,12}},{{8,12},{0,12}},{{0,12},{0,8}}} };
Symbol charK = { 'K',3, {{{0,0},{0,12}},{{0,6},{8,0}},{{0,6},{8,12}}} };
Symbol charL = { 'L',2, {{{0,0},{0,12}},{{0,12},{8,12}}} };
Symbol charM = { 'M',4, {{{0,12},{0,0}}, {{0,0},{4,4}}, {{4,4},{8,0}}, {{8,0},{8,12}}} };
Symbol charN = { 'N',3, {{{0,12},{0,0}}, {{0,0},{8,12}}, {{8,12},{8,0}}} };
Symbol charO = { 'O',8, {{{0,2},{2,0}},{{2,0},{6,0}},{{6,0},{8,2}},{{8,2},{8,10}},{{8,10},{6,12}},
                      {{6,12},{2,12}},{{2,12},{0,10}},{{0,10},{0,2}}}};
Symbol charP = { 'P',4, {{{0,0},{8,0}}, {{8,0},{8,6}}, {{8,6},{0,6}}, {{0,0},{0,12}}} };
Symbol charQ = { 'Q',9, {{{0,2},{2,0}},{{2,0},{6,0}},{{6,0},{8,2}},{{8,2},{8,10}},{{8,10},{6,12}},
                      {{6,12},{2,12}},{{2,12},{0,10}},{{0,10},{0,2}},{{8,12},{5,9}}}};
Symbol charR = { 'R',5, {{{0,0},{8,0}},{{8,0},{8,6}},{{8,6},{0,6}},{{0,0},{0,12}},{{2,7},{8,12}}}};
Symbol charS = { 'S',9, {{{8,0},{2,0}},{{2,0},{0,2}},{{0,2},{0,4}},{{0,4},{2,6}},{{2,6},{6,6}},
                      {{6,6},{8,8}},{{8,8},{8,10}},{{8,10},{6,12}},{{6,12},{0,12}}}};
Symbol charT = { 'T',2, {{{0,0},{8,0}},{{4,0},{4,12}}} };
Symbol charU = { 'U',3, {{{0,0},{0,12}},{{0,12},{8,12}},{{8,12},{8,0}}} };
Symbol charV = { 'V',2, {{{0,0},{4,12}},{{4,12},{8,0}}} };
Symbol charW = { 'W',4, {{{0,0},{0,12}}, {{0,12},{4,7}}, {{4,7},{8,12}}, {{8,12},{8,0}}} };
Symbol charX = { 'X',2, {{{0,0},{8,12}},{{8,0},{0,12}}} };
Symbol charY = { 'Y',3, {{{0,0},{4,5}},{{4,5},{8,0}},{{4,5},{4,12}}} };
Symbol charZ = { 'Z',3, {{{0,0},{8,0}},{{8,0},{0,12}},{{0,12},{8,12}}} };
Symbol char_ = { '_',1, {{{0,12},{8,12}}}};
Symbol charSPC = { ' ',1, {{{4,12},{6,12}}}};

Symbol *tabChars[] = { &charA, &charB, &charC, &charD, &charE, &charF, &charG, &charH, &charI, &charJ, &charK, &charL, &charM,
                      &charN, &charO, &charP, &charQ, &charR, &charS, &charT, &charU, &charV, &charW, &charX, &charY, &charZ, &char_, &charSPC };

//-------------------------------------------------------------------
//-------------------------------------------------------------------
void Symbol_Draw(Symbol *ptrSymbol,SDL_Renderer *renderer,float x,float y)
{
    float   x1,y1,x2,y2;
    //---------------------------------------------
    
    for (int i=0;i<ptrSymbol->nbSegments;++i){
        x1 = x + ptrSymbol->segments[i].start.x;
        y1 = y + ptrSymbol->segments[i].start.y;
        x2 = x + ptrSymbol->segments[i].end.x;
        y2 = y + ptrSymbol->segments[i].end.y;
        SDL_RenderDrawLineF(renderer,x1,y1,x2,y2);
    }


}

Symbol *Char2Symbol(char c)
{
    Symbol *ptrSymbol;
    //--------------------------------------------------------------
    for (int i=0; i<28;++i){
        if (ptrSymbol=tabChars[i]){
            if (ptrSymbol->c==c){
                return ptrSymbol;
            }
        }
    }
    return NULL;
}

int Char2IndexSymbol(char c)
{
    //--------------------------------------------------------------
    for (int i=0; i<28;++i){
        if (tabChars[i]->c==c){
            return i;
        }
    }
    return -1;
}

void DisplayMessage(SDL_Renderer *renderer, float xt, float yt, const char *msg)
{
    int i,ic;
    float xc;
    char c;
    Symbol *ptrSymbol;
    //--------------------------------------------------------------
    if (strlen(msg)){
        ic = 0;
        while(c=msg[ic++]){
            if (ptrSymbol=Char2Symbol(c)){
                xc = ic*14.0 + xt;
                Symbol_Draw( ptrSymbol, renderer, xc, yt);
            }
        }
    }
}

void DisplayScore(SDL_Renderer *renderer, float x, float y, int score)
{
    //--------------------------------------------------------------
    int base,n,ic=0;
    for(int i = 5;i>=0;--i){
        base = pow(10,i);
        n = (int) (score / base);
        Symbol_Draw(nums[n], renderer, x+ic*14.0, y);
        score -= n*base;
        ic++;
    }

}

float MessageWidth(const char *msg)
{
    //--------------------------------------------------------------
    return strlen(msg)*14.0;
}

void LoadHighScores(Game *game)
{
    int  i, ival;
    char mot[82];
    FILE *fich;
    //--------------------------------------------------------------
    if ( (game) &&(fich=fopen("highscores.txt","r")) ){

        //
        i = 0;
        while( (i<10) &&!feof(fich) ){
            fscanf(fich,"%s %d", mot, &ival);
            game->highScores[i].score = ival;
            strncpy(game->highScores[i].name,mot,8);
            game->highScores[i].name[8] = '\0';
            printf("%d>>>> %s : %d\n", i, game->highScores[i].name, game->highScores[i].score);
            i++;
        }

        //--
        fclose(fich);
    }
}

void SaveHighScores(Game *game)
{
    FILE *fich;
    //--------------------------------------------------------------
    if ( (game) &&(fich=fopen("highscores.txt","w")) ){
        for(int i=0;i<10;++i){
            fprintf(fich,"%s %d\n", game->highScores[i].name, game->highScores[i].score);
        }
        //--
        fclose(fich);
    }

}

int IsHighScore(Game *game)
{
    //--------------------------------------------------------------
    for(int i=0;i<10;++i){
        if (game->score>=game->highScores[i].score){
            return i;
        }
    }
    return -1;
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

void NewLevel( Game *game)
{
    int nbRocks = game->iLevel * 3 + 6;
    //-----------------------------------------------------
    for(int i = 0; i<nbRocks;++i){
        AddNewRock(&game->listRocks);

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
    if (joystick){

        if (event.type == SDL_JOYBUTTONDOWN){
            printf("Joystick event button down %d\n",event.jbutton.button);
            if (event.jbutton.button==5){
                game->mode = PLAY_MODE;
                ProcessEvent = &PlayModeProcessEvent;
                if (myShip=(Ship *) calloc(1,sizeof(Ship))){
                    myShip->iTrigger = 0;
                    Ship_SetThrust(myShip, 0);
                    Ship_SetRotate(myShip, 0);
                    Ship_SetLocation(myShip, 320, 240);
                    Ship_SetAngle( myShip, -90.0f);
                }
            }

        }else if (event.type == SDL_JOYBUTTONUP){
            printf("Joystick event button up %d\n",event.jbutton.button);

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
    if (joystick){

        if (event.type == SDL_JOYBUTTONDOWN){
            printf("Joystick event button down %d\n",event.jbutton.button);
            if (event.jbutton.button==5){
                game->mode = IDLE_MODE;
                ProcessEvent = &IdleModeProcessEvent;
            }

        }else if (event.type == SDL_JOYBUTTONUP){
            printf("Joystick event button up %d\n",event.jbutton.button);

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

    if (event.type == SDL_QUIT){
        return SDL_TRUE;

    }else if ((event.type == SDL_KEYDOWN) && (!event.key.repeat)){
        //printf("Keydown\n");
        if(event.key.keysym.sym == SDLK_ESCAPE){
            return SDL_TRUE;
            //printf("Escape Key\n");
        }else if(event.key.keysym.sym == SDLK_SPACE){
            game->mode = IDLE_MODE;
            ProcessEvent = &IdleModeProcessEvent;

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

void DrawHighScores(SDL_Renderer *renderer, Game *myGame)
{
    int x,y;
    char mot[82];
    SDL_Color light_grey = {200, 200, 200, 255};
    SDL_Color red = {255, 0, 0, 255};
    //-----------------------------------------------------

    SDL_SetRenderDrawColor(renderer, light_grey.r, light_grey.g, light_grey.b, light_grey.a);
    const char strTitle[]="HIGH SCORES";
    DisplayMessage(renderer, WIN_WIDTH/2-MessageWidth(strTitle)/2, WIN_HEIGHT/8.5, strTitle);

    int xName = WIN_WIDTH / 2 - 14.0*10.0;
    int xScore = WIN_WIDTH / 2 + 14.0*4.0;
    int dy = 20.0;
    int yTop = WIN_HEIGHT/4.5;
    for(int i=0;i<10;++i){
        if (i==myGame->iHighScore){
            SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
        }else{
            SDL_SetRenderDrawColor(renderer, light_grey.r, light_grey.g, light_grey.b, light_grey.a);
        }
        DisplayMessage(renderer, xName, yTop, myGame->highScores[i].name);
        DisplayScore(renderer, xScore, yTop, myGame->highScores[i].score);
        //--
        yTop += dy; 
    }

    SDL_SetRenderDrawColor(renderer, light_grey.r, light_grey.g, light_grey.b, light_grey.a);
    yTop = WIN_HEIGHT-6.0*20.0;
    const char strFooter1[]="PRESS UP  DOWN TO SET CHARACTER";
    DisplayMessage(renderer, WIN_WIDTH/2-MessageWidth(strFooter1)/2, yTop, strFooter1);

    yTop += 20.0;
    const char strFooter2[]="PRESS LEFT RIGHT TO MOVE CURSOR";
    DisplayMessage(renderer, WIN_WIDTH/2-MessageWidth(strFooter2)/2, yTop, strFooter2);

    yTop += 40.0;
    const char strFooter3[]="PRESS TRIGGER TO CONTINUE";
    DisplayMessage(renderer, WIN_WIDTH/2-MessageWidth(strFooter3)/2, yTop, strFooter3);

}


int HighScoresModeProcessEvent(SDL_Event event, SDL_Joystick *joystick,Game *game,Ship *ship)
{
    //-----------------------------------------------------
    if (joystick){

        if (event.type == SDL_JOYBUTTONDOWN){
            printf("Joystick event button down %d\n",event.jbutton.button);
            if (event.jbutton.button==5){
                game->mode = IDLE_MODE;
                ProcessEvent = &IdleModeProcessEvent;
                if (!strncmp(game->highScores[game->iHighScore].name,"        ",8)){
                    strcpy(game->highScores[game->iHighScore].name,"________");
                }
            }
            SaveHighScores(game);

        }else if (event.type == SDL_JOYBUTTONUP){
            printf("Joystick event button up %d\n",event.jbutton.button);

        }else if (event.type == SDL_JOYHATMOTION){
            printf("Joystick event hat \n");
            if ( event.jhat.value & SDL_HAT_UP )
            {
                /* Do up stuff here */
                printf("HAT UP\n");
                char c = game->highScores[game->iHighScore].name[game->iCharHighScore];
                int ic = Char2IndexSymbol(c);
                if (ic==-1){
                    c = 'A';
                }else{
                    if (ic>0){
                        ic--;
                        c = tabChars[ic]->c;
                    }else{
                        c = tabChars[27]->c;
                    }
                }
                game->highScores[game->iHighScore].name[game->iCharHighScore] = c;


            }
            if ( event.jhat.value & SDL_HAT_DOWN )
            {
                /* Do up stuff here */
                printf("HAT DOWN\n");
                char c = game->highScores[game->iHighScore].name[game->iCharHighScore];
                int ic = Char2IndexSymbol(c);
                if (ic==-1){
                    c = 'A';
                }else{
                    if (ic>=27){
                        c = tabChars[0]->c;
                    }else{
                        ic++;
                        c = tabChars[ic]->c;
                    }
                }
                game->highScores[game->iHighScore].name[game->iCharHighScore] = c;

            }
            if ( event.jhat.value & SDL_HAT_LEFT )
            {
                /* Do left stuff here */
                printf("HAT LEFT\n");
                if (game->iCharHighScore>0){
                    game->iCharHighScore--;
                }

            }
            if ( event.jhat.value & SDL_HAT_RIGHT )
            {
                /* Do left stuff here */
                printf("HAT RIGHT\n");
                if (game->iCharHighScore<8){
                    game->iCharHighScore++;
                }

            }

            if ( event.jhat.value & SDL_HAT_RIGHTDOWN )
            {
                /* Do right and down together stuff here */
                printf("HAT RIGHTDOWN\n");
            }
        }
    }

    if (event.type == SDL_QUIT){
        return SDL_TRUE;

    }else if ((event.type == SDL_KEYDOWN) && (!event.key.repeat)){
        //printf("Keydown\n");
        if(event.key.keysym.sym == SDLK_ESCAPE){
            return SDL_TRUE;
            //printf("Escape Key\n");
        }else if(event.key.keysym.sym == SDLK_SPACE){
            game->mode = IDLE_MODE;
            ProcessEvent = &IdleModeProcessEvent;
            if (!strncmp(game->highScores[game->iHighScore].name,"        ",8)){
                strcpy(game->highScores[game->iHighScore].name,"________");
            }
            SaveHighScores(game);
        }else if(event.key.keysym.sym == SDLK_LEFT){
            if (game->iCharHighScore>0){
                game->iCharHighScore--;
            }
        }else if(event.key.keysym.sym == SDLK_RIGHT){
            if (game->iCharHighScore<8){
                game->iCharHighScore++;
            }
        }else if(event.key.keysym.sym == SDLK_UP){
            char c = game->highScores[game->iHighScore].name[game->iCharHighScore];
            int ic = Char2IndexSymbol(c);
            if (ic==-1){
                c = 'A';
            }else{
                if (ic>0){
                    ic--;
                    c = tabChars[ic]->c;
                }else{
                    c = tabChars[27]->c;
                }
            }
            game->highScores[game->iHighScore].name[game->iCharHighScore] = c;
        }else if(event.key.keysym.sym == SDLK_DOWN){
            char c = game->highScores[game->iHighScore].name[game->iCharHighScore];
            int ic = Char2IndexSymbol(c);
            if (ic==-1){
                c = 'A';
            }else{
                if (ic>=27){
                    c = tabChars[0]->c;
                }else{
                    ic++;
                    c = tabChars[ic]->c;
                }
            }
            game->highScores[game->iHighScore].name[game->iCharHighScore] = c;
        }else if(event.key. keysym.sym == SDLK_p){
            game->fPause ^= SDL_TRUE;
        }
    }else if ((event.type == SDL_KEYUP)){
        printf("KeyUp\n");
    }
    return SDL_FALSE;

}


int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Mix_Chunk *laserSound = NULL;
    Mix_Chunk *explosionSound = NULL;

    SDL_Color orange = {255, 127, 40, 255};
    SDL_Color dark_blue = {10, 10, 150, 255};

    Explosion *listExplosions = NULL;

    srand(time(NULL));   // Initialization, should only be called once.


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
    Uint32 timeoutShipLost;

    myShip = NULL;
    NewLevel(&myGame);

    myGame.mode = IDLE_MODE;
    ProcessEvent = &IdleModeProcessEvent;
    myGame.score = 0;
    myGame.nbLifes = 3;
    myGame.listBullets = NULL;

    LoadHighScores(&myGame);

    // myGame.iCharHighScore = 0;
    // myGame.iHighScore = 1;
    // strcpy(myGame.highScores[1].name, "        ");
    // myGame.mode = HIGH_SCORES_MODE;
    // ProcessEvent = &HighScoresModeProcessEvent;


    SDL_bool fShipLost = SDL_FALSE;
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

            UpdateRockPositions(myGame.listRocks);
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
                    if (ptrRock=CheckBulletHitRocks(ptrBullet, myGame.listRocks)){
                        AddNewExplosion(&listExplosions,ptrRock->x,ptrRock->y, ptrRock->v.x, ptrRock->v.y);
                        Mix_PlayChannel( -1, explosionSound, 0 );
                        if (ptrRock->rayMax>15.0){
                            DivideRocks(&myGame.listRocks, ptrRock);
                            myGame.score += 50;
                        }else{
                            myGame.score += 100;
                        }
                        if ((mySaucer==NULL)&&((rand()%6)==0)) {
                            mySaucer = NewSaucer();
                        }
                    }else if (mySaucer){
                        float vx = ptrBullet->x - mySaucer->x;
                        float vy = ptrBullet->y - mySaucer->y;
                        float d = sqrt(vx*vx+vy*vy);
                        if (d<24.0){
                            AddNewExplosion(&listExplosions,mySaucer->x,mySaucer->y, mySaucer->v.x, mySaucer->v.y);
                            Mix_PlayChannel( -1, explosionSound, 0 );
                            free(mySaucer);
                            mySaucer = NULL;
                            myGame.score += 500;
                        }
                    }
                    //
                    ptrBullet = ptrBullet->next;
                }while(ptrBullet);
            }

            UpdateRocksList(&myGame.listRocks);
            if (myGame.listRocks==NULL){
                myGame.iLevel++;
                NewLevel(&myGame);
            }

            //-- Check Ship <-> Rock collision
            if (myShip){
                
                Rock *ptrRock;
                if (ptrRock=myGame.listRocks){
                    float vx,vy,d;
                    do{
                        for (int i=0;i<3;++i){
                            vx = ptrRock->x - myShip->vertices[i].x;
                            vy = ptrRock->y - myShip->vertices[i].y;
                            d = sqrt(vx*vx+vy*vy);
                            if (d<ptrRock->rayMax){
                                fShipLost = SDL_TRUE;
                                ptrRock->fDeleted = SDL_TRUE;
                                AddNewExplosion(&listExplosions,ptrRock->x,ptrRock->y, ptrRock->v.x, ptrRock->v.y);
                                AddNewExplosion(&listExplosions,myShip->x, myShip->y, myShip->v.x, myShip->v.y);
                                Ship_SetLocation( myShip, WIN_WIDTH/2, WIN_HEIGHT/2);
                                Ship_SetVelocity( myShip, 0.0, 0.0);
                                Mix_PlayChannel( -1, explosionSound, 0 );
                                break;
                            }
                        }
                        if (mySaucer){
                            float vx = mySaucer->x - myShip->x;
                            float vy = mySaucer->y - myShip->y;
                            float d = sqrt(vx*vx+vy*vy);
                            if (d<22.0){
                                fShipLost = SDL_TRUE;
                                AddNewExplosion(&listExplosions,myShip->x,myShip->y, myShip->v.x, myShip->v.y);
                                Mix_PlayChannel( -1, explosionSound, 0 );
                                free(mySaucer);
                                mySaucer = NULL;
                            }
                        }

                    }while((fShipLost==SDL_FALSE) && (ptrRock=ptrRock->next));
                }

                if (fShipLost){
                    
                    free(myShip);
                    myShip = NULL;
                    myGame.nbLifes--;
                    timeoutShipLost = SDL_GetTicks();

                }

            }else if (fShipLost){
                Uint32 curTicks = SDL_GetTicks();
                if ((curTicks-timeoutShipLost)>500){
                    if ((myGame.listBullets==NULL)&&(listExplosions==NULL)){
                        fShipLost = SDL_FALSE;
                        if (myGame.nbLifes==0){
                            
                            int ih = IsHighScore(&myGame);
                            if ((ih>=0) && (myGame.score)){

                                myGame.mode = HIGH_SCORES_MODE;
                                ProcessEvent = &HighScoresModeProcessEvent;

                                myGame.iCharHighScore = 0;
                                myGame.iHighScore = ih;
                                myGame.highScores[ih].score = myGame.score;
                                strcpy(myGame.highScores[ih].name,"        ");

                            }else{
                                myGame.mode = IDLE_MODE;
                                ProcessEvent = &IdleModeProcessEvent;

                            }

                            FreeRocks(&myGame.listRocks);
                            myGame.score = 0;
                            myGame.nbLifes = 3;
                            myGame.iLevel = 0;

                        }else{

                            if (myShip=(Ship *) calloc(1,sizeof(Ship))){
                                myShip->iTrigger = 0;
                                Ship_SetThrust(myShip, 0);
                                Ship_SetRotate(myShip, 0);
                                Ship_SetLocation(myShip, 320, 240);
                                Ship_SetAngle( myShip, -90.0f);
                            }

                        }

                    }
                }

            }

            //--
           if (mySaucer) Saucer_UpdatePosition(mySaucer);

        }


        //
        SDL_SetRenderDrawColor(renderer, dark_blue.r, dark_blue.g, dark_blue.b, dark_blue.a);
        SDL_RenderClear(renderer);

        //
        if (myShip) Ship_Draw( myShip, renderer);
        
        if (mySaucer) Saucer_Draw(mySaucer, renderer);

        DrawBullets( myGame.listBullets, renderer);
        DrawRocks(myGame.listRocks, renderer);
        DrawExplosions(listExplosions, renderer);

        const char strStart[]="PRESS TRIGGER TO START";
        const char strGameOver[]="GAME OVER";

        switch (myGame.mode){
            case IDLE_MODE:
                DisplayMessage(renderer, (WIN_WIDTH-MessageWidth(strStart))/2.0, WIN_HEIGHT/2, strStart);
                break;
            case PLAY_MODE:
                Draw_Score(renderer, myGame.score);
                break;
            case HIGH_SCORES_MODE:
                DrawHighScores(renderer, &myGame);
                break;
            case GAME_OVER_MODE:
                DisplayMessage(renderer, (WIN_WIDTH-MessageWidth(strGameOver))/2.0, WIN_HEIGHT/2, strGameOver);
                break;
        }

        //
        SDL_RenderPresent(renderer);

        //
        SDL_Delay(5);

    }

    // Free Game Objects
    FreeBullets(&myGame.listBullets);
    FreeRocks(&myGame.listRocks);
    FreeExplosions(&listExplosions);
    if (myShip){
        free(myShip);
    }
    if (mySaucer){
        free(mySaucer);
    }

    //SDL_Delay(3000);

    //--
    SaveHighScores(&myGame);

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

