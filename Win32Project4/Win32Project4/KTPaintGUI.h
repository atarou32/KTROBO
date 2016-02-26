#ifndef KTPAINTGUI_H
#define KTPAINTGUI_H

#pragma once
//#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#define KTPAINT_HSV_S_MAX 100
#define KTPAINT_HSV_S_MIN 0
#define KTPAINT_HSV_V_MAX 100
#define KTPAINT_HSV_V_MIN 0
#define KTPAINT_HSV_H_MAX 360
#define KTPAINT_HSV_H_MIN 0


struct MYTRIANGLEPOINT {
	int x;
	int y;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class KTPaintGUI
{
public:
	KTPaintGUI(void);
	~KTPaintGUI(void);

	static void startUp();
	static void shutDown();
	void drawRectangleToWindow(HDC hdc, unsigned char r, unsigned char g, unsigned char b, int left, int top, int right, int bottom);
	void drawAkiRectangleToWindow(HDC hdc, int left, int top, int right, int bottom);
	void drawGradiationTriangleToWindow(HDC hdc, MYTRIANGLEPOINT* points);
	void drawGradiationCircleToWindow(HDC hdc, POINT center, float radius);
	void clear(HDC hdc);
};




#endif