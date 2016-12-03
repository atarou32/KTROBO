#ifndef MYTESTFORVERTEXTEXTURE_H
#define MYTESTFORVERTEXTEXTURE_H


#pragma once
#include "KTRoboGraphics.h"
#include "MyDefine.h"
#include "MyTextureLoader.h"


#define MYTEST_SHADER_FOR_VERTEXTEXTURE_FILENAME "resrc/shader/debugvertextexture.fx"
#define MYTEST_SHADER_FOR_VERTEXTEXTURE_STRUCT_NUM 512
using namespace KTROBO;

class MyTestForVertexTexture
{
private:
	ID3D11ComputeShader* shader;
	MYMATRIX buf[4];
	ID3D11Buffer* buffer;
	ID3D11UnorderedAccessView* buffer_view;
	ID3D11SamplerState* sampler_state;
	ID3D11Buffer* buffer2;

public:
	MyTestForVertexTexture();
	void Init(KTROBO::Graphics* g);

	~MyTestForVertexTexture(void);
	void Del() {
		if (shader) {
			shader->Release();
			shader = 0;
		}
		if (buffer) {
			buffer->Release();
			buffer = 0;
		}
		if (buffer_view) {
			buffer_view->Release();
			buffer_view = 0;
		}
		if (buffer2) {
			buffer2->Release();
			buffer2 = 0;
		}

		if (sampler_state) {
			sampler_state->Release();
			sampler_state = 0;
		}

	}

	void readVertexTexture(Graphics* g , MyTextureLoader::MY_TEXTURE_CLASS* tex);

	void writeInfo(Graphics* g);
	void _writeinfo(Graphics* g, int buf_index);


};



#endif