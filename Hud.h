/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Hud.h
Date:		March 2025
Last Updated:	09/03/2025

Description: Header file for The Hud class.
*/
#pragma once
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include "LuaHelper.h"

using namespace std;

class Hud
{
private:
    //constants for lua*************************************   
    int SmallTextSize;
    int MediumTextSize;
    int LargeTextSize;
    string FontNameSmall;
    string FontNameMedium;
    string FontNameLarge;
    string livesIconName;
    //*********************************************
    
    //pointers
    lua_State* L;
    ALLEGRO_FONT* p_Large_Text = NULL;//Large size font
    ALLEGRO_FONT* p_Medium_Text = NULL;//Medium size font
    ALLEGRO_FONT* p_Small_Text = NULL;//Smaller size font
    ALLEGRO_BITMAP* p_LivesIcon = NULL;
    const char* m_HudMessage = NULL;//this constant is used for creating various different messages

public:
    //constructor and destructor
    Hud();
    ~Hud();

    void LoadContent();   
    void SetMessage(const char* message);
    void DrawString(ALLEGRO_FONT* text,  int r, int g, int b, int posX, int posY);
    void DrawLives(int posX, int posY, int PlayerLives);

    //getters
    ALLEGRO_FONT* GetLargeText();//get method
    ALLEGRO_FONT* GetMediumText();//get method
    ALLEGRO_FONT* GetSmallText();//get method
};

