#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.141592654

// gcc sdl01.c -o sdl01 $(sdl2-config --cflags --libs)

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

void AddBullet(Bullet **listBullets, float x, float y, SDL_FPoint u)
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

void UpdateBulletStates(Bullet **listBullets)

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
                if (ptrNotDeleted==NULL){
                    // Firts not deleted
                    ptrNotDeleted = ptrBullet;
                    *listBullets = ptrNotDeleted;

                }else{

                    // Append to not deleted list
                    ptrNotDeleted->next = ptrBullet;
                    ptrNotDeleted = ptrBullet;
                }
                ptrBullet->next = NULL;
            }

            ptrBullet = ptrNext;

        }while(ptrBullet);

        if (ptrNotDeleted==NULL){
            // list is empty
            *listBullets = NULL;
        }


    }
}

void Add_NewRock(Rock **listRocks)
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
    SDL_Color orange = {255, 127, 40, 255};
    //-------------------------------------------------
    if (ptrRock=listRocks){

        SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);

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

void CheckBulletHitRocks(Bullet *ptrBullet, Rock *listRocks)
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
                    break;
                }
            }
            //--
            ptrRock = ptrRock->next;
        }while(ptrRock);
    }

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

    SDL_Color orange = {255, 127, 40, 255};
    SDL_Color dark_blue = {10, 10, 255, 255};

    Ship myShip;

    Bullet  *listBullets = NULL;
    Rock    *listRocks = NULL;

    int iRotate = 0;
    int iAccelerate = 0;
    int iTrigger = 0;

    Ship_SetLocation( &myShip, 320, 240);

    Ship_SetAngle( &myShip, -90.0f);


    float ra = myShip.angle*PI/180.0;
    Ship_SetVelocity(&myShip, 0.2f*cos(ra), 0.2f*sin(ra));


    srand(time(NULL));   // Initialization, should only be called once.

    for(int i = 0; i<15;++i){
        Add_NewRock(&listRocks);

    }

    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    window = SDL_CreateWindow("SDL2", 100, 100,
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


    Uint32 lastBulletTicks = SDL_GetTicks();
    SDL_Event event;
    while(!quit)
    {
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                quit = SDL_TRUE;
            }else if ((event.type == SDL_KEYDOWN) && (!event.key.repeat)){
                printf("Keydown\n");
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
                    iTrigger = 1;
                    lastBulletTicks = SDL_GetTicks();
                    AddBullet( &listBullets, myShip.x, myShip.y, myShip.u);
                }
            }else if ((event.type == SDL_KEYUP)){
                printf("KeyUp\n");
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
            if ((curTicks-lastBulletTicks)>250){
                lastBulletTicks = curTicks;
                AddBullet( &listBullets, myShip.x, myShip.y, myShip.u);
            }
        }

        // Update Game states
        Ship_UpdatePosition(&myShip);
        UpdateRockPositions(listRocks);
        UpdateBulletPositions(listBullets);
        UpdateBulletStates(&listBullets);

        Bullet *ptrBullet;
        if (ptrBullet=listBullets){
            do{
                //
                CheckBulletHitRocks(ptrBullet, listRocks);
                //
                ptrBullet = ptrBullet->next;
            }while(ptrBullet);
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
        // printf("nbr bullets = %d\n",nb);

        //int r = rand() % 20;
        //printf("random : %d\n",r);

        //
        SDL_SetRenderDrawColor(renderer, dark_blue.r, dark_blue.g, dark_blue.b, dark_blue.a);
        SDL_RenderClear(renderer);

        //
        Ship_Draw( &myShip, renderer);

        DrawBullets( listBullets, renderer);
        DrawRocks(listRocks, renderer);

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

    if(NULL != window)
        SDL_DestroyWindow(window);

    SDL_Quit();
    return EXIT_SUCCESS;
}

