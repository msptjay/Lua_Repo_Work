/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	LuaHelper.h
Date:		March 2025
Last Updated:	14/03/2025

Description: Header file for the Lua helper file
*/

#pragma once

#include <string>
#include <functional>
#include <map>

extern "C"
{
#include "../lua-5.4.4/include/lua.h"
#include "../lua-5.4.4/include/lauxlib.h"
#include "../lua-5.4.4/include/lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "../lua-5.4.4/liblua54.a")
#endif // _WIN32

using namespace std;

bool LuaOK(lua_State* L, int id);

//functions to get and interpret different types from lua
//get an int
int LuaGetInt(lua_State* L, const string& name);

//get a double
double LuaGetDouble(lua_State* L, const string& name);

//get a float
double LuaGetFloat(lua_State* L, const string& name);

//get a bool
bool LuaGetBool(lua_State* L, const string& name);

//get a string
string LuaGetString(lua_State* L, const string& name);


//structs to represent positions and colours
//vector 2 x and y values
struct Vector2
{
	float x, y;

	void FromLua(lua_State* L, const string& name)
	{
		lua_getglobal(L, name.c_str());
		if (!lua_istable(L, -1))
			assert(false);

		lua_pushstring(L, "x");
		lua_gettable(L, -2);//pops x off and replaces with the value
		x = (float)lua_tonumber(L, -1);//got the value
		lua_pop(L, 1);//remove it from the stack

		lua_pushstring(L, "y");
		lua_gettable(L, -2);//pops x off and replaces with the value
		y = (float)lua_tonumber(L, -1);//got the value
		lua_pop(L, 1);//remove it from the stack
	}

};

//r g b colour
struct RGBColour
{
	int r, g, b;

	void FromLua(lua_State* L, const string& name)
	{
		lua_getglobal(L, name.c_str());
		if (!lua_istable(L, -1))
			assert(false);

		lua_pushstring(L, "r");
		lua_gettable(L, -2);//pops x off and replaces with the value
		r = (int)lua_tointeger(L, -1);//got the value
		lua_pop(L, 1);//remove it from the stack

		lua_pushstring(L, "g");
		lua_gettable(L, -2);//pops x off and replaces with the value
		g = (int)lua_tointeger(L, -1);//got the value
		lua_pop(L, 1);//remove it from the stack

		lua_pushstring(L, "b");
		lua_gettable(L, -2);//pops x off and replaces with the value
		b = (int)lua_tointeger(L, -1);//got the value
		lua_pop(L, 1);//remove it from the stack
	}
};

//r g b a clolour
struct RGBAColour
{
	int r, g, b, a;

	void FromLua(lua_State* L, const string& name)
	{
		lua_getglobal(L, name.c_str());
		if (!lua_istable(L, -1))
			assert(false);

		lua_pushstring(L, "r");
		lua_gettable(L, -2);//pops x off and replaces with the value
		r = (int)lua_tointeger(L, -1);//got the value
		lua_pop(L, 1);//remove it from the stack

		lua_pushstring(L, "g");
		lua_gettable(L, -2);//pops x off and replaces with the value
		g = (int)lua_tointeger(L, -1);//got the value
		lua_pop(L, 1);//remove it from the stack

		lua_pushstring(L, "b");
		lua_gettable(L, -2);//pops x off and replaces with the value
		b = (int)lua_tointeger(L, -1);//got the value
		lua_pop(L, 1);//remove it from the stack

		lua_pushstring(L, "a");
		lua_gettable(L, -2);//pops x off and replaces with the value
		a = (int)lua_tointeger(L, -1);//got the value
		lua_pop(L, 1);//remove it from the stack
	}	
};

//gets individual string from a specific table key
string StringFromLuaTable(lua_State* L, const string& name, const char* elementKey);


//***********************************************************************************************
//****************************Part 2 Function declarations***************************************
//***********************************************************************************************

//the random number generator, which has been completed as an example for you
int CallLuaRandomNumber(lua_State* L, const string& fname);

//this function calls the Up function defined in Lua script
//It returns nothing as the function passes a reference to Y position 
//and changes it directly
void CallLuaMoveUp(lua_State* L, const string& fname, float Speed, float& yVal);

//this function calls the Down function defined in Lua script
//It returns nothing as the function passes a reference to Y position 
//and changes it directly
void CallLuaMoveDown(lua_State* L, const string& fname,int spriteHeight, float Speed, float& yVal);

//this function calls the IncreaseScoreAndLives function defined in Lua script
//It returns nothing as the function passes a reference to Lives and Score 
//and changes them directly
void CallLuaIncreaseScoreAndLives(lua_State* L, const string& fname, int& Lives, int& Score);

//this function calls the ResetScore function defined in Lua script
//It returns nothing as the function passes a reference to LastScore and Score 
//and changes them directly
void CallLuaResetScore(lua_State* L, const string& fname, int& LastScore, int& Score);


//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************


//***********************************************************************************************
//****************Part 3 and 4 Dispatcher Class for registering C++ function signatures**********
//****************************to allow use of C++ functions in Lua scripts***********************
//***********************************************************************************************
class Dispatcher
{
public:
	struct Command
	{		
		//any function that returns nothing and takes no input
		typedef function<void(void)> voidvoidfunc;
		voidvoidfunc VoidVoidFunct;
		//*********************************************************************
		//**************add any other signatures here - PART 4*****************
		//*********************************************************************
		typedef function<void(int)> voidintfunction;
		voidintfunction VoidIntFunct;
	};

	//Call once at start
	void Init(lua_State* L)
	{
		lua_register(L, "CDispatcher", LuaCall);
	}

	//register game function
	void Register(const string& name, Command cmd)
	{
		assert(library.find(name) == library.end());
		library[name] = cmd;
	}

	//lua calls this then the data gets dispatched to the named function
	//lua is C based so needs help to call class methods
	static int LuaCall(lua_State* L);
	//this is where game functions are stored
	
	
private:
	static map<string, Command> library;
};

//this is used to call any function in lua which calls a C++ function
//as long as the function has no input and output
void CallVoidVoidFunc(lua_State* L, const string& fname);
//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
