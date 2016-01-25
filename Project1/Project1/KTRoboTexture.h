#ifndef KTROBOTEXTURE_H
#define KTROBOTEXTURE_H

#pragma once
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "MyDefine.h"

namespace KTROBO {

interface ITexture{
public:
	TO_LUA int getTexture(char* tex_name); // すでにロードされていた場合はロードは行われない
	TO_LUA int getRenderTex(int tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	TO_LUA int getRenderBillBoard(int tex_index, unsigned int color, MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	TO_LUA void setRenderTexParam(int render_tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	TO_LUA void setRenderBillBoardParam(int bill_index, unsigned int color, MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);
	
	TO_LUA void setRenderTexColor(int render_tex_index, unsigned int color);
	TO_LUA void setRenderBillBoardColor(int bill_index, unsigned int color);

	TO_LUA void setRenderTexPos(int render_tex_index, int x, int y);
	TO_LUA void setRenderBillBoardPos(int bill_index, MYMATRIX* world);

	TO_LUA void setRenderTexWH(int render_tex_index, int width, int height);
	TO_LUA void setRenderBillBoardWH(int bill_index, float width, float height);

	TO_LUA void setRenderTexTexPos(int render_tex_index, int tex_x, int tex_y, int tex_width, int tex_height);
	TO_LUA void setRenderBillBoardTexPos(int bill_index, int tex_x, int tex_y, int tex_width, int tex_height);

	TO_LUA void setRenderTexIsRender(int render_tex_index, bool t);
	TO_LUA void setRenderBillBoardIsRender(int bill_index, bool t);
	
	TO_LUA void deleteRenderTex(int render_tex_index);
	TO_LUA void deleteRenderBillBoard(int bill_index);
	TO_LUA void deleteAll();
};


class Texture
{
public:
	Texture(void);
	~Texture(void);
};



}

#endif