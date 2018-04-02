#ifndef MYGYOURETUKEISAN_H
#define MYGYOURETUKEISAN_H

#pragma once
#include "KTRoboMesh.h"
#include "vector"
#include "MyDefine.h"
#include "set"

using namespace KTROBO;
using namespace std;

#define GYOURETU_MAX 24


class MyGyouretuKeisan
{
public:
	MyGyouretuKeisan(void);
	~MyGyouretuKeisan(void);
	float det(const float* mat, int retu);
	void getMatrixInverse(float* mat, int retu, float* out_mat);
	void getMatrixTranspose(float* mat, int gyou, int retu, float* out_mat);
	void getMatrixMultiply(int gyou, int retu, float* mat, int gyou2, int retu2, float* mat2, float* out_mat);
	void getInverseYFromY(int gyou, int retu, float* mat, int XM, int NTHETA, float* out_mat);

};


class MyIKMODOKI {
private:
	float dthetadayo[GYOURETU_MAX*GYOURETU_MAX];
	float ymat[GYOURETU_MAX*GYOURETU_MAX];
	float inv_ymat[GYOURETU_MAX*GYOURETU_MAX];
	MYVECTOR3 mokuhyou;
	string mokuhyou_bonename;
	string moto_bonename;
	Mesh* mesh;
	set<string> x_freebone;
	set<string> y_freebone;
	set<string> z_freebone;
	map<string, int> bone_rotxdayo;
	map<string, int> bone_rotydayo;
	map<string, int> bone_rotzdayo;
	MYMATRIX world;

	void calcYMat();
	
public:
	MyIKMODOKI(Mesh* m, MYMATRIX* wo, MYVECTOR3* mo, const char* moto, const char* moku) {
		setMesh(m);
		setMokuhyou(mo);
		setbonename(moto, moku);
		world = *wo;
		for (int i = 0; i < GYOURETU_MAX * GYOURETU_MAX; i++) {
			dthetadayo[i] = 0;
			ymat[i] = 0;
			inv_ymat[i] = 0;
		}
	}
	float getfreenum() {
		return x_freebone.size()+y_freebone.size()+z_freebone.size();
	}
	bool hasFreeBone(char* bone_name) {
		if (x_freebone.find(bone_name) != x_freebone.end()) {
			return true;
		}
		if (y_freebone.find(bone_name) != y_freebone.end()) {
			return true;
		}
		if (z_freebone.find(bone_name) != z_freebone.end()) {
			return true;
		}
		return false;

	}

	void updateStep();
	float getdthetaXBone(char* bone_name) {
		if (bone_rotxdayo.find(bone_name) != bone_rotxdayo.end()) {
			return dthetadayo[bone_rotxdayo[bone_name]];
		}
		return 0;
	}

	float getdthetaYBone(char* bone_name) {
		if (bone_rotydayo.find(bone_name) != bone_rotydayo.end()) {
			return dthetadayo[bone_rotydayo[bone_name]];
		}
		return 0;
	}

	float getdthetaZBone(char* bone_name) {
		if (bone_rotzdayo.find(bone_name) != bone_rotzdayo.end()) {
			return dthetadayo[bone_rotzdayo[bone_name]];
		}
		return 0;
	}

	void setXFreeBone(char* bone_name) {
		x_freebone.insert(string(bone_name));
	}
	void setYFreeBone(char* bone_name) {
		y_freebone.insert(string(bone_name));
	}

	void setZFreeBone(char* bone_name) {
		z_freebone.insert(string(bone_name));
	}

	void setMesh(Mesh* m) {
		mesh = m;
	}
	void setMokuhyou(MYVECTOR3* mo) {
		mokuhyou = *mo;
	}
	void setbonename(const char* moto, const char* moku) {
		mokuhyou_bonename = string(moku);
		moto_bonename = string(moto);
	}
	

};













/*

		impl->now_kakera->setOffsetMatrixToMesh(impl->hon_mesh->mesh);
		MyIKMODOKI ik(impl->hon_mesh->mesh, &MYVECTOR3(0,-10,11),impl->ik_bone_moto.c_str(), impl->ik_bone_saki.c_str());
		int bsize = impl->hon_mesh->mesh->Bones.size();
		MeshBone* saki = impl->hon_mesh->mesh->Bones[impl->hon_mesh->mesh->BoneIndexes[impl->ik_bone_saki.c_str()]];
		MeshBone* moto = impl->hon_mesh->mesh->Bones[impl->hon_mesh->mesh->BoneIndexes[impl->ik_bone_moto.c_str()]];
		MeshBone* sakip = saki->parent_bone;
		MeshBone* bsakip = saki->parent_bone;
		MYMATRIX mat;
		MyMatrixMultiply(mat, saki->combined_matrix, saki->matrix_local);
		MYVECTOR3 vec(0,0,0);
		MyVec3TransformCoord(vec,vec,mat);
		vec.float3.z -= 10.0f;
		vec = MYVECTOR3(0,-15,11);
		ik.setMokuhyou(&vec);
		while (sakip && strcmp(bsakip->bone_name , moto->bone_name )!=0) {

			if (strcmp(bsakip->bone_name, "migiSakotuBone")==0) {
			} else if(strcmp(bsakip->bone_name, "migiArmBone")==0) {
				ik.setXFreeBone(sakip->bone_name);
			ik.setZFreeBone(sakip->bone_name);

			} else if(strcmp(bsakip->bone_name, "migiArmSitaBone")==0) {

				ik.setZFreeBone(sakip->bone_name);
			}else {
			ik.setXFreeBone(sakip->bone_name);
			ik.setYFreeBone(sakip->bone_name);
			ik.setZFreeBone(sakip->bone_name);
			}
			bsakip = sakip;
			sakip = sakip->parent_bone;
		}

		for (int s=0;s<6;s++) {
			impl->now_kakera->setOffsetMatrixToMesh(impl->hon_mesh->mesh);
			ik.updateStep();
			for (int i=0;i<bsize;i++) {

				MeshBone* bone = impl->hon_mesh->mesh->Bones[i];
				float drotx = ik.getdthetaXBone(bone->bone_name);
				float droty = ik.getdthetaYBone(bone->bone_name);
				float drotz = ik.getdthetaZBone(bone->bone_name);
				if (abs(drotx) > 0.0001f) {
				float nrotx = this->getHonMeshBoneRotX(now_index,i);
				float hon_rotx = this->getHonRotX(bone->bone_name,nrotx+drotx);
				this->setHonMeshBoneRotX(now_index, i, hon_rotx);
				}
				if (abs(droty) > 0.0001f) {
				float nroty = this->getHonMeshBoneRotY(now_index,i);
				float hon_roty = this->getHonRotY(bone->bone_name,nroty+droty);
				this->setHonMeshBoneRotY(now_index, i, hon_roty);
				}
				if (abs(drotz) > 0.0001f) {
				float nrotz = this->getHonMeshBoneRotZ(now_index,i);
				float hon_rotz = this->getHonRotZ(bone->bone_name, nrotz+drotz);
				this->setHonMeshBoneRotZ(now_index, i, hon_rotz);
				}
			}
			impl->now_kakera->setOffsetMatrixToMesh(impl->hon_mesh->mesh);
		}
		impl->setIsAnimate(false);
	}


*/











#endif