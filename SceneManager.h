/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	SceneManager.h
Date:		March 2025
Last Updated:	12/03/2025

Description: Header file for The Scene Manager. 
*/
#pragma once
#include <allegro5/allegro.h>
#include"Input.h"
#include "Game.h"

//globals
enum E_GameStates
{
    MENU, GAME, GAME_OVER,QUIT
};

class Audio;
class GameOver;
class PlayGame;
class Menu;//forward declaration to let the compiler know that Menu class exists
class Hud;//forward declaration to let the compiler know that Hud class exists
class SceneManager
{
private:
    //constants for lua******************************************** 
    Vector2 HiScorePosition;
    RGBColour HiScoreTextColour;
    Vector2 LastScorePosition;
    RGBColour LastScoreTextColour;

    RGBColour ButtonHoverTextColour;
    RGBColour ButtonTextColour;

    RGBColour MenuClearColour;
    RGBColour GameOverClearColour;
    // ************************************************************
    
    //pointers
    lua_State* L;
    Menu* p_GameMenu;
    PlayGame* p_Play;
    GameOver* p_End;
    Audio* p_GameAudio;
    Hud* p_GameHud;

    //members
    E_GameStates e_scene;    
    int m_HiScore = 0;

public:
    //constructor and destructor
    SceneManager(Game* game, float Delta);
    ~SceneManager();    
    
    //methods
    void Update(Input* input, Game* game);
    void Draw(Input* input, Game* game);    
    void SwitchState(E_GameStates state);

    //getters
    E_GameStates GetState();
};