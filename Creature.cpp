/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Creature.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: source file for Creature class.
This is the base clas for the player and enemies
*/
#include "Creature.h"
#include "Audio.h"

Creature::Creature(Level* lev, int inputSpeed,Game* game)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
        assert(false);

    //constants**********************************
    AnimFrames = LuaGetInt(L, "maxDeathAnimationFrames");
    FrameTimeLimit = LuaGetDouble(L, "timeToPlayFrames");
    //*******************************************

    PositionX = 0;
    PositionY = 0;
    StartPosX = 0;
    StartPosY = 0;
    Speed = 0;
    exploding = false;
    IsUp = true;        
    Speed = inputSpeed;    
    animFrameIndex = 0;
    FrameTimer = 0.0f;       
    IsDead = true;
    IsEnemy = false;
    DeathPosX = 0;
    DeathPosY = 0;

    p_Game = game;
    p_CurrentLevel = lev;
    
}

Creature::~Creature()
{
    if (enemySprites.size()>0)
    {
        for (size_t it = 0; it < enemySprites.size(); it++)
        {
            al_destroy_bitmap(enemySprites[it]);
        }
        enemySprites.clear();
    }
    if (Sprite)
    {
        al_destroy_bitmap(Sprite);
    }
    for (size_t it = 0; it < ExplosionSprite.size(); it++)
    {
        al_destroy_bitmap(ExplosionSprite[it]);
    }
    ExplosionSprite.clear();
    if (L)
    {
        lua_settop(L, 0);
        lua_close(L);
    }
}

void Creature::LoadContent(bool enemy)
{
    IsEnemy = enemy;    
    if (enemy)
    {        
        enemySprites.push_back(al_load_bitmap(StringFromLuaTable(L, "enemySprites", "a").c_str()));
        enemySprites.push_back(al_load_bitmap(StringFromLuaTable(L, "enemySprites", "b").c_str()));
        enemySprites.push_back(al_load_bitmap(StringFromLuaTable(L, "enemySprites", "c").c_str()));
        enemySprites.push_back(al_load_bitmap(StringFromLuaTable(L, "enemySprites", "d").c_str()));
        enemySprites.push_back(al_load_bitmap(StringFromLuaTable(L, "enemySprites", "e").c_str()));
        enemySprites.push_back(al_load_bitmap(StringFromLuaTable(L, "enemySprites", "f").c_str()));
    }
    else
    {
        Sprite = al_load_bitmap(LuaGetString(L, "PlayerSprite").c_str());
    }
    ExplosionSprite.push_back(al_load_bitmap(StringFromLuaTable(L, "explosionFrames", "a").c_str()));
    ExplosionSprite.push_back(al_load_bitmap(StringFromLuaTable(L, "explosionFrames", "b").c_str()));
    ExplosionSprite.push_back(al_load_bitmap(StringFromLuaTable(L, "explosionFrames", "c").c_str()));
    ExplosionSprite.push_back(al_load_bitmap(StringFromLuaTable(L, "explosionFrames", "d").c_str()));
    ExplosionSprite.push_back(al_load_bitmap(StringFromLuaTable(L, "explosionFrames", "e").c_str()));
    ExplosionSprite.push_back(al_load_bitmap(StringFromLuaTable(L, "explosionFrames", "f").c_str()));
    ExplosionSprite.push_back(al_load_bitmap(StringFromLuaTable(L, "explosionFrames", "g").c_str()));
    ExplosionSprite.push_back(al_load_bitmap(StringFromLuaTable(L, "explosionFrames", "h").c_str()));
    ExplosionSprite.push_back(al_load_bitmap(StringFromLuaTable(L, "explosionFrames", "i").c_str()));
}

void Creature::Draw()
{
    if (IsEnemy && !IsDead)
    {        
        if (IsEnemy && exploding)
        {            
            al_draw_bitmap(ExplosionSprite[animFrameIndex], DeathPosX, DeathPosY, 0);
        }
        else
            al_draw_bitmap(enemySprites[p_CurrentLevel->GetLevelNum() - 1], PositionX, PositionY, 0);
    }
    else
        if (!IsEnemy && !IsDead)
        {
            al_draw_bitmap(Sprite, PositionX, PositionY, 0);
        }
        else
            al_draw_bitmap(ExplosionSprite[animFrameIndex], DeathPosX, DeathPosY, 0);
}

