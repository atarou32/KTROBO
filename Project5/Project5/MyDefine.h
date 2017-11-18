#ifndef MYDEFINE_H
#define MYDEFINE_H
#pragma once
#include "d3dx11.h"
#include "xnamath.h"


namespace KTROBO {
#define KTROBO_GUI_PNG "resrc/sample/gui.png"
};

namespace MYDEFINE {

#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
	((COLOR_ARGB)((((a)&0xFF)<<24)|(((r)&0xFF)<<16|(((g)&0xFF)<<8)|((b)&0xFF))))
#define MY_DEFAULT_COLOR(a) (SETCOLOR_ARGB((a)&0xFF,25,25,55))

#define SAFE_RELEASE(ptr) {if(ptr) {(ptr)->Release();(ptr) =NULL;}}
#define SAFE_DELETE(ptr) { if(ptr) {delete (ptr); (ptr) =NULL;}}
const bool FULLSCREEN = false;
const UINT GAME_WIDTH = 1000;
const UINT GAME_HEIGHT = 800;
const UINT ESC_KEY = 0x1B;
#define MY_DEFAULT_TEXT_HEIGHT 20

#define MYVECTOR2 XMFLOAT2
#define MYVECTOR3 MYXMFLOAT3

struct MYXMFLOAT3 {
	MYXMFLOAT3() {};
	MYXMFLOAT3(float x, float y, float z) {
		this->float3.x = x;
		this->float3.y = y;
		this->float3.z = z;
	}

	XMFLOAT3 float3;

	MYXMFLOAT3 operator * (float t) {
		return MYXMFLOAT3(this->float3.x * t , this->float3.y * t , this->float3.z * t);
	}

	MYXMFLOAT3 operator / (float t) {
		return MYXMFLOAT3(this->float3.x / t , this->float3.y / t , this->float3.z / t);
	}

	MYXMFLOAT3 operator + (MYXMFLOAT3& t) {
		return MYXMFLOAT3(this->float3.x + t.float3.x, this->float3.y + t.float3.y, this->float3.z + t.float3.z);
	}

	MYXMFLOAT3 operator - (MYXMFLOAT3& t) {
		return MYXMFLOAT3(this->float3.x - t.float3.x, this->float3.y - t.float3.y, this->float3.z - t.float3.z);
	}

	MYXMFLOAT3 operator - () {
		return MYXMFLOAT3(-this->float3.x, -this->float3.y, -this->float3.z);
	}

	MYXMFLOAT3 operator = (MYXMFLOAT3& t) {
		
		this->float3.x = t.float3.x;
		this->float3.y = t.float3.y;
		this->float3.z = t.float3.z;
		return *this;

		//return MYXMFLOAT3(t.float3.x, t.float3.y, t.float3.z);
	}

