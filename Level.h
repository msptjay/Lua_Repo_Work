/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Level.h
Date:		March 2025
Last Updated:	12/03/2025

Description: Header file for Level manager.
*/
#pragma once
#include<allegro5/allegro.h>
#include<vector>
#include<string>
#include "LuaHelper.h"

class Level
{
private:
    //pointers
    lua_State* L;

    //dispatcher for PART 3 - calling C++ methods in the script 
    Dispatcher disp;

    //constants for lua************************************    
    int LevelNum;
    RGBColour BGWallTint;
    RGBAColour WindowTint;
    //these ones are contained in the LoadContent method
    std::vector<ALLEGRO_BITMAP*> p_WallTex;
    ALLEGRO_BITMAP* p_WallTex2;
    ALLEGRO_BITMAP* p_StationTex1;
    ALLEGRO_BITMAP* p_StationTex2;
    ALLEGRO_BITMAP* p_StationTex3;
    ALLEGRO_BITMAP* p_StationTex4;
    ALLEGRO_BITMAP* p_StationTex5;
    //*****************************************************

    //members
    int m_WallDimensionsX, m_WallDimensionsY;
    std::vector<std::string> LevelContents;      
    float m_screeenWidth, m_screenHeight;
    //***************************************************
     //make this function private for PART3*************** 
     //***************************************************
    void BuildNewLevel();
    //***************************************************
public:
    //constructor and destructor
    Level(int scrWidth, int scrHeight);
    ~Level();

    //methods
    void LoadContent();
    void Draw(float currPosition);    
    void IncreaseLevel();
    
    void resetLevels(int levelNumber);
    bool IsWall(int x, int y);
    bool IsInSameRow(int y1, int y2);
    void DrawForeground(float currPosition);

    //getters
    int GetWallWidth();
    int GetWallHeight();
    bool GetLevelObject(int col, int row, char character);
    int GetLevelWidth();
    int GetLevelHeight();
    int GetLevelNum();
    std::vector<std::string> GetLevel();
    int SetBackToStart();

    


    //Init method for PART 3 - calling C++ methods in the script 
    void Init(Dispatcher& disp)
    {
        Dispatcher::Command::voidvoidfunc f{ [this](void) {return BuildNewLevel(); } };
        disp.Register("BuildNewLevel", Dispatcher::Command{f});
    }
};

