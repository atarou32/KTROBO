#ifndef KTROBO_GAMEN_GARAGE_H
#define KTROBO_GAMEN_GARAGE_H
#pragma once
#include "KTRoboGamen.h"

namespace KTROBO {

class Koumoku_Parts_Category : public Koumoku {
public:
	Koumoku_Parts_Category(int clicked_) : Koumoku(clicked_) {
	}
	~Koumoku_Parts_Category();

	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp);// focused_koumoku ��koumoku�ł�byouga�͌Ă΂��
	void focusedByouga(Graphics* g, GUI* gui, float dsecond, int stamp);
	void clickedExe(Gamen* gamen, GamenPart* gp, KoumokuList* kl); // set_enable ��false �̂Ƃ��̓��^�[�����邱�� 
};

class Gamen_GARAGE : public Gamen
{
public:
	Gamen_GARAGE(void);
	~Gamen_GARAGE(void);


	KoumokuList* parts_category_list;
	KoumokuList* parts_leg_category_list;
	KoumokuList* parts_rweapon_category_list;
	KoumokuList* parts_lweapon_category_list;
	KoumokuList* parts_rkata_category_list;
	KoumokuList* parts_lkata_category_list;
	KoumokuList* parts_inside_category_list;



	void byouga(Graphics* g, GUI* gui, float dsecond, int stamp);
};

}

#endif