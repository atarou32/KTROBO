#ifndef KTROBOTEXTURE_H
#define KTROBOTEXTURE_H

#pragma once
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "MyDefine.h"
#include "KTRoboGraphics.h"
#include "MyTextureLoader.h"
#include "KTRoboCS.h"

namespace KTROBO {

interface ITexture{ 
	// 注意：luaのコルーチンからたくさんこの関数を呼ばないこと
	// たぶん今の仕組みだとロックがかからないまま（スレッドが同じなので・・・）
	// アクセスするので競合が生まれてしまう
	// lua側でコルーチンから関数呼び出しをストックしておいて、一箇所で順々に呼び出すような仕組みがlua側に必要

public:
	TO_LUA int getTexture(char* tex_name); // すでにロードされていた場合はロードは行われない
	// 内部でRENDERDATA_CSをロックすること
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
	
	TO_LUA void setRenderTexChangeTex(int render_tex_index, int tex_index, int new_tex_index);
	TO_LUA void setRenderBillBoardChangeTex(int bill_index, int tex_index, int new_tex_index);

	TO_LUA void deleteRenderTex(int render_tex_index); // vectorから消す　deleteは他のスレッドが動いていないことを確認したほうがいいかもしれない
	TO_LUA void deleteRenderBillBoard(int bill_index); // vectorから消す  createはvectorの長さが変わるなら他のスレッドをとめる必要がある is_use をfalseからtrueにする場合
	// のときはロック時間に気をつけること（ロックしてis_useを取得してからis_useをtrueにしてからロックを切ること
	TO_LUA void lightdeleteRenderTex(int render_tex_index); // vectorから消さない　indexバッファは更新するis_use is_render is_need_load をfalseにする texのis_need_loadをtrueにする
	TO_LUA void lightdeleteRenderBillBoard(int bill_index); // vectorから消さない  実際のindexバッファの更新はvertextotextureの中で行う
	TO_LUA void deleteAll();
};


class TexturePart {
private:
	bool is_use;
	MyTextureLoader::MY_TEXTURE_CLASS* tex_class;
	bool is_need_load;
	bool is_index_load;// indexbufferがクリエイトされているかどうか
	ID3D11Buffer* indexbuffer; // 
	int index_count;
public:
	TexturePart(int index_c) {
		is_use =true;
		tex_class = 0;
		is_need_load = false;
		is_index_load = false;
		indexbuffer = 0;
		index_count = index_c;
	}

	int getIndexCount() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		int ans = index_count;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}
	//わからないなりにすすめていく
	void setIsIndexLoad(bool t) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		is_index_load = t;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
	}

	bool getIsIndexLoad() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		bool ans = is_index_load;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}



	void setIsNeedLoad(bool t) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		is_need_load = t;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
	}

	bool getIsNeedLoad() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		bool ans = is_need_load;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}


	void setIsUse(bool t) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		is_use = t;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
	}


	bool getIsUse() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		bool ans = is_use;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}

	MyTextureLoader::MY_TEXTURE_CLASS* getClass() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		MyTextureLoader::MY_TEXTURE_CLASS* ans = tex_class;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}

	void loadClass(MyTextureLoader* loader, char* filename) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		try {
		tex_class = loader->loadClass(filename);
		}catch (GameError* err) {
			CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
			throw err;
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
	}

};


// このクラスにアクセスするときはRENDERDATA_CSをロックすること
class RenderTex
{
public:
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
	bool is_use;
};

// このクラスにアクセスするときはRENDERDATA_CSをロックすること
class RenderBillBoard 
{
public:
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
	bool is_use;
};

class Texture
{
private:
	MyTextureLoader* loader;
	vector<RenderBillBoard*> bill_boards;
	vector<RenderTex*> render_texs;
	vector<TexturePart*> parts;
	map<string, int> texturepart_index;

public:
	Texture(void);
	~Texture(void);
	void render(Graphics* g); // 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画スレッドで呼ぶ
	void sendinfoToVertexTexture(Graphics* g);// 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画補助スレッドで呼ぶ
	void createIndexBuffer(Graphics* g);// ロードスレッドで呼ぶ

	int getTexture(char* tex_name); // すでにロードされていた場合はロードは行われない
	int getRenderTex(int tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	int getRenderBillBoard(int tex_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexParam(int render_tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	void setRenderBillBoardParam(int bill_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexColor(int render_tex_index, unsigned int color);
	void setRenderBillBoardColor(int bill_index, unsigned int color);

	void setRenderTexPos(int render_tex_index, int x, int y);
	void setRenderBillBoardPos(int bill_index, YARITORI MYMATRIX* world);

	void setRenderTexWH(int render_tex_index, int width, int height);
	void setRenderBillBoardWH(int bill_index, float width, float height);

	void setRenderTexTexPos(int render_tex_index, int tex_x, int tex_y, int tex_width, int tex_height);
	void setRenderBillBoardTexPos(int bill_index, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexIsRender(int render_tex_index, bool t);
	void setRenderBillBoardIsRender(int bill_index, bool t);
	
	void setRenderTexChangeTex(int render_tex_index, int tex_index, int new_tex_index);
	void setRenderBillBoardChangeTex(int bill_index, int tex_index, int new_tex_index);

	void deleteRenderTex(int render_tex_index); // ロックをどうするか考えどころ
	void deleteRenderBillBoard(int bill_index); // ロックをどうするか考えどころ
	void lightdeleteRenderTex(int render_tex_index);
	void lightdeleteRenderBillBoard(int bill_index);

	void deleteAll();

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
// struct{ 
// id
// tex_index
// color
// x
// y
// width
// height
// tex_x
// tex_y
// tex_width
// tex_height
// } を更新または生成破棄されたrendertexに関して作成する
// drawinstanced で格納する値のインデックスを指定する
// geometry shaderで複製使用と思ったけど・・・・
// struct {
// id
// value
// offset (color x+y width+height tex_x+tex_y tex_width+tex_height)
// geometryshaderでこっちのほうが複製しやすそう
// drawinstancedは必要なし// インデックスバッファもいらないような気がする

// 格納する値
// color x y width height tex_x tex_y tex_width tex_height
// 4   2   2  2     2       2      2     2       2  = 20
// R8G8B8A8 だと１テクセルに４だから　ひとつのテクスには5テクセル必要
// 512*512 のテクスチャだと  およそ50000のテクスを登録できるたぶんこんなに使われない
// id tex_index に関してはCPU側で情報を持たせて管理する
// ビルボードの場合は頂点情報は
// id
// tex_index
// color
// world
// width
// height
// tex_x
// tex_y
// tex_width
// tex_height
// 格納する値は
// struct {
// id
// valueint
// valuefloat
// offset
// isint
// geometryで複製する？
// color world width height tex_x tex_y tex_width tex_height
// 4    4* 16    4     4      2     2    2          2  = 84  21 テクセル必要
// 512*512のテクスチャだとおよそ10000のビルボードを登録できるなんとか大丈夫かな

// 破棄に関して　インデックスバッファを更新すれば描画はされなくなる
// 再利用に関して たぶんテクスや特にビルボードは生成破棄がはげしい
// 空いているidを検索して入れ込めるような仕組みが必要な気がする
// 


#endif