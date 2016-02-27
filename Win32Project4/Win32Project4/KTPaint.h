#ifndef KTPAINT_H
#define KTPAINT_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "KTRoboGraphics.h"
#include "KTPaintGUI.h"
#include "KTPaintSheet.h"

#define KTPAINT_PEN_ID 1
#define KTPAINT_PENCIL_ID 2
#define KTPAINT_NURI_ID 3
#define KTPAINT_ERASER_ID 4
class KTPaint
{
private:
	HINSTANCE hInst;
	HWND parent_window;
	HWND colorpen_window;
	HWND loadsave_window;
	HCURSOR temp_cursor;
	int now_paint_id;
	KTROBO::Graphics* g;
	KTPaintGUI gui;
	KTPAINT_pen pens[KTPAINT_PEN_NUM_MAX];
	int now_pen_index;
	KTPaintSheet sheet;
	short transx;
	short transy;
	float zoom;
	bool is_activate;
	HDC hdcMem;
public:
	short getTransX() {return transx;}
	short getTransY() {return transy;}
	void setTransX(short x) {transx = x;g->setPenInfo(g,transx,transy,zoom,pens);}
	void setTransY(short y) {transy = y;g->setPenInfo(g,transx,transy,zoom,pens);}
	float getZoom() {return zoom;}
	void setZoom(float z) {zoom = z;g->setPenInfo(g,transx,transy,zoom,pens);}

	float gradiation_circle_radius;
	float gradiation_circle_theta;
	POINT gradiation_triangle_pos;
	unsigned char now_gcolor_r;
	unsigned char now_gcolor_g;
	unsigned char now_gcolor_b;

	unsigned char now_color_r;
	unsigned char now_color_g;
	unsigned char now_color_b;
public:
	HDC getHdcMem() {return hdcMem;}
	KTPaintGUI* getGUI() {return &gui;}
	void createKoWindow(HWND hWnd);
	void paint();
	KTPaint(HINSTANCE hins);
	~KTPaint(void);
	void Init(HWND hwnd);
	void Release();
	void setCursorNow();
	void setCursorToPencil();
	void setCursorToPen();
	void setCursorToNuri();
	void setCursorToEraser();
	void setGColorTheta(ULONG mouse_x, ULONG mouse_y);
	bool isInCircleColorPen(ULONG mouse_x, ULONG mouse_y);
	bool isInTriangleColorPen(ULONG mouse_x, ULONG mouse_y);
	void setNowColor(COLORREF c);
	void setNowGColor(COLORREF c);

	void writeWithPen(POINT mpo, POINT po, UINT pressure_old, UINT pressure_new, bool reset);
	void render();
	void activate(){is_activate = true;}
	void deactivate() {is_activate=false;}

};

LRESULT CALLBACK ColorPenWindowProc(HWND , UINT , WPARAM , LPARAM );
LRESULT CALLBACK LoadSaveWindowProc(HWND , UINT , WPARAM , LPARAM );

#endif