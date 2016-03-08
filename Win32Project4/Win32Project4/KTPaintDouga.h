#ifndef KTPAINTDOUGA_H
#define KTPAINTDOUGA_H

#pragma once
#define _WIN32_DCOM
#define WIN32_LEAN_AND_MEAN
#pragma comment(lib,"d3d9.lib")

#include "Windows.h"
#include "objbase.h"
#include "dshow.h"
#include "d3d9.h"
#include "vmr9.h"
#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
//#include "dxtrans.h"
#include "qedit.h"


class KTPaintDouga
{
private:
	
	IGraphBuilder* pGB;
	IBaseFilter* pBF;
	IVMRFilterConfig* pConf;
	ICaptureGraphBuilder2* pCGB2;
	IMediaControl* pMediaCont;
	bool inited;
	LPDIRECT3D9 g_pD3D;
    LPDIRECT3DDEVICE9 g_pD3DDev;
	IMediaEventEx *pMediaEventEx;
	IBaseFilter *pSampleGrabberFilter;
    ISampleGrabber *pSampleGrabber;
	IVideoWindow  *pVideoWindow;
	IVMRWindowlessControl *Cont; 
public:

	long *pBuffer;       // © ’Ç‰Á
	BITMAPINFO bmi;         // © ’Ç‰Á
	HBITMAP hBitmap;        // © ’Ç‰Á
	HDC hdcMem;          // © ’Ç‰Á

	KTPaintDouga(void);
	~KTPaintDouga(void);
	bool Init(HWND hwnd,HWND loadsave, int width, int height,int nCmdShow);
	bool Init2(HWND hwnd,HINSTANCE hinst);
	void Init3(HWND hwnd, int nCmdShow);
	void Del();
	void Run(HWND hwnd, HDC hdc);
	

	void Stop() {
		pMediaCont->Stop();
	}

	LRESULT OnGraphNotify(WPARAM wParam, LPARAM lParam);
};



#endif