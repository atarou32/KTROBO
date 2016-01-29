#ifndef KTROBOTEXTURE_H
#define KTROBOTEXTURE_H

#pragma once
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "MyDefine.h"
#include "KTRoboGraphics.h"
#include "MyTextureLoader.h"
#include "KTRoboCS.h"
#include <set>

namespace KTROBO {

interface ITexture{ 
	// 注意：luaのコルーチンからたくさんこの関数を呼ばないこと
	// たぶん今の仕組みだとロックがかからないまま（スレッドが同じなので・・・）
	// アクセスするので競合が生まれてしまう
	// lua側でコルーチンから関数呼び出しをストックしておいて、一箇所で順々に呼び出すような仕組みがlua側に必要

public:
	TO_LUA int getTexture(char* tex_name, int index_count); // すでにロードされていた場合はロードは行われない
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
	RenderTex() {
		id = 0;
		tex_index = 0;
		color = 0;
		x = 0;
		y = 0;
		width = 0;
		height = 0;
		tex_x = 0;
		tex_y = 0;
		tex_width = 0;
		tex_height = 0;
		is_need_load = false;
		is_render = false;
		is_use = true;
	};
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
	RenderBillBoard() {
		id = 0;
		tex_index = 0;
		color = 0;
		width = 0;
		height = 0;
		tex_x = 0;
		tex_y = 0;
		tex_width = 0;
		tex_height = 0;
		is_need_load = false;
		is_render = false;
		is_use = true;
		MyMatrixIdentity(world);
	};
};

#define KTROBO_TEXTURE_INDEXBUFFER_MAX 1024*3
#define KTROBO_TEXTURE_INDEXBUFFER_DEFAULT 128*3 // 少ないかも
#define KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT 512

class Texture
{
private:
	static MYSHADERSTRUCT mss_for_render_tex;
	static MYSHADERSTRUCT mss_for_render_bill;
	static MYSHADERSTRUCT mss_for_vertextex;
	static ID3D11Buffer* vertexbuffer;

public:
	static void Init(Graphics* g);
	static void Del();
	static void loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename, char* vs_func_name, char* gs_func_name,
								char* ps_func_name, unsigned int ds_width,unsigned int ds_height,
								D3D11_INPUT_ELEMENT_DESC* layout, int numoflayout, bool blend_enable);


private:
	RenderTex dummy_rendertex; // vectorのインデックスを崩さないためにこれをしようする
	RenderBillBoard dummy_billboard;
	MyTextureLoader* loader;
private:
	vector<RenderBillBoard*> bill_boards;
	vector<RenderTex*> render_texs;
private:
	vector<TexturePart*> parts;
	map<string, int> texturepart_index;
	set<RenderTex*> unuse_render_texs; // 使われていないrender_tex
	set<RenderBillBoard*> unuse_bill_boards;
	set<int> unuse_render_tex_ids; // 削除されて使われてないrender_texのID
	set<int> unuse_bill_board_ids;
public:
	map<int,int> render_tex_indexs; // id index のペア
	map<int,int> bill_board_indexs;
private:
	set<int> erase_render_tex_ids; // 削除する予定のrender_tex
	set<int> erase_bill_board_ids; // 削除する予定のbillboard
	bool is_all_delete;
	MyTextureLoader::MY_TEXTURE_CLASS* vtex_tex;
	MyTextureLoader::MY_TEXTURE_CLASS* vtex_bill;

	void _render(Graphics* g, int part_size, int p_index);
	void _createIndexBuffer(Graphics* g, int psize, int p_index);
public:
	Texture(MyTextureLoader* l) {
		loader = l;
		is_all_delete = false;
		vtex_tex = loader->makeClass(KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT, KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT);
		vtex_bill = loader->makeClass(KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT, KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT);
	}

	~Texture(void){};
	void render(Graphics* g); // 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画スレッドで呼ぶ
	void sendinfoToVertexTexture(Graphics* g);// 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画補助スレッドで呼ぶ
	void updateIndexBuffer(Graphics* g);//描画補助スレッドで呼ぶ
	void createIndexBuffer(Graphics* g);// ロードスレッドで呼ぶ 適正な初期値でcreateする
	void deletedayo(Graphics* g); // delete処理を行う　ロードスレッドで呼ぶ 細切れにdeleteする

