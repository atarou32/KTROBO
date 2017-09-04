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
	KoumokuList_Parts(int cid, int c2id, Texture* t) : KoumokuList(t) {
		category_id = cid;
		category2_id = c2id;
		metadata = 0;
	}
	~KoumokuList_Parts() {
		~KoumokuList();
	}

	static char* getFilenameFromCID();


};

class Koumoku_Parts : public Koumoku {
private:
	RoboParts* parts;
public:
	Texture* t;
	Koumoku_Parts(int clicked_) : Koumoku(clicked_) {
	}
	~Koumoku_Parts(){};

};


class Koumoku_Parts_Category : public Koumoku {
public:
	Texture* t;

	Koumoku_Parts_Category(int clicked_) : Koumoku(clicked_) {
	}
	~Koumoku_Parts_Category();
	void Init(Texture* t, MyTextureLoader* loader, char* name);
	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked);// focused_koumoku ÇÃkoumokuÇ≈Ç‡byougaÇÕåƒÇŒÇÍÇÈ
	void focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked);
	//void clickedExe(Gamen* gamen, GamenPart* gp, KoumokuList* kl); // set_enable Ç™false ÇÃÇ∆Ç´ÇÕÉäÉ^Å[ÉìÇ∑ÇÈÇ±Ç∆
	void _exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl);
};

class Gamen_GARAGE : public Gamen , public INPUTSHORICLASS
{
private:
	int pressed_up_count;
	int pressed_down_count;
	Texture* t;


public:
	Gamen_GARAGE(void);
	~Gamen_GARAGE(void);
	Robo* robo;
private:
	int clearrobogamen;
public:
	KoumokuList* parts_category_list;
	KoumokuList* parts_leg_category_list;
	KoumokuList* parts_rweapon_category_list;
	KoumokuList* parts_lweapon_category_list;
	KoumokuList* parts_rkata_category_list;
	KoumokuList* parts_lkata_category_list;
	KoumokuList* parts_inside_category_list;

	KoumokuList* temp_focused_list;



	void Init(Graphics* g, AtariHantei* hantei,Texture* t, MyTextureLoader* loader);
	void Release();
	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp);
	void clickedShori(int id);
	void clickedShoriWithData(int id, void* data);
	void clickedEscape();
	bool handleMessage(int msg, void* data, DWORD time);
};

}

#endif