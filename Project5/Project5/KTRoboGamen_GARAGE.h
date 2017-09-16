#ifndef KTROBO_GAMEN_GARAGE_H
#define KTROBO_GAMEN_GARAGE_H
#pragma once
#include "KTRoboGamen.h"
#include "KTRoboInput.h"
#ifndef KTROBO_ROBO_H
#include "KTRoboRobo.h"
#endif

namespace KTROBO {
class GamenGARAGE_partsParam {
public:
	static const int string_haba = 20;
	static const int string_haba_place_width = 20 * 16;
	static const int string_haba_place_height = 22;
private:
	MYRECT place;
	Texture* t;
	int robo_parts_param_name;
	int robo_parts_param_suuti;
	int robo_parts_param_suuti_hikaku;
	bool is_hikaku;
	bool is_use;

	void setChara2dayo(Robo* robo, char* dataname, char* dataname_for_disp, int suuti, RoboParts* new_parts, RoboParts* compare_parts);

public:
	GamenGARAGE_partsParam(Texture* t) {
		this->t = t;
		place.left = 0;place.right = 1;
		place.top = 0;
		place.bottom = 1;

		robo_parts_param_name = t->getRenderText("test",0,-50,string_haba,string_haba_place_width,string_haba_place_height);
		robo_parts_param_suuti = t->getRenderText("test",0,-50,string_haba,string_haba_place_width,string_haba_place_height);
		robo_parts_param_suuti_hikaku = t->getRenderText("test",0,-50,string_haba,string_haba_place_width,string_haba_place_height);
		is_hikaku = true;
		is_use = false;
	}

	~GamenGARAGE_partsParam(){};
	bool setChara(MYRECT* placedayo, RoboDataMetaData* metadata, RoboParts* parts, int index, RoboParts* compare_parts);
	bool setChara2(MYRECT* placedayo, Robo* robo, RoboParts* new_parts, int category_id, int index,RoboParts* compare_parts);
	
	void setVisible(bool t);
	bool getIsUse() {
		return is_use;
	}
	void setNoUse() {
		is_use = false;
	}
	static void getSuutiHikakuChara(int suuti, char* chara);
	static void getSuutiChara(int suuti, char* chara);

};
class Gamen_GARAGE;
class GamenGARAGE_partYESNOEQUIP : public GamenPart {

public:
	KoumokuList* mae_focused_list;
	KoumokuList* kl;
	void equip();
private:
	bool is_empty_mode;
	Robo* robo;
	RoboParts* robo_parts;
	Texture* t;
	Graphics* g;
	MyTextureLoader* tex_loader;
	Gamen_GARAGE* gg;
	int clear_gamen;
	int question_str;
	int category_id;
public:
	GamenGARAGE_partYESNOEQUIP() {
		robo = 0;
		robo_parts = 0;
		t = 0;
		tex_loader = 0;
		clear_gamen = 0;
		question_str = 0;
		kl = 0;
		mae_focused_list = 0;
		g = 0;
		gg = 0;
		is_empty_mode = false;
		category_id = 0;
	}

	~GamenGARAGE_partYESNOEQUIP() {
		if (kl) {
			delete kl;
			kl = 0;

		}
	}
public:
	void setGG(Gamen_GARAGE* gg) {
		this->gg = gg;
	}
	void setEmptyMode(bool ttt ) {
		is_empty_mode = ttt;
	}
	RoboParts* getRoboParts() {
		return robo_parts;
	}
	int getCategoryID() {
		return category_id;
	}

	void setGraphics(Graphics* g);
	void setTexture(Texture* tex);
	void setTextureLoader(MyTextureLoader* loader);
	void makeKL();
	void setRobo(Robo* robo);
	void setRoboParts(RoboParts* robo_parts, int category_id);
	void setMaeFocusedList(KoumokuList* kll);
	void setFocused(Gamen* g, bool te);
	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp);
};


class GamenGARAGE_partRoboParam : public GamenPart {
	private:
		Robo* robo;
		RoboParts* robo_parts;
		RoboParts* robo_parts_compare;
		RoboDataMetaData* metadata;

		Texture* t;
		int clear_robo_param_gamen;
		vector<int> robo_parts_name_category;
		vector<int> robo_parts_name;

		vector<GamenGARAGE_partsParam*> gamen_robo_params;

		vector<GamenGARAGE_partsParam*> gamen_parts_params;

	public:
		void Init(Robo* robo, Graphics* g, Texture* t, MyTextureLoader* loader);
		~GamenGARAGE_partRoboParam() {
			vector<GamenGARAGE_partsParam*>::iterator it = gamen_parts_params.begin();
			while(it != gamen_parts_params.end()) {
				GamenGARAGE_partsParam* oo = *it;
				delete oo;
				oo = 0;
				it++;
			}
			gamen_parts_params.clear();

			it = gamen_robo_params.begin();
			while(it != gamen_robo_params.end()) {
				GamenGARAGE_partsParam* oo = *it;
				delete oo;
				oo = 0;
				it++;
			}
			gamen_robo_params.clear();
		}
		RoboParts* getRoboParts() {
			return this->robo_parts;
		}

