#include "MyTextureLoader.h"
#include "KTRoboLog.h"
#include "stringconverter.h"

#include "KTRoboGameError.h"
#include "stdlib.h"
#include "stdio.h"

MyTextureLoader::MyTextureLoader(void)
{

	
g = NULL;
//map<string,int> MyTextureLoader::texturenames_index;
//vector<MYTEXTURE> MyTextureLoader::textures;
//vector<MyTextureLoader::MY_TEXTURE_CLASS*> MyTextureLoader::tex_classes;
//map<string,int> MyTextureLoader::tex_classnames_index;
render_target_texture_count = 0;

}


MyTextureLoader::~MyTextureLoader(void)
{
}

void MyTextureLoader::init(KTROBO::Graphics* g) {
	MyTextureLoader::g = g;
}

void MyTextureLoader::del() {
	vector<MYTEXTURE>::iterator it;
	it = textures.begin();
	while(it != textures.end()) {
		MYTEXTURE tex = *it;
		tex->Release();
		tex = 0;
		it = it + 1;
	}
	textures.clear();
	texturenames_index.clear();

	vector<MY_TEXTURE_CLASS*>::iterator it2;
	it2 = tex_classes.begin();
	while(it2 != tex_classes.end()) {
		MY_TEXTURE_CLASS* c = *it2;
		if (c->view) {
		c->view->Release();
		}
		if (c->target_view) {
			c->target_view->Release();
		}
		delete c;
		c =0 ;
		it2 = it2 + 1;
	}
	tex_classes.clear();
	tex_classnames_index.clear();
}

MyTextureLoader::MY_TEXTURE_CLASS* MyTextureLoader::_makeClass(int width, int height, int SampleCount, int Quality, bool is_render_target, bool is_shader_resource) {

		//みつからなかった
		ID3D11Resource* texture = NULL;
		stringconverter sc;
		char filename[512];
		memset(filename,0,512);
		_itoa_s(render_target_texture_count,filename,10);
		render_target_texture_count++;
		WCHAR a[512];
		sc.charToWCHAR(filename,a);	

	
		D3D11_TEXTURE2D_DESC texDesc;
		ID3D11Texture2D* texture2d; 
		memset(&texDesc,0,sizeof(texDesc));
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		texDesc.BindFlags = 0;//D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		if (is_shader_resource) {
			texDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}
		if (is_render_target) {
			texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		}

		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.CPUAccessFlags = 0;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.SampleDesc.Count = SampleCount;
		texDesc.SampleDesc.Quality = Quality;

		HRESULT hr = g->getDevice()->CreateTexture2D(&texDesc, NULL, &texture2d);
		if (FAILED(hr)) {
			char buf[1024];
			memset(buf,0,1024);
			sprintf_s(buf,512,"テクスチャ作成に失敗");
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, buf);
		}

		MY_TEXTURE_CLASS* c = new MY_TEXTURE_CLASS();
		c->height = texDesc.Height;
		c->width = texDesc.Width;
		c->tex = texture2d;
		memset(c->tex_filename,0,512);
		strcpy_s(c->tex_filename, 512, filename);
		c->tex_filename[511] = '\0';
		c->target_view = 0;
		c->view = 0;
		// rendertargetview を作成する

		if (is_render_target) {
		D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
		memset(&rtDesc,0,sizeof(rtDesc));
		rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	/*	if (SampleCount > 1) {
			rtDesc.Texture2D.MipSlice = 1;
		}*/
		hr = g->getDevice()->CreateRenderTargetView(texture2d, &rtDesc, &c->target_view);
		if (FAILED(hr)) {
			char buf[1024];
			memset(buf,0,1024);
			texture2d->Release();
			sprintf_s(buf,512,"%s のテクスチャ rendertargetmakeに失敗", filename);
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, buf);
		}
		}

		// view を作成する
		if (is_shader_resource || is_render_target) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srcDesc;
		memset(&srcDesc,0,sizeof(srcDesc));
		srcDesc.Format = texDesc.Format;
		srcDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srcDesc.Texture2D.MostDetailedMip = 0;
		srcDesc.Texture2D.MipLevels = texDesc.MipLevels;
		hr = g->getDevice()->CreateShaderResourceView(texture2d, &srcDesc, &c->view);
		if (FAILED(hr)) {
			char buf[1024];
			memset(buf,0,1024);
			texture2d->Release();
			c->target_view->Release();
			delete c;
			sprintf_s(buf,512,"%s のテクスチャ読み込みに失敗", filename);
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, buf);
		}
		}
		tex_classnames_index[filename] = tex_classes.size();
		texturenames_index[filename] = textures.size();
		tex_classes.push_back(c);
		textures.push_back(texture2d);
		return c;









}

