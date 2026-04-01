/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	PlayGame.cpp
Date:		March 2025
Last Updated:	12/03/2025

Description: source file for Play state.
*/
#include "PlayGame.h"
#include "Level.h"
#include "Hud.h"
#include "Game.h"
#include "Projectile.h"
#include "Enemy.h"
#include"PlayerCharacter.h"
#include"Audio.h"

PlayGame::PlayGame(Game* game, Audio* GA, float Delta)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
        assert(false);

    //constants******************************************
    InvincibleLimit = LuaGetFloat(L,"invincibilityTime");//the time the player is invincible after losing a life       
    maxLevels = LuaGetInt(L, "MaximumLevels");//maximum number of levels
    maxEnemies = LuaGetInt(L, "maximumPooledEnemies");//the maximum number of pooled enemies        
    shotTimerLimit = LuaGetFloat(L, "timeBetweenShots");//the minimum time between shots
    maxPlayerBullets = LuaGetInt(L, "MaximumPooledPlayerShots");//the maximum number of bullets to pool for the player
    maxEnemyBullets = LuaGetInt(L, "MaximumPooledEnemyShots");//the maximum number of bullets to pool for the enemies 
    sceneMoveAmmount = LuaGetFloat(L, "SceneScrollAmmount");//the scene scroll speed
    levelExit = LuaGetInt(L, "DistanceFromEndLevelExit");//how far from the level end does the player exit to the next level
    playerBulletSprite = LuaGetString(L, "PlayerBulletSprite");
    enemyBulletSprite = LuaGetString(L, "EnemyBulletSprite"); 

    LivesTextCol.FromLua(L, "LivesTextColour");
    LevelTextCol.FromLua(L, "LevelTextColour");
    ScoreTextCol.FromLua(L, "ScoreTextColour");
    FPSTextCol.FromLua(L, "FramerateColour");

    LivesTextPos.FromLua(L, "livesTextPosition");
    LivesIconPos.FromLua(L, "LivesIconImagePosition");
    LevelTextPos.FromLua(L, "LevelTextPosition");
    ScoreTextPos.FromLua(L, "ScoreTextPosition");
    FPSTextPos.FromLua(L, "FrameratePosition");
    //***************************************************

    m_IsPaused = false;
    m_HudLastScore = 0;
    m_InvincibleTimer = 0;
    m_Shooting = false;
    m_ShotTimer = shotTimerLimit;
    m_CanShoot = true; 
    p_GameAudio = GA;   
    p_GameLevel = new Level(al_get_display_width(game->GetDisplay()), al_get_display_height(game->GetDisplay()));
    p_GameLevel->LoadContent();
    for (int index = 0; index < maxEnemies; index++)
    {
        p_enemies.push_back(new Enemy(p_GameLevel, 2, game));
    }
    for (int col = 0; col < p_GameLevel->GetLevelWidth(); col++)
    {
        for (int row = 0; row < p_GameLevel->GetLevelHeight(); row++)
        {
            if (p_GameLevel->GetLevelObject(col, row, 'p'))
            {
                p_Player = new PlayerCharacter(p_GameLevel, 2, game, GA);
                p_Player->Activate(col * p_GameLevel->GetWallHeight(), row * p_GameLevel->GetWallWidth());
            }
            if (p_GameLevel->GetLevelObject(col, row, 'E'))
            {
                for (size_t index = 0; index < p_enemies.size(); index++)
                {
                    if (p_enemies[index]->GetIsDead() == true)
                    {
                        p_enemies[index]->Activate(col * p_GameLevel->GetWallHeight(), row * p_GameLevel->GetWallWidth());
                        break;
                    }
                }
            }
        }
    }
    m_CurrentPosX = 0;
    m_CurrentPosY = 0;

    //*************************************************************
    //*********************PART 3 code*****************************
    //*************************************************************
    disp.Init(L);
    Init(disp);
    //*************************************************************
}

PlayGame::~PlayGame()
{
    if (L)
    {
        lua_settop(L, 0);
        lua_close(L);
    }
}

void PlayGame::LoadContent()
{
    p_Player->LoadContent(false);
    for (size_t index = 0; index < p_enemies.size(); index++)//add sprites to each pooled enemy
    {
        p_enemies[index]->LoadContent(true);
    }
    p_background = al_load_bitmap(LuaGetString(L, "ScrollingBGImage").c_str());
    for (int index = 0; index < maxPlayerBullets; index++)    {

        p_PlayerBullets.push_back(new Projectile());
        p_PlayerBullets[index]->LosdContent(playerBulletSprite.c_str());
    }
    for (int index = 0; index < maxEnemyBullets; index++)
    {
        p_EnemyBullets.push_back(new Projectile());
        p_EnemyBullets[index]->LosdContent(enemyBulletSprite.c_str());
    }
}

