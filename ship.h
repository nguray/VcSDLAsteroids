#pragma once

#include <SDL2/SDL.h>

typedef struct Ship{

    float x;
    float y;
    float angle;
    int   iThrust;
    int   iRotate;
    int   iTrigger;
    SDL_bool fHide;
    SDL_FPoint u;
    SDL_FPoint n;
    SDL_FPoint v;
    SDL_FPoint vertices[3];

} Ship;

void Ship_Draw( Ship *ptrShip, SDL_Renderer *renderer);
void Ship_SetAngle( Ship *ptrShip, float a);
void Ship_SetLocation(Ship *ptrShip, int x, int y);
void Ship_SetVelocity(Ship *ptrShip, float vx, float vy);
void Ship_UpdatePosition(Ship *ptrShip);
void Ship_Accelerate(Ship *ptrShip, float pac);
void Ship_Decelerate(Ship *ptrShip, float pac);
void Ship_SetThrust(Ship *ptrShip, int iThrust);
void Ship_SetRotate(Ship *ptrShip, int iRotate);

