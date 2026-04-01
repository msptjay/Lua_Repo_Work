/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	main.cpp
Date:		March 2025
Last Updated:	14/03/205

Description:main entry point for program
*/

#include "Game.h"
#include "LuaHelper.h"

int main()
{	
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
		assert(false);
		
	//constants*************************************
	const int width = LuaGetInt(L, "width");
	const int height = LuaGetInt(L, "height");
	const int ScrPosX = LuaGetInt(L, "screenX");
	const int scrPosY = LuaGetInt(L, "screenY");
	//*********************************************
	lua_close(L);
	Game* Game_manager = new Game(width, height, ScrPosX, scrPosY);
	delete Game_manager;
	Game_manager = nullptr;
	return 0;
}
