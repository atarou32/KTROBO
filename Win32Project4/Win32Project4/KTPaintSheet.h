#ifndef KTPAINTSHEET_H
#define KTPAINTSHEET_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

class KTPAINT_enpituline {
public:
	unsigned short x;
	unsigned short y;
	char dx;
	char dy;
	unsigned char alpha;
	unsigned char color_index;
};

class KTPAINT_penline {
public:
	unsigned short x;
	unsigned short y;
	char dx;
	char dy;
	unsigned char width;
	unsigned char nwidth;
};

class KTPaintSheet
{
private:
	KTPAINT_enpituline elines[1000];
	KTPAINT_penline plines[1000];
	int eline_max;
	int pline_max;
public:
	KTPaintSheet(void);
	~KTPaintSheet(void);

	int getElineMax() {return eline_max;}
	int getPlineMax() {return pline_max;}
	KTPAINT_enpituline* getEline() {return elines;}
	KTPAINT_penline* getPline() {return plines;}
	void setEline(POINT mpo, POINT po, char alpha, unsigned char color_index);
	void setPline(POINT mpo, POINT po, unsigned char width, unsigned char nwidth);

};






#endif