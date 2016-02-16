#ifndef MYSOUND_H
#define MYSOUND_H
#define WIN32_LEAN_AND_MEAN
#include "xact3.h"
#include "MyDefine.h"

#pragma once
class MySound
{
private:
	IXACT3Engine* xactengine;
	IXACT3WaveBank* wavebank;
	IXACT3SoundBank* soundbank;
	XACTINDEX cueI;
	void* map_wave_bank;
	void* sound_bank_data;
	bool initialized;
public:
	MySound(void);
	virtual ~MySound(void);
	HRESULT initialize();
	void run();
	void playCue(const char* cue);
	void stopCue(const char* cue);
};

#endif