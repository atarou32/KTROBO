#ifndef KTROBOTEXT_H
#define KTROBOTEXT_H

#pragma once
#include "KTRoboFont.h"
#include "KTRoboGraphics.h"
#include "stringconverter.h"

namespace KTROBO{

#define MYTEXT_LENGTH 64
#define MYTEXT_SHADER_FILENAME "resrc/shader/simpletext.fx"
#define MYTEXT_SHADER_VS "VSFunc"
#define MYTEXT_SHADER_GS "GSFunc"
#define MYTEXT_SHADER_PS "PSFunc"
#define MYTEXT_RENDER_STRUCT_SIZE 64*6

struct MYTEXT_RENDER_STRUCT {
	MYVECTOR2 pos;
	MYVECTOR2 tex;
	DWORD color;
};

class Text
{
private:
	static MYSHADERSTRUCT mss;
	static void loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename, char* vs_func_name, char* gs_func_name,
								char* ps_func_name, unsigned int ds_width,unsigned int ds_height,
								D3D11_INPUT_ELEMENT_DESC* layout, int numoflayout, bool blend_enable);
	static Font* fo;
	static ID3D11Buffer* render_vertexbuffer;
public:
	Text(WCHAR* str, int length);
	WCHAR str[MYTEXT_LENGTH];
	char return_str[MYTEXT_LENGTH*2];

	~Text(void);
	static void Init(Graphics* g, Font* fo);
	static void Del();
	int getStrlen() {
		return wcslen(str);
	}
	char* getStr() {
		stringconverter sc;
		sc.wcharTochar(str, return_str);
		return return_str;
	}
		
	void changeText(WCHAR* new_str, int length);
	void render(Graphics* g, DWORD color, float x, float y, float height,float w, float h);
	float getWidth(float height) {
		return height * wcslen(this->str);
	}
};

}

#endif