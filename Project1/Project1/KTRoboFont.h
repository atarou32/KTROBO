#ifndef KTROBOFONT_H
#define KTROBOFONT_H

#pragma once
#include "D3DX11.h"
#include "map"
#include "MyDefine.h"
#include "KTRoboGraphics.h"

#include <vector>
using namespace std;
using namespace MYDEFINE;

namespace KTROBO {

#define MYFONT_TEXTURE_COUNT 5
#define MYFONT_TEXTURE_WIDTH 4096
#define MYFONT_TEXTURE_HEIGHT 4096
#define MYFONT_FONTSIZE 64
#define MYFONT_KANJI_TXT "resrc/kanji_all.txt"
class Font
{
public:
	HFONT font;
	KTROBO::Graphics* g;
	
	MYTEXTURE fonttextures[MYFONT_TEXTURE_COUNT];
	MyShaderResourceView fonttextureviews[MYFONT_TEXTURE_COUNT];
	map<int,int> fonttexture_index;
	map<int,int> fontindex;
	void Draw(ID3D11Texture2D* tex);
	void Init(char* filename, char* fontname, KTROBO::Graphics* g);
	void Release();
	void copyBufFromFont(UINT32** pData, char* buffer, int fsize, HDC hdc);
	void copyBufToTexture(UINT32** pData, char* buffer);
	Font(void);
	~Font(void);
};




}


#endif