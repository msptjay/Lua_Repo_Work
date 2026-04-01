/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Audio.h
Date:		March 2025
Last Updated:	15/03/2025

Description: Header file for Audio class.
*/
#pragma once
#include"allegro5/allegro_acodec.h"
#include <vector>
#include "LuaHelper.h"

class Audio
{
private:
	//pointers
	lua_State* L;
	//constants for lua****************************************
	int MaxSamples;
	int MaxMusicTracks;
	std::vector<ALLEGRO_SAMPLE*> p_MusicTrack;
	std::vector<ALLEGRO_SAMPLE*> p_Sfx;
	float MusicGain;
	//*************************************************
	
	bool m_IsMuted;
public:
	Audio();
	~Audio();

	void PlaySfx(int sampleNumber, float gain);
	void PlayMusic(int TrackNumber);
	void StopMusic();
	bool GetIsMuted();
	void MuteMusic();
};

