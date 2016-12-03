#include "MyTestForVertexTexture.h"
#include "KTRoboCS.h"
#include "KTRoboDebugText.h"
#include "memory.h"
#include "D3Dcommon.h"
#include "KTRoboDebugText.h"
#include "stringconverter.h"
#include "KTRoboGame.h"


using namespace KTROBO;

MyTestForVertexTexture::~MyTestForVertexTexture(void)
{Del();
}


struct CALCMYTESTSTRUCT {
	MYMATRIX mat;
};

MyTestForVertexTexture::MyTestForVertexTexture() {
		memset(buf,0, sizeof(buf));
		buffer = 0;
		buffer_view = 0;
		shader = 0;
		buffer2 = 0;
		sampler_state = 0;
}

void MyTestForVertexTexture::Init(KTROBO::Graphics* g) {
		ID3DBlob* pblob=0;

		try {
			CompileShaderFromFile(MYTEST_SHADER_FOR_VERTEXTEXTURE_FILENAME , "CalcCS", "cs_5_0",&pblob,false);
			HRESULT hr = g->getDevice()->CreateComputeShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&this->shader);
			if (FAILED(hr)) {
				pblob->Release();
				delete pblob;
				pblob = 0;
				shader = 0;
				throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
			}
			if (pblob) {
				pblob->Release();
			//	delete pblob;
				pblob = 0;
			}




		}catch (GameError* err) {
			if (pblob) {
				pblob->Release();
				pblob = 0;
			
			}
			Del();
			throw err;
		}


	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CALCMYTESTSTRUCT) *  MYTEST_SHADER_FOR_VERTEXTEXTURE_STRUCT_NUM;// KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE;
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bd.CPUAccessFlags = 0;//D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	
	//ANIMETEXTURELOADSTRUCT* ss = new ANIMETEXTURELOADSTRUCT[KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE];
	//D3D11_SUBRESOURCE_DATA initData;
	//memset(&initData, 0, sizeof(initData));
	//initData.pSysMem = ss;
	
	HRESULT hr = g->getDevice()->CreateBuffer(&bd, NULL ,&buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}


	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory( &uavDesc, sizeof(uavDesc) );
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = sizeof(CALCMYTESTSTRUCT) *MYTEST_SHADER_FOR_VERTEXTEXTURE_STRUCT_NUM/4;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

	hr = g->getDevice()->CreateUnorderedAccessView(buffer, &uavDesc, &buffer_view);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "uav make error");;
	}

	bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_STAGING;
	bd.ByteWidth = sizeof(CALCMYTESTSTRUCT) *  MYTEST_SHADER_FOR_VERTEXTEXTURE_STRUCT_NUM;// KTROBO_MESH_INSTANCED_COMBINED_MATRIX_CALC_STRUCT_TEMPSIZE;
	bd.BindFlags = 0;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	
	//ANIMETEXTURELOADSTRUCT* ss = new ANIMETEXTURELOADSTRUCT[KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE];
	//D3D11_SUBRESOURCE_DATA initData;
	//memset(&initData, 0, sizeof(initData));
	//initData.pSysMem = ss;
	
	hr = g->getDevice()->CreateBuffer(&bd, NULL ,&buffer2);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}

	D3D11_SAMPLER_DESC descS;
	memset(&descS, 0, sizeof(descS));
	descS.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	
	descS.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	descS.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	descS.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	descS.BorderColor[0] = 0;
	descS.BorderColor[1] = 0;
	descS.BorderColor[2] = 0;
	descS.BorderColor[3] = 0;
	descS.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descS.MaxAnisotropy = 1;
	descS.MinLOD = -D3D11_FLOAT32_MAX;
	descS.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g->getDevice()->CreateSamplerState(&descS, &sampler_state);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "sampler make error");;
	}
}