MyTextureLoader::MY_TEXTURE_CLASS* MyTextureLoader::loadClass(char* filename) {
	
	MYTEXTURE te = load(filename);
	if (te == NULL) {
		return NULL;
	} else {
		if (tex_classnames_index.find(filename) == tex_classnames_index.end()) {
			// 見つからなかった
			return NULL;
		} else {
			return tex_classes[tex_classnames_index[filename]];
		}
	}
}

MyTextureLoader::MY_TEXTURE_CLASS* MyTextureLoader::makeClass(int width, int height) {
	return this->_makeClass(width,height, 1, 0, true, true);
}

MyTextureLoader::MY_TEXTURE_CLASS* MyTextureLoader::makeMSAAClass(KTROBO::Graphics* g, int width, int height) {
	UINT SampleCount = 4;
	UINT Quality = 0;
	g->getDevice()->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, SampleCount, &Quality);
	Quality += -1;
	if (Quality < 0) {
		Quality = 0;
	}
	SampleCount = 1;
	Quality = 0;
	return this->_makeClass(width, height, SampleCount, Quality,true,false);
}

MYTEXTURE MyTextureLoader::load(char* filename) {

	if (texturenames_index.find(filename) == texturenames_index.end()) {
		//みつからなかった
		ID3D11Resource* texture = NULL;
		stringconverter sc;
		WCHAR a[512];
		memset(a,0,sizeof(WCHAR)*512);
		sc.charToWCHAR(filename,a);
		
		HRESULT hr = D3DX11CreateTextureFromFileW(g->getDevice(), a, NULL,NULL, &texture, NULL);

		if (FAILED(hr)) {
			char buf[1024];
			memset(buf,0,1024);
			sprintf_s(buf,512,"%s のテクスチャ読み込みに失敗", filename);
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, buf);
		}

		D3D11_TEXTURE2D_DESC texDesc;
		
		ID3D11Texture2D* texture2d; 
		
		texture2d = static_cast<ID3D11Texture2D*>(texture);
		
		texture2d->GetDesc(&texDesc);
			
		MY_TEXTURE_CLASS* c = new MY_TEXTURE_CLASS();
		c->height = texDesc.Height;
		c->width = texDesc.Width;
		c->tex = texture2d;
		memset(c->tex_filename,0,512);
		strcpy_s(c->tex_filename, 512, filename);
		c->tex_filename[511] = '\0';
		c->target_view = 0;
	
		// view を作成する
		D3D11_SHADER_RESOURCE_VIEW_DESC srcDesc;
		memset(&srcDesc,0,sizeof(srcDesc));
		srcDesc.Format = texDesc.Format;
		srcDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srcDesc.Texture2D.MostDetailedMip = 0;
		srcDesc.Texture2D.MipLevels = texDesc.MipLevels;
		hr = g->getDevice()->CreateShaderResourceView(texture2d, &srcDesc, &c->view);
		if (FAILED(hr)) {
			char buf[1024];
			memset(buf,0,1024);
			texture2d->Release();
			sprintf_s(buf,512,"%s のテクスチャ読み込みに失敗", filename);
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, buf);
		}
	
		tex_classnames_index[filename] = tex_classes.size();
		texturenames_index[filename] = textures.size();
		tex_classes.push_back(c);
		textures.push_back(texture2d);
		
		return texture2d;
	} else {
		return textures[texturenames_index[filename]];

	}

}