void Creature::Left(int Audioindex, float volume, float speedVal, float currPos)
{    
    if (!p_CurrentLevel->IsWall(PositionX - speedVal, PositionY) &&
        !p_CurrentLevel->IsWall(PositionX - speedVal, PositionY + (GetSpriteHeight()-1)))
    {        
        PositionX -= speedVal;        
    }   
}

void Creature::Right(int Audioindex, float volume, float speedVal, float currPos)
{
    if (!p_CurrentLevel->IsWall(PositionX + (GetSpriteWidth()-1) + speedVal, PositionY) &&
        !p_CurrentLevel->IsWall(PositionX + (GetSpriteWidth()-1) + speedVal, PositionY + (GetSpriteHeight()-1)))
    {        
        PositionX += speedVal;        
    }
}

void Creature::Up(float speedVal, float& currPos)
{
    if (!p_CurrentLevel->IsWall(PositionX + currPos, PositionY - speedVal) &&
        !p_CurrentLevel->IsWall(PositionX + (GetSpriteWidth()-1) + currPos, PositionY - speedVal))
    {
        //**********************Part 2***************************
        //comment this if statement to use the lua function******
        //*******************************************************
        if (PositionY > 0)
        PositionY -= speedVal;
        //*******************************************************
        
        //Uncomment this to use the lua function
        //CallLuaMoveUp(L, "MoveUp", speedVal, PositionY);
    }
    else
    {
        IsUp = false;
    }
}

void Creature::Down(float speedVal, float currPos)
{
    if (!p_CurrentLevel->IsWall(PositionX + currPos, PositionY + (GetSpriteHeight()-1) + speedVal) &&
        !p_CurrentLevel->IsWall(PositionX + (GetSpriteWidth()-1) + currPos, PositionY + (GetSpriteHeight()-1) + speedVal))
    {
        int height = GetSpriteHeight() - 1;//don't comment this as you'll need it for the lua function

        //*******************Part 2******************************
        //comment this if statement to use the lua function******
        //*******************************************************
        if (PositionY + height + speedVal <= 1080)
        PositionY += speedVal;
        //*******************************************************

        //Uncomment this to use the lua function
        //CallLuaMoveDown(L, "MoveDown", height, speedVal, PositionY);
    }
    else
    {
        IsUp = true;
    }
}

float Creature::GetPositionX()
{
	return PositionX;
}

float Creature::GetPositionY()
{
	return PositionY;
}

int Creature::GetSpriteWidth()
{
    if (IsEnemy)
    {
        return al_get_bitmap_width(enemySprites[p_CurrentLevel->GetLevelNum() - 1]);
    }
    else
        return al_get_bitmap_width(Sprite);
}

int Creature::GetSpriteHeight()
{
    if (IsEnemy)
    {
        return al_get_bitmap_height(enemySprites[p_CurrentLevel->GetLevelNum() - 1]);
    }
    else
        return al_get_bitmap_height(Sprite);
}

bool Creature::CollidesWith(Creature* creature)
{
    if (PositionX <= creature->GetPositionX() + creature->GetSpriteWidth() - 1 &&
        PositionX + GetSpriteWidth() - 1 >= creature->GetPositionX() &&
        PositionY <= creature->GetPositionY() + creature->GetSpriteHeight() - 1 &&
        PositionY + GetSpriteHeight() - 1 >= creature->GetPositionY())
    {
        return true;
    }
    return false;
}

void Creature::SetFrame(double delta)
{
    FrameTimer += delta;
    if (FrameTimer >= FrameTimeLimit)
    {
        animFrameIndex++;
        FrameTimer = 0.0f;
        if (animFrameIndex == AnimFrames)
        {
            animFrameIndex = 0;
        }
    }
}

int Creature::GetSpeed()
{
	return Speed;
}

bool Creature::GetIsDead()
{
	return IsDead;
}

void Creature::SetIsDead(bool dead)
{
    IsDead = dead;
}

void Creature::SetDeathPos()
{
    DeathPosX = PositionX;
    DeathPosY = PositionY;
}

void Creature::Activate(int posX, int posY)
{
    IsDead = false;
    StartPosX = posX;
    StartPosY = posY;
    PositionX = StartPosX;
    PositionY = StartPosY;
}

void Creature::SetExploding()
{
    exploding = true;
}
