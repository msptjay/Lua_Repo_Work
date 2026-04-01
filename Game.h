/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Game.h
Date:		March 2025
Last Updated:	12/03/2025

Description: Header file for Game Manager. All declarations go here
*/

#pragma once

//includes go here
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "LuaHelper.h"

using namespace std;

class SceneManager;//forward declaration to let the compiler know that Scene Manager class exists
class Input;//forward declaration to let the compiler know that input class exists
class Game
{
private:
	

	//constants for lua*************************************
	int frames_per_second;
	//*********************************************
	
	//dispatcher for PART 3 - calling C++ methods in the script 
	Dispatcher disp;


	//methods	
	void Run();//the method the whole game runs from

	//pointers
	lua_State* L;
	ALLEGRO_TIMER* p_Timer;
	Input* p_Input_manager = NULL;
	ALLEGRO_DISPLAY* p_Display = NULL;
	SceneManager* p_SceneManager = NULL;
	
	//members
	double m_delta;
	double m_current_time;
	double m_last_time;
public:
	//constructor and destructor
	Game(int width, int height, int scrPositionX, int scrPositionY);
	~Game();
	
	//Init method for PART 3 - calling C++ methods in the script 
	void Init(Dispatcher& disp)
	{
		Dispatcher::Command::voidvoidfunc f{ [this](void){return Run(); } };
		disp.Register("Run", Dispatcher::Command{ f });
	}

	//methods	
	int randomNumber();//random number generator

	//getters
	ALLEGRO_DISPLAY* GetDisplay();
	float GetDelta();
};