void MyTestForVertexTexture::_writeinfo(Graphics* g, int buf_index) {


	WCHAR text[512];
	char bufff[512];
	stringconverter sc;
	//buf[0].m[0][0] = 3.1415;
	sprintf_s(bufff,512,"%f ,%f, %f, %f",this->buf[buf_index].m[0][0],buf[buf_index].m[0][1],buf[buf_index].m[0][2],buf[buf_index].m[0][3]);
	sc.charToWCHAR(bufff,text);
	KTROBO::DebugTexts::instance()->setText(g, wcslen(text), text);

	sprintf_s(bufff,512,"%3.3f ,%3.3f, %3.3f, %3.3f",this->buf[buf_index].m[1][0],buf[buf_index].m[1][1],buf[buf_index].m[1][2],buf[buf_index].m[1][3]);
	sc.charToWCHAR(bufff,text);
	KTROBO::DebugTexts::instance()->setText(g, wcslen(text), text);

	sprintf_s(bufff,512,"%3.3f ,%3.3f, %3.3f, %3.3f",this->buf[buf_index].m[2][0],buf[buf_index].m[2][1],buf[buf_index].m[2][2],buf[buf_index].m[2][3]);
	sc.charToWCHAR(bufff,text);
	KTROBO::DebugTexts::instance()->setText(g, wcslen(text), text);

	sprintf_s(bufff,512,"%3.3f ,%3.3f, %3.3f, %3.3f",this->buf[buf_index].m[3][0],buf[buf_index].m[3][1],buf[buf_index].m[3][2],buf[buf_index].m[3][3]);
	sc.charToWCHAR(bufff,text);
	KTROBO::DebugTexts::instance()->setText(g, wcslen(text), text);



}

void MyTestForVertexTexture::writeInfo(Graphics* g) {
	_writeinfo(g, 0);
	_writeinfo(g, 1);
	_writeinfo(g, 2);
	_writeinfo(g, 3);
}	

void MyTestForVertexTexture::readVertexTexture(Graphics* g, MyTextureLoader::MY_TEXTURE_CLASS* tex) {


	D3D11_MAPPED_SUBRESOURCE subresource;
	unsigned int stride = sizeof(CALCMYTESTSTRUCT);
	unsigned int offset = 0;


	CS::instance()->enter(CS_DEVICECON_CS, "calc combined matrix");
	
	ID3D11ShaderResourceView* testtt = NULL;
	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
	g->getDeviceContext()->CSSetShader(shader, NULL,0);
	ID3D11UnorderedAccessView* test2[]  = {buffer_view};
	g->getDeviceContext()->CSSetUnorderedAccessViews(0,1, test2, 0);
	g->getDeviceContext()->CSSetShaderResources(0,1,&tex->view);

	for (int i = 0 ; i < 16;i++) {
		g->getDeviceContext()->CSSetSamplers(i,1,&sampler_state);
	}
	g->getDeviceContext()->Dispatch(1,1,1);
	g->getDeviceContext()->Flush();
	ID3D11UnorderedAccessView* test[] = {
		{NULL}
	};
	g->getDeviceContext()->CSSetUnorderedAccessViews(0,1, test, 0);
	ID3D11ShaderResourceView* test3 = NULL;
	g->getDeviceContext()->CSSetShaderResources(0,1,&test3);
	g->getDeviceContext()->CSSetShader(NULL,0,0);
	

	g->getDeviceContext()->CopyResource(buffer2,buffer);
	g->getDeviceContext()->Map(buffer2,0, D3D11_MAP_READ, 0, &subresource);
	//memcpy(buf, subresource.pData, sizeof(CALCMYTESTSTRUCT) * 4);//KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
	CALCMYTESTSTRUCT* ts = (CALCMYTESTSTRUCT*)subresource.pData;
	buf[0] = ts[0].mat;
	buf[1] = ts[1].mat;
	buf[2] = ts[2].mat;
	buf[3] = ts[3].mat;
	g->getDeviceContext()->Unmap(buffer2,0);

	CS::instance()->leave(CS_DEVICECON_CS, "calc combined matrix");
}