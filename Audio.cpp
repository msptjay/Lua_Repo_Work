/*
Lua Assignment Game
Author:		Philip Alassad
Filename:	Audio.cpp
Date:		March 2025
Last Updated:	15/03/2025

Description: source file for Audio class.
*/
#include "Audio.h"
#include <iostream>

Audio::Audio()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	if (!LuaOK(L, luaL_dofile(L, "Script.lua")))
		assert(false);

	m_IsMuted = false;
	if (!al_install_audio())
	{
		fprintf(stderr, "Allegro initialisation failed");
		return;
	}
	if (!al_reserve_samples(20))
	{
		fprintf(stderr, "Allegro initialisation failed");
		return;
	}
	if (!al_init_acodec_addon())
	{
		fprintf(stderr, "Allegro initialisation failed");
		return;
	}

	//constants********************************************************
	MaxSamples = LuaGetInt(L, "maximumSFX");
	MaxMusicTracks = LuaGetInt(L, "maxMusicTracks");
	MusicGain = LuaGetFloat(L, "musicGain");
	p_Sfx.push_back(al_load_sample(StringFromLuaTable(L, "SFX", "a").c_str()));
	p_Sfx.push_back(al_load_sample(StringFromLuaTable(L, "SFX", "b").c_str()));
	p_Sfx.push_back(al_load_sample(StringFromLuaTable(L, "SFX", "c").c_str()));
	p_Sfx.push_back(al_load_sample(StringFromLuaTable(L, "SFX", "d").c_str()));

	p_MusicTrack.push_back(al_load_sample(StringFromLuaTable(L, "musicTracks", "a").c_str()));
	p_MusicTrack.push_back(al_load_sample(StringFromLuaTable(L, "musicTracks", "b").c_str()));
	p_MusicTrack.push_back(al_load_sample(StringFromLuaTable(L, "musicTracks", "c").c_str()));
	//******************************************************************

	PlayMusic(2);
}

Audio::~Audio()
{
	for (int num = 0; num < MaxSamples; num++)
	{
		al_destroy_sample(p_Sfx[num]);
	}
	p_Sfx.clear();
	for (int num = 0; num < MaxMusicTracks; num++)
	{
		al_destroy_sample(p_MusicTrack[num]);
	}
	p_MusicTrack.clear();
	al_uninstall_audio();
	if (L)
	{
		lua_settop(L, 0);
		lua_close(L);
	}
}

void Audio::PlaySfx(int sampleNumber, float gain)
{
	al_play_sample(p_Sfx[sampleNumber], gain, ALLEGRO_AUDIO_PAN_NONE, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void Audio::PlayMusic(int TrackNumber)
{
	m_IsMuted = false;
	al_play_sample(p_MusicTrack[TrackNumber], MusicGain, ALLEGRO_AUDIO_PAN_NONE, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}

void Audio::StopMusic()
{
	al_stop_samples();
}

bool Audio::GetIsMuted()
{
	return m_IsMuted;
}

void Audio::MuteMusic()
{
	StopMusic();
	m_IsMuted = true;
}
