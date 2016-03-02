#ifndef KTPAINTNURI_H
#define KTPAINTNURI_H
#pragma once
#include "MyDefine.h"
#include "KTRoboGraphics.h"
#include "KTRoboGameError.h"
#include "KTPaintSheet.h"

#include <set>
#include <vector>
#include <map>
using namespace std;

#define OUT_

// タンテン　自己閉　の点も含む


class KTPAINT_kouten {
public:
	int index;
	unsigned short x;
	unsigned short y;
	set<pair<KTPAINT_kouten*, pair<KTPAINT_penkyokuline*, pair<int,int>>>> keiros;
	set<pair<KTPAINT_penkyokuline*, int>> iti; // penkyokulineはbigline, ２個目のintは bigline内のインデックス
	KTPAINT_kouten() {
		index =0 ;
		x = 0;
		y = 0;
	}
};


class KTPAINT_koutens {
public:
	int bigline_id;
	vector<KTPAINT_kouten*> ks;
	KTPAINT_koutens() {
		bigline_id = 0;
	}
};

#define KTPAINT_SHEET_KOUTEN_MAX KTPAINT_SHEET_KYOKULINE_MAX

class KTPaintNuri
{
private:
	void addNewKoutenOfKeiro(int kouten_index, int line1_index,  KTPAINT_penkyokuline* line); 
	bool isKousaLine(KTPAINT_penline* line1, KTPAINT_penline* line2);
	bool isKousaLine2(KTPAINT_penline* line1_mae,KTPAINT_penline* line1_ato, KTPAINT_penline* line2_mae, KTPAINT_penline* line2_ato);
	void makeKouTen(int line1_index, int line2_index, KTPAINT_penkyokuline* bigline1, KTPAINT_penkyokuline* bigline2, KTPAINT_penline* linesdayo);
	void kousaKyokusenAndMakeKouten(KTPAINT_penkyokuline* line1, KTPAINT_penkyokuline* line2, KTPAINT_penline* lines);

	vector<KTPAINT_koutens*> koutenss;
	KTPAINT_kouten koutens[KTPAINT_SHEET_KOUTEN_MAX];
	int kouten_max;
	// 交差しない線に関してはkoutensに持たない ので　交差が判明したときに初めての場合は3点増える

public:
	KTPaintNuri(void);
	~KTPaintNuri(void);
	void koutenShori(KTPAINT_penkyokuline* lines, int penkyoku_line_max, KTPAINT_penline* line_infos);
	void printKouten(KTROBO::Graphics* g, KTPAINT_penline* line_infos);
	void koutenShoriLinePlus(KTPAINT_penkyokuline* new_line, KTPAINT_penkyokuline* lines, int penkyoku_line_max, KTPAINT_penline* line_infos);



};




#endif