E_GameStates PlayGame::Update(float Delta, Game* game, Input* input)
{
    m_DeltaTime = Delta;
    p_Player->CheckWallCol();
    if (input->key_is_pressed(KEY_P))
    {
        if (!m_IsPaused)
        {
            m_IsPaused = true;
        }
        else
            m_IsPaused = false;
    }
    if (input->key_is_pressed(KEY_M))
    {
        if (!p_GameAudio->GetIsMuted())
        {
            p_GameAudio->MuteMusic();;
        }
    }
    if (input->key_is_pressed(KEY_ENTER))
    {
        if (p_GameAudio->GetIsMuted())
        {
            p_GameAudio->PlayMusic(1);
        }
    }
    if (!m_IsPaused)
    {
        if (m_Shooting)
        {
            m_ShotTimer -= Delta;
            if (m_ShotTimer <= 0)
            {
                m_Shooting = false;
                m_ShotTimer = shotTimerLimit;
            }
        }
        if (p_Player->GetInvncible() == true)
        {
            m_InvincibleTimer += Delta;
            if (m_InvincibleTimer >= InvincibleLimit)
            {
                m_InvincibleTimer = 0;
                p_Player->SetInvincible(false);
            }
        }
        if (p_Player->GetIsDead() == true)
        {
            p_Player->SetFrame(Delta);
            m_InvincibleTimer += Delta;
            if (m_InvincibleTimer >= 0.5f)
            {
                m_InvincibleTimer = 0;
                p_Player->SetInvincible(true);
                p_Player->SetIsDead(false);
            }
        }        

        if (p_Player->GetPositionX() < al_get_display_width(game->GetDisplay())
            && p_Player->GetPositionX() > -p_Player->GetSpriteWidth()
            && m_CurrentPosX < (p_GameLevel->GetLevelWidth() * p_GameLevel->GetWallWidth()) - al_get_display_width(game->GetDisplay()))
        {
            if (p_Player->GetIsDead() == false)//only move if the player is alive
            {
                m_CurrentPosX += sceneMoveAmmount * Delta;//move the scene at the same pace as the player            
            }
        }

        if (input->key_is_pressed(KEY_ESCAPE))//if esc pressed, return to main menu
        {
            if (!p_GameAudio->GetIsMuted())
            {
                p_GameAudio->StopMusic();
                p_GameAudio->PlayMusic(2);
            }
            m_HudLastScore = GetScore();
            ResetAll(p_GameLevel->SetBackToStart());
            p_Player->ResetLives();
            p_Player->ResetScore();
            input->ShowCursor();
            return MENU;
        }


        if (input->key_is_pressed(KEY_W) && p_Player->GetIsDead() == false)
        {
            p_Player->Up(600 * Delta, m_CurrentPosX);
        }
        if (input->key_is_pressed(KEY_S) && p_Player->GetIsDead() == false)
        {
            p_Player->Down(600 * Delta, m_CurrentPosX);
        }
        if (input->key_is_pressed(KEY_A) && p_Player->GetIsDead() == false)
        {
            if (p_Player->GetPositionX() < al_get_display_width(game->GetDisplay()) && p_Player->GetPositionX() > 0)
            {
                p_Player->Left(0, 1.0f, 600 * Delta, m_CurrentPosX);
            }
        }
        if (input->key_is_pressed(KEY_D) && p_Player->GetIsDead() == false)
        {
            if (p_Player->GetPositionX() < (al_get_display_width(game->GetDisplay()) - 100) && p_Player->GetPositionX() < (p_GameLevel->GetLevelWidth() * p_GameLevel->GetWallWidth()) - p_Player->GetSpriteWidth())
            {
                p_Player->Right(0, 1.0f, 600 * Delta, m_CurrentPosX);
            }
        }
        //shooting code *******************************************************************
        if (input->key_is_pressed(KEY_SPACE) && p_Player->GetIsDead() == false &&
            !m_Shooting && p_Player->GetInvncible() == false && m_CanShoot == true)
        {
            m_Shooting = true;
            m_CanShoot = false;
            p_GameAudio->PlaySfx(2, 0.5);
            for (size_t index = 0; index < p_PlayerBullets.size(); index++)
            {
                if (p_PlayerBullets[index]->GetInUse() == false)
                {
                    p_PlayerBullets[index]->SetInUse(p_Player->GetPositionX() + p_Player->GetSpriteWidth(),
                        p_Player->GetPositionY() + (p_Player->GetSpriteHeight() / 2));
                    break;
                }
            }

        }
        if (!input->key_is_pressed(KEY_SPACE) && p_Player->GetIsDead() == false && !m_CanShoot)
        {
            m_CanShoot = true;
        }
        //***********************************************************************
        //**************************PART 3***************************************
        //***Hot key to reset level by calling the reset function in lua*********
        //***********************************************************************
        if (input->key_is_pressed(KEY_R))
        {
            //Call the Lua function which calls the reset C++ function here
            //uncomment these when you are ready
            /*CallVoidVoidFunc(L, "ResetEverything");
            CallVoidVoidFunc(L, "BuildLevel");*/
        }
        //***********************************************************************
        for (size_t index = 0; index < p_enemies.size(); index++)
        {
            if (p_enemies[index]->GetExploding() == true && p_enemies[index]->GetIsDead() == false)
            {
                p_enemies[index]->SetFrame(Delta);
                p_enemies[index]->IncrementDeathTimer(Delta);
                if (p_enemies[index]->GetDeathTimer() >= 0.5f)
                {
                    p_enemies[index]->resetTimer();
                    p_enemies[index]->SetNotExploding();
                    p_enemies[index]->SetIsDead(true);

                }
            }
            if (!p_enemies[index]->GetIsDead() // only move if in use*************************
                && !p_enemies[index]->GetExploding()
                && p_enemies[index]->GetPositionX() >/*added player position to stop shooting 
                and collision checks when enemies pass the player*/ 
                p_Player->GetPositionX() - al_get_display_width(game->GetDisplay()))
            {
                if (!(p_enemies[index]->GetPositionX() > al_get_display_width(game->GetDisplay())))
                {
                    //***************Part 2**********************************
                    //comment this line to use Lua random number function****
                    //*******************************************************
                    int num = game->randomNumber();
                    //*******************************************************
                    
                    //*******************************************************
                    //uncomment this line to use lua function****************
                    //*******************************************************
                    //int num = CallLuaRandomNumber(L, "randomNumber");
                    //*******************************************************

                    if (num > 9990)
                    {
                        for (size_t index1 = 0; index1 < p_EnemyBullets.size(); index1++)
                        {
                            if (p_EnemyBullets[index1]->GetInUse() == false)
                            {
                                p_GameAudio->PlaySfx(3, 0.25);
                                p_EnemyBullets[index1]->SetInUse(p_enemies[index]->GetPositionX(),
                                    p_enemies[index]->GetPositionY() + (p_enemies[index]->GetSpriteHeight() / 2));
                                break;
                            }
                        }
                    }
                    
                    p_enemies[index]->MoveVertically(240 * Delta, m_CurrentPosX);

                }
            }
            //this will stop enemies from moving horizontally when the player is shot
            if (!p_Player->GetIsDead())
            p_enemies[index]->Scroll(sceneMoveAmmount * Delta);//scroll enemy with the scene
            if (p_enemies[index]->CollidesWith(p_Player) || p_Player->CollidesWith(p_enemies[index]))//check collisions with all enemies 
            {

                if (p_Player->GetIsDead() == false && p_Player->GetInvncible() == false && p_enemies[index]->GetIsDead() == false && p_enemies[index]->GetExploding() == false)
                {
                    p_Player->SetIsDead(true);//set the player as dead
                    p_Player->SetDeathPos();//record where the player died so the explosion is in the right place                        
                    p_GameAudio->PlaySfx(1, 0.5);//play death sound 
                    p_Player->DecreaseLives();//lose a life                       
                    p_Player->ResetPosition(0, p_Player->GetStarPosY());//set back to start 
                    p_enemies[index]->SetExploding();
                    p_enemies[index]->SetDeathPos();
                }
            }
        }
        for (size_t index = 0; index < p_EnemyBullets.size(); index++)
        {
            if (p_EnemyBullets[index]->GetInUse() == true)
            {
                p_EnemyBullets[index]->Update(p_GameLevel, true, m_CurrentPosX);
                if (p_EnemyBullets[index]->CollidesWith(p_Player))
                {
                    if (p_Player->GetIsDead() == false && p_Player->GetInvncible() == false)
                    {
                        p_EnemyBullets[index]->ResetBullet();
                        p_Player->SetIsDead(true);//set the player as dead
                        p_Player->SetDeathPos();//record where the player died so the explosion is in the right place                                                                         
                        p_GameAudio->PlaySfx(1, 0.5);//play death sound 
                        p_Player->DecreaseLives();//lose a life                                
                        p_Player->ResetPosition(0, p_Player->GetStarPosY());//set back to start    
                    }
                }
                if (p_EnemyBullets[index]->GetPositionX() < 0)
                {
                    p_EnemyBullets[index]->ResetBullet();
                }
            }
        }
        for (size_t index = 0; index < p_PlayerBullets.size(); index++)
        {
            if (p_PlayerBullets[index]->GetInUse() == true)
            {
                p_PlayerBullets[index]->Update(p_GameLevel, false, m_CurrentPosX);
                for (size_t index1 = 0; index1 < p_enemies.size(); index1++)
                {
                    if (p_enemies[index1]->GetExploding() == false && p_enemies[index1]->GetIsDead() == false)
                    {
                        if (p_PlayerBullets[index]->CollidesWith(p_enemies[index1]))
                        {
                            p_PlayerBullets[index]->ResetBullet();
                            p_enemies[index1]->Destroyed(p_Player);
                            p_GameAudio->PlaySfx(1, 0.5);//play death sound 
                        }
                    }
                }
                if (p_PlayerBullets[index]->GetPositionX() > al_get_display_width(game->GetDisplay()))
                {
                    p_PlayerBullets[index]->ResetBullet();
                }
            }
        }

        if (p_Player->GetPositionX() + m_CurrentPosX >= p_GameLevel->GetLevelWidth() * p_GameLevel->GetWallWidth() - levelExit)
        {
            if (p_GameLevel->GetLevelNum() < maxLevels)
            {
                p_GameLevel->IncreaseLevel();
                ResetAll(p_GameLevel->GetLevelNum());
            }
            else
            {
                if (!p_GameAudio->GetIsMuted())
                {
                    p_GameAudio->StopMusic();
                    p_GameAudio->PlayMusic(2);
                }
                m_HudLastScore = GetScore();
                p_Player->ResetLives();
                p_Player->ResetScore();
                ResetAll(p_GameLevel->SetBackToStart());
                input->ShowCursor();
                return MENU;
            }
        }
        if (p_Player->GetLives() <= 0)//if player dies enter game over
        {
            if (!p_GameAudio->GetIsMuted())
            {
                p_GameAudio->StopMusic();
                p_GameAudio->PlayMusic(0);
            }
            m_HudLastScore = GetScore();
            p_Player->ResetLives();
            p_Player->ResetScore();
            ResetAll(p_GameLevel->SetBackToStart());
            return GAME_OVER;
        }
    }
    return GAME;
}

