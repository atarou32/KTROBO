#ifndef KTPAINTSHEET_H
#define KTPAINTSHEET_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "MyDefine.h"
#include <map>
#include <set>
#include <vector>
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

	void setPenWidth(int pen_index);

	KTPAINT_pen() {

		for (int i=0;i<16;i++) {
			pen_width.m[i/4][i%4] = /*標準ペン4+1*i/12.0f;*//*細ペン2+0.2*i/15.0f;*//*Gペン2+4.2*i/15.0f;*///2+0.8*i/4.00f;//2+1*(i+1)/8.0f;
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
#define KTPAINT_SHEET_LINE_MAX  4096*6
#define KTPAINT_SHEET_DUMMY_MAX  4096
#define KTPAINT_SHEET_KYOKULINE_MAX 2048

class KTPAINT_penheiryouikipart {
public:
	unsigned short kouten_x;
	unsigned short kouten_y;
	unsigned short keiro_first_index;
	unsigned short keiro_last_index;
	unsigned short kouten_index;
	unsigned short keiro_index;// kouten の何番目の経路かということ　kyokuline_indexは keiro_first_index, keiro_last_index から推測できるはず？
};

class KTPAINT_penheiryouiki {
public:
	unsigned short startheiryouiki;
	unsigned short endheiryouiki;
	unsigned short daen_index;
	unsigned short hen_id;
	unsigned short is_use_pline; // 0 use_heipline, 1 use_pline
	unsigned short is_transport; // 0 transport 1 no transport
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
#define KTPAINT_PENHEIRYOUIKI_PART_MAX  2048
#define KTPAINT_PENHEIRYOUIKI_DAEN_MAX  2048
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_MAX 4096*2*2
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX 64
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_RADIUS_SAISHO 12
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_FUKUMANAI 65535

#define KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_UNUSE 0
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_MAIN 1
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_SUB 2
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONLINE 3
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_ONKOUTEN 3
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_STATUS_LABELED 4

#define KTPAINT_PENHEIRYOUIKI_BUBBLE_LABEL_MAX 65535
#define KTPAINT_PENHEIRYOUIKI_BUBBLE_HEI_MAX 1024
class KTPAINT_bubble {
public:
	unsigned short x;
	unsigned short y;
	float radius;
	KTPAINT_bubble* ketugou[4];
	unsigned short fukumuten[KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX];
	unsigned short fukumuten_first[KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX];
	unsigned short fukumuten_last[KTPAINT_PENHEIRYOUIKI_BUBBLE_FUKUMUTEN_MAX];
	unsigned short label_index_tate;
	unsigned short label_index_yoko;
	unsigned short status;
	unsigned short hei_index;
};

class KTPAINT_bubblehei {
public:
	unsigned short label;
	set<unsigned short> fukumu_kouten_indexs;
	set<pair<pair<unsigned short,unsigned short>,pair<unsigned short, unsigned short>>> fukumu_kouten_indexs_fl;
	//kouten_x,kouten_y とfirst_index,last_index
};






class KTPaintSheet
{
private:

	KTPAINT_penheiryouikipart hei_part[KTPAINT_PENHEIRYOUIKI_PART_MAX];
	KTPAINT_penheiryouiki hei[KTPAINT_PENHEIRYOUIKI_MAX];
	KTPAINT_penheiryouikidaen hei_daen[KTPAINT_PENHEIRYOUIKI_DAEN_MAX];
	map<pair<int, int>, int> oyakokankei;// KTPAINT_HEI* KTPAINT_HEI* int
	// int 0 最初 がoya  1 最後がoya 2 不明

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

	int undo_pline_max;
	int undo_kyokupline_max;
	int undo_hei_max;
	int undo_hei_part_max;

	bool oyakoKankeiHeiryouiki(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, 
		KTPAINT_penheiryouiki** out_oya_ryou, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts);
	void motomeruJyusin(KTPAINT_penheiryouiki* ryou, KTPAINT_penheiryouikidaen* daen, KTPAINT_penheiryouikipart* parts);
	void motomeruJyusinPline(KTPAINT_penheiryouiki* ryou, KTPAINT_penheiryouikidaen* daen, KTPAINT_penheiryouikipart* parts);
	bool addTempPartsCount(KTPAINT_penheiryouikipart* temp_parts,int* temp_c,int maxx,KTPAINT_penheiryouiki* ryou1,KTPAINT_penheiryouikipart* parts);
	int karuiOyakoHantei(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts);
	int tyuuOyakoHantei(KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2, KTPaintNuri* nuri, KTPAINT_penheiryouikipart* parts);


	bool isInHeiryouiki(unsigned short x, unsigned short y, KTPAINT_penheiryouiki* heid,  KTPAINT_penheiryouikipart* parts); 
	bool isInHeiryouikiPline(unsigned short x, unsigned short y, KTPAINT_penheiryouiki* heid,  KTPAINT_penheiryouikipart* parts);

	void insertOyakoKankei(int te12, KTPAINT_penheiryouiki* ryou1, KTPAINT_penheiryouiki* ryou2);
	int getTempHeiFromSitenAndID(KTPAINT_kouten* siten, KTPAINT_kouten* koutens, int* keiro_indexs,int keiro_ID);
	int getTempHeiFromSitenAndID2(KTPAINT_kouten* siten, KTPAINT_kouten** mae_k, KTPAINT_kouten** kk,  KTPAINT_kouten* koutens, 
		int* keiro_indexs,KTPAINT_kouten** temp_koutens,int* keiro_depth);
	
	int getTempHeiFromSitenAndID3(KTPAINT_kouten* siten, KTPAINT_kouten** mae_k, KTPAINT_kouten** kk, 
											KTPAINT_kouten* koutens, int* keiro_indexs,
											KTPAINT_kouten** temp_koutens, int* keiro_depth,
											KTPAINT_bubblehei* heis);
	int getTempHeiFromSitenAndID4(KTPAINT_kouten* siten, KTPAINT_kouten** mae_k, KTPAINT_kouten** kk, 
											KTPAINT_kouten* koutens, int* keiro_indexs,
											KTPAINT_kouten** temp_koutens, int* keiro_depth,
											KTPAINT_bubblehei* heis);
	
	
	
	
	
	bool tryTourokuTempHeiToHei(KTPAINT_penheiryouiki* temp_hei, KTPAINT_penheiryouikipart* temp_heipart,
		DWORD color, bool onaji_check=true, unsigned short is_pline=0);
	bool labeldukeBubble(KTPAINT_bubble* bubble, unsigned short label_index, unsigned short count, KTPAINT_bubblehei* heis, KTPAINT_kouten* koutens); 
public:
	void simulationStepStart(unsigned short width, unsigned short height,KTPaintNuri* nuri);
	void simulationBubbleStep(unsigned short width, unsigned short height, KTPaintNuri* nuri, short transx, short transy, float zoom);
	void copyto(KTPaintSheet* dest);
	void copyHeiTo(KTPaintSheet* dest);

public:
	KTPaintSheet(void);
	~KTPaintSheet(void);
	void erase(POINT po);

	void resetUndoHei() {
		undo_hei_max =0;
		undo_hei_part_max = 0;
	}
	void enpituClear() {
		eline_max = 0;
	}

	void undoHei() {
		if (hei_max) {
			if (undo_hei_max < hei_max) {
			undo_hei_max = hei_max;
			undo_hei_part_max = hei_part_max;
			}
			hei_max--;
			hei_part_max -= hei[hei_max].endheiryouiki - hei[hei_max].startheiryouiki-1;
		}
	}

	void redoHei() {

		if (undo_hei_max) {
			if (undo_hei_max > hei_max) {
				hei_max++;
				hei_part_max += hei[hei_max-1].endheiryouiki - hei[hei_max-1].startheiryouiki + 1;
			}
		}
	}

	void redoPline() {
		if (undo_kyokupline_max) {
			if (undo_kyokupline_max > kyokupline_max) {
				kyokupline_max++;
				pline_max += kyoku_plines[kyokupline_max-1].end_index - kyoku_plines[kyokupline_max-1].start_index + 1;

			}
		}
	}
	void undoPline() {
		if (this->kyokupline_max) {
			if (undo_kyokupline_max < kyokupline_max) {
				undo_kyokupline_max = kyokupline_max;
				undo_pline_max = pline_max;
			}
			pline_max = kyoku_plines[kyokupline_max-1].start_index;
			unsigned short start_inde = kyoku_plines[kyokupline_max-1].start_index;
			unsigned short end_inde = kyoku_plines[kyokupline_max-1].end_index;

			kyokupline_max--;
			// start_indexからend_indexまで閉領域が使ってる場合は閉領域を消してしまう
			for (int i=0;i<hei_max;i++) {
				if (!hei[i].is_use_pline) continue;
				if (!hei[i].is_transport) {
					for (int k=hei[i].startheiryouiki;k<hei[i].endheiryouiki;k++) {
						if (start_inde <= hei_part[k].keiro_first_index <= end_inde) {
							hei[i].is_transport = 1;
							break;
						}
						if (start_inde <= hei_part[k].keiro_last_index <= end_inde) {
							hei[i].is_transport = 1;
							break;
						}
					}
				}
			}
		}
	}
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
		undo_kyokupline_max = 0;
		undo_pline_max = 0;
	}
	void setPlineEnd() {
		pline_end_index = pline_max;
		if (kyokupline_max < KTPAINT_SHEET_KYOKULINE_MAX) {
			kyoku_plines[kyokupline_max].start_index = pline_start_index;
			kyoku_plines[kyokupline_max].end_index = pline_end_index;
			if (kyoku_plines[kyokupline_max].end_index) {
				kyoku_plines[kyokupline_max].end_index--;
			}
			kyoku_plines[kyokupline_max].kyoku_index = 0;//biglineは０にする
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
			hei_kyoku_plines[hei_kyokupline_max].kyoku_index = 0;//biglineは０にする
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
	// 太さは16段階に分かれる
	void setPline(POINT mpo, POINT po, unsigned char width, unsigned char nwidth, unsigned char pen_index);
	void setHeiPline(POINT mpo, POINT po, unsigned char width, unsigned char nwidth, unsigned char pen_index);
	void clearP() {
		pline_max = 0;
	}
	void calcHeiryouiki(KTPaintNuri* nuri, KTPAINT_bubblehei* hei, DWORD color, POINT po);
	bool calcHeiryouikiPlus(KTPaintNuri* nuri, DWORD color);// trueなら閉領域が増えたことを意味する
	int bunkatuDaenWithLine(int hei_index, int start_index, int end_index,int start_daen_index, int end_daen_index);
	KTPAINT_penheiryouiki* getHei() {return hei;}
	KTPAINT_penheiryouikipart* getHeiPart() {return hei_part;}
	KTPAINT_penheiryouikidaen* getHeiDaen() {return hei_daen;}
	int getHeiMax() {return hei_max;}
	int getHeiKyokuPLineMax() {return hei_kyokupline_max;}
	int getHeiPLineMax() {return hei_pline_max;}
	KTPAINT_penline* getHeiPLine() {return hei_plines;}
	KTPAINT_penkyokuline* getHeiKyokuPLine() {return hei_kyoku_plines;}
	int getHeiDaenMax() {return hei_daen_max;}
	int getHeiPartMax() {return hei_part_max;}
};






#endif