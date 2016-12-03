#ifndef MYTEXTURE_LOADER_H
#define MYTEXTURE_LOADER_H

#pragma once
#include "D3D11.h"
#include "D3DX11.h"
#include "D3DX11core.h"
#include <vector>
#include <map>
#include <string>
#include "MyDefine.h"
#include "KTRoboGraphics.h"

using namespace std;
using namespace MYDEFINE;
using namespace KTROBO;
//class KTROBO::Graphics;
class MyTextureLoader
{
public:
	struct MY_TEXTURE_CLASS {
		MYTEXTURE tex;
		MyShaderResourceView view;
		MyRenderTargetView target_view;
		char tex_filename[512];
		int width;
		int height;
	};

	MyTextureLoader(void);
	~MyTextureLoader(void);
	KTROBO::Graphics* g;
	void init(KTROBO::Graphics* g);
	void del();
	map<string,int> texturenames_index;
	vector<MYTEXTURE> textures;
	vector<MY_TEXTURE_CLASS*> tex_classes;
	map<string, int> tex_classnames_index;
	int render_target_texture_count;
private:
	MYTEXTURE load(char* filename);
	MyTextureLoader::MY_TEXTURE_CLASS* _makeClass(int width, int height, int SampleCount, int Quality, bool is_render_target, bool is_shader_resource);
public:
	MyTextureLoader::MY_TEXTURE_CLASS* loadClass(char* filename);
	MyTextureLoader::MY_TEXTURE_CLASS* makeMSAAClass(KTROBO::Graphics* g, int width,int height);
	MyTextureLoader::MY_TEXTURE_CLASS* makeClass(int width, int height);
};



#endif