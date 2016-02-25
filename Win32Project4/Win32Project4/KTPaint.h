#ifndef KTPAINT_H
#define KTPAINT_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "KTRoboGraphics.h"

class KTPaint
{
private:
	HINSTANCE hInst;
	HWND parent_window;
	HWND colorpen_window;
	HWND loadsave_window;

	KTROBO::Graphics* g;
public:
	void createKoWindow(HWND hWnd); 
	KTPaint(HINSTANCE hins);
	~KTPaint(void);
	void Init(HWND hwnd);
	void Release();
	void setCursorToPencil();
	void setCursorToPen();
	void setCursorToNuri();
	void setCursorToEraser();



};

LRESULT CALLBACK ColorPenWindowProc(HWND , UINT , WPARAM , LPARAM );
LRESULT CALLBACK LoadSaveWindowProc(HWND , UINT , WPARAM , LPARAM );

#endif