#ifndef KTPAINTSHEET_H
#define KTPAINTSHEET_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "MyDefine.h"

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

class KTPaintSheet
{
private:
	KTPAINT_enpituline elines[KTPAINT_SHEET_LINE_MAX];
	KTPAINT_penline plines[KTPAINT_SHEET_LINE_MAX];
	KTPAINT_penkyokuline kyoku_plines[KTPAINT_SHEET_KYOKULINE_MAX];
	int eline_max;
	int pline_max;
	int kyokupline_max;
	int pline_start_index;
	int pline_end_index;
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
			kyoku_plines[kyokupline_max].kyoku_index = 0;//bigline‚Í‚O‚É‚·‚é
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
	// ‘¾‚³‚Í16’iŠK‚É•ª‚©‚ê‚é
	void setPline(POINT mpo, POINT po, unsigned char width, unsigned char nwidth, unsigned char pen_index);
	void clearP() {
		pline_max = 0;
	}
};






#endif