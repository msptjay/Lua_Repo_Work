/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Enemy.h
Date:		March 2025
Last Updated:	12/03/2025

Description: header file for Enemy class.
Derives from Creature base class
*/
#include "Creature.h"

#pragma once

class PlayerCharacter;
class Enemy : public Creature
{
private:

    //constants for lua*************************************
    int m_scoreVal;
    //*********************************************
    
    //members      
    double deathTimer;
    //methods
    void Die(PlayerCharacter* player);

public:
    //constructor and destructor
    Enemy(Level* lev, int speed,Game* game);        
    ~Enemy();

    void MoveVertically(float delta, float currPos);    
    void SetNotExploding();
    void IncrementDeathTimer(double delta);
    void resetTimer();    
    void Destroyed(PlayerCharacter* player);
    void Scroll(float pos);

    //getters
    bool GetExploding();
    double GetDeathTimer();
};

