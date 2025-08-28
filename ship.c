
#include "game.h"
#include "ship.h"

const SDL_Point ShipShape[] = {{0,15},{10,-10},{-10,-10}};
const SDL_Point miniShipShape[] = {{0,-10},{7,7},{-7,7}};

void Ship_Draw( Ship *ptrShip, SDL_Renderer *renderer)
{
    SDL_Point pts[5];
    SDL_Color white = {220, 220, 220, 128};
    SDL_Color orange = {255, 127, 40, 255};
    float x0,y0,x1,y1,x2,y2,x3,y3,x4,y4;

    if (ptrShip->fHide) return;

    // Draw space ship
    SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);

    x1 = ptrShip->x + ShipShape[0].y * ptrShip->u.x + ShipShape[0].x * ptrShip->n.x;
    y1 = ptrShip->y + ShipShape[0].y * ptrShip->u.y + ShipShape[0].x * ptrShip->n.y;
    SDL_RenderDrawLineF(renderer,ptrShip->x,ptrShip->y,x1,y1);


    x2 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x;
    y2 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y;
    SDL_RenderDrawLineF(renderer,ptrShip->x,ptrShip->y,x2,y2);

    x3 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x;
    y3 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y;
    SDL_RenderDrawLineF(renderer,ptrShip->x,ptrShip->y,x3,y3);

    pts[0].x = x1;
    pts[0].y = y1;
    pts[1].x = x2;
    pts[1].y = y2;
    pts[2].x = x3;
    pts[2].y = y3;
    pts[3].x = x1;
    pts[3].y = y1;
    SDL_RenderDrawLines(renderer, pts, 4);

    //-- Store vertices for collision checking
    ptrShip->vertices[0] = (SDL_FPoint) {x1,y1};
    ptrShip->vertices[1] = (SDL_FPoint) {x2,y2};
    ptrShip->vertices[2] = (SDL_FPoint) {x3,y3};

    //-- Draw remain life
    x0 = WIN_WIDTH - 100.0f;
    y0 = 14.0;
    for (int i=0; i<myGame.nbLifes; ++i){

        x1 = x0 + miniShipShape[0].x;
        y1 = y0 + miniShipShape[0].y;
        SDL_RenderDrawLineF(renderer, x0, y0, x1, y1);


        x2 = x0 + miniShipShape[1].x;
        y2 = y0 + miniShipShape[1].y;
        SDL_RenderDrawLineF(renderer, x0, y0, x2, y2);

        x3 = x0 + miniShipShape[2].x;
        y3 = y0 + miniShipShape[2].y;
        SDL_RenderDrawLineF(renderer, x0, y0, x3, y3);

        pts[0].x = x1;
        pts[0].y = y1;
        pts[1].x = x2;
        pts[1].y = y2;
        pts[2].x = x3;
        pts[2].y = y3;
        pts[3].x = x1;
        pts[3].y = y1;
        SDL_RenderDrawLines(renderer, pts, 4);

        x0 += 20.0;
    }

    void Draw_ExhaustGas(SDL_Renderer *renderer, float x1, float y1, float x2, float y2)
    {
        SDL_Point pts[5];
        SDL_Color white = {220, 220, 220, 128};
        //---------------------------------------------------------------
        float vx = x2-x1;
        float vy = y2-y1; 
        float norm = sqrt(vx*vx+vy*vy);
        SDL_FPoint u = {vx/norm,vy/norm};
        SDL_FPoint n = {-u.y,u.x};
        pts[0].x = x1 + u.x;
        pts[0].y = y1 + u.y;
        pts[1].x = x1 + 1.5*n.x + 1.5*u.x;
        pts[1].y = y1 + 1.5*n.y + 1.5*u.y;
        pts[2].x = x2;
        pts[2].y = y2;
        pts[3].x = x1 - 1.5*n.x + 1.5*u.x;
        pts[3].y = y1 - 1.5*n.y + 1.5*u.y;
        pts[4] = pts[0];
        SDL_RenderDrawLines(renderer, pts, 5);

    }

    if (ptrShip->iThrust==1){
        // Draw Exhaust gas during accelerationinner
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
        
        //_Draw right acceleratio exhaust gas
        x1 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x - 4.0*ptrShip->n.x;
        y1 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y - 4.0*ptrShip->n.y;
        x2 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x - 4.0*ptrShip->n.x - 10.0*ptrShip->u.x;
        y2 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y - 4.0*ptrShip->n.y - 10.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

        //_Draw left acceleratio exhaust gas
        x1 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x + 4.0*ptrShip->n.x;
        y1 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y + 4.0*ptrShip->n.y;
        x2 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x + 4.0*ptrShip->n.x - 10.0*ptrShip->u.x;
        y2 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y + 4.0*ptrShip->n.y - 10.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

    }else if (ptrShip->iThrust==-1){
        // Draw Exhaust gas during deceleration
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

        // Draw right deceleratio exhaust gas
        x1 = ptrShip->x - 6.0*ptrShip->n.x;
        y1 = ptrShip->y - 6.0*ptrShip->n.y;
        x2 = ptrShip->x - 8.0*ptrShip->n.x + 10.0*ptrShip->u.x;
        y2 = ptrShip->y - 8.0*ptrShip->n.y + 10.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

        // Draw left deceleratio exhaust gas
        x1 = ptrShip->x + 6.0*ptrShip->n.x;
        y1 = ptrShip->y + 6.0*ptrShip->n.y;
        x2 = ptrShip->x + 8.0*ptrShip->n.x + 10.0*ptrShip->u.x;
        y2 = ptrShip->y + 8.0*ptrShip->n.y + 10.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

    }

    if (ptrShip->iRotate>0){
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

        // Draw left decceleration exhaust gas
        x1 = ptrShip->x - 6.0*ptrShip->n.x;
        y1 = ptrShip->y - 6.0*ptrShip->n.y;
        x2 = ptrShip->x - 8.0*ptrShip->n.x + 8.0*ptrShip->u.x;
        y2 = ptrShip->y - 8.0*ptrShip->n.y + 8.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

        //_Draw right acceleratio exhaust gas
        x1 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x - 4.0*ptrShip->n.x;
        y1 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y - 4.0*ptrShip->n.y;
        x2 = ptrShip->x + ShipShape[1].y * ptrShip->u.x + ShipShape[1].x * ptrShip->n.x - 4.0*ptrShip->n.x - 8.0*ptrShip->u.x;
        y2 = ptrShip->y + ShipShape[1].y * ptrShip->u.y + ShipShape[1].x * ptrShip->n.y - 4.0*ptrShip->n.y - 8.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

    }else if (ptrShip->iRotate<0){
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);

        // Draw right decceleration exhaust gas
        x1 = ptrShip->x + 6.0*ptrShip->n.x;
        y1 = ptrShip->y + 6.0*ptrShip->n.y;
        x2 = ptrShip->x + 8.0*ptrShip->n.x + 8.0*ptrShip->u.x;
        y2 = ptrShip->y + 8.0*ptrShip->n.y + 8.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

        //_Draw left acceleratio exhaust gas
        x1 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x + 4.0*ptrShip->n.x;
        y1 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y + 4.0*ptrShip->n.y;
        x2 = ptrShip->x + ShipShape[2].y * ptrShip->u.x + ShipShape[2].x * ptrShip->n.x + 4.0*ptrShip->n.x - 8.0*ptrShip->u.x;
        y2 = ptrShip->y + ShipShape[2].y * ptrShip->u.y + ShipShape[2].x * ptrShip->n.y + 4.0*ptrShip->n.y - 8.0*ptrShip->u.y;
        Draw_ExhaustGas( renderer, x1, y1, x2, y2);

    }
}

void Ship_SetAngle( Ship *ptrShip, float a)
{
    //--------------------------------------
    ptrShip->angle = a;

    ptrShip->u.x = cos(a*M_PI/180.0);
    ptrShip->u.y = sin(a*M_PI/180.0);

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

void Ship_SetThrust(Ship *ptrShip, int iThrust)
{
    //
    ptrShip->iThrust = iThrust;
}

void Ship_SetRotate(Ship *ptrShip, int iRotate)
{
    //
    ptrShip->iRotate = iRotate;
}
