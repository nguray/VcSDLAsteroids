#pragma once

#include <SDL2/SDL.h>

typedef struct {
    float x;
    float y;
    SDL_bool fHide;
    SDL_FPoint u;
    SDL_FPoint n;
    SDL_FPoint v;

} Saucer;

Saucer *NewSaucer();
void Saucer_Draw( Saucer *ptrSaucer, SDL_Renderer *renderer);
void Saucer_UpdatePosition(Saucer *ptrSaucer);
