/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Hud.cpp
Date:		March 2025
Last Updated:	09/03/2025

Description: Source file for The Hud class.
*/
#include "Hud.h"
#include <iostream>
#include <conio.h>


using namespace std;

Hud::Hud()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
        assert(false);

    //constants**********************************************************************
    SmallTextSize = LuaGetInt(L, "smallTextSize");
    MediumTextSize = LuaGetInt(L, "mediumTextSize");
    LargeTextSize = LuaGetInt(L, "largeTextSize");

    FontNameSmall = LuaGetString(L, "fontName1");
    FontNameMedium = LuaGetString(L, "fontName2");
    FontNameLarge = LuaGetString(L, "fontName3");

    livesIconName = LuaGetString(L, "LivesIcon");
    //*******************************************************************************


    p_Large_Text = al_load_ttf_font(FontNameLarge.c_str(), LargeTextSize, 0);
    if (!p_Large_Text)
    {
        fprintf(stderr, "Large text creation failed");       
        al_destroy_font(p_Large_Text);
        return;
    }

    p_Medium_Text = al_load_ttf_font(FontNameMedium.c_str(), MediumTextSize, 0);
    if (!p_Medium_Text)
    {
        fprintf(stderr, "Large text creation failed");
        al_destroy_font(p_Medium_Text);
        return;
    }

    p_Small_Text = al_load_ttf_font(FontNameSmall.c_str(), SmallTextSize, 0);
    if (!p_Small_Text)
    {
        fprintf(stderr, "Small text creation failed");
        al_destroy_font(p_Small_Text);
        return;
    }   
}

Hud::~Hud()
{
    if (L)
    {
        lua_settop(L, 0);
        lua_close(L);
    }
	al_destroy_font(p_Large_Text);
    al_destroy_font(p_Medium_Text);
	al_destroy_font(p_Small_Text);
}

void Hud::LoadContent()
{
    p_LivesIcon = al_load_bitmap(livesIconName.c_str());
}

ALLEGRO_FONT* Hud::GetLargeText()
{
	return  p_Large_Text;
}
ALLEGRO_FONT* Hud::GetMediumText()
{
    return  p_Medium_Text;
}

ALLEGRO_FONT* Hud::GetSmallText()
{
	return  p_Small_Text;
}

void Hud::SetMessage(const char* message)
{
    m_HudMessage = message;
}

void Hud::DrawString(ALLEGRO_FONT* text, int r, int g, int b, int posX, int posY)
{
    al_draw_textf(text, al_map_rgb(r, g, b), posX, posY, 0, m_HudMessage);
}

void Hud::DrawLives(int posX, int posY, int PlayerLives)
{
    for (int lives = 0; lives < PlayerLives; lives++)
    {
        al_draw_bitmap(p_LivesIcon,posX + al_get_text_width(p_Medium_Text,m_HudMessage) + (lives * al_get_bitmap_width(p_LivesIcon)) * 2,
            posY,0);
    }
}

