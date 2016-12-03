#include "MySound.h"
#include "KTRoboGameError.h"
#include "stringconverter.h"
#include "MyDefine.h"

using namespace KTROBO;


MySound::MySound(void)
{
	xactengine = 0;
	wavebank = 0;
	soundbank = 0;
	cueI = 0;
	map_wave_bank = 0;
	sound_bank_data = 0;
	initialized = false;
}


MySound::~MySound(void)
{
	if (xactengine) {
		xactengine->ShutDown();
		xactengine->Release();
	}
	if (sound_bank_data) {
		delete[] sound_bank_data;
		sound_bank_data = NULL;
	}

	if (map_wave_bank) {
		UnmapViewOfFile(map_wave_bank);
		map_wave_bank = NULL;
	}

	if (initialized) {
		CoUninitialize();
		initialized = false;
	}

}

HRESULT MySound::initialize() {
	HRESULT result = XACT3CreateEngine(0, &xactengine);
	if (FAILED(result) || xactengine == NULL) {
		return E_FAIL;
	}
	XACT_RUNTIME_PARAMETERS xactparams = {0};
	xactparams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	result = xactengine->Initialize(&xactparams);
	if (FAILED(result)) {
		return result;
	}

	result = E_FAIL;
	stringconverter sc;
	
	WCHAR buf[512];
	sc.charToWCHAR(KTROBO::WAVE_BANK,buf);
	HANDLE hfile = CreateFile(buf,GENERIC_READ,FILE_SHARE_READ,NULL, OPEN_EXISTING,0,NULL);
	if (hfile != INVALID_HANDLE_VALUE) {
		int fileSize = GetFileSize(hfile,NULL);
		if (fileSize != -1) {
			HANDLE hmap = CreateFileMapping(hfile, NULL, PAGE_READONLY, 0 , fileSize, NULL);
			if (hmap) {
				map_wave_bank = MapViewOfFile(hmap, FILE_MAP_READ,0,0,0);
				if (map_wave_bank) {
					result = xactengine->CreateInMemoryWaveBank(map_wave_bank, fileSize, 0, 0, &wavebank);
					CloseHandle(hmap);
				}
			}
			CloseHandle(hfile);
		}
		
	}

	if (FAILED(result)) {
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR,"no audio init");
	}

	result = E_FAIL;
	
	sc.charToWCHAR(KTROBO::SOUND_BANK,buf);
	hfile = CreateFile(buf,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hfile != INVALID_HANDLE_VALUE) {
		int fileSize = GetFileSize(hfile, NULL);
		if (fileSize != -1) {
			sound_bank_data = new BYTE[fileSize];
			if (sound_bank_data) {
				DWORD bytesread=0;
				if (0 != ReadFile(hfile, sound_bank_data, fileSize, &bytesread,NULL)) {
					result = xactengine->CreateSoundBank(sound_bank_data, fileSize, 0,0,&soundbank);
				}
			}
		}
		CloseHandle(hfile);
		
	}

	if (FAILED(result)) {
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR,"no audio init");
	}
	CoInitialize(0);
	initialized = true;
	return MB_OK;

}


void MySound::run() {
	if (xactengine == NULL) {
		return;
	}
	xactengine->DoWork();
}


void MySound::playCue(const char* cue) {
	if (soundbank == NULL) return;
	cueI = soundbank->GetCueIndex(cue);
	soundbank->Play(cueI, 0, 0, NULL);

}


void MySound::stopCue(const char* cue) {
	if (soundbank == NULL) return;
	cueI = soundbank->GetCueIndex(cue);
	soundbank->Stop(cueI,XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}

