#ifndef KTROBODEBUGTEXT_H
#define KTROBODEBUGTEXT_H


#pragma once
#include "KTRoboFont.h"
#include "KTRoboGraphics.h"
#include "MyDefine.h"
namespace KTROBO {

#define KTROBO_DEBUG_TEXT_SHADER_FILENAME "resrc/shader/debugtext.fx"
#define KTROBO_DEBUG_TEXT_SHADER_VS "VSFunc"
#define KTROBO_DEBUG_TEXT_SHADER_PS "PSFunc"
struct SimpleDebugTextVertex {
	MYVECTOR3 pos;
	MYVECTOR3 tex_coord_plus_tex_index;
	SimpleDebugTextVertex() {
		pos = MYVECTOR3(0,0,0);
		tex_coord_plus_tex_index = MYVECTOR3(0,0,0);

	}

	SimpleDebugTextVertex& operator =(const SimpleDebugTextVertex& obj) {
		this->pos.float3.x = obj.pos.float3.x;
		this->pos.float3.y = obj.pos.float3.y;
		this->pos.float3.z = obj.pos.float3.z;
		this->tex_coord_plus_tex_index.float3.x = obj.tex_coord_plus_tex_index.float3.x;
		this->tex_coord_plus_tex_index.float3.y = obj.tex_coord_plus_tex_index.float3.y;
		this->tex_coord_plus_tex_index.float3.z = obj.tex_coord_plus_tex_index.float3.z;
		return (*this);
	}


};

class DebugText
{
private:
	WCHAR* raw_text;
	SimpleDebugTextVertex* texts;
	int text_count;
	int vertex_count;
public:
	DebugText(void) {
		raw_text = 0;
		texts = 0;
		text_count = 0;
		vertex_count = 0;
	}

	~DebugText(void) {
		if (raw_text) {
		delete[] raw_text;
		raw_text = 0;
		}
		if (texts) {
		delete[] texts;
		texts = 0;
		}
	}

	int getTextCount() {
		return text_count;
	}

	WCHAR* getRawText() {
		return raw_text;
	}

	int getVertexCount() {
		return vertex_count;
	}

	SimpleDebugTextVertex* getVertex() {
		return texts;
	}

	void setVertex(Font* f, int screen_width, int screen_height, int line_num, int height, int wchar_num, WCHAR* text);
};


#define KTROBO_DEBUG_TEXT_NUM 25
#define KTROBO_DEBUG_TEXT_HEIGHT 30

class DebugTexts {
private:
	DebugTexts(void) {
		insert_text_point = 0;
		p_blendstate = 0;
		p_sampler = 0;
		p_vertexlayout = 0;
		vs = 0;
		ps = 0;
	};
	DebugTexts(DebugTexts& k){};
	DebugTexts& operator =(const DebugTexts& obj) {}
	DebugText texts[KTROBO_DEBUG_TEXT_NUM];
	int insert_text_point;
	Font* f;
	
	int vertex_counts[MYFONT_TEXTURE_COUNT];
	ID3D11Buffer* p_vertexbuffers[MYFONT_TEXTURE_COUNT];

	void makeVertexBuffer(Graphics* g);


	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* p_vertexlayout;
	ID3D11BlendState* p_blendstate;
public:
	ID3D11SamplerState* p_sampler;
private:
	ID3D11RasterizerState* p_rasterstate;

public:
	static DebugTexts* instance() {
		static DebugTexts ins;
		return &ins;
	}

	void Init(Graphics* g, Font* f);
	void Del();
	void setText(Graphics* g, int wchar_num, WCHAR* text);
	void render(Graphics* g);
};


}
#endif