
#include "game.h"
#include "saucer.h"


const SDL_FPoint SaucerShape[] = { {-20,0},{-9,3},{-4,8},{4,8},{9,3},{20,0},
                                    {9,-3},{4,-8},{-4,-8},{-9,-3} };

void Saucer_Draw( Saucer *ptrSaucer, SDL_Renderer *renderer)
{
    SDL_Point pts[16];
    SDL_Color orange = {255, 127, 40, 255};
    //--------------------------------------------------------
    SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);

    float x = ptrSaucer->x;
    float y = ptrSaucer->y;
    
    pts[0] = (SDL_Point) {SaucerShape[0].x+x,SaucerShape[0].y+y};
    pts[1] = (SDL_Point) {SaucerShape[1].x+x,SaucerShape[1].y+y};
    pts[2] = (SDL_Point) {SaucerShape[2].x+x,SaucerShape[2].y+y};
    pts[3] = (SDL_Point) {SaucerShape[3].x+x,SaucerShape[3].y+y};
    pts[4] = (SDL_Point) {SaucerShape[4].x+x,SaucerShape[4].y+y};
    pts[5] = (SDL_Point) {SaucerShape[5].x+x,SaucerShape[5].y+y};
    pts[6] = (SDL_Point) {SaucerShape[6].x+x,SaucerShape[6].y+y};
    pts[7] = (SDL_Point) {SaucerShape[7].x+x,SaucerShape[7].y+y};
    pts[8] = (SDL_Point) {SaucerShape[8].x+x,SaucerShape[8].y+y};
    pts[9] = (SDL_Point) {SaucerShape[9].x+x,SaucerShape[9].y+y};
    pts[10] = pts[0];
    SDL_RenderDrawLines(renderer, pts, 11);

    SDL_RenderDrawLineF(renderer, pts[1].x, pts[1].y, pts[4].x, pts[4].y);

    //SDL_RenderDrawLineF(renderer, pts[6].x, pts[6].y, pts[9].x, pts[9].y);

}


void Saucer_UpdatePosition(Saucer *ptrSaucer)
{
    //--------------------------------------
    float x = ptrSaucer->x + ptrSaucer->v.x;
    float y = ptrSaucer->y + ptrSaucer->v.y;

    float vx = fabs(ptrSaucer->v.x); 
    float vy = fabs(ptrSaucer->v.y); 
    if (x<0.0f){
        x = WIN_WIDTH - vx;
    }else if (x>=WIN_WIDTH){
        x = vx;
    }

    if (y<0.0f){
        y = WIN_HEIGHT - vy;
    }else if (y>=WIN_HEIGHT){
        y = vy;
    }

    ptrSaucer->x = x;
    ptrSaucer->y = y;

}