void PlayGame::DrawScene(Game* game)
{
    for (int bgnum = 0; bgnum < 100; bgnum++)
    {
        al_draw_tinted_bitmap(p_background, al_map_rgb(255, 255, 255), (al_get_bitmap_width(p_background) * bgnum) - m_CurrentPosX * 0.5f, 0, 0);
        al_draw_tinted_bitmap(p_background, al_map_rgb(255, 255, 255), (al_get_bitmap_width(p_background) * bgnum) - m_CurrentPosX * 0.5f, al_get_bitmap_height(p_background), 0);
        al_draw_tinted_bitmap(p_background, al_map_rgb(255, 255, 255), (al_get_bitmap_width(p_background) * bgnum) - m_CurrentPosX * 0.5f, al_get_bitmap_height(p_background)*2, 0);
        al_draw_tinted_bitmap(p_background, al_map_rgb(255, 255, 255), (al_get_bitmap_width(p_background) * bgnum) - m_CurrentPosX * 0.5f, al_get_bitmap_height(p_background) * 3, 0);
    }
    p_GameLevel->Draw(m_CurrentPosX);
    //code for flashing invincible player ***********************************************************
    if (m_InvincibleTimer >= 0.0f && m_InvincibleTimer <= 0.25f || m_InvincibleTimer >= 0.5f && m_InvincibleTimer <= 0.75f
        || m_InvincibleTimer >= 1.0f && m_InvincibleTimer <= 1.25f || m_InvincibleTimer >= 1.5f && m_InvincibleTimer <= 1.75f
        || m_InvincibleTimer >= 2.0f && m_InvincibleTimer <= 2.25f || m_InvincibleTimer >= 2.5f && m_InvincibleTimer <= 2.75f
        || m_InvincibleTimer >= 3.0f && m_InvincibleTimer <= 3.25f || m_InvincibleTimer >= 3.5f && m_InvincibleTimer <= 3.75f
        || m_InvincibleTimer >= 4.0f && m_InvincibleTimer <= 4.25f || m_InvincibleTimer >= 4.5f && m_InvincibleTimer <= 4.75f)
    {
        p_Player->Draw();
    }

    for (size_t index = 0; index < p_enemies.size(); index++)
    {
        // only draw in use and on screen enemies ************************************
        if (p_enemies[index]->GetIsDead() == false 
            && p_enemies[index]->GetPositionX()  > 0
            && (p_enemies[index]->GetPositionX() + (p_enemies[index]->GetSpriteWidth() - 1) < (p_GameLevel->GetLevelWidth() * p_GameLevel->GetWallWidth())))
        {
            p_enemies[index]->Draw();
        }
    }

    for (size_t index = 0; index < p_EnemyBullets.size(); index++)
    {
        if (p_EnemyBullets[index]->GetInUse() == true)
        {
            p_EnemyBullets[index]->Draw();
        }
    }
    for (size_t index = 0; index < p_PlayerBullets.size(); index++)
    {
        if (p_PlayerBullets[index]->GetInUse() == true)
        {
            p_PlayerBullets[index]->Draw();
        }
    }
    p_GameLevel->DrawForeground(m_CurrentPosX);
}

