/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Projectile.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: source file for The Projectile class.
*/
#include "Projectile.h"
#include "Level.h"
#include "Creature.h"

Projectile::Projectile()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
        assert(false);

    //constants**************************************
    Speed = LuaGetFloat(L, "bulletSpeed");
    //***********************************************

	m_InUse = false;
	m_PositionX = 0;
	m_PositionY = 0;	
}

Projectile::~Projectile()
{
	al_destroy_bitmap(p_bullet);
    if (L)
    {
        lua_settop(L, 0);
        lua_close(L);
    }
}

void Projectile::LosdContent(const char* name)
{
	p_bullet = al_load_bitmap(name);
}

void Projectile::Update(Level* CurrentLevel, bool isEnemyFiring, float currPos)
{
    if (isEnemyFiring)//shoot left
    {
        if (!CurrentLevel->IsWall(m_PositionX - Speed + currPos, m_PositionY) &&
            !CurrentLevel->IsWall(m_PositionX - Speed + currPos, m_PositionY + (al_get_bitmap_height(p_bullet) - 1)))
        {
            m_PositionX -= Speed;
        }
        else
        {
            m_InUse = false;
        }
    }
    else if (!isEnemyFiring)//shoot right
    {
        if (!CurrentLevel->IsWall(m_PositionX + (al_get_bitmap_width(p_bullet) - 1) + Speed + currPos, m_PositionY) &&
            !CurrentLevel->IsWall(m_PositionX + (al_get_bitmap_width(p_bullet) - 1) + Speed + currPos, m_PositionY + (al_get_bitmap_height(p_bullet) - 1)))
        {
            m_PositionX += Speed;
        }
        else
        {
            m_InUse = false;
        }
    }
}

void Projectile::Draw()
{
    al_draw_bitmap(p_bullet, m_PositionX, m_PositionY, 0);
}

bool Projectile::CollidesWith(Creature* creature)
{
    if (m_PositionX <= creature->GetPositionX() + creature->GetSpriteWidth() - 1 &&
        m_PositionX + al_get_bitmap_width(p_bullet) - 1 >= creature->GetPositionX() &&
        m_PositionY <= creature->GetPositionY() + creature->GetSpriteHeight() - 1 &&
        m_PositionY + al_get_bitmap_height(p_bullet) - 1 >= creature->GetPositionY())
    {
        m_InUse = false;
        return true;
    }
    return false;
}

void Projectile::SetInUse(int StartPosX, int StartPosY)
{
    m_PositionX = StartPosX;
    m_PositionY = StartPosY;
    m_InUse = true;
}

bool Projectile::GetInUse()
{
	return m_InUse;
}

void Projectile::ResetBullet()
{
    m_InUse = false;
    m_PositionX = 0;
    m_PositionY = 0;
}

int Projectile::GetPositionX()
{
	return m_PositionX;
}

int Projectile::GetPositionY()
{
	return m_PositionY;
}
