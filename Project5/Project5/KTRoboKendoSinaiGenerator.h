#ifndef KTROBOKENDOSINAIGENERATOR_H
#define KTROBOKENDOSINAIGENERATOR_H


#pragma once
#include "KTRoboGUI.h"
#include "KTRoboKendo.h"
#include "KTRoboInput.h"

class lua_State;


namespace KTROBO {

class Texture;
class KendoSinaiNigiruhitoPart {
public:
	float dt;
	float anime[KTROBO_MESH_BONE_MAX];

	KendoSinaiNigiruhitoPart() {
		dt = 0;
		for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
			anime[i] = 0;
		}
	}
};



class KendoSinaiGenerator : public INPUTSHORICLASS
{
private:
	GUI* gui;
	int screen_window_id;
	int furu_play_toggle_button_id;
	int slider_sinai_dt_id;

	int slider_nigiruhito_anime_id;
	int nigiruhito_anime_set_button_id;
	int sinai_anime_set_button_id;

	int undo_button_id;
	int erase_anime_set_button_id;
	int erase_furu_sinai_button_id;

	Texture* tex;

	// sinai の位置と姿勢
	MYVECTOR3 dpos;
	float drotx;
	float drotz;
	
	vector<SinaiFuruPart> furu_parts;
	vector<int> furu_parts_tex;

	vector<KendoSinaiNigiruhitoPart> nigiruhito_parts;
	vector<int> nigiruhito_parts_tex;

	// 現在のパラメータ
	float dt;
	float anime[KTROBO_MESH_BONE_MAX];

public:
	KendoSinaiGenerator(void);
	~KendoSinaiGenerator(void);

	float getNigiruhitoAnimeTemp();
	void setSinaiPosRot(SinaiFuru* sinaifuru);

	void Init(HWND hw, Texture* tex, lua_State* l, int screen_width, int screen_height);
	void Del();
	bool handleMessage(int msg, void* data, DWORD time);
};




interface ISinaiFuruAnimeMaker {
public:
	TO_LUA virtual void togglePlayAnime()=0;
	TO_LUA virtual void nigiruhitoSetAnime()=0;
	TO_LUA virtual void nigiruhitoEraseAnime()=0;
	TO_LUA virtual void sinaiEraseAnime()=0;
	TO_LUA virtual void sinaiSetAnime()=0;
	TO_LUA virtual void undo()=0;
	TO_LUA virtual void sinaiNigiruhitoUpdate()=0;
};

class SinaiFuruAnimeMaker : public ISinaiFuruAnimeMaker {
private:
	UMeshUnit* nigiruhito;
	Sinai* sinai;
	KendoSinaiGenerator* generator;

public:
	SinaiFuruAnimeMaker();
	~SinaiFuruAnimeMaker();
	void Init(KendoSinaiGenerator* generator, UMeshUnit* nigiruhito, Sinai* sinai);
	void Release() {

	}

	void togglePlayAnime();
	void nigiruhitoSetAnime();
	void nigiruhitoEraseAnime();
	void sinaiEraseAnime();
	void sinaiSetAnime();
	void undo();
	void sinaiNigiruhitoUpdate();

	void update(float dt, SinaiFuru* furu);

};

class SinaiFuruAnimeMakers {
private:
	vector<SinaiFuruAnimeMaker*> makers;
public:
	int makeInst();
	

	SinaiFuruAnimeMaker* getInstance(int index);

	ISinaiFuruAnimeMaker* getInterface(int index);

	SinaiFuruAnimeMakers() {}
	~SinaiFuruAnimeMakers() {
		Release();
	}

	void Release() {
		int size = makers.size();
		for (int i= 0; i<size;i++) {
			makers[i]->Release();
			delete makers[i];
			makers[i] = 0;
		}
		makers.clear();
	}
};

}
#endif