void PlayGame::Draw(Hud* GameHud, Game* Game)
{
    string convert;
    string add;
    
    DrawScene(Game);
    //Draw HUD            
    //draw string calls now refer to bb width
    GameHud->SetMessage("Lives: ");
    GameHud->DrawString(GameHud->GetMediumText(), LivesTextCol.r, LivesTextCol.g, LivesTextCol.b, LivesTextPos.x, LivesTextPos.y);
    GameHud->DrawLives(LivesIconPos.x, LivesIconPos.y, p_Player->GetLives());
    add = "Level: ";
    convert = add + std::to_string(p_GameLevel->GetLevelNum());
    GameHud->SetMessage(convert.c_str());
    GameHud->DrawString(GameHud->GetMediumText(), LevelTextCol.r, LevelTextCol.g, LevelTextCol.b,al_get_display_width(Game->GetDisplay())/2 - LevelTextPos.x, LevelTextPos.y);
    add = "Score: ";
    convert = add + std::to_string(GetScore());
    GameHud->SetMessage(convert.c_str());
    GameHud->DrawString(GameHud->GetMediumText(), ScoreTextCol.r, ScoreTextCol.g, ScoreTextCol.b, al_get_display_width(Game->GetDisplay()) - ScoreTextPos.x, ScoreTextPos.y);
    float fps = 10000.0f*m_DeltaTime;
    add = "Fps: ";
    convert = add + std::to_string((int)fps);
    GameHud->SetMessage(convert.c_str());
    GameHud->DrawString(GameHud->GetMediumText(), FPSTextCol.r, FPSTextCol.g, FPSTextCol.b, al_get_display_width(Game->GetDisplay()) - FPSTextPos.x, FPSTextPos.y);
}