	float operator[](int i) {
		// [] ‰‰ŽZŽq‚Ì‘½d’è‹`
		if (i == 0) {
			return this->float3.x;
		} else if(i == 1) {
			return this->float3.y;
		} else if (i == 2){
			return this->float3.z;
		}
		return this->float3.x;
	}
};

static XMVECTOR tempvec;

#define MYVECTOR4 XMFLOAT4
#define MYXMVECTOR XMVECTOR
#define MYMATRIX XMFLOAT4X4
#define MyMatrix4x4Store(float4x4, m) (XMStoreFloat4x4(&float4x4,m))
#define MyMatrixMultiply(out, a1, a2) (MyMatrix4x4Store(out, XMMatrixMultiply((XMLoadFloat4x4(&a1)),(XMLoadFloat4x4(&a2)))))
#define MyMatrixInverse(out, deter, in) (MyMatrix4x4Store(out, XMMatrixInverse((&tempvec),(XMLoadFloat4x4(&in)))))
//#define MyMatrixInverse(out,deter, in) (MyMatrixIdentity(out))
#define MyMatrixTranspose(out,in) (MyMatrix4x4Store(out, XMMatrixTranspose((XMLoadFloat4x4(&in)))))
#define MyMatrixRotationX(out, f) (MyMatrix4x4Store(out, XMMatrixRotationX((f))))
#define MyMatrixRotationY(out, f) (MyMatrix4x4Store(out, XMMatrixRotationY((f))))
#define MyMatrixRotationZ(out, f) (MyMatrix4x4Store(out, XMMatrixRotationZ((f))))
#define MyMatrixTranslation(out, x,y,z) (MyMatrix4x4Store(out, XMMatrixTranslation((x),(y),(z))))
#define MyMatrixScaling(out,x,y,z) (MyMatrix4x4Store(out, XMMatrixScaling((x),(y),(z))))
#define MyMatrixRotationAxis(out, vec,angle) (MyMatrix4x4Store(out, XMMatrixRotationAxis((XMLoadFloat3(&vec.float3)),(angle))))
#define MyMatrixLookAtRH(out,from,at,up) (MyMatrix4x4Store(out, XMMatrixLookAtRH((XMLoadFloat3(&from.float3)),(XMLoadFloat3(&at.float3)),(XMLoadFloat3(&up.float3)))))
#define MyMatrixLookAtLH(out,from,at,up) (MyMatrix4x4Store(out, XMMatrixLookAtLH((XMLoadFloat3(&from.float3)),(XMLoadFloat3(&at.float3)),(XMLoadFloat3(&up.float3)))))
#define MyMatrixIdentity(out) (MyMatrix4x4Store(out, XMMatrixIdentity()))
#define MYTEXTURE ID3D11Texture2D*
#define MYRESOURCE ID3D11Resource*
#define MyVec3Store(f,v) (XMStoreFloat3(&f.float3,v))

#define MyVec3Subtract(out,in1, in2) (MyVec3Store(out, XMVectorSubtract(XMLoadFloat3(&in1.float3), XMLoadFloat3(&in2.float3))))
#define MyVec3Length(in) (XMVectorGetX(XMVector3Length((XMLoadFloat3(&in.float3)))))
#define MyVec3Normalize(out,in) (MyVec3Store(out,XMVector3Normalize(XMLoadFloat3(&in.float3))))
#define MyVec3Dot(in1,in2) (XMVectorGetX(XMVector3Dot((XMLoadFloat3(&in1.float3)),(XMLoadFloat3(&in2.float3)))))
#define MyVec3Cross(out, in1, in2) (MyVec3Store(out, XMVector3Cross(XMLoadFloat3(&in1.float3),(XMLoadFloat3(&in2.float3)))))
#define MyVec3TransformCoord(out, invec,inmat) (MyVec3Store(out, XMVector3TransformCoord((XMLoadFloat3((&invec.float3))),(XMLoadFloat4x4((&inmat))))))
#define MyVec3TransformNormal(out, invec,inmat) (MyVec3Store(out, XMVector3TransformNormal((XMLoadFloat3((&invec.float3))),(XMLoadFloat4x4((&inmat))))))
#define MyMatrixPerspectiveFovRH(out,a,b,c,d) (MyMatrix4x4Store(out, XMMatrixPerspectiveFovRH((a),(b),(c),(d))))
#define MyMatrixPerspectiveFovLH(out,a,b,c,d) (MyMatrix4x4Store(out, XMMatrixPerspectiveFovLH((a),(b),(c),(d))))

#define MyVec3Unproject(out,pos,viewport,proj,view,world) (MyVec3Store(out, XMVector3Unproject(XMLoadFloat3((&pos.float3)), \
	(viewport.TopLeftX),(viewport.TopLeftY), (viewport.Width), (viewport.Height), (viewport.MinDepth), (viewport.MaxDepth), XMLoadFloat4x4((&proj)),XMLoadFloat4x4((&view)),XMLoadFloat4x4((&world)))))
#define MyShaderResourceView ID3D11ShaderResourceView*
#define MyRenderTargetView ID3D11RenderTargetView*
#define MYVIEWPORT D3DVIEWPORT11
}







namespace KTROBO {

#define TASKTHREADS_UPDATEANIMEFRAMENADO 1
#define TASKTHREADS_UPDATEMAINRENDER 2
#define TASKTHREADS_UPDATEPOSBUTUKARI 0
#define TASKTHREADS_LOADDESTRUCT 3
#define TASKTHREADS_AIDECISION 4

#define RENDERTIME_IGNORETIME 1200
#define RENDERTIME_SETTIME 33
#define POSBUTUKARITIME_SETTIME 33
#define RENDERTIME_FRAMECOUNT 60

}
namespace KTROBO {
extern const char* SOUND_BANK; //="resrc/audio/SoundBank.xsb";
extern const char* WAVE_BANK; //= "resrc/audio/WaveBank.xwb";
extern const char* yumes[6];// = {
}
#endif

