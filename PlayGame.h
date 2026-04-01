/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	PlayGame.h
Date:		March 2025
Last Updated:	12/03/2025

Description: Header file for Play state. 
*/
#pragma once
#include<vector>
#include"SceneManager.h"
#include "LuaHelper.h"

class PlayerCharacter;
class Enemy;
class Projectile;
class Level;
class PlayGame
{
private:
    //constants for lua*************************************
    float InvincibleLimit;//the time the player is invincible after losing a life       
    int maxLevels;//maximum number of levels
    int maxEnemies;//the maximum number of pooled enemies        
    double shotTimerLimit;//the minimum time between shots
    int maxPlayerBullets;//the maximum number of bullets to pool for the player
    int maxEnemyBullets;//the maximum number of bullets to pool for the enemies 
    float sceneMoveAmmount;
    int levelExit;

    RGBColour LivesTextCol;
    RGBColour LevelTextCol;
    RGBColour ScoreTextCol;
    RGBColour FPSTextCol;

    Vector2 LivesTextPos;
    Vector2 LivesIconPos;
    Vector2 LevelTextPos;
    Vector2 ScoreTextPos;
    Vector2 FPSTextPos;

    string playerBulletSprite;
    string enemyBulletSprite;
    //This 1 done in LoadContent method    
    ALLEGRO_BITMAP* p_background;
    //***********************************************

    //pointers
    lua_State* L;


    std::vector<Projectile*> p_PlayerBullets;
    std::vector<Projectile*> p_EnemyBullets;
    PlayerCharacter* p_Player;
    Level* p_GameLevel;        
    Audio* p_GameAudio;   
    std::vector<Enemy*> p_enemies;

   //members
   float m_CurrentPosX, m_CurrentPosY;
   double m_DeltaTime;   
   float m_InvincibleTimer;
   int m_HudLastScore;    
   double m_ShotTimer;    
   bool m_Shooting;
   bool m_CanShoot;     
   bool m_IsPaused;

  
   //***************************************************
   //make this function private for PART3*************** 
   //***************************************************
   void ResetAll(int levelNumber);
   //***************************************************
public:
    //constructor and destructor
    PlayGame(Game* game, Audio* GA, float Delta);
    ~PlayGame();

    //methods
    void LoadContent();
    E_GameStates Update(float Delta, Game* game, Input* input);
    void DrawScene(Game* game);
    void Draw(Hud* GameHud, Game* Game);

   
    //getters
    int GetScore();
    int GetLastScore();

    //Init method for PART 3 - calling C++ methods in the script 
    void Init(Dispatcher& disp)
    {
        Dispatcher::Command::voidintfunction f{ [this](int num) {return ResetAll(num); } };
        disp.Register("ResetAll", Dispatcher::Command{nullptr, f });
        
    }
    //dispatcher for PART 3 - calling C++ methods in the script 
    Dispatcher disp;
};