		void setRoboParts(RoboParts* robo_parts, RoboDataMetaData* metadata, RoboParts* robo_parts_compare, int category_id);
		void calcRoboParam(RoboParts* new_parts, RoboParts* robo_parts_compare, int category_id);
		void hanneiPartsName();
		void setFocused(Gamen* g, bool t);
		void byouga(Graphics* g, GUI* gui, float dsecond, int stamp);
	
};

class Gamen_GARAGE;

// ほかのIDとかぶらないようにすること
#define KTROBO_GAMEN_GARAGE_KOUMOKU_YESNOEQUIP_YES_ID 1000000 
#define KTROBO_GAMEN_GARAGE_KOUMOKU_YESNOEQUIP_NO_ID 1000001 
#define KTROBO_GAMEN_GARAGE_GUI_PNG "resrc/sample/gui.png"


class KoumokuNOEQUIP : public Koumoku {
public:
	Gamen_GARAGE* g;
	GamenGARAGE_partYESNOEQUIP* g_part;
	Texture* t;
public:
	KoumokuNOEQUIP(Texture* t, Gamen_GARAGE* g, GamenGARAGE_partYESNOEQUIP* g_part) : Koumoku(KTROBO_GAMEN_GARAGE_KOUMOKU_YESNOEQUIP_NO_ID) {
		this->g = g;
		this->g_part = g_part;
		this->t = t;
	}
	~KoumokuNOEQUIP() {
	}
	void init() {
			int tex_id = t->getTexture(KTROBO_GAMEN_GARAGE_GUI_PNG);
		gui_koumoku_name_id = t->getRenderText("いいえ", place.left,place.top,place.bottom - place.top, place.right - place.left,
			place.bottom - place.top);
		t->setRenderTextIsRender(gui_koumoku_name_id,true);

		gui_koumoku_name_bg_id = t->getRenderTex(tex_id,0xFFFFFFFF,place.left,place.top,place.right-place.left,place.bottom - place.top,
			0,0,100,100);
		t->setRenderTexIsRender(gui_koumoku_name_bg_id,true);
	



	}
	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp,bool has_clicked);// focused_koumoku のkoumokuでもbyougaは呼ばれる
	void focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked);
	void _exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl);

};

class KoumokuYESEQUIP : public Koumoku {
public:
	Gamen_GARAGE* g;
	GamenGARAGE_partYESNOEQUIP* g_part;
	Texture* t;
public:
	~KoumokuYESEQUIP() {
	}
	KoumokuYESEQUIP(Texture* t, Gamen_GARAGE* g, GamenGARAGE_partYESNOEQUIP* g_part) : Koumoku(KTROBO_GAMEN_GARAGE_KOUMOKU_YESNOEQUIP_YES_ID) {
		this->g = g;
		this->g_part = g_part;
		this->t = t;
		}
	void init() {
			int tex_id = t->getTexture(KTROBO_GAMEN_GARAGE_GUI_PNG);
		gui_koumoku_name_id = t->getRenderText("はい", place.left,place.top,place.bottom - place.top, place.right - place.left,
			place.bottom - place.top);
		t->setRenderTextIsRender(gui_koumoku_name_id,true);

		gui_koumoku_name_bg_id = t->getRenderTex(tex_id,0xFFFFFFFF,place.left,place.top,place.right-place.left,place.bottom - place.top,
			0,0,100,100);
		t->setRenderTexIsRender(gui_koumoku_name_bg_id,true);



	};

	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp,bool has_clicked);// focused_koumoku のkoumokuでもbyougaは呼ばれる
	void focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked);
	void _exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl);

	void equipParts();

};

class KoumokuList_Parts : public KoumokuList {
private:
	int category_id;
	int category2_id;
	RoboDataMetaData* metadata;
	RoboParts* focused_parts;
	Gamen_GARAGE* g;
public:
	int parts_setumei_text;

public:
	int getCategoryID() {
		return category_id;
	}
	int getCategoryID2() {
		return category2_id;
	}

	KoumokuList_Parts(Gamen_GARAGE* g, int cid, int c2id, Texture* t) : KoumokuList(t) {
		category_id = cid;
		category2_id = c2id;
		metadata = 0;
		parts_setumei_text=0;
		focused_parts = 0;
		this->g = g;
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

	RoboDataMetaData* getMetaData() {
		return metadata;
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
	void setFocusedParts(RoboParts* parts) {
		this->focused_parts = parts;
	}
	RoboParts* getFocusedParts() {
		return focused_parts;
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
	virtual void clickedUp();
	virtual void clickedDown();

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
	Texture* t2;
	MyTextureLoader* loader;
public:
	GamenGARAGE_partRoboParam gamenpart_roboparam;
	GamenGARAGE_partYESNOEQUIP gamenpart_yesnoequip;

	RoboParts* getRoboPartsFromCID(int cid);
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
	bool canBeEmpty(int category_id);

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
	KoumokuList_Parts* getPartsList(int category_id, int category_id2);


	void Init(Graphics* g, AtariHantei* hantei,Texture* t, Texture* t2, MyTextureLoader* loader);
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