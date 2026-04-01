/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Enemy.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: source file for Enemy class.
Derives from Creature base class
*/
#include "Enemy.h"
#include "PlayerCharacter.h"

void Enemy::Die(PlayerCharacter* player)
{    
    player->IncreaseScoreAndLives(m_scoreVal);    
}

Enemy::Enemy(Level* lev, int speed,Game* game)
    : Creature(lev, speed,game)
{    
    exploding = false;
    deathTimer = 0;

    //constants*************************************************
    m_scoreVal = LuaGetInt(L, "enemyPointValue");
    //**********************************************************
}

Enemy::~Enemy()
{
	
}

void Enemy::MoveVertically(float delta, float currPos)
{
    if (IsUp)
    {
        Up(delta, currPos);       
    }
    else
    {
        Down(delta, currPos);        
    }
}

bool Enemy::GetExploding()
{
    return exploding;
}

void Enemy::SetNotExploding()
{
    exploding = false;
}

void Enemy::IncrementDeathTimer(double delta)
{
    deathTimer += delta;
}

void Enemy::resetTimer()
{
    deathTimer = 0;
}

double Enemy::GetDeathTimer()
{
    return deathTimer;
}

void Enemy::Destroyed(PlayerCharacter* player)
{
    exploding = true;
    Die(player);
    DeathPosX = PositionX;
    DeathPosY = PositionY;
}

void Enemy::Scroll(float pos)
{
    PositionX -= pos;
}


