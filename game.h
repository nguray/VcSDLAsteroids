#pragma once

#include "ship.h"
#include "bullet.h"
#include "rock.h"
#include "saucer.h"

#ifndef  M_PI
#define  M_PI  3.1415926535897932384626433
#endif
#define WIN_WIDTH  640
#define WIN_HEIGHT 480

enum{
    IDLE_MODE,
    PLAY_MODE,
    HIGH_SCORES_MODE,
    GAME_OVER_MODE
};

typedef struct HighScore{
    int     score;
    char    name[12];
} HighScore;


typedef struct Game {
    int mode;
    int iLevel;
    int nbLifes;
    int score ;
    SDL_bool fPause;
    Uint32  lastBulletTicks;
    Bullet  *listBullets;
    Rock    *listRocks;
    int       iCharHighScore;
    int       iHighScore;
    HighScore highScores[10];
} Game;

extern Game myGame;
extern Ship   *myShip;
extern Saucer *mySaucer;