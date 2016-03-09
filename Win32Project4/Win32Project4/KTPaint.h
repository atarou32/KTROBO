#ifndef KTPAINT_H
#define KTPAINT_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "KTRoboGraphics.h"
#include "KTPaintGUI.h"
#include "KTPaintSheet.h"
#include "MyTextureLoader.h"
#include "KTPaintNuri.h"
#include "KTPaintDouga.h"

#define KTPAINT_PEN_ID 1
#define KTPAINT_PENCIL_ID 2
#define KTPAINT_NURI_ID 3
#define KTPAINT_ERASER_ID 4
#define KTPAINT_HEIPEN_ID 5

class KTPaintSheetList {
public:
	short transx;
	short transy;
	float zoom;
	int count;
	KTPaintSheet* now_sheet;
	KTPaintSheetList* mae_sheet;
	KTPaintSheetList* next_sheet;
	KTPaintSheetList() {
		transx = 0;
		transy = 0;
		zoom = 1;
		now_sheet = 0;
		mae_sheet =0;
		next_sheet = 0;
		count = 1;
	}
};

class KTPaint
{
private:
	HINSTANCE hInst;
public:
	HWND parent_window;
	HWND colorpen_window;
	HWND loadsave_window;
private:
	HCURSOR temp_cursor;
	int now_paint_id;
	KTROBO::Graphics* g;
	KTPaintGUI gui;
public:
	KTPaintDouga douga;
private:
	KTPAINT_pen pens[KTPAINT_PEN_NUM_MAX];
	int now_pen_index;
	int now_penkyokuline_start;
	int temp_pressure;
	vector<KTPaintSheetList*> sheets;
	KTPaintSheetList* now_sheet;
	KTPaintSheetList* root_sheet;
	bool is_activate;
	bool is_render_next_sheet;
	bool is_render_before_sheet;
	bool is_render_pencil_line;
public:
	bool is_mode_dougasaisei;
	bool is_mode_pausedougabyouga;
	void setModeByougaSeisiDouga(bool t) {
		is_mode_pausedougabyouga = t;
		if (t) {
			// ÃŽ~“®‰æ‚ð•`‰æ‚³‚¹‚é
			SendMessage(parent_window, WM_PAINT,0,0);
		}
		
	}
	
private:
	int now_count;
	HDC hdcMem;
	MyTextureLoader loader;
	MyTextureLoader::MY_TEXTURE_CLASS* tex_class;
	MyTextureLoader::MY_TEXTURE_CLASS* tex_class_back_buffer;
	MyTextureLoader::MY_TEXTURE_CLASS* tex_class2;
	MyTextureLoader::MY_TEXTURE_CLASS* render_tex_class;
	MyTextureLoader::MY_TEXTURE_CLASS* back_tex_class;
	MyTextureLoader::MY_TEXTURE_CLASS* exam_class;
	KTPaintNuri nuridayo;
public:
	void setIsRenderPencilLine(bool t) {
		is_render_pencil_line = t;
	}

	bool getIsRenderNextSheet() {
		return is_render_next_sheet;
	}
	bool getIsRenderBeforeSheet() {
		return is_render_before_sheet;
	}
	void setIsRenderNextSheet(bool t) {
		is_render_next_sheet = t;
	}
	void setIsRenderBeforeSheet(bool t) {
		is_render_before_sheet = t;
	}

	HINSTANCE getHInst() {return hInst;}
	short getTransX() {return now_sheet->transx;}
	short getTransY() {return now_sheet->transy;}
	void setTransX(short x) {now_sheet->transx = x;g->setPenInfo(g,g->getScreenWidth(),g->getScreenHeight(),now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);}
	void setTransY(short y) {now_sheet->transy = y;g->setPenInfo(g,g->getScreenWidth(),g->getScreenHeight(),now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);}
	float getZoom() {return now_sheet->zoom;}
	void setZoom(float z) {now_sheet->zoom = z;g->setPenInfo(g,g->getScreenWidth(),g->getScreenHeight(),now_sheet->transx,now_sheet->transy,now_sheet->zoom,pens);}

	float gradiation_circle_radius;
	float gradiation_circle_theta;
	POINT gradiation_triangle_pos;
	unsigned char now_gcolor_r;
	unsigned char now_gcolor_g;
	unsigned char now_gcolor_b;

	unsigned char now_color_r;
	unsigned char now_color_g;
	unsigned char now_color_b;


