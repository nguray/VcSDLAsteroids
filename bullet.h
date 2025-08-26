#pragma once

#include <SDL2/SDL.h>

typedef struct Bullet {

    float           x;
    float           y;
    SDL_FPoint      v;
    SDL_bool        fDeleted;
    struct Bullet   *next;

} Bullet;

void AddNewBullet(Bullet **listBullets, float x, float y, SDL_FPoint u);
void UpdateBulletPositions(Bullet *listBullets);
void DrawBullets(Bullet *listBullets, SDL_Renderer *renderer);
void UpdateBulletsList(Bullet **listBullets);
void FreeBullets(Bullet **listBullets);

