/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	GameOver.h
Date:		March 2025
Last Updated:	12/03/2025

Description: Header file for GameOver class
*/
#pragma once
#include "SceneManager.h"

class Hud;
class Input;
class Audio;
class GameOver
{
private:
    //pointers
    lua_State* L;
    Audio* p_GameAudio;

    //constants for Lua*************************************    
    double timeLimit;
    RGBColour GameOverTextColour;
    Vector2  GameOverMessagePosition;
    string GameOverMessage;
    //******************************************************   

    //members
    double m_totalTime;

    bool m_IsPlaying;

public:
    GameOver(Audio* GA);
    ~GameOver();

    E_GameStates Update(double DeltaTime, Input* input);
    void Draw(Hud* hud);
};

