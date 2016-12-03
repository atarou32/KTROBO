#ifndef KTROBOTELOPTEXT_H
#define KTROBOTELOPTEXT_H

#pragma once
//#include "xnamath.h"
#include "MyDefine.h"
#include "KTRoboGraphics.h"
#include "KTRoboClock.h"
#include "KTRoboFont.h"

namespace KTROBO {
#define KTROBO_TELOP_TEXT_SHADER_FILENAME "resrc/shader/teloptext.fx"
#define KTROBO_TELOP_TEXT_SHADER_VS "VSFunc"
#define KTROBO_TELOP_TEXT_SHADER_PS "PSFunc"


struct TelopTextsCBuf1 {
	float plustime;
	float text_speed;
	float offset;
	float offset2;
};

struct TelopTextsCBuf2 {
	int screen_width;
	int screen_height;
	int transparent_text_line_num;
	int text_height;
	MYVECTOR4 text_color;
};

class TelopTexts
{
private:

	Font* f;
	float sum_plustime;

	int line_num;
	ID3D11Buffer* p_vertexbuffers[MYFONT_TEXTURE_COUNT];
	int vertex_counts[MYFONT_TEXTURE_COUNT];

	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* p_vertexlayout;
	ID3D11BlendState* p_blendstate;
	ID3D11SamplerState* p_sampler;
	ID3D11RasterizerState* p_rasterstate;

	TelopTextsCBuf1 cbuf1;
	TelopTextsCBuf2 cbuf2;
	ID3D11Buffer* cbuf1_buffer;
	ID3D11Buffer* cbuf2_buffer;

public:
	TelopTexts(void);
	~TelopTexts(void);

	void Init(Graphics* g, Font* f);
	void Del();
	void readFile(Graphics* g, char* filename, int height, int transparent_text_line_num, MYVECTOR4* text_color, float text_speed);
	bool isRenderFinished();
	void plusTime(Graphics* g, float dt);
	void render(Graphics* g);
};


}

#endif