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
			pen_width.m[i/4][i%4] = 1;//2*(i+1)/8.0f;
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

#define KTPAINT_SHEET_LINE_MAX 50000

class KTPaintSheet
{
private:
	KTPAINT_enpituline elines[KTPAINT_SHEET_LINE_MAX];
	KTPAINT_penline plines[KTPAINT_SHEET_LINE_MAX];
	int eline_max;
	int pline_max;

	int pline_start_index;
	int pline_end_index;
public:
	KTPaintSheet(void);
	~KTPaintSheet(void);
	void setPlineStart() {
		pline_start_index = pline_max;
	}
	void setPlineEnd() {
		pline_end_index = pline_max;
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