	HWND sheet_index_label;
	HWND combo;

public:
	void setSheetIndexLabel(HWND h) {
		sheet_index_label = h;
	}
	void setCombo(HWND h) {
		combo = h;
	}
	HDC getHdcMem() {return hdcMem;}
	KTPaintGUI* getGUI() {return &gui;}
	void createKoWindow(HWND hWnd);
	void paint();
	KTPaint(HINSTANCE hins);
	~KTPaint(void);
	void Init(HWND hwnd,HINSTANCE hInst,int nCmdShow);
	void Release();
	void setCursorNow();
	void setCursorToPencil();
	void setCursorToPen();
	void setCursorToNuri();
	void setCursorToEraser();
	void setCursorToHeipen();
	void setGColorTheta(ULONG mouse_x, ULONG mouse_y);
	bool isInCircleColorPen(ULONG mouse_x, ULONG mouse_y);
	bool isInTriangleColorPen(ULONG mouse_x, ULONG mouse_y);
	void setNowColor(COLORREF c);
	void setNowGColor(COLORREF c);

	int getNowSheetLineNum() {
		return now_sheet->now_sheet->getPlineMax();
	}
	void updateDispLineNum();

	void startDrawLine() {
		if (this->now_paint_id == KTPAINT_PEN_ID) {
		now_sheet->now_sheet->setPlineStart();
		} else if(now_paint_id == KTPAINT_HEIPEN_ID) {
			now_sheet->now_sheet->setHeiPlineStart();
			//now_penkyokuline_start = now_sheet->now_sheet->getHeiKyokuPLineMax();
		}
	}
	void writeWithPen(POINT mpo, POINT po, UINT pressure_old, UINT pressure_new, bool reset);
	void endDrawLine() {
		if (this->now_paint_id == KTPAINT_PEN_ID) {

		now_sheet->now_sheet->setPlineEnd();
		now_sheet->now_sheet->heikinPline();
		} else if(now_paint_id == KTPAINT_HEIPEN_ID) {
			now_sheet->now_sheet->setHeiPlineEnd();
			nuridayo.koutenShori(now_sheet->now_sheet->getHeiKyokuPLine(), now_penkyokuline_start, now_sheet->now_sheet->getHeiKyokuPLineMax(),
				now_sheet->now_sheet->getHeiPLine());
			DWORD color = (0xFF << 24) + (now_color_r << 16) + (now_color_g << 8) + now_color_b;
			if (now_sheet->now_sheet->calcHeiryouikiPlus(&nuridayo,color)) {
				// ‚¢‚Ü‚Ü‚Å‚Ìheiü‚ðÁ‚·
				renderlineToTex();
				now_penkyokuline_start = now_sheet->now_sheet->getHeiKyokuPLineMax();
			}
		}

	//	nuridayo.koutenShoriLinePlus(now_sheet->now_sheet->getLastKyokuPLine(), now_sheet->now_sheet->getKyokuPLines(), 
	//		now_sheet->now_sheet->getKyokuPLineMax(), now_sheet->now_sheet->getPline());
	}
	void render();
	void renderlineToTex();
	void clearSheetTransInfoNado();
	void activate(){is_activate = true;}
	void deactivate() {is_activate=false;}
	KTROBO::Graphics* getGraphics() {return g;}

	bool is_dougasaisei() {return is_mode_dougasaisei;}
	bool setSheetNext() {
		HDC hdc = GetDC(parent_window);
		douga.Run(parent_window,hdc);
		is_mode_dougasaisei = true;
		if (now_sheet->next_sheet) {
			now_sheet = now_sheet->next_sheet;
			now_count = 0;
			ReleaseDC(parent_window, hdc);
			return true;
		}
		ReleaseDC(parent_window, hdc);
		return false;
	}
	bool setSheetBefore() {
		HDC hdc = GetDC(parent_window);
		douga.Stop(parent_window, hdc);
		is_mode_dougasaisei = false;

		ReleaseDC(parent_window, hdc);
		if (now_sheet->mae_sheet) {
			now_sheet = now_sheet->mae_sheet;
			now_count = 0;
			return true;
		}
		return false;
	}

	void makeNewSheet();
};

LRESULT CALLBACK ColorPenWindowProc(HWND , UINT , WPARAM , LPARAM );
LRESULT CALLBACK LoadSaveWindowProc(HWND , UINT , WPARAM , LPARAM );

#endif