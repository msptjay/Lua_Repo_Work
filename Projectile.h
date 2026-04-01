/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Projectile.h
Date:		March 2025
Last Updated:	12/03/2025

Description: Header file for The Projectile class.
*/
#pragma once
#include<allegro5/allegro.h>
#include "LuaHelper.h"

class Level;
class Creature;
class Projectile
{
private:
    //constants for lua***************************************    
    float Speed;
    //********************************************************

    //members
    bool m_InUse;     
    float m_PositionX, m_PositionY;
    
    //pointers
    ALLEGRO_BITMAP* p_bullet = NULL;
    lua_State* L;

public:
    //constructor and destructor
    Projectile();
    ~Projectile();

    void LosdContent(const char* name);
    void Update(Level* CurrentLevel, bool isEnemyFiring, float currPos);
    void Draw();
    bool CollidesWith(Creature* creature);
    void SetInUse(int StartPosX, int StartPosY);    
    void ResetBullet();    

    //getters
    int GetPositionX();
    int GetPositionY();
    bool GetInUse();
};

