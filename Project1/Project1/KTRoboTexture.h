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
	TO_LUA int getRenderBillBoard(int tex_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	TO_LUA void setRenderTexParam(int render_tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	TO_LUA void setRenderBillBoardParam(int bill_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);
	
	TO_LUA void setRenderTexColor(int render_tex_index, unsigned int color);
	TO_LUA void setRenderBillBoardColor(int bill_index, unsigned int color);

	TO_LUA void setRenderTexPos(int render_tex_index, int x, int y);
	TO_LUA void setRenderBillBoardPos(int bill_index, YARITORI MYMATRIX* world);

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

// テクスとビルボードで構造が異なりまた、呼ばれるエフェクトファイルも異なる
// 毎度おなじみ頂点テクスチャにテクスとビルボード情報を格納する
// 異なるテクスチャのテクスとビルボードがあることにどうやって対応すればいいのか
// 案1 入力頂点情報は固定で頂点テクスチャにtex_indexを含めてif (tex_index == now_index)のときだけ
// 描画する　それ以外のときはpixel shaderでdiscardする
// 案2 描画関数がよばれるその都度入力頂点をCPU側でテクスチャごとに作り直してGPU側に送ってテクスチャの種類ごとに描画を行う
// 案3 テクスチャごとに十分な大きさのインデックスバッファを用意しておき　idの昇順に並んだ配列 を入力頂点情報とする 
// インデックスバッファ,頂点テクスチャが更新されるのは
// テクスとビルボードがアニメーションしたり情報が変更になったとき
// 案3がいいような気がする でも、ほかのテクスチャに変わる場合はどうすればいいのか・・・・
// 変わる前のテクスチャと変わる後のテクスチャのインデックスに関して頂点テクスチャ、インデックスバッファの更新を行えばよい

// 案3でいこう
// 頂点テクスチャに情報を格納するための頂点情報は
// 結構難儀かもしれない
// 
class RenderTex
{
	unsigned short id;
	unsigned short tex_index; // 頂点情報に含める必要があるのか？
	unsigned int color;
	short x;
	short y;
	unsigned short width;
	unsigned short height;
	short tex_x;
	short tex_y;
	unsigned short tex_width;
	unsigned short tex_height;
	bool is_need_load;
	bool is_render;
};

class RenderBillBoard {
	unsigned short id; // 
	unsigned short tex_index;
	unsigned int color;
	MYMATRIX world;
	float width;
	float height;
	short tex_x;
	short tex_y;
	unsigned short tex_width;
	unsigned short tex_height;
	bool is_need_load;
	bool is_render;
};

#endif