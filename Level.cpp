/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Level.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: source file for Level manager.
*/
#include "Level.h"
#include <fstream>
#include <iostream>
#include <string>
Level::Level(int scrWidth, int scrHeight)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
        assert(false);

    //constants******************************************
    LevelNum = LuaGetInt(L, "startingLevel");
    BGWallTint.FromLua(L, "nonCollidableBackgroundWallTint");
    WindowTint.FromLua(L, "WindowTint");
    //***************************************************

    m_screeenWidth = scrWidth;
    m_screenHeight = scrHeight;	

    //*************************************************************
    //*********************PART 3 code*****************************
    //*************************************************************
    disp.Init(L);
    Init(disp);
    //*************************************************************
    
    
    //***********************PART 3****************************************
    //Uncomment this to call BuildNewLevel() in the script file
    //*********************************************************************
    //CallVoidVoidFunc(L, "BuildLevel");//BuildLevel is what the lua function is called
    //*********************************************************************

    //*********************************************************************
    //***********************PART 3****************************************
    //comment this to call BuildNewLevel() in the script file**************
    //*********************************************************************
    BuildNewLevel();
    //*********************************************************************
}

Level::~Level()
{
    for (size_t it = 0; it < p_WallTex.size(); it++)
    {
        al_destroy_bitmap(p_WallTex[it]);        
    }
    p_WallTex.clear();
    al_destroy_bitmap(p_StationTex1);
    al_destroy_bitmap(p_StationTex2);
    al_destroy_bitmap(p_StationTex3);
    al_destroy_bitmap(p_StationTex4);
    al_destroy_bitmap(p_StationTex5);
    al_destroy_bitmap(p_WallTex2);
    if (L)
    {
        lua_settop(L, 0);
        lua_close(L);
    }
}

void Level::LoadContent()
{   
    p_WallTex.push_back(al_load_bitmap(StringFromLuaTable(L, "LevelBasedWallTextures", "a").c_str()));
    p_WallTex.push_back(al_load_bitmap(StringFromLuaTable(L, "LevelBasedWallTextures", "b").c_str()));
    p_WallTex.push_back(al_load_bitmap(StringFromLuaTable(L, "LevelBasedWallTextures", "c").c_str()));
    p_WallTex.push_back(al_load_bitmap(StringFromLuaTable(L, "LevelBasedWallTextures", "d").c_str()));
    p_WallTex.push_back(al_load_bitmap(StringFromLuaTable(L, "LevelBasedWallTextures", "e").c_str()));
    p_WallTex.push_back(al_load_bitmap(StringFromLuaTable(L, "LevelBasedWallTextures", "f").c_str()));
    p_WallTex2 = al_load_bitmap(LuaGetString(L, "nonCollidableBackgroundWall").c_str());
    p_StationTex1 = al_load_bitmap(LuaGetString(L, "foregroundWindowImage").c_str());
    
    m_WallDimensionsX = al_get_bitmap_width(p_WallTex[0]);
    m_WallDimensionsY = al_get_bitmap_height(p_WallTex[0]);
}

void Level::Draw(float currPosition)
{
    for (int col = 0; col < GetLevelWidth(); col++)
    {
        for (int row = 0; row < GetLevelHeight(); row++)
        {
            if (LevelContents[row][col] == 'W' && col * m_WallDimensionsX > currPosition - m_WallDimensionsX && col * m_WallDimensionsX < currPosition + m_screeenWidth)
            {                
                al_draw_bitmap(p_WallTex[LevelNum - 1], al_get_bitmap_width(p_WallTex[LevelNum - 1]) * col - currPosition, al_get_bitmap_height(p_WallTex[LevelNum - 1]) * row, 0);
            }
            if (LevelContents[row][col] == 'w' && col * m_WallDimensionsX > currPosition - m_WallDimensionsX && col * m_WallDimensionsX < currPosition + m_screeenWidth)
            {
                al_draw_tinted_bitmap(p_WallTex2, al_map_rgb(BGWallTint.r, BGWallTint.g, BGWallTint.b), al_get_bitmap_width(p_WallTex2) * col - currPosition, al_get_bitmap_height(p_WallTex2) * row, 0);
            }           
        }
    }
}
void Level::DrawForeground(float currPosition)
{
    for (int col = 0; col < GetLevelWidth(); col++)
    {
        for (int row = 0; row < GetLevelHeight(); row++)
        {
            if (LevelContents[row][col] == 'M' && col * al_get_bitmap_width(p_StationTex1) > currPosition - m_WallDimensionsX && col * al_get_bitmap_width(p_StationTex1) < currPosition + m_screeenWidth )
            {
                al_draw_tinted_bitmap(p_StationTex1, al_map_rgba(WindowTint.r, WindowTint.g, WindowTint.b, WindowTint.a), al_get_bitmap_width(p_StationTex1) * col - currPosition, al_get_bitmap_height(p_StationTex1) * row, 0);
            }            
        }
    }
}
bool Level::GetLevelObject(int col, int row, char character)
{
    if (LevelContents[row][col] == character)
    {
        return true;
    }
    return false;
}

std::vector<std::string> Level::GetLevel()
{
	return LevelContents;
}

int Level::SetBackToStart()
{
    LevelNum = LuaGetInt(L, "startingLevel");
    return LevelNum;
}

int Level::GetLevelWidth()
{
	return LevelContents[0].size();
}

int Level::GetLevelHeight()
{
	return LevelContents.size();
}

void Level::IncreaseLevel()
{
    LevelNum++;
}

void Level::BuildNewLevel()
{
    LevelContents.clear();
    std::string filename = "assets/levels/level ";
    filename += std::to_string(LevelNum);
    filename += ".txt";
    
    std::ifstream levelfile(filename);
    if (!levelfile) //Always test the file open.
    {
        std::cout << "Error opening output file" << std::endl;
        system("pause");
        return;
    }

    std::string line;
    while (std::getline(levelfile, line))
    {
        LevelContents.push_back(line);
        std::cout << line << std::endl;
    }
    
}

void Level::resetLevels(int levelNumber)
{
    LevelNum = levelNumber;
    //*********************************************************************
    //***********************PART 3****************************************
    //Uncomment this to call BuildNewLevel() in the script file
    //*********************************************************************
    //CallVoidVoidFunc(L, "BuildLevel");
    //*********************************************************************
    
    //*********************************************************************
    //***********************PART 3****************************************
    //comment this to call BuildNewLevel() in the script file**************
    //*********************************************************************
    BuildNewLevel();
    //*********************************************************************
}

int Level::GetLevelNum()
{
	return LevelNum;
}

bool Level::IsWall(int x, int y)
{
    if (LevelContents[(int)(y / m_WallDimensionsY)][(int)(x / m_WallDimensionsX)] == 'W')
    {
        return true;
    }
    return false;
}

bool Level::IsInSameRow(int y1, int y2)
{
    int first = y1 / (int)m_WallDimensionsY;
    int second = y2 / (int)m_WallDimensionsY;
    if (first == second)
        return true;
    return false;
}



int Level::GetWallWidth()
{
    return al_get_bitmap_width(p_WallTex[0]);
}

int Level::GetWallHeight()
{
    return al_get_bitmap_height(p_WallTex[0]);
}


