#pragma once

#include <SDL2/SDL.h>

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


void AddNewRock(Rock **listRocks);
void UpdateRockPositions(Rock *listRocks);
void DrawRocks( Rock *listRocks, SDL_Renderer *renderer);
void UpdateRocksList(Rock **listRocks);
void FreeRocks(Rock **listRocks);

