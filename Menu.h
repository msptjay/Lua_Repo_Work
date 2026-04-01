/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Menu.h
Date:		March 2025
Last Updated:	09/03/2025

Description: header file for The Menu class. 
*/
#pragma once
#include <allegro5/allegro.h>
#include "SceneManager.h"
#include "Input.h"
#include "LuaHelper.h"

class Menu
{
private:
	//pointers
	lua_State* L;	
	Audio* p_Audio = NULL;


	//constants for Lua*************************************
	ALLEGRO_BITMAP* p_ButtonImage = NULL;
	int m_ScreenWidth, m_ScreenHeight;
	int m_Button1PosX, m_Button2PosX;
	int m_Button1PosY, m_Button2PosY;
	RGBColour buttonTint;//see helper file
	//*********************************************
	

	//members
	bool m_highlightPlay = false;
	bool m_highlightQuit = false;
public:
	//constructor and destructor
	Menu(ALLEGRO_DISPLAY* disp, Audio* GA);
	~Menu();

	//methods
	E_GameStates Update(Input* input);
	void Draw(Input* input);	

	//getters
	int GetPlayButtonPosX();
	int GetPlayButtonPosY();
	int GetExitButtonPosX();
	int GetExitButtonPosY();
	bool GetHighlightPlay();
	bool GetHighlightQuit();
};

