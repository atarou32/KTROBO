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
#include "KTRoboGraphics.h"
#include "MyTextureLoader.h"
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
	IBaseFilter* teefil;
	IBaseFilter* nr;

	REFTIME length;
	LONGLONG frame_length;
	IMediaEvent   *pimex;
	IMediaSeeking *pims;	 
	bool is_canseekabsolute;
	bool is_canseekforward;
	bool is_canseekbackward;
	MyTextureLoader* loader;
	MyTextureLoader::MY_TEXTURE_CLASS* bitmap_class;
	unsigned char alpha;
public:
	void transportBitmapToTextureClass(KTROBO::Graphics* g);
	void setAlpha(unsigned char g) {alpha = g;}
	MyTextureLoader::MY_TEXTURE_CLASS* getBitmapClass() {return bitmap_class;}
public:

	long *pBuffer;       // Å© í«â¡
	BITMAPINFO bmi;         // Å© í«â¡
	HBITMAP hBitmap;        // Å© í«â¡
	HDC hdcMem;          // Å© í«â¡
	LONGLONG alt;
	LONGLONG alt2;
	KTPaintDouga(void);
	~KTPaintDouga(void);
	bool Init(HWND hwnd,HWND loadsave, int width, int height,int nCmdShow, KTROBO::Graphics* g);
	bool Init2(HWND hwnd,HINSTANCE hinst);
	void Init3(HWND hwnd, int nCmdShow);
	bool Init5(HWND hwnd, HWND loadsave, int width, int height,int nCmdShow, KTROBO::Graphics* g, char* title);
	bool getInited() {return inited;}
	void Del();
	void Run(HWND hwnd, HDC hdc);
	LONGLONG getAllFrame() {
		return frame_length;
	}

	void copyBufferOfVideoFrame(HWND hWnd);
	void setFrame(HWND hwnd, HDC hdc, PSTR text_frame, int offset);
	LONGLONG Pause(HWND hwnd, HDC hdc);
	void Stop(HWND hwnd, HDC hdc);


	LRESULT OnGraphNotify(WPARAM wParam, LPARAM lParam);
};



#endif