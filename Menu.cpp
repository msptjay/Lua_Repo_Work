/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Menu.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: Source file for The Menu class. 
*/
#include "Menu.h"
#include <iostream>
#include <conio.h>
#include <string>
#include"Audio.h"
using namespace std;

Menu::Menu(ALLEGRO_DISPLAY* disp, Audio* GA)
{		
	L = luaL_newstate();
	luaL_openlibs(L);
	if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
		assert(false);

	//the width and height should already be defined in the script under init variables***
	m_ScreenWidth = LuaGetInt(L, "width");
	m_ScreenHeight = LuaGetInt(L, "height");

	//constants**********************************************************************
	m_Button1PosY = LuaGetInt(L, "button1Ypos");
	m_Button2PosY = LuaGetInt(L, "button2Ypos");
	p_ButtonImage = al_load_bitmap(LuaGetString(L, "buttonName").c_str());
	if (!p_ButtonImage)
	{
		fprintf(stderr, "button.png bitmap creation failed");		
		return;
	}
	buttonTint.FromLua(L, "buttonTint");
	//********************************************************************************
	m_Button1PosX = (m_ScreenWidth / 2) - (al_get_bitmap_width(p_ButtonImage)/2);
	m_Button2PosX = (m_ScreenWidth / 2) - (al_get_bitmap_width(p_ButtonImage) / 2);
	m_highlightPlay = false;
	m_highlightQuit = false;
	p_Audio = GA;
}

Menu::~Menu()
{
	al_destroy_bitmap(p_ButtonImage);
	if (L)
	{
		lua_settop(L, 0);
		lua_close(L);
	}
}

E_GameStates Menu::Update(Input* input)
{	
	if (input->key_is_pressed(KEY_M))
	{
		if (!p_Audio->GetIsMuted())
		{
			p_Audio->MuteMusic();
		}		
	}
	if (input->key_is_pressed(KEY_ENTER))
	{
		if (p_Audio->GetIsMuted())
		{
			p_Audio->StopMusic();
			p_Audio->PlayMusic(2);
		}
	}
	if (input->GetMouseX() >= m_Button1PosX && input->GetMouseX() <= m_Button1PosX + al_get_bitmap_width(p_ButtonImage)
		&& input->GetMouseY() >= m_Button1PosY && input->GetMouseY() <= m_Button1PosY + al_get_bitmap_height(p_ButtonImage)
		&& input->GetLeftButton() == true)
	{
		input->HideCursor();
		if (!p_Audio->GetIsMuted())
		{
			p_Audio->StopMusic();
			p_Audio->PlayMusic(1);
		}
		p_Audio->PlaySfx(0, 1.0);
		return GAME;
	}
	if (input->GetMouseX() >= m_Button2PosX && input->GetMouseX() <= m_Button2PosX + al_get_bitmap_width(p_ButtonImage)
		&& input->GetMouseY() >= m_Button2PosY && input->GetMouseY() <= m_Button2PosY + al_get_bitmap_height(p_ButtonImage)
		&& input->GetLeftButton() == true)
	{
		p_Audio->PlaySfx(0, 1.0);
		return QUIT;
	}
	input->ShowCursor();
	return MENU;
}

void Menu::Draw(Input* input)
{	

	if (input->GetMouseX() >= m_Button1PosX && input->GetMouseX() <= m_Button1PosX + al_get_bitmap_width(p_ButtonImage)
		&& input->GetMouseY() >= m_Button1PosY && input->GetMouseY() <= m_Button1PosY + al_get_bitmap_height(p_ButtonImage))
	{
		m_highlightPlay = true;		
		al_draw_tinted_bitmap(p_ButtonImage,al_map_rgb(buttonTint.r, buttonTint.g, buttonTint.b), m_Button1PosX, m_Button1PosY, 0);		
	}
	else
	{
		m_highlightPlay = false;
		al_draw_bitmap(p_ButtonImage, m_Button1PosX, m_Button1PosY, 0);
	}
	if (input->GetMouseX() >= m_Button2PosX && input->GetMouseX() <= m_Button2PosX + al_get_bitmap_width(p_ButtonImage)
		&& input->GetMouseY() >= m_Button2PosY && input->GetMouseY() <= m_Button2PosY + al_get_bitmap_height(p_ButtonImage))
	{
		m_highlightQuit = true;
		al_draw_tinted_bitmap(p_ButtonImage, al_map_rgb(buttonTint.r, buttonTint.g, buttonTint.b), m_Button2PosX, m_Button2PosY, 0);
	}
	else
	{
		m_highlightQuit = false;
		al_draw_bitmap(p_ButtonImage, m_Button1PosX, m_Button2PosY, 0);
	}
}

int Menu::GetPlayButtonPosX()
{
	return m_Button1PosX;
}

int Menu::GetPlayButtonPosY()
{
	return m_Button1PosY;
}

int Menu::GetExitButtonPosX()
{
	return m_Button2PosX;
}

int Menu::GetExitButtonPosY()
{
	return m_Button2PosY;
}

bool Menu::GetHighlightPlay()
{
	return m_highlightPlay;
}

bool Menu::GetHighlightQuit()
{
	return m_highlightQuit;
}
