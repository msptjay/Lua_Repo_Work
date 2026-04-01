/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	PlayerCharacter.h
Date:		March 2025
Last Updated:	15/03/2025

Description: header file for PlayerCharacter class.
Derives from Creature base class
*/
#pragma once
#include "Creature.h"

class Audio;
class PlayerCharacter : public Creature
{
private:
    //pointers    
    Audio* p_GameAudio = NULL;

    //constants for lua*************************************
    int MaxLives;//the maximum number of lives possible
    int InitialLives;//the starting lives number
    int ScoreMultiplier;//the score multiplier when the player is due a bonus and has max lives
    int ScoreMultForBonus;//the multiplier for the player to get an extra life or score bonus (if the score modulus this = 0, then give a bonus). should be a multiple of ten
    //******************************************************

    int m_Lives;    
    int m_Score;    
    int m_LastScore;
    bool m_isInvincible;

public:
    //constructor and destructor
    PlayerCharacter(Level* lev, int speed, Game* game, Audio* GA);
    ~PlayerCharacter();
   
    void DecreaseLives();
    void IncreaseLives();
    void ResetLives();    
    void SetInvincible(bool invincible);    
    void Left(int Audioindex, float volume, float speedVal, float currPos);
    void Right(int Audioindex, float volume, float speedVal, float currPos);
    void Up(float speedVal, float currPos);
    void Down(float speedVal, float currPos);
    void IncreaseScoreAndLives(int Val);
    void ResetScore();    
    void ResetPosition(int posx, int posy);
    void CheckWallCol();
    //getters
    float GetStarPosX();
    float GetStarPosY();
    int GetLastScore();
    bool GetInvncible();
    int GetScore();
    int GetLives();
};

