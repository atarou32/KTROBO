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

	KTPAINT_pen(int index) {

		if (index ==1) {
			for (int i=0;i<16;i++) {
			pen_width.m[i/4][i%4] = 4+4*(i+1)/8.0f;
			pen_width_calcurator.m[i/4][i%4] = 300+(400+i*100)*i;
			}
			return;
		}

		for (int i=0;i<16;i++) {
			pen_width.m[i/4][i%4] = 2+1*(i+1)/8.0f;
			pen_width_calcurator.m[i/4][i%4] = 300+(100)*i;
		}
	}

	KTPAINT_pen() {

		for (int i=0;i<16;i++) {
			pen_width.m[i/4][i%4] = 2+0.8*i/4.00f;//2+1*(i+1)/8.0f;
			pen_width_calcurator.m[i/4][i%4] = 300+(30)*i;
		}
			pen_width_calcurator.m[0][0] = 400;
			pen_width_calcurator.m[0][1] = 500;
			pen_width_calcurator.m[0][2] = 550;
			pen_width_calcurator.m[0][3] = 570;
			pen_width_calcurator.m[1][0] = 590;
			pen_width_calcurator.m[1][1] = 610;
			pen_width_calcurator.m[1][2] = 630;
			pen_width_calcurator.m[1][3] = 650;
			pen_width_calcurator.m[2][0] = 700;
			pen_width_calcurator.m[2][1] = 730;
			pen_width_calcurator.m[2][2] = 760;
			pen_width_calcurator.m[2][3] = 790;
			pen_width_calcurator.m[3][0] = 820;
			pen_width_calcurator.m[3][1] = 850;
			pen_width_calcurator.m[3][2] = 880;
			pen_width_calcurator.m[3][3] = 1024;
		
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

#define KTPAINT_SHEET_HEILINE_MAX 4096*6
#define KTPAINT_SHEET_LINE_MAX 4096*6
#define KTPAINT_SHEET_DUMMY_MAX 4096
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
	unsigned short hen_id;
	DWORD color;
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

#define KTPAINT_PENHEIRYOUIKI_MAX 512
#define KTPAINT_PENHEIRYOUIKI_PART_MAX 2048
#define KTPAINT_PENHEIRYOUIKI_DAEN_MAX 512
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_MAX 4096*2
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX 16
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_RADIUS_SAISHO 10
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI 65535

#define KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_UNUSE 0
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_MAIN 1
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_SUB 2

class KTPAINT_bubble {
public:
	unsigned short x;
	unsigned short y;
	float radius;
	KTPAINT_bubble* ketugou[8];
	unsigned short fukumuten[KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX];
	unsigned short oya_ketugou;
	unsigned short status;
};







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
	int hei_pline_max;
	int hei_kyokupline_max;
	KTPAINT_penline hei_plines[KTPAINT_SHEET_HEILINE_MAX];
	KTPAINT_penkyokuline hei_kyoku_plines[KTPAINT_SHEET_KYOKULINE_MAX];
	KTPAINT_enpituline elines[KTPAINT_SHEET_DUMMY_MAX];
	KTPAINT_penline plines[KTPAINT_SHEET_LINE_MAX];
	KTPAINT_penkyokuline kyoku_plines[KTPAINT_SHEET_KYOKULINE_MAX];
	int eline_max;
	int pline_max;
	int kyokupline_max;
	int pline_start_index;
	int pline_end_index;

	bool oyakoKankeiHeiryouiki(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPAINT_penheiryouiki* ryou3, 
		KTPAINT_penheiryouiki** out_oya_ryou, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts);
	void motomeruJyusin(KTPAINT_penheiryouiki* ryou, KTPAINT_penheiryouikidaen* daen, KTPAINT_penheiryouikipart* parts);
	bool addTempPartsCount(KTPAINT_penheiryouikipart* temp_parts,int* temp_c,int maxx,KTPAINT_penheiryouiki* ryou1,KTPAINT_penheiryouikipart* parts);
	int karuiOyakoHantei(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts);
	bool isInHeiryouiki(unsigned short x, unsigned short y, KTPAINT_penheiryouiki* heid, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts); 
	void insertOyakoKankei(int te12, KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2);
	int getTempHeiFromSitenAndID(KTPAINT_kouten* siten, KTPAINT_kouten* koutens, int* keiro_indexs,int keiro_ID);
	int getTempHeiFromSitenAndID2(KTPAINT_kouten* siten, KTPAINT_kouten** mae_k, KTPAINT_kouten** kk,  KTPAINT_kouten* koutens, 
		int* keiro_indexs,KTPAINT_kouten** temp_koutens,int* keiro_depth);
	bool tryTourokuTempHeiToHei(KTPAINT_penheiryouiki* temp_hei, KTPAINT_penheiryouikipart* temp_heipart, DWORD color, bool onaji_check=true);

public:
	void simulationStepStart(unsigned short width, unsigned short height,KTPaintNuri* nuri);
	void simulationBubbleStep(short min_d, short max_d, float radius_d, KTPaintNuri* nuri);

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
	void setHeiPlineStart() {
		pline_start_index = hei_pline_max;
	}
	int getHeiPlineStart() {
		return pline_start_index;
	}

	void setHeiPlineEnd() {
		pline_end_index = hei_pline_max;
		if (hei_kyokupline_max < KTPAINT_SHEET_KYOKULINE_MAX) {
			hei_kyoku_plines[hei_kyokupline_max].start_index = pline_start_index;
			hei_kyoku_plines[hei_kyokupline_max].end_index = pline_end_index;
			if (hei_kyoku_plines[hei_kyokupline_max].end_index) {
				hei_kyoku_plines[hei_kyokupline_max].end_index--;
			}
			hei_kyoku_plines[hei_kyokupline_max].kyoku_index = 0;//biglineÇÕÇOÇ…Ç∑ÇÈ
			hei_kyoku_plines[hei_kyokupline_max].kyoku_id = hei_kyokupline_max;
			hei_kyokupline_max++;
		}
	}
	void heikinPline();
	int getElineMax() {return eline_max;}
	int getPlineMax() {return pline_max;}
	KTPAINT_enpituline* getEline() {return elines;}
	KTPAINT_penline* getPline() {return plines;}
	void setEline(POINT mpo, POINT po, unsigned char alpha, unsigned char color_index);
	// ëæÇ≥ÇÕ16íiäKÇ…ï™Ç©ÇÍÇÈ
	void setPline(POINT mpo, POINT po, unsigned char width, unsigned char nwidth, unsigned char pen_index);
	void setHeiPline(POINT mpo, POINT po, unsigned char width, unsigned char nwidth, unsigned char pen_index);
	void clearP() {
		pline_max = 0;
	}
	void calcHeiryouiki(KTPaintNuri* nuri, DWORD color);
	bool calcHeiryouikiPlus(KTPaintNuri* nuri, DWORD color);// trueÇ»ÇÁï¬óÃàÊÇ™ëùÇ¶ÇΩÇ±Ç∆Çà”ñ°Ç∑ÇÈ
	int bunkatuDaenWithLine(int hei_index, int start_index, int end_index,int start_daen_index, int end_daen_index);
	KTPAINT_penheiryouiki* getHei() {return hei;}
	KTPAINT_penheiryouikipart* getHeiPart() {return hei_part;}
	KTPAINT_penheiryouikidaen* getHeiDaen() {return hei_daen;}
	int getHeiMax() {return hei_max;}
	int getHeiKyokuPLineMax() {return hei_kyokupline_max;}
	int getHeiPLineMax() {return hei_pline_max;}
	KTPAINT_penline* getHeiPLine() {return hei_plines;}
	KTPAINT_penkyokuline* getHeiKyokuPLine() {return hei_kyoku_plines;}
};






#endif