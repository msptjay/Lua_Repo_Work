/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Input.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: Source file for Input class
*/

#include "Input.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>

using namespace std;
//Constructor
Input::Input(Game* game)
{
	m_leftButton = false;
	m_mouseX = 0;
	m_mouseY = 0;
	p_game = game;
	//initialise keypress array
	for (int i = 0; i < SIZE_OF_KEYS_PRESSED_ENUM; i++)
	{
		m_keysPressed[i] = false;
	}

	// need to create an event queue so allegro can respond to events, such as keypresses
	p_event = al_create_event_queue();
	
	if (!p_event)
	{
		fprintf(stderr, "Event queue creation failed");
		al_destroy_event_queue(p_event);
		return;
	}
	// register keyboard with the event queue, so keypress events can be processed
	al_register_event_source(p_event, al_get_keyboard_event_source());

	al_install_mouse();
	al_register_event_source(p_event, al_get_mouse_event_source());
	al_set_system_mouse_cursor(game->GetDisplay(), ALLEGRO_SYSTEM_MOUSE_CURSOR_PRECISION);
}

Input::~Input()
{
	al_destroy_event_queue(p_event);	
}

//Methods
void Input::update()
{
//Body of method
	ALLEGRO_EVENT allegro_event;
	
	// get the next event from the event queue
	bool got_event = al_get_next_event(p_event, &allegro_event);

	if (got_event == false) return; // no event waiting
	else if (got_event && allegro_event.type == ALLEGRO_EVENT_TIMER)
	{
		// if logic taking too long ensures frame is rendered occasionally and input is responsive
		// this essentially ignores logic updates if there are a lot of consecutive ones

		if (++m_consecutive_logic_updates < max_consecutive_logic_updates)
		{
			m_update_logic = true;
		}

		// only render frame if no events left to process
		if (al_is_event_queue_empty(p_event))
		{
			m_render_frame = true;
			m_consecutive_logic_updates = 0;
		}

	}
	// test if a key is pressed down, update global keypress array
	if (allegro_event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (allegro_event.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
			m_keysPressed[KEY_ESCAPE] = true;
			break;
		case ALLEGRO_KEY_A:
			m_keysPressed[KEY_A] = true;
			break;
		case ALLEGRO_KEY_D:
			m_keysPressed[KEY_D] = true;
			break;
		case ALLEGRO_KEY_W:
			m_keysPressed[KEY_W] = true;
			break;
		case ALLEGRO_KEY_S:
			m_keysPressed[KEY_S] = true;
			break;
		case ALLEGRO_KEY_M:
			m_keysPressed[KEY_M] = true;
			break;
		case ALLEGRO_KEY_P:
			m_keysPressed[KEY_P] = true;
			break;
		case ALLEGRO_KEY_SPACE:
			m_keysPressed[KEY_SPACE] = true;
			break;
		case ALLEGRO_KEY_ENTER:
			m_keysPressed[KEY_ENTER] = true;
			break;
		case ALLEGRO_KEY_R:
			m_keysPressed[KEY_R] = true;
			break;
		default:
			break;
		}
	}
	// Test if a key is let go, update global keypress array
	else if (allegro_event.type == ALLEGRO_EVENT_KEY_UP)
	{
		switch (allegro_event.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
			m_keysPressed[KEY_ESCAPE] = false;
			break;
		case ALLEGRO_KEY_A:
			m_keysPressed[KEY_A] = false;
			break;
		case ALLEGRO_KEY_D:
			m_keysPressed[KEY_D] = false;
			break;
		case ALLEGRO_KEY_W:
			m_keysPressed[KEY_W] = false;
			break;
		case ALLEGRO_KEY_S:
			m_keysPressed[KEY_S] = false;
			break;
		case ALLEGRO_KEY_M:
			m_keysPressed[KEY_M] = false;
			break;
		case ALLEGRO_KEY_P:
			m_keysPressed[KEY_P] = false;
			break;
		case ALLEGRO_KEY_SPACE:
			m_keysPressed[KEY_SPACE] = false;
			break;
		case ALLEGRO_KEY_ENTER:
			m_keysPressed[KEY_ENTER] = false;
			break;
		case ALLEGRO_KEY_R:
			m_keysPressed[KEY_R] = false;
			break;
		default:
			break;
		}
	}

	if (allegro_event.type == ALLEGRO_EVENT_MOUSE_AXES)
	{		
		m_mouseX = allegro_event.mouse.x;
		//cout << mouseX << endl;
		m_mouseY = allegro_event.mouse.y;
		//cout << mouseY << endl;
	}
	ALLEGRO_MOUSE_STATE msestate;
	if (allegro_event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{		
 		al_get_mouse_state(&msestate); 
		if (al_mouse_button_down(&msestate, 1))
		{
			m_leftButton = true;
			//HideCursor();
			//cout << m_leftButton << endl;
		}
	}
	if (allegro_event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		al_get_mouse_state(&msestate);
		if (RI_MOUSE_BUTTON_1_UP)
		{
			m_leftButton = false;
			//ShowCursor();
			//cout << m_leftButton << endl;
		}
	}	
}

bool Input::key_is_pressed(KEYS_PRESSED_LIST key)
{
	if (!KEY_ESCAPE)
	{
		return m_keysPressed[key];
	}
	else
		return false;
}

void Input::set_key_to_false(KEYS_PRESSED_LIST key)
{
	if (m_keysPressed[key] == true)
	{
		m_keysPressed[key] = false;
	}
}

void Input::set_key_to_true(KEYS_PRESSED_LIST key)
{
	m_keysPressed[key] = true;
}
ALLEGRO_EVENT_QUEUE* Input::Get_event()
{
	return p_event;
}

bool Input::GetLeftButton()
{	
	return m_leftButton;
}

void Input::HideCursor()
{
	al_hide_mouse_cursor(p_game->GetDisplay());
}

void Input::ShowCursor()
{
	al_show_mouse_cursor(p_game->GetDisplay());
}

int Input::GetMouseX()
{
	return m_mouseX;
}

int Input::GetMouseY()
{
	return m_mouseY ;
}

bool Input::GetUpdateLogic()
{
	return m_update_logic;
}

bool Input::GetRenderFrame()
{
	return m_render_frame;
}