	int getTexture(char* tex_name, int index_count=KTROBO_TEXTURE_INDEXBUFFER_DEFAULT); // すでにロードされていた場合はロードは行われない
	int getRenderTex(int tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	int getRenderBillBoard(int tex_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexParam(int render_tex_id, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	void setRenderBillBoardParam(int bill_id, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexColor(int render_tex_id, unsigned int color);
	void setRenderBillBoardColor(int bill_id, unsigned int color);

	void setRenderTexPos(int render_tex_id, int x, int y);
	void setRenderBillBoardPos(int bill_id, YARITORI MYMATRIX* world);

	void setRenderTexWH(int render_tex_id, int width, int height);
	void setRenderBillBoardWH(int bill_id, float width, float height);

	void setRenderTexTexPos(int render_tex_id, int tex_x, int tex_y, int tex_width, int tex_height);
	void setRenderBillBoardTexPos(int bill_id, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexIsRender(int render_tex_id, bool t);
	void setRenderBillBoardIsRender(int bill_id, bool t);
	
	void setRenderTexChangeTex(int render_tex_id, int tex_index, int new_tex_index);
	void setRenderBillBoardChangeTex(int bill_id, int tex_index, int new_tex_index);

	void deleteRenderTex(int render_tex_id); // ロックをどうするか考えどころ
	void deleteRenderBillBoard(int bill_id); // ロックをどうするか考えどころ
	void lightdeleteRenderTex(int render_tex_id);
	void lightdeleteRenderBillBoard(int bill_id);

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




class TexturePart {
private:
	bool is_use;
	MyTextureLoader::MY_TEXTURE_CLASS* tex_class;
	bool is_need_load;
	bool is_index_load;// indexbufferがクリエイトされているかどうか
	
	int index_count_max; // 最大インデックス数 buffer はこれの3倍
public:
	int index_count_tex; // 現在のインデックス数
	int index_count_bill;
	ID3D11Buffer* indexbuffer_tex; // 
	ID3D11Buffer* indexbuffer_bill;
public:
	set<int> render_tex_ids;
	set<int> bill_board_ids;
public:

	TexturePart(int index_c) {
		is_use =true;
		tex_class = 0;
		is_need_load = false;
		is_index_load = false;
		indexbuffer_tex = 0;
		indexbuffer_bill = 0;
		index_count_max = index_c;
		index_count_bill = 0;
		index_count_tex = 0;
	}

	void createIndexBuffer(Graphics* g);

	// RENDERDATAをロックすること
	void del() {
		if (is_index_load) {
			if (indexbuffer_tex) {
				indexbuffer_tex->Release();
				indexbuffer_tex = 0;
			}
			if (indexbuffer_bill) {
				indexbuffer_bill->Release();
				indexbuffer_bill = 0;
			}
		}
		render_tex_ids.clear();
		bill_board_ids.clear();
		// tex_classはローダが管理する
	}

	void eraseRenderTexId(int render_tex_id) {
		CS::instance()->enter(CS_RENDERDATA_CS, "eraserendertexid");
		render_tex_ids.erase(render_tex_id);
		CS::instance()->leave(CS_RENDERDATA_CS, "eraserendertexid");
	}

	void eraseBillBoardId(int bill_id) {
		CS::instance()->enter(CS_RENDERDATA_CS, "erasebillboardid");
		bill_board_ids.erase(bill_id);
		CS::instance()->leave(CS_RENDERDATA_CS, "erasebillboardid");
	}


	void setRenderTexId(int render_tex_id) {
		CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexid");
		render_tex_ids.insert(render_tex_id);
		CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexid");
	}


	void setBillBoardId(int bill_id) {
		CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardid");
		bill_board_ids.insert(bill_id);
		CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardid");
	}



	int getIndexCount() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		int ans = index_count_max;
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











}
#endif