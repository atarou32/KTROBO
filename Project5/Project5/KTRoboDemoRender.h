#ifndef KTROBODEMORENDER_H
#define KTROBODEMORENDER_H


#pragma once
#include "KTRoboGraphics.h"
#include "KTRoboFont.h"
#include "MyTextureLoader.h"

namespace KTROBO {

class KTRoboDemoRender
{
public:
	Font* font;
	MyTextureLoader* tex_loader;
private:
	ID3D11VertexShader* vs;
	ID3D11GeometryShader* gs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* p_vertexlayout;
	ID3D11Buffer* p_vertexbuffer;
	ID3D11BlendState* p_blendstate;
	ID3D11SamplerState* p_sampler;
	ID3D11RasterizerState* p_rasterstate;

	MyTextureLoader::MY_TEXTURE_CLASS* render_target_tex;
	MyTextureLoader::MY_TEXTURE_CLASS* msaa_tex;
public:
	KTRoboDemoRender();
	~KTRoboDemoRender(void);
	void Init(Graphics* g);

	void Render(Graphics* g, MyTextureLoader::MY_TEXTURE_CLASS* cc);
	void Release();
};

}

#endif