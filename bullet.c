#include "game.h"
#include "bullet.h"

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

