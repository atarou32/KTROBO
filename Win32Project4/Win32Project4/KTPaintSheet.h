#ifndef KTPAINTSHEET_H
#define KTPAINTSHEET_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "MyDefine.h"
#include <map>
using namespace std;
class KTPaintNuri;
class KTPAINT_kouten;

class KTPAINT_enpituline {
public:
	unsigned short x;
	unsigned short y;
	char dx;
	char dy;
	unsigned char alpha;
	unsigned char color_index;
};


#define KTPAINT_PEN_NUM_MAX 16
class KTPAINT_pen {
public:
	MYMATRIX pen_width;
	MYMATRIX pen_width_calcurator;
	unsigned char getWidthIndexFromPressure(UINT pressure) {
		unsigned char ans = 0;
		for (int i=0;i<16;i++) {
			if (pen_width_calcurator.m[i/4][i%4] > pressure) {
				return ans;
			} else {
				ans = i;
			}
		}
		return ans;
	}

	KTPAINT_pen() {
		for (int i=0;i<16;i++) {
			pen_width.m[i/4][i%4] = 1+1*(i+1)/8.0f;
			pen_width_calcurator.m[i/4][i%4] = 300+100*i;
		}
	}

};


class KTPAINT_penline {
public:
	unsigned short x;
	unsigned short y;
	char dx;
	char dy;
	unsigned char width_and_nwidth;
	unsigned char pen_index;
};

class KTPAINT_penkyokuline {
public:
	unsigned short kyoku_index;
	unsigned short start_index;
	unsigned short end_index;
	unsigned short kyoku_id;
};

#define KTPAINT_SHEET_LINE_MAX 50000
#define KTPAINT_SHEET_KYOKULINE_MAX 2048

class KTPAINT_penheiryouikipart {
public:
	unsigned short kouten_x;
	unsigned short kouten_y;
	unsigned short keiro_first_index;
	unsigned short keiro_last_index;
	unsigned short kouten_index;
	unsigned short keiro_index;// kouten ÇÃâΩî‘ñ⁄ÇÃåoòHÇ©Ç∆Ç¢Ç§Ç±Ç∆Å@kyokuline_indexÇÕ keiro_first_index, keiro_last_index Ç©ÇÁêÑë™Ç≈Ç´ÇÈÇÕÇ∏ÅH
};

class KTPAINT_penheiryouiki {
public:
	unsigned short startheiryouiki;
	unsigned short endheiryouiki;
	unsigned short daen_index;
	unsigned short daen_calced;//0 mada 1 sudeni
	unsigned short daen_many_start;
	unsigned short daen_many_end;
	unsigned short hen_id;
	unsigned short offset;
};

class KTPAINT_penheiryouikidaen {
public:
	unsigned short heiryouiki_index;
	unsigned short width;
	unsigned short height;
	unsigned short x;
	unsigned short y;
	unsigned short index;
	float theta;
};

#define KTPAINT_PENHEIRYOUIKI_MAX 4096
#define KTPAINT_PENHEIRYOUIKI_PART_MAX 4096*4
#define KTPAINT_PENHEIRYOUIKI_DAEN_MAX 4096

class KTPaintSheet
{
private:
	KTPAINT_penheiryouikipart hei_part[KTPAINT_PENHEIRYOUIKI_PART_MAX];
	KTPAINT_penheiryouiki hei[KTPAINT_PENHEIRYOUIKI_MAX];
	KTPAINT_penheiryouikidaen hei_daen[KTPAINT_PENHEIRYOUIKI_DAEN_MAX];
	map<pair<int, int>, int> oyakokankei;// KTPAINT_HEI* KTPAINT_HEI* int
	// int 0 ç≈èâ Ç™oya  1 ç≈å„Ç™oya 2 ïsñæ

	int hei_daen_max;
	int hei_part_max;
	int hei_max;
	KTPAINT_enpituline elines[KTPAINT_SHEET_LINE_MAX];
	KTPAINT_penline plines[KTPAINT_SHEET_LINE_MAX];
	KTPAINT_penkyokuline kyoku_plines[KTPAINT_SHEET_KYOKULINE_MAX];
	int eline_max;
	int pline_max;
	int kyokupline_max;
	int pline_start_index;
	int pline_end_index;

	bool oyakoKankeiHeiryouiki(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPAINT_penheiryouiki* ryou3, KTPAINT_penheiryouiki** out_oya_ryou, KTPaintNuri* nuri);
	void motomeruJyusin(KTPAINT_penheiryouiki* ryou, KTPAINT_penheiryouikidaen* daen);
	bool addTempPartsCount(KTPAINT_penheiryouikipart* temp_parts,int* temp_c,int maxx,KTPAINT_penheiryouiki* ryou1);
	int karuiOyakoHantei(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPaintNuri* nuri);
	bool isInHeiryouiki(unsigned short x, unsigned short y, KTPAINT_penheiryouiki* heid, KTPaintNuri* nuri); 
	void insertOyakoKankei(int te12, KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2);
	int getTempHeiFromSitenAndID(KTPAINT_kouten* siten, KTPAINT_kouten* koutens, int* keiro_indexs,int keiro_ID);
	void tryTourokuTempHeiToHei(KTPAINT_penheiryouiki* temp_hei, KTPAINT_penheiryouikipart* temp_heipart);
public:
	KTPaintSheet(void);
	~KTPaintSheet(void);
	KTPAINT_penkyokuline* getKyokuPLines() {return kyoku_plines;}
	int getKyokuPLineMax() {return kyokupline_max;}
	KTPAINT_penkyokuline* getLastKyokuPLine() {
		if (kyokupline_max) {
			return &kyoku_plines[kyokupline_max-1];
		} else {
			return &kyoku_plines[0];
		}
	}
	void setPlineStart() {
		pline_start_index = pline_max;
	}
	void setPlineEnd() {
		pline_end_index = pline_max;
		if (kyokupline_max < KTPAINT_SHEET_KYOKULINE_MAX) {
			kyoku_plines[kyokupline_max].start_index = pline_start_index;
			kyoku_plines[kyokupline_max].end_index = pline_end_index;
			if (kyoku_plines[kyokupline_max].end_index) {
				kyoku_plines[kyokupline_max].end_index--;
			}
			kyoku_plines[kyokupline_max].kyoku_index = 0;//biglineÇÕÇOÇ…Ç∑ÇÈ
			kyoku_plines[kyokupline_max].kyoku_id = kyokupline_max;
			kyokupline_max++;
		}
	}
	void heikinPline();
	int getElineMax() {return eline_max;}
	int getPlineMax() {return pline_max;}
	KTPAINT_enpituline* getEline() {return elines;}
	KTPAINT_penline* getPline() {return plines;}
	void setEline(POINT mpo, POINT po, char alpha, unsigned char color_index);
	// ëæÇ≥ÇÕ16íiäKÇ…ï™Ç©ÇÍÇÈ
	void setPline(POINT mpo, POINT po, unsigned char width, unsigned char nwidth, unsigned char pen_index);
	void clearP() {
		pline_max = 0;
	}
	void calcHeiryouiki(KTPaintNuri* nuri);
	int bunkatuDaenWithLine(int hei_index, int start_index, int end_index,int start_daen_index, int end_daen_index);
	KTPAINT_penheiryouiki* getHei() {return hei;}
	KTPAINT_penheiryouikipart* getHeiPart() {return hei_part;}
	KTPAINT_penheiryouikidaen* getHeiDaen() {return hei_daen;}
	int getHeiMax() {return hei_max;}
};






#endif