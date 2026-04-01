/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Input.h
Date:		March 2025
Last Updated:	12/03/2025

Description: Header file for Input class
*/

#pragma once

//includes go here
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include"Game.h"

//Globals
enum KEYS_PRESSED_LIST
{
	KEY_ESCAPE, KEY_A, KEY_D, KEY_W, KEY_S, KEY_M, KEY_P, KEY_SPACE, KEY_ENTER, KEY_R, SIZE_OF_KEYS_PRESSED_ENUM
};

class Input 
{
private:
	//constants NOT SET IN LUA*************************************  
	const int max_consecutive_logic_updates = 10;
	
	//pointers
	ALLEGRO_EVENT_QUEUE* p_event = NULL;
	Game* p_game;

	//members	
	int m_mouseX, m_mouseY;	
	int m_consecutive_logic_updates = 0;	

	bool m_update_logic = true;
	bool m_render_frame = true;
	bool m_keysPressed[SIZE_OF_KEYS_PRESSED_ENUM];
	bool m_leftButton;

public:
	//constructor and destructor
	Input(Game* game);
	~Input();

	//methods
	void update(void);
	bool key_is_pressed(KEYS_PRESSED_LIST key);	
	void set_key_to_false(KEYS_PRESSED_LIST key);
	void set_key_to_true(KEYS_PRESSED_LIST key);	
	void HideCursor();
	void ShowCursor();
	
	//getters
	ALLEGRO_EVENT_QUEUE* Get_event();
	int GetMouseX();
	int GetMouseY();
	bool GetUpdateLogic();
	bool GetRenderFrame();
	bool GetLeftButton();
};