/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Creature.h
Date:		March 2025
Last Updated:	12/03/2025

Description: header file for Creature class. 
This is the base class for the player and enemies
*/
#pragma once
#include <vector>
#include<allegro5/allegro.h>
#include"Level.h"
#include "Game.h"
#include "LuaHelper.h"

class Creature
{
protected:
    Game* p_Game = NULL;
    Level* p_CurrentLevel = NULL;
    lua_State* L;

    //constants for lua*************************************
    int AnimFrames;
    double FrameTimeLimit;
    ALLEGRO_BITMAP* Sprite = NULL;
    //look in LoadContent method for these
    std::vector<ALLEGRO_BITMAP*> ExplosionSprite;
    std::vector<ALLEGRO_BITMAP*> enemySprites;
    //*********************************************

    bool exploding;    
    bool IsUp;
    bool IsDead;
    bool IsEnemy;
        
    float PositionX;
    float PositionY;
    float StartPosX;
    float StartPosY;
    float Speed;    
    
    int animFrameIndex;
    double FrameTimer;
   
    int DeathPosX, DeathPosY;//to record where the death animation should play   
    
public:
    //constructor and destructor
    Creature(Level* lev, int inputSpeed,Game* game);
    ~Creature();

    void LoadContent(bool enemy);
    void Draw();
    void Left(int Audioindex, float volume, float speedVal, float currPos);
    void Right(int Audioindex, float volume, float speedVal, float currPos);
    void Up(float speedVal, float& currPos);
    void Down(float speedVal, float currPos);    
    bool CollidesWith(Creature* creature);    
    void SetFrame(double delta);    
    void SetIsDead(bool dead);
    void SetDeathPos();
    void Activate(int posX, int posY);
    void SetExploding();

    //getters
    bool GetIsDead();
    int GetSpriteWidth();
    int GetSpriteHeight();
    float GetPositionX();
    float GetPositionY();
    int GetSpeed();
};
