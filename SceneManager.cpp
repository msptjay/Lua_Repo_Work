/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	SceneManager.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: Source file for The Scene manager. 
*/
#include "SceneManager.h"
#include "Hud.h"
#include "Menu.h"
#include"PlayGame.h"
#include "GameOver.h"
#include"Audio.h"

using namespace std;

SceneManager::SceneManager(Game* game, float Delta)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
        assert(false);

    //constants**********************************************************************
    HiScorePosition.FromLua(L, "HiScorePosition");
    HiScoreTextColour.FromLua(L, "HiScoreTextColour");
    LastScorePosition.FromLua(L, "LastScorePosition");
    LastScoreTextColour.FromLua(L, "LastScoreTextColour");

    ButtonHoverTextColour.FromLua(L, "ButtonHoverTextColour");
    ButtonTextColour.FromLua(L, "ButtonTextColour");

    MenuClearColour.FromLua(L, "MenuClearColour");
    GameOverClearColour.FromLua(L, "GameOverClearColour");
    //*******************************************************************************
   
    p_GameAudio = new Audio();
    p_GameHud = new Hud();
    p_GameHud->LoadContent();   
    e_scene = MENU;
    p_GameMenu = new Menu(game->GetDisplay(), p_GameAudio);
    p_Play = new PlayGame(game, p_GameAudio, Delta);
    p_Play->LoadContent();
    p_End = new GameOver(p_GameAudio);
   
    m_HiScore = 0;   
}

SceneManager::~SceneManager()
{
    //delete ALL objects in reverse order they were created 
    //(i.e. everything which had new applied in the constructor)
    
    delete p_End;
    p_End = nullptr;
    delete p_Play;
    p_Play = nullptr;    
    delete p_GameAudio;
    p_GameAudio = nullptr; 
    delete p_GameMenu;
    p_GameMenu = nullptr;   
    delete p_GameHud;
    p_GameHud = nullptr;

    if (L)
    {
        lua_settop(L, 0);
        lua_close(L);
    }
}

void SceneManager::Update(Input* input, Game* game)
{
    float delta = game->GetDelta();
    
    switch (e_scene)
    {
    case MENU:        
        e_scene = p_GameMenu->Update(input);       
        break;
    case GAME:        
        SwitchState(p_Play->Update(delta, game,input));
        break;
    case GAME_OVER:       
        SwitchState(p_End->Update(delta, input));       
        break;
    default:
        break;
    }
}

void SceneManager::Draw(Input* input, Game* game)
{
    //***************************************************
    // these two locals are for adding string to int and 
    // converting both to const char* using c_str()
    //see the code below...
    string convert;
    string add;
    //***************************************************
    switch (e_scene)
    {
     case MENU:
         
         al_clear_to_color(al_map_rgb(MenuClearColour.r, MenuClearColour.g, MenuClearColour.b));
         p_GameMenu->Draw(input);
         add = "Last Score: ";
         convert = add + std::to_string(p_Play->GetLastScore());
         p_GameHud->SetMessage(convert.c_str());
         p_GameHud->DrawString(p_GameHud->GetMediumText(), LastScoreTextColour.r, LastScoreTextColour.g, LastScoreTextColour.b, LastScorePosition.x, LastScorePosition.y);
         if (p_Play->GetLastScore() > m_HiScore)
         {
             m_HiScore = p_Play->GetLastScore();
         }
         add = "HiScore: ";
         convert = add + std::to_string(m_HiScore);
         p_GameHud->SetMessage(convert.c_str());
         p_GameHud->DrawString(p_GameHud->GetMediumText(), HiScoreTextColour.r, HiScoreTextColour.g, HiScoreTextColour.b,al_get_display_width(game->GetDisplay())/2+ HiScorePosition.x, HiScorePosition.y);
         //********************************************************************************************************************************************
         //this code places the button text on the play and quit buttons. 
         // the text colour changes when the mouse hovers over the button
         //the button code is contained in the Menu class and sets the hovering conditions
         p_GameHud->SetMessage("Play");
         if (p_GameMenu->GetHighlightPlay() == false)
         {
             p_GameHud->DrawString(p_GameHud->GetMediumText(), ButtonTextColour.r, ButtonTextColour.g, ButtonTextColour.b, p_GameMenu->GetPlayButtonPosX() + 13, p_GameMenu->GetPlayButtonPosY());
         }else
             p_GameHud->DrawString(p_GameHud->GetMediumText(), ButtonHoverTextColour.r, ButtonHoverTextColour.g, ButtonHoverTextColour.b, p_GameMenu->GetPlayButtonPosX() + 13, p_GameMenu->GetPlayButtonPosY());
         p_GameHud->SetMessage("Quit");
         if (p_GameMenu->GetHighlightQuit() == false)
         {
             p_GameHud->DrawString(p_GameHud->GetMediumText(), ButtonTextColour.r, ButtonTextColour.g, ButtonTextColour.b, p_GameMenu->GetExitButtonPosX() + 30, p_GameMenu->GetExitButtonPosY() - 3);
         }else
             p_GameHud->DrawString(p_GameHud->GetMediumText(), ButtonHoverTextColour.r, ButtonHoverTextColour.g, ButtonHoverTextColour.b, p_GameMenu->GetExitButtonPosX() + 30, p_GameMenu->GetExitButtonPosY() - 3);
         //********************************************************************************************************************************************
         break;
     case GAME:
         al_clear_to_color(al_map_rgb(255, 255, 0));         
         p_Play->Draw(p_GameHud, game);        
         break;
     case GAME_OVER:
         al_clear_to_color(al_map_rgb(GameOverClearColour.r, GameOverClearColour.g, GameOverClearColour.b));
         p_End->Draw(p_GameHud);          
         break;
     default:
         break;
    }
}

E_GameStates SceneManager::GetState()
{
    return e_scene;
}

void SceneManager::SwitchState(E_GameStates state)
{
    e_scene = state;
}
