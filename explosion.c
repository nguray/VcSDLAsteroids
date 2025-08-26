
/*-------------------------------------------------------------------*\




\*-------------------------------------------------------------------*/
#include "game.h"
#include "explosion.h"
#include <math.h>

void AddNewExplosion(Explosion **listExplosions,float x,float y,float vx,float vy)
{
    float ra,e_vx,e_vy;
    Explosion *ptrExplosion,*ptrCur;
    //------------------------------------------------
    float coeffRa = M_PI/180.0f;
    if (ptrExplosion = (Explosion *) calloc(1,sizeof(Explosion))){
        // Compute steering angle
        if (fabs(vx)<0.000001f){
            ra = 90.0;
        }else if (fabs(vy)<0.000001f){
            ra = 0.0f;
        }else{
            ra = atan2f(vy, vx);
        }
        ptrExplosion->iState = 0;
        ptrExplosion->nbVertices = 8;
        for (int i=0; i<8; ++i){
            e_vx = 2.5f*cos(ra + i*45*coeffRa) + vx; 
            e_vy = 2.5f*sin(ra + i*45*coeffRa) + vy;
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
            ;
        }while(ptrExplosion = ptrExplosion->next);
        
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

void FreeExplosions(Explosion **listExplosions)
{
    Explosion *ptrCur,*ptrNext;
    //--------------------------------------------------------
    if (ptrCur=*listExplosions){
        while(ptrCur){
            ptrNext = ptrCur->next;
            free(ptrCur);
            ptrCur = ptrNext;
        }
        *listExplosions = NULL;
    }

}
