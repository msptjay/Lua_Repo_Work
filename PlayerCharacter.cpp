/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	PlayerCharacter.cpp
Date:		March 2025
Last Updated:	15/03/2025

Description: source file for PlayerCharacter class.
Derives from Creature base class
*/
#include "PlayerCharacter.h"
#include "Audio.h"
PlayerCharacter::PlayerCharacter(Level* lev, int speed, Game* game, Audio* GA)
	: Creature(lev, speed,game)
{   
    //constants******************************
    MaxLives = LuaGetInt(L, "MaxLives");
    InitialLives = LuaGetInt(L, "InitialLives");
    ScoreMultiplier = LuaGetInt(L, "ScoreMultiplier");
    ScoreMultForBonus = LuaGetInt(L, "ScoreMultipleToGetBonus");
    //***************************************

    m_Lives = InitialLives;
    p_GameAudio = GA;	
	m_Score = 0;
	m_isInvincible = false;
	m_LastScore = 0;
}

PlayerCharacter::~PlayerCharacter()
{
    
}

int PlayerCharacter::GetLives()
{
	return m_Lives;
}

void PlayerCharacter::DecreaseLives()
{
	m_Lives -= 1;
}

void PlayerCharacter::IncreaseLives()
{
	m_Lives++;
}

void PlayerCharacter::ResetLives()
{
	m_Lives = InitialLives;
}

int PlayerCharacter::GetScore()
{
	return m_Score;
}

void PlayerCharacter::SetInvincible(bool invincible)
{
	m_isInvincible = invincible;
}

bool PlayerCharacter::GetInvncible()
{
	return m_isInvincible;
}

void PlayerCharacter::Left(int Audioindex, float volume, float speedVal, float currPos)
{
     if (!p_CurrentLevel->IsWall(PositionX - speedVal + currPos, PositionY) &&
         !p_CurrentLevel->IsWall(PositionX - speedVal + currPos, PositionY + (GetSpriteHeight() - 1)) || m_isInvincible)
     {         
         PositionX -= speedVal;        
     }
     else
     {         
         if (/*!IsDead && */!m_isInvincible)
         {
             DeathPosX = PositionX;
             DeathPosY = PositionY;
             p_GameAudio->PlaySfx(1, 0.5);//play death sound 
             DecreaseLives();//lose a life
             ResetPosition(0, StartPosY);//set back to start
             SetIsDead(true);            
         }        
     }
    
}

void PlayerCharacter::Right(int Audioindex, float volume, float speedVal, float currPos)
{
    if (!p_CurrentLevel->IsWall(PositionX + (GetSpriteWidth() - 1) + speedVal + currPos, PositionY) &&
        !p_CurrentLevel->IsWall(PositionX + (GetSpriteWidth() - 1) + speedVal + currPos, PositionY + (GetSpriteHeight() - 1)) || m_isInvincible)
    {
        PositionX += speedVal;       
    }
    else
    {
        if (/*!IsDead && */!m_isInvincible)
        {
            DeathPosX = PositionX;
            DeathPosY = PositionY;
            p_GameAudio->PlaySfx(1, 0.5);//play death sound 
            DecreaseLives();//lose a life
            ResetPosition(0, StartPosY);//set back to start
            SetIsDead(true);            
        }        
    }
}

void PlayerCharacter::Up(float speedVal, float currPos)
{
    if (!p_CurrentLevel->IsWall(PositionX + currPos, PositionY - speedVal) &&
        !p_CurrentLevel->IsWall(PositionX + (GetSpriteWidth() - 1) + currPos, PositionY - speedVal) || m_isInvincible)
    {
        if (PositionY > 0)
            PositionY -= speedVal;
    }
    else
    {
        if (!IsDead && !m_isInvincible)
        {
            DeathPosX = PositionX;
            DeathPosY = PositionY;
            p_GameAudio->PlaySfx(1, 0.5);//play death sound 
            DecreaseLives();//lose a life
            ResetPosition(0, StartPosY);//set back to start
            SetIsDead(true);            
        }
    }
}

void PlayerCharacter::Down(float speedVal, float currPos)
{
    if (!p_CurrentLevel->IsWall(PositionX + currPos, PositionY + (GetSpriteHeight() - 1) + speedVal) &&
        !p_CurrentLevel->IsWall(PositionX + (GetSpriteWidth() - 1) + currPos, PositionY + (GetSpriteHeight() - 1) + speedVal) || m_isInvincible)
    {
        if (PositionY + (GetSpriteHeight() - 1) + speedVal <= 1080)
            PositionY += speedVal;
    }
    else
    {
        if (!IsDead && !m_isInvincible)
        {
            DeathPosX = PositionX;
            DeathPosY = PositionY;
            p_GameAudio->PlaySfx(1, 0.5);;//play death sound 
            DecreaseLives();//lose a life
            ResetPosition(0, StartPosY);//set back to start
            SetIsDead(true);            
        }
    }
}

void PlayerCharacter::IncreaseScoreAndLives(int Val)
{
    //******************Part 2*******************************
    //comment this entire code block to use the lua function*
    //*******************************************************
    m_Score += Val;
    if (m_Score % ScoreMultForBonus == 0)
    {
        if (m_Lives < MaxLives)
        {
            IncreaseLives();
        }
        else
        {
            m_Score += Val * ScoreMultiplier;
        }
    }
    //********************************************************

    //Uncomment this to use the lua function
    //CallLuaIncreaseScoreAndLives(L, "IncreaseScoreAndLives", m_Lives, m_Score);

}

void PlayerCharacter::ResetScore()
{
    //*******************Part 2******************************
    //comment this entire code block to use the lua function*
    //*******************************************************
    m_LastScore = m_Score;
    m_Score = 0;
    //********************************************************

    //Uncomment this to use the lua function
    //CallLuaResetScore(L, "ResetScore", m_LastScore, m_Score);
}

int PlayerCharacter::GetLastScore()
{
	return m_LastScore;
}

void PlayerCharacter::ResetPosition(int posx, int posy)
{
    PositionX = posx;
    PositionY = posy;
}
//this function seems to fix the wall collision issue
void PlayerCharacter::CheckWallCol()
{
    if (p_CurrentLevel->IsWall(PositionX+(GetSpriteWidth() - 1), PositionY)||
        p_CurrentLevel->IsWall(PositionX + (GetSpriteWidth() - 1), PositionY-(GetSpriteHeight()-1)))
    {
        if (!IsDead && !m_isInvincible)
        {
            DeathPosX = PositionX;
            DeathPosY = PositionY;
            p_GameAudio->PlaySfx(1, 0.5);;//play death sound 
            DecreaseLives();//lose a life
            ResetPosition(0, StartPosY);//set back to start
            SetIsDead(true);
        }
    }
}

float PlayerCharacter::GetStarPosX()
{
	return StartPosX;
}

float PlayerCharacter::GetStarPosY()
{
	return StartPosY;
}
