/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	GameOver.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: source file for GameOver class
*/
#include "GameOver.h"
#include "Hud.h"
#include"Input.h"
#include "Audio.h"

GameOver::GameOver(Audio* GA)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
        assert(false);

    //constants**********************************************************************
    timeLimit = LuaGetFloat(L, "GameOverTimeLimit");
    GameOverTextColour.FromLua(L, "GameOverTextColour");
    GameOverMessagePosition.FromLua(L, "GameOverMessagePosition");
    GameOverMessage = LuaGetString(L, "GameOverMessage");
    //*******************************************************************************

	p_GameAudio = GA;
	m_totalTime = 0.0f;
    m_IsPlaying = false;//init is playing to false 
}

GameOver::~GameOver()
{
    if (L)
    {
        lua_settop(L, 0);
        lua_close(L);
    }
}

E_GameStates GameOver::Update(double DeltaTime, Input* input)
{   
    if (input->key_is_pressed(KEY_M))
    {
        if (!p_GameAudio->GetIsMuted())
        {
            p_GameAudio->MuteMusic();;
        }
    }
    if (input->key_is_pressed(KEY_ENTER))
    {
        if (p_GameAudio->GetIsMuted())
        {
            p_GameAudio->PlayMusic(0);
        }
    }
    //The states transition back to the main menu after a set time
    m_totalTime += DeltaTime;//+ dt to total
    if (m_totalTime >= timeLimit)//if we reach the limit
    {
        if (!p_GameAudio->GetIsMuted())
        {
            p_GameAudio->StopMusic();
            p_GameAudio->PlayMusic(2);
        }
        m_totalTime = 0.0f;//reset time
        input->ShowCursor();
        return MENU;//enter the MENU state
    }
    return GAME_OVER;
}

void GameOver::Draw(Hud* hud)
{
    hud->SetMessage(GameOverMessage.c_str());
    hud->DrawString(hud->GetMediumText(), GameOverTextColour.r, GameOverTextColour.g, GameOverTextColour.b, GameOverMessagePosition.x, GameOverMessagePosition.y);
}