void PlayGame::ResetAll(int levelNumber)
{
    //reset everything      
    m_CurrentPosX = 0;
    m_CurrentPosY = 0;//reset current pos
    p_Player->SetInvincible(false);
    p_Player->SetIsDead(false);    
    m_InvincibleTimer = 0;
    p_GameLevel->resetLevels(levelNumber);
    for (size_t index = 0; index < p_enemies.size(); index++)
    {
        p_enemies[index]->SetIsDead(true);
    }    
    for (int col = 0; col < p_GameLevel->GetLevelWidth(); col++)
    {
        for (int row = 0; row < p_GameLevel->GetLevelHeight(); row++)
        {
            if (p_GameLevel->GetLevelObject(col, row, 'p'))
            {
                p_Player->ResetPosition(col * p_GameLevel->GetWallHeight(), row * p_GameLevel->GetWallWidth());
            }
            if (p_GameLevel->GetLevelObject(col, row, 'E'))
            {
                for (size_t index = 0; index < p_enemies.size(); index++)
                {
                    if (p_enemies[index]->GetIsDead() == true)
                    {
                        p_enemies[index]->Activate(col * p_GameLevel->GetWallHeight(), row * p_GameLevel->GetWallWidth());
                        p_enemies[index]->SetNotExploding();                        
                        break;
                    }
                }
            }
        }
    }
    if (p_Player->GetLives() == 0)
    {
        p_Player->ResetLives();       
    }    
}

int PlayGame::GetScore()
{
	return p_Player->GetScore();
}

int PlayGame::GetLastScore()
{
	return m_HudLastScore;
}
