/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	LuaHelper.cpp
Date:		March 2025
Last Updated:	14/03/2025

Description: Source file for the Lua helper file
*/

#include <assert.h>
#include <iostream>

#include "LuaHelper.h"

bool LuaOK(lua_State* L, int id)
{
	if (id != LUA_OK)
	{
		cout << lua_tostring(L, -1) << endl;
		return false;
	}
	return true;
}

int LuaGetInt(lua_State* L, const string& name)
{
	lua_getglobal(L, name.c_str());
	if (!lua_isinteger(L, -1))
		assert(false);
	int result = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);
	return result;
}

double LuaGetDouble(lua_State* L, const string& name)
{
	lua_getglobal(L, name.c_str());
	if (!lua_isnumber(L, -1))
		assert(false);
	double result = (double)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

double LuaGetFloat(lua_State* L, const string& name)
{
	lua_getglobal(L, name.c_str());
	if (!lua_isnumber(L, -1))
		assert(false);
	float result = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}
bool LuaGetBool(lua_State* L, const string& name)
{
	lua_getglobal(L, name.c_str());
	if (!lua_isboolean(L, -1))
		assert(false);
	bool result = (bool)lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}

string LuaGetString(lua_State* L, const string& name)
{
	lua_getglobal(L, name.c_str());
	if (!lua_isstring(L, -1))
		assert(false);
	string result = (string)lua_tostring(L, -1);
	lua_pop(L, 1);
	return result;
}

string StringFromLuaTable(lua_State* L, const string& name, const char* elementKey)
{
	lua_getglobal(L, name.c_str());
	if (!lua_istable(L, -1))
		assert(false);

	lua_pushstring(L, elementKey);
	lua_gettable(L, -2);
	string value = (string)lua_tostring(L, -1);//got the value
	lua_pop(L, 1);//remove it from the stack
	return value;
}

//*********************************************************************
//************************PART 2 Lua functions*************************
//*********************************************************************

//this one has been solved for you in Script.lua and PlayGame.cpp
int CallLuaRandomNumber(lua_State* L, const string& fname)
{
	lua_getglobal(L, fname.c_str());
	if (!lua_isfunction(L, -1))
		assert(false);
	//pcall calls a function in protected mode the params are as follows:
	//(state, total input params, total return values, errorfunc)
	if (!LuaOK(L, lua_pcall(L, 0, 1, 0)))
		assert(false);
	int result = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);
	return result;
}

void CallLuaMoveUp(lua_State* L, const string& fname, float Speed, float& yVal)
{
	lua_getglobal(L, fname.c_str());
	if (!lua_isfunction(L, -1))
		assert(false);
	lua_pushnumber(L, Speed);
	lua_pushnumber(L, yVal);
	//pcall calls a function in protected mode the params are as follows:
	//(state, total input params, total return values, errorfunc)
	if (!LuaOK(L, lua_pcall(L, 2, 1, 0)))
		assert(false);
	yVal = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_settop(L, 0);
}

void CallLuaMoveDown(lua_State* L, const string& fname, int spriteHeight, float Speed, float& yVal)
{
	lua_getglobal(L, fname.c_str());
	if (!lua_isfunction(L, -1))
		assert(false); 
	lua_pushinteger(L, spriteHeight);
	lua_pushnumber(L, Speed);
	lua_pushnumber(L, yVal);
	//pcall calls a function in protected mode the params are as follows:
	//(state, total input params, total return values, errorfunc)
	if (!LuaOK(L, lua_pcall(L, 3, 1, 0)))
		assert(false);
	yVal = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_settop(L, 0);
}

void CallLuaIncreaseScoreAndLives(lua_State* L, const string& fname, int& Lives, int& Score)
{
	lua_getglobal(L, fname.c_str());
	if (!lua_isfunction(L, -1))
		assert(false);	
	lua_pushinteger(L, Lives);
	lua_pushinteger(L, Score);
	//pcall calls a function in protected mode the params are as follows:
	//(state, total input params, total return values, errorfunc)
	if (!LuaOK(L, lua_pcall(L, 2, 2, 0)))
		assert(false);
	Lives = (int)lua_tointeger(L, -2);
	Score = (int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	lua_settop(L, 0);
}

void CallLuaResetScore(lua_State* L, const string& fname, int& LastScore, int& Score)
{
	lua_getglobal(L, fname.c_str());
	if (!lua_isfunction(L, -1))
		assert(false);
	lua_pushinteger(L, LastScore);
	lua_pushinteger(L, Score);
	//pcall calls a function in protected mode the params are as follows:
	//(state, total input params, total return values, errorfunc)
	if (!LuaOK(L, lua_pcall(L, 2, 2, 0)))
		assert(false);
	LastScore = (int)lua_tointeger(L, -2);
	Score = (int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	lua_settop(L, 0);
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

//**********************************************************************
//***************PART 3 C++ functions to call in Lua********************
//************PART 4 extra C++ functions to call in Lua*****************
//**********************************************************************
int Dispatcher::LuaCall(lua_State* L)
{
	string name = lua_tostring(L, 1);
	map<string, Command>::iterator it = library.find(name);
	assert(it != library.end());
	Command& cmd = (*it).second;	
	if (cmd.VoidVoidFunct)
	{		
		cmd.VoidVoidFunct();
		return 1;
	}
	else 	
	if (cmd.VoidIntFunct)
	{
		int param = (int)lua_tointeger(L, 2);
		cmd.VoidIntFunct(param);
		lua_pop(L, 1);
	}
	else//add more cases here
		assert(false);	
	return 1;
}

void CallVoidVoidFunc(lua_State* L, const string& fname)
{
	lua_getglobal(L, fname.c_str());
	if (!lua_isfunction(L, -1))
		assert(false);
	if (!LuaOK(L, lua_pcall(L, 0, 0, 0)))
		assert(false);
}

map<string, Dispatcher::Command> Dispatcher::library;
//**********************************************************************
//**********************************************************************
//**********************************************************************