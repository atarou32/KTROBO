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

	void calcYMat();
	
public:
	MyIKMODOKI(Mesh* m, MYVECTOR3* mo, const char* moto, const char* moku) {
		setMesh(m);
		setMokuhyou(mo);
		setbonename(moto, moku);
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
#endif