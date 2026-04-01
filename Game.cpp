/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Game.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: Source file for The Game manager. This is where allegro is initiallised and the game loop is run
*/
#include <conio.h>
#include <ctime>

#include "Game.h"
#include "SceneManager.h"
#include "Input.h"

//Constructor
Game::Game(int width, int height, int scrPositionX, int scrPositionY)
{
	L = luaL_newstate();
	luaL_openlibs(L);
	if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
		assert(false);

	//constants*********************************************
	frames_per_second = LuaGetInt(L, "FPS");
	//******************************************************

	m_current_time = 0;
	m_delta = 0;
	m_last_time = 0;
	p_Timer = NULL;

	srand(time(NULL));//Sets the random seed for the whole game	
	if (!al_init())
	{
		fprintf(stderr, "Allegro initialisation failed");		
		return;
	}
	p_Timer = al_create_timer(1.0 / frames_per_second);
	if (!p_Timer) {
		fprintf(stderr, "failed to create timer!\n");
		return;
	}
	// tell allegro program needs keyboard input
	if (!al_install_keyboard())
	{
		fprintf(stderr, "Keyboard initialisation failed");		
		return;
	}

	p_Display = al_create_display(width, height); // create a display window
	if (!p_Display)
	{
		fprintf(stderr, "Display initialisation failed");
		al_destroy_timer(p_Timer);
		al_destroy_display(p_Display);
		return;
	}

	al_set_window_position(p_Display, scrPositionX, scrPositionY); // position display window on screen


	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();  // initialize the True Type Font addon	
	al_init_image_addon(); // initialise allegro image loading 
	
	p_Input_manager = new Input(this);
	p_SceneManager = new SceneManager(this, m_delta);
	// register  display with the event queue, so display change events can be processed
	al_register_event_source(p_Input_manager->Get_event(), al_get_display_event_source(p_Display));
	al_register_event_source(p_Input_manager->Get_event(), al_get_timer_event_source(p_Timer));
	al_start_timer(p_Timer);
	m_last_time = al_get_time();

	//*************************************************************
	//*********************PART 3 code*****************************
	//*************************************************************
	disp.Init(L);
	Init(disp);

	//*************************************************************
	//uncomment this to call Run() in the lua script file**********
	//*************************************************************
	CallVoidVoidFunc(L, "RunGame");
	//*************************************************************
	
	//*************************************************************
	//comment this to use the script to call Run()*****************
	//*************************************************************
	//Run();
	//*************************************************************
}
 
Game::~Game()
{
	al_destroy_timer(p_Timer);
	al_destroy_display(p_Display);	
	delete p_SceneManager;
	p_SceneManager = nullptr;
	delete p_Input_manager;
	p_Input_manager = nullptr;
	if (L)
	{
		lua_settop(L, 0);
		lua_close(L);
	}
}




void Game::Run()
{
	while (p_SceneManager->GetState() != QUIT)
	{			
 		if (p_Input_manager->GetUpdateLogic())
		{		
			p_Input_manager->update();
			m_current_time = al_get_time();
			m_delta = m_current_time - m_last_time;
			m_last_time = m_current_time;
			p_SceneManager->Update(p_Input_manager, this);
		}
		if (p_Input_manager->GetRenderFrame())
		{
			al_clear_to_color(al_map_rgb(255, 0, 0));
			p_SceneManager->Draw(p_Input_manager, this);
			al_flip_display();
		}
	}
	return;
}

int Game::randomNumber()//random number generator
{
	//Gives the remainder of a division of the random seed by the maximum range  
	//(this will always give an answer between 0 and Max-1)
	//Then adds one, to return a value in the range from 1 to Max (instead of 0 to Max-1)
	return (rand() % 10000) + 1;
}

ALLEGRO_DISPLAY* Game::GetDisplay()
{
	return p_Display;
}

float Game::GetDelta()
{	
	return m_delta;
}

