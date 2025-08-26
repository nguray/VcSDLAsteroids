#pragma once

#include <SDL2/SDL.h>

typedef struct Explosion {
    int                 iState;
    int                 nbVertices;
    SDL_FPoint          vertices[8];
    SDL_FPoint          velocities[8];
    SDL_bool            fDeleted;
    struct Explosion    *next;
} Explosion;

//---------------------------------------------------
void AddNewExplosion(Explosion **listExplosions,float x,float y,float vx,float vy);
void UpdateExplosions(Explosion *listExplosions);
void DrawExplosions(Explosion *listExplosions, SDL_Renderer *renderer);
void UpdateExplosionssList(Explosion **listExplosions);
void FreeExplosions(Explosion **listExplosions);
