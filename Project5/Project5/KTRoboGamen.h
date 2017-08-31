#ifndef KTROBO_GAMEN_H
#define KTROBO_GAMEN_H

#pragma once
#include "KTRoboGraphics.h"
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
};

class GamenPart {
public:
	GamenPart() {};
	~GamenPart() {};

	virtual void byouga(Graphics* g, GUI* gui, float dsecond, int stamp)=0;
};


class KoumokuList;
class Koumoku {
private:
	bool is_enable;
	MYRECT place;
	int clicked_id;
protected:
	int gui_koumoku_name_id;
	int gui_koumoku_name_bg_id;
public:
	Koumoku(int clicked_) {is_enable = true;
		place.left = 0;
		place.right = 100;
		place.top = 0;
		place.bottom = 100;
		clicked_id = clicked_;
		gui_koumoku_name_id = 0;
		gui_koumoku_name_bg_id = 0;
	};
	~Koumoku() {};

	bool clicked(int mouse_x, int mouse_y);
	bool moved(int mouse_x, int mouse_y);
	void setEnabled(bool t);
	bool getEnabled() {return is_enable;}
	void setSize(int x, int y, int width, int height);
	int getID();

	virtual void byouga(Graphics* g, GUI* gui, float dsecond, int stamp)=0;// focused_koumoku ��koumoku�ł�byouga�͌Ă΂��
	virtual void focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp)=0;
	virtual void clickedExe(Gamen* gamen, GamenPart* gp, KoumokuList* kl)=0; // set_enable ��false �̂Ƃ��̓��^�[�����邱�� 
};
class KoumokuList {
private:
	int cursor;
	vector<Koumoku*> koumokus;
	Koumoku* focused_koumoku;
	bool is_visible;
	bool is_enable;
	char title[128];
	MYRECT place;
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
	KoumokuList() {
		for(int i=0;i<128;i++) {
			title[i] = 0;
		}
		cursor = 0; focused_koumoku=0; is_visible = true; is_enable = true;
		place.left = 0;
		place.bottom = 100;
		place.right = 100;
		place.top = 0;
	};

	~KoumokuList() {
		vector<Koumoku*>::iterator it = koumokus.begin();
		while(it != koumokus.end()) {
			Koumoku* k = *it;
			delete k;
			k = 0;

			it ++;
		}
		koumokus.clear();
	
	};

	void clicked(Gamen* gamen, GamenPart* gp, int mouse_x, int mouse_y);
	void clickedEnter(Gamen* gamen, GamenPart* gp);
	void clickedUp();
	void clickedDown();
	void moved(int mouse_x, int mouse_y);
	void setVisible(bool t);
	void setEnable(bool t);
	void setEnableKoumoku(int index);
	void setDisableKoumoku(int index);
	void setSize(int x, int y, int width, int height);
	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp);
	void setname(char* na) {
		int len = strlen(na);
		for (int i=0;i<127 && i < len;i++) {
			title[i] = na[i];
		}
		title[127] = 0;
	}
};

}

#endif