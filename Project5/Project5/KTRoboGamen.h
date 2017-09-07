#ifndef KTROBO_GAMEN_H
#define KTROBO_GAMEN_H

#pragma once
#ifndef KTROBO_GRAPHICS_H
#include "KTRoboGraphics.h"
#endif
#include "MyButukari.h"
#include "KTRoboGUI.h"
#include <vector>
using namespace std;

namespace KTROBO {
class Gamen
{
public:
	Gamen(void);
	~Gamen(void);
	virtual void byouga(Graphics* g, GUI* gui, float dsecond, int stamp)=0;
	virtual void clickedShori(int id)=0;
	virtual void clickedShoriWithData(int id, void* data)=0;

};

class GamenPart {
protected:
	bool is_focused;
public:
	GamenPart() {is_focused = false;};
	~GamenPart() {};

	virtual void setFocused(Gamen* g, bool t)=0;
	virtual void byouga(Graphics* g, GUI* gui, float dsecond, int stamp)=0;
	
};


class KoumokuList;
class Koumoku {
private:
	bool is_enable;
	bool is_visible;
protected:
	MYRECT place;
	int clicked_id;
protected:
	int gui_koumoku_name_id;
	int gui_koumoku_name_bg_id;
public:
	Koumoku(int clicked_) {is_enable = true;is_visible=true;
		place.left = 0;
		place.right = 100;
		place.top = 0;
		place.bottom = 100;
		clicked_id = clicked_;
		gui_koumoku_name_id = 0;
		gui_koumoku_name_bg_id = 0;
	};
	virtual ~Koumoku(){};

	bool clicked(int mouse_x, int mouse_y);
	bool moved(int mouse_x, int mouse_y);
	void setEnabled(bool t);
	bool getEnabled() {return is_enable;}
	void setVisible(Texture* t, bool tt) {
		if (gui_koumoku_name_id) {
			t->setRenderTextIsRender(gui_koumoku_name_id,tt);
		}
		if (gui_koumoku_name_bg_id) {
			t->setRenderTexIsRender(gui_koumoku_name_bg_id,tt);
		}
		return;
	}
	bool getVisible() {
		return is_visible;
	}

	void setSize(Texture* t,int x, int y, int width, int height);
	int getID();
	virtual void setFocused(KoumokuList* kl){};

	virtual void byouga(Graphics* g, GUI* gui, float dsecond, int stamp,bool has_clicked)=0;// focused_koumoku ÇÃkoumokuÇ≈Ç‡byougaÇÕåƒÇŒÇÍÇÈ
	virtual void focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp, bool has_clicked)=0;
	void clickedExe(Gamen* gamen, GamenPart* gp, KoumokuList* kl); // set_enable Ç™false ÇÃÇ∆Ç´ÇÕÉäÉ^Å[ÉìÇ∑ÇÈÇ±Ç∆
	virtual void _exedayo(Gamen* gamen, GamenPart* gp, KoumokuList* kl)=0;
	virtual bool hasLoad() {return false;}
	virtual void load(Graphics* g, MyTextureLoader* loader) {};
};
class KoumokuList {
public:
	Texture* t;
private:
	int cursor;
protected:
	vector<Koumoku*> koumokus;
protected:
	Koumoku* focused_koumoku;
private:
	bool is_visible;
	bool is_enable;
	bool has_clicked_forbyouga;
	char title[128];
	MYRECT place;
	bool hyouji3_mode;
private:
	void setFocusedKoumoku(Koumoku* k, int index) {
		focused_koumoku = k;
		k->setFocused(this);
		setFocusedKoumokuHyouji3Mode(focused_koumoku,index);
	}
private:

	void setHasClicked(bool t) {
		has_clicked_forbyouga = t;
	}

	bool hasEnabledKoumoku() {
	  int size = koumokus.size();
	  for (int i = 0; i< size; i++) {
		  if (koumokus[i]->getEnabled()) {
			  return true;
		  }
	  }
	  return false;
	}
	
public:
	bool getHasClicked() {
		return has_clicked_forbyouga;
	}
	int getKoumokuSize() {
		return koumokus.size();
	}

	int getCursor() {return cursor;}
	int getCursorIndex(Koumoku* k) {
		int size= koumokus.size();
		for (int i=0;i<size; i++) {
			if (k == koumokus[i]) {
				return i;
			}
		}
		return -1;
	}

	KoumokuList(Texture* t) {
		for(int i=0;i<128;i++) {
			title[i] = 0;
		}
		cursor = 0; focused_koumoku=0; is_visible = true; is_enable = true;
		place.left = 0;
		place.bottom = 100;
		place.right = 100;
		place.top = 0;
		hyouji3_mode = false;
		this->t = t;
		has_clicked_forbyouga = false;
		
	};

	virtual ~KoumokuList();
	

	void clicked(Gamen* gamen, GamenPart* gp, int mouse_x, int mouse_y);
	void clickedEnter(Gamen* gamen, GamenPart* gp);
	virtual void clickedUp();
	virtual void clickedDown();
	void moved(int mouse_x, int mouse_y);
	void setVisible(Texture* t, bool tt);
	void setEnable(bool t);
	bool getEnable() {
		return is_enable;
	}
	bool getVisible() {
		return is_visible;
	}


	void setEnableKoumoku(int index);
	void setDisableKoumoku(int index);
	void setSize(Texture* t, int x, int y, int width, int height);
	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp);
	void setHyouji3Mode(bool t) {
		hyouji3_mode = t;
	}
	void setFocusedKoumokuHyouji3Mode(Koumoku* kk, int index);
	bool getHyouji3Mode() {
		return hyouji3_mode;
	}

	void setname(char* na) {
		int len = strlen(na);
		for (int i=0;i<127 && i < len;i++) {
			title[i] = na[i];
		}
		title[127] = 0;
	}
	void setKoumoku(Koumoku* k ) {
		koumokus.push_back(k);
	}
	virtual bool hasLoad() {
		return true;
	}
	virtual void load(Graphics* g, MyTextureLoader* loader){};
};

}

#endif