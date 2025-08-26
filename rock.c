#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"
#include "rock.h"

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
        a = ((float) (rand() % 360))*M_PI/180.0f;
        ptrRock->v.x = 0.3f * cos(a);
        ptrRock->v.y = 0.3f * sin(a);

        a = 0.0f;
        ptrRock->nbVertices = 0;
        ptrRock->rayMax = 0.0f;
        ptrRock->rayMin = 1000.0;
        for(int i=0;i<10;++i){
            a += ((float) (rand() % 40 + 20))*M_PI/180.0f;
            if (a>2*M_PI) break;
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

