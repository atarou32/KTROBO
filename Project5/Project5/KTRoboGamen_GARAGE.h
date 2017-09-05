#ifndef KTROBO_GAMEN_GARAGE_H
#define KTROBO_GAMEN_GARAGE_H
#pragma once
#include "KTRoboGamen.h"
#include "KTRoboInput.h"
#ifndef KTROBO_ROBO_H
#include "KTRoboRobo.h"
#endif

namespace KTROBO {

class KoumokuList_Parts : public KoumokuList {
private:
	int category_id;
	int category2_id;
	RoboDataMetaData* metadata;
public:
	int parts_setumei_text;

public:
	KoumokuList_Parts(int cid, int c2id, Texture* t) : KoumokuList(t) {
		category_id = cid;
		category2_id = c2id;
		metadata = 0;
		parts_setumei_text=0;
	}
	virtual ~KoumokuList_Parts() {
	
		Release();
		KoumokuList::~KoumokuList();
	}
	void Release() {
		if (metadata) {
			delete metadata;
			metadata = 0;
		}
	}
	void setMetaData(RoboDataMetaData* meta) {
		metadata = meta;
	}
	void InitKoumokus(Texture* t, MyTextureLoader* loader);
	char* getPartsFilenameFromCID();
	char* getMetaPartsFilenameFromCID();
	bool hasLoad() {
		int size = koumokus.size();
		for (int i=0; i< size; i++) {
			if (!koumokus[i]->hasLoad()) {
				return false;
			}
		}
		return true;
	}

	void load(Graphics* g, MyTextureLoader* loader);
	bool isCID(int cid, int cid2) {
		if (cid == category_id) {
			if (cid2 == category2_id) {
				return true;
			}
		}
		return false;
	}
};

class Koumoku_Parts : public Koumoku {
private:
	RoboParts* parts;
public:
	Texture* t;
	
	Koumoku_Parts(int clicked_, RoboParts* p) : Koumoku(clicked_) {
	  parts = p;
	}
	virtual ~Koumoku_Parts(){
	  Release();
	};
	void Release() {
		if (parts) {
			parts->Release();
			delete parts;
			parts = 0;
		}
	}
	bool hasLoad();
	void load(Graphics* g, MyTextureLoader* loader);
	void Init(Texture* t, MyTextureLoader* loader);
	void setFocused(KoumokuList* kl);
	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked);// focused_koumoku のkoumokuでもbyougaは呼ばれる
	void focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked);
	//void clickedExe(Gamen* gamen, GamenPart* gp, KoumokuList* kl); // set_enable がfalse のときはリターンすること
	void _exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl);
};


class Koumoku_Parts_Category : public Koumoku {
public:
	Texture* t;

	Koumoku_Parts_Category(int clicked_) : Koumoku(clicked_) {
	}
	~Koumoku_Parts_Category(){};
	void Init(Texture* t, MyTextureLoader* loader, char* name);
	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked);// focused_koumoku のkoumokuでもbyougaは呼ばれる
	void focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked);
	//void clickedExe(Gamen* gamen, GamenPart* gp, KoumokuList* kl); // set_enable がfalse のときはリターンすること
	void _exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl);
};

class Gamen_GARAGE : public Gamen , public INPUTSHORICLASS
{
private:
	int pressed_up_count;
	int pressed_down_count;
	Texture* t;
	MyTextureLoader* loader;

public:
	Gamen_GARAGE(void);
	~Gamen_GARAGE(void);
	Robo* robo;
private:
	int clearrobogamen;
	int clearpartsgamen;
	int clearcommentgamen;
	void clickedKoumokuListPartsdayo(KoumokuList_Parts* kp, RoboParts* kk);
	void clickedShoriCParts(vector<KoumokuList_Parts*>* kp, RoboParts* kk, int category_id, int category_id2, char* namedayo);
	KoumokuList* getCategoryList(int category_id);

public:
	KoumokuList* parts_category_list;
	KoumokuList* parts_leg_category_list;
	KoumokuList* parts_rweapon_category_list;
	KoumokuList* parts_lweapon_category_list;
	KoumokuList* parts_rkata_category_list;
	KoumokuList* parts_lkata_category_list;
	KoumokuList* parts_inside_category_list;

	KoumokuList* temp_focused_list;
	KoumokuList_Parts* parts_head_list;
	KoumokuList_Parts* parts_booster_list;
	KoumokuList_Parts* parts_core_list;
	KoumokuList_Parts* parts_engine_list;
	KoumokuList_Parts* parts_fcs_list;
	KoumokuList_Parts* parts_arm_list;

	vector<KoumokuList_Parts*> parts_inside_lists;
	vector<KoumokuList_Parts*> parts_leg_lists;
	vector<KoumokuList_Parts*> parts_rarmweapon_lists;
	vector<KoumokuList_Parts*> parts_larmweapon_lists;
	vector<KoumokuList_Parts*> parts_rkataweapon_lists;
	vector<KoumokuList_Parts*> parts_lkataweapon_lists;

	KoumokuList_Parts* getPartsList(vector<KoumokuList_Parts*>* lists, int category_id, int category_id2);

	void Init(Graphics* g, AtariHantei* hantei,Texture* t, MyTextureLoader* loader);
	void Release();
	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp);
	void loadData(Graphics* g, float dsecond, int stamp);
	void clickedShori(int id);
	void clickedShoriWithData(int id, void* data);
	void clickedEscape();
	bool handleMessage(int msg, void* data, DWORD time);
};

}

#endif