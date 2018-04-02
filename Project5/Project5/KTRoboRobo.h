#ifndef KTROBO_ROBO_H
#define KTROBO_ROBO_H

#pragma once
#include "KTRoboMesh.h"
#include "MyTokenAnalyzer.h"
#include "KTRoboAtari.h"
#include "KTRoboInput.h"
//#include "KTROBOArmPositioner.h"
#include "KTRoboWeapon.h"
#ifndef KTROBO_GRAPHICS_H
#include "KTRoboGraphics.h"
#endif

namespace KTROBO {
class Robo;
struct RoboDataPart {
public:
	int int_data;
	char data_name[32];
	char data_name2[32];
	char string_data[96];
	float float_data;
	RoboDataPart() {
		int_data = 0;
		memset(data_name,0,32);
		memset(data_name2,0,32);
		memset(string_data,0,96);
		float_data = 0;
	}
};

class RoboData {
	vector<RoboDataPart*> datas;
	static RoboDataPart emptydata;

public:
	void setData(int int_data, char* data_name, char* data_name2, char* string_data, float float_data) {
		RoboDataPart* p = new RoboDataPart();
		p->int_data = int_data;
		strcpy_s(p->data_name,32,data_name);
		strcpy_s(p->data_name2,32,data_name2);
		strcpy_s(p->string_data,96,string_data);
		p->float_data = float_data;
		datas.push_back(p);
	}
	RoboData* clone() {
		RoboData* new_data = new RoboData();
		vector<RoboDataPart*>::iterator it;
		it = datas.begin();
		while(datas.end() != it) {
			RoboDataPart* p = *it;
			new_data->setData(p->int_data,p->data_name,p->data_name2, p->string_data,p->float_data);
			it = it + 1;
		}
		return new_data;
	}

	RoboDataPart* getData(char* data_name) {
		vector<RoboDataPart*>::iterator it;
		it = datas.begin();
		while(it != datas.end()) {
			RoboDataPart* p = *it;
			if (strcmp(data_name,p->data_name)==0) {
				return p;
			}
			it = it + 1;
		}

	//	throw new GameError(KTROBO::FATAL_ERROR, "no data");
		return &emptydata;
	}

	~RoboData() {
		vector<RoboDataPart*>::iterator it;
		it = datas.begin();
		while(it != datas.end()) {
			RoboDataPart* p = *it;
			delete p;
			p = 0;
			it = it + 1;
		}
		datas.clear();
	}
};
struct RoboMetaDataPart {
public:
	char data_name[32];
	char data_name2[32];
	char data_type[32];
	char data_sentence[32];
	char data_compare[32];
	void clear() {
		memset(data_name,0,32);
		memset(data_name2,0,32);
		memset(data_type,0,32);
		memset(data_sentence,0,32);
		memset(data_compare,0,32);
	}
	void readline(MyTokenAnalyzer* ma) {
		ma->GetToken();
		if (strlen(ma->Toke()) < 32) {
		strcpy_s(data_name,32,ma->Toke());
		}
		ma->GetToken();
		if (strlen(ma->Toke()) < 32) {
		strcpy_s(data_name2,32,ma->Toke());
		}
		ma->GetToken();
		if (strlen(ma->Toke()) < 32) {
		strcpy_s(data_type,32,ma->Toke());
		}
		ma->GetToken();
		if (strlen(ma->Toke()) < 32) {
		strcpy_s(data_sentence,32,ma->Toke());
		}
		ma->GetToken();
		if (strlen(ma->Toke()) < 32) {
		strcpy_s(data_compare,32,ma->Toke());
		}
	}

	RoboMetaDataPart() {
		memset(data_name,0,32);
		memset(data_name2,0,32);
		memset(data_type,0,32);
		memset(data_sentence,0,32);
		memset(data_compare,0,32);
	}
};

class RoboDataMetaData {
	vector<RoboMetaDataPart*> metadatas;
public:
	vector<RoboMetaDataPart*>* getMetaDatas() {
		return &metadatas;
	}

	void setData(char* data_name, char* data_name2, char* data_type, char* data_sentence, char* data_compare) {
		RoboMetaDataPart* p = new RoboMetaDataPart();
		strcpy_s(p->data_name,32,data_name);
		strcpy_s(p->data_name2,32,data_name2);
		strcpy_s(p->data_type, 32, data_type);
		strcpy_s(p->data_sentence,32,data_sentence);
		strcpy_s(p->data_compare,32,data_sentence);
		metadatas.push_back(p);
	}
	RoboMetaDataPart* getData(char* data_name) {
		vector<RoboMetaDataPart*>::iterator it;
		it = metadatas.begin();
		while(it != metadatas.end()) {
			RoboMetaDataPart* p = *it;
			if (strcmp(data_name,p->data_name)==0) {
				return p;
			}
			it = it + 1;
		}

		return 0;
	}

	~RoboDataMetaData() {
		vector<RoboMetaDataPart*>::iterator it;
		it = metadatas.begin();
		while(it != metadatas.end()) {
			RoboMetaDataPart* p = *it;
			delete p;
			p = 0;
			it = it + 1;
		}
		metadatas.clear();
	}
};

class RoboParts {
public:
	RoboData* data;
protected:
	bool mesh_loaded;
public:
	RoboParts() {data = 0;mesh_loaded=false;}
	virtual ~RoboParts();
	
	virtual float getR(){return 1;};
	virtual MYVECTOR3 getC() {return MYVECTOR3(0,0,0);} 
	void loadData(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data);
	virtual void loadMesh(Graphics* g, MyTextureLoader* loader);
	virtual void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj){};
	virtual void Release()=0;
	virtual RoboParts* myNew()=0;
	bool hasMeshLoaded() {return mesh_loaded;}
	virtual void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader){ return;}; // clone を装備させる
	virtual bool isEmpty() {return false;}

};

class RoboPartsEmpty: public RoboParts {
public:
	RoboPartsEmpty() {
		data = new RoboData();
		data->setData(0,"name","名称","なし",0);
	}
	~RoboPartsEmpty() {
		Release();
	}

	void Release() {
		if (data) {
			delete data;
			data = 0;
		}
	}

	RoboParts* myNew() {
		return new RoboPartsEmpty();
	}

	bool isEmpty() {return true;}

	void emptyRArmWeapon(Robo* robo, bool is_delete);
	void emptyLArmWeapon(Robo* robo, bool is_delete);
	void emptyRShoulderWeapon(Robo* robo, bool is_delete);
	void emptyLShoulderWeapon(Robo* robo, bool is_delete);
	void emptyInsideWeapon(Robo* robo, bool is_delete);
	void equipRobo(Robo* robo) {
		// なにもしない
		return;
	}
};
	
class RoboHead : public RoboParts {
public:
	Mesh* head;
	Mesh* head2;
	Mesh* head3;

	//RoboData* data;

public:
	RoboHead() {
		
		head = 0;
		head2 = 0;
		head3 = 0;
		data = 0;
	}

	float getR() {
		float R=0;
		int count=0;
		if (head) {
			R += sqrt(MyVec3Dot(head->houkatuobb.e,head->houkatuobb.e));
			count++;
		}

		if (head2) {
			R += sqrt(MyVec3Dot(head2->houkatuobb.e,head2->houkatuobb.e));
			count++;
		}

		if (head3) {
			R += sqrt(MyVec3Dot(head3->houkatuobb.e,head3->houkatuobb.e));
			count++;
		}

		if (count) {
			R = R / count;
		}

		return R;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RoboHead* shead) {
		if (shead->head) {
			head = shead->head->clone();
		}
		if (shead->head2) {
			head2 = shead->head2->clone();
		}
		if( shead->head3) {
			head3 = shead->head3->clone();
		}
		if (shead->data) {
			data = shead->data->clone();
		}
		mesh_loaded = true;
	}

	void Release(){
	
		if (data) {
			delete data;
			data = 0;
		}
		if (head) {
			head->Release();
			delete head;
			head = 0;
		}
		if (head2) {
			head2->Release();
			delete head2;
			head2 = 0;
		}
		if (head3) {
			head3->Release();
			delete head3;
			head3 = 0;
		}
	}
	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new RoboHead();
	}

	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
	


};

class RoboArm : public RoboParts{
public:
	Mesh* rarm;
	Mesh* larm;
//	RoboData* data;
public:
	RoboArm() {
		rarm = 0;
		larm = 0;
		data = 0;
	}

	float getR() {
		float R=0;
		int count=0;
		if (rarm) {
			R += sqrt(MyVec3Dot(rarm->houkatuobb.e,rarm->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RoboArm* sarm) {
		if (sarm->rarm) {
		rarm = sarm->rarm->clone();
		}
		if (sarm->larm) {
		larm = sarm->larm->clone();
		}
		if (sarm->data) {
		data = sarm->data->clone();
		}
		mesh_loaded = true;
	}

	void Release(){
	
		if (data) {
			delete data;
			data = 0;
		}
		if (rarm) {
			rarm->Release();
			delete rarm;
			rarm = 0;
		}
		if (larm) {
			larm->Release();
			delete larm;
			larm = 0;
		}
	
	
	}
	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new RoboArm();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};

class RoboLeg : public RoboParts {
public:
	Mesh* leg;
//	RoboData* data;
public:
	RoboLeg() {
		leg = 0;
		data = 0;
	}

	float getR() {
		float R=0;
		int count=0;
		if (leg) {
			R += sqrt(MyVec3Dot(leg->houkatuobb.e,leg->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}
	MYVECTOR3 getC() {
		return leg->houkatuobb.c;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RoboLeg* sleg) {
		if (sleg->leg) {
			leg = sleg->leg->clone();
		}
		if (sleg->data) {
			data = sleg->data->clone();
		}
		mesh_loaded = true;
	}
	void Release(){
		if (data) {
			delete data;
			data = 0;
		}
		if (leg) {
			leg->Release();
			delete leg;
			leg = 0;
		}
	
	
	}
	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new RoboLeg();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};

class RoboBody : public RoboParts{
public:
	Mesh* body;
//	RoboData* data;
public:
	RoboBody() {
		body = 0;
		data = 0;
	}
	float getR() {
		float R=0;
		int count=0;
		if (body) {
			R += sqrt(MyVec3Dot(body->houkatuobb.e,body->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}
	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RoboBody* sbody) {
		if (sbody->body) {
		body = sbody->body->clone();
		}
		if (sbody->data) {
			data = sbody->data->clone();
		}
		mesh_loaded = true;
	}

	void Release() {
	
		if (data) {
			delete data;
			data = 0;
		}
		if (body) {
			body->Release();
			delete body;
			body = 0;
		}
	
	
	
	
	}
	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new RoboBody();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};


class RArmWeapon : public RoboParts {
public:
	Mesh* weapon;
//	RoboData* data;

	WeaponFireRifle wf_rifle;
public:
	RArmWeapon() {
		weapon = 0;
		data = 0;
	}
	float getR() {
		float R=0;
		int count=0;
		if (weapon) {
			R += sqrt(MyVec3Dot(weapon->houkatuobb.e,weapon->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}
	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RArmWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
		mesh_loaded = true;
	}
	void Release(){
	
		if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
	
	}

	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new RArmWeapon();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};

class LArmWeapon : public RoboParts{
public:
	Mesh* weapon;
//	RoboData* data;
public:
	LArmWeapon() {
		weapon = 0;
		data = 0;
	}
	float getR() {
		float R=0;
		int count=0;
		if (weapon) {
			R += sqrt(MyVec3Dot(weapon->houkatuobb.e,weapon->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}
	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(LArmWeapon* sweapon) {
		if (sweapon->weapon) {
			weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
			data = sweapon->data->clone();
		}
		mesh_loaded = true;
	}
	void Release(){
	
			if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
	
	
	
	
	}
	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new LArmWeapon();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};

class RShoulderWeapon : public RoboParts {
public:
		Mesh* weapon;
//		RoboData* data;
public:
	RShoulderWeapon() {
		weapon = 0;
		data = 0;
	}
	float getR() {
		float R=0;
		int count=0;
		if (weapon) {
			R += sqrt(MyVec3Dot(weapon->houkatuobb.e,weapon->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}
	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader); 
	void init(RShoulderWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
		mesh_loaded = true;
	}
	void Release(){
	
	
			if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
	
	
	
	}
	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new RShoulderWeapon();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};

class LShoulderWeapon : public RoboParts{
public:
	Mesh* weapon;
//	RoboData* data;
public:
	LShoulderWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(LShoulderWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
		mesh_loaded = true;
	}
	float getR() {
		float R=0;
		int count=0;
		if (weapon) {
			R += sqrt(MyVec3Dot(weapon->houkatuobb.e,weapon->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}
	void Release(){
	
		if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
		
	
	
	
	}
	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new LShoulderWeapon();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};

class InsideWeapon : public RoboParts {
public:
	Mesh* weapon;
//	RoboData* data;
public:
	InsideWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(InsideWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
		mesh_loaded = true;
	}

	void Release(){
	
	
		if (data) {
			delete data;
			data = 0;
		}
		if (weapon) {
			weapon->Release();
			delete weapon;
			weapon = 0;
		}
	
		
	
	
	}

	float getR() {
		float R=0;
		int count=0;
		if (weapon) {
			R += sqrt(MyVec3Dot(weapon->houkatuobb.e,weapon->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}
	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new InsideWeapon();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};

class RoboEngine : public RoboParts {

public:
	Mesh* mesh;
//	RoboData* data;
public:
	RoboEngine() {
		mesh = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RoboEngine* sengine) {
		if (sengine->mesh) {
		mesh = sengine->mesh->clone();
		}
		if (sengine->data) {
		data = sengine->data->clone();
		}
		mesh_loaded = true;
	}

	void Release(){
	
	
		if (data) {
			delete data;
			data = 0;
		}
		if (mesh) {
			mesh->Release();
			delete mesh;
			mesh = 0;
		}
	
		
	
	
	}
	float getR() {
		float R=0;
		int count=0;
		if (mesh) {
			R += sqrt(MyVec3Dot(mesh->houkatuobb.e,mesh->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}

	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new RoboEngine();
	}

	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};

class RoboBooster : public RoboParts {


public:
	Mesh* mesh;
//	RoboData* data;
public:
	RoboBooster() {
		mesh = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RoboBooster* sengine) {
		if (sengine->mesh) {
		mesh = sengine->mesh->clone();
		}
		if (sengine->data) {
		data = sengine->data->clone();
		}
		mesh_loaded = true;
	}

	void Release(){
	
	
		if (data) {
			delete data;
			data = 0;
		}
		if (mesh) {
			mesh->Release();
			delete mesh;
			mesh = 0;
		}
	
		
	
	
	}


	void loadMesh(Graphics* g, MyTextureLoader* loader);
	float getR() {
		float R=0;
		int count=0;
		if (mesh) {
			R += sqrt(MyVec3Dot(mesh->houkatuobb.e,mesh->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);

	RoboParts* myNew() {
		return new RoboBooster();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};

class RoboFCS : public RoboParts {


public:
	Mesh* mesh;
//	RoboData* data;
public:
	RoboFCS() {
		mesh = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RoboFCS* sengine) {
		if (sengine->mesh) {
		mesh = sengine->mesh->clone();
		}
		if (sengine->data) {
		data = sengine->data->clone();
		}
		mesh_loaded = true;
	}

	void Release(){
	
	
		if (data) {
			delete data;
			data = 0;
		}
		if (mesh) {
			mesh->Release();
			delete mesh;
			mesh = 0;
		}
	
		
	
	
	}


	float getR() {
		float R=0;
		int count=0;
		if (mesh) {
			R += sqrt(MyVec3Dot(mesh->houkatuobb.e,mesh->houkatuobb.e));
			count++;
		}
		if (count) {
			R = R / count;
		}

		return R;
	}

	void loadMesh(Graphics* g, MyTextureLoader* loader);
	void drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	RoboParts* myNew() {
		return new RoboFCS();
	}
	// レンダースレッドとメッセージスレッドの両方をロックして行うこと
	void equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader);
	
};




class RoboState {
public:
	bool isJump();
	bool isJumpKABE();
	bool isBoosterHi();
	virtual bool isCanBoost(Robo* robo);
	virtual bool isCanMoveWhenBoost(Robo* robo);
	virtual void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	virtual void leave(Robo* robo, RoboState* now_state, RoboState* before_state)=0;
	virtual int getStateID()=0;
	virtual void exec(Graphics* g, Robo* robo, float dsecond, int stamp)=0;
};

class RoboMovingState_STOP : public RoboState {
public:
	RoboMovingState_STOP(){};
	~RoboMovingState_STOP(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_FORWARD : public RoboState {
public:
	RoboMovingState_FORWARD(){};
	~RoboMovingState_FORWARD(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_BACK : public RoboState {
public:
	RoboMovingState_BACK(){};
	~RoboMovingState_BACK(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_RIGHT : public RoboState {
public:
	RoboMovingState_RIGHT(){};
	~RoboMovingState_RIGHT(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_LEFT : public RoboState {
public:
	RoboMovingState_LEFT(){};
	~RoboMovingState_LEFT(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_RIGHTTURN : public RoboState {
public:
	RoboMovingState_RIGHTTURN(){};
	~RoboMovingState_RIGHTTURN(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_LEFTTURN : public RoboState {
public:
	RoboMovingState_LEFTTURN(){};
	~RoboMovingState_LEFTTURN(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};


class RoboMovingState_LEFTFORWARD : public RoboState {
public:
	RoboMovingState_LEFTFORWARD(){};
	~RoboMovingState_LEFTFORWARD(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};


class RoboMovingState_RIGHTFORWARD : public RoboState {
public:
	RoboMovingState_RIGHTFORWARD(){};
	~RoboMovingState_RIGHTFORWARD(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};


class RoboMovingState_LEFTBACK : public RoboState {
public:
	RoboMovingState_LEFTBACK(){};
	~RoboMovingState_LEFTBACK(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};


class RoboMovingState_RIGHTBACK : public RoboState {
public:
	RoboMovingState_RIGHTBACK(){};
	~RoboMovingState_RIGHTBACK(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};




class RoboMovingState_JUMP : public RoboState {
private:
	float t;
public:
	RoboMovingState_JUMP(){t = 0;};
	~RoboMovingState_JUMP(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};



class RoboMovingState_FORWARDJUMP : public RoboState {
private:
	float t;
public:
	RoboMovingState_FORWARDJUMP(){};
	~RoboMovingState_FORWARDJUMP(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_BACKJUMP : public RoboState {
private:
	float t;
public:
	RoboMovingState_BACKJUMP(){};
	~RoboMovingState_BACKJUMP(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_LEFTJUMP : public RoboState {
private:
	float t;
public:
	RoboMovingState_LEFTJUMP(){};
	~RoboMovingState_LEFTJUMP(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_RIGHTJUMP : public RoboState {
private:
	float t;
public:
	RoboMovingState_RIGHTJUMP(){};
	~RoboMovingState_RIGHTJUMP(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};



class RoboMovingState_JUMPKABE : public RoboState {
private:
	float t;
public:
	RoboMovingState_JUMPKABE(){t = 0;};
	~RoboMovingState_JUMPKABE(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};



class RoboMovingState_FORWARDJUMPKABE : public RoboState {
private:
	float t;
public:
	RoboMovingState_FORWARDJUMPKABE(){};
	~RoboMovingState_FORWARDJUMPKABE(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_BACKJUMPKABE : public RoboState {
private:
	float t;
public:
	RoboMovingState_BACKJUMPKABE(){};
	~RoboMovingState_BACKJUMPKABE(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_LEFTJUMPKABE : public RoboState {
private:
	float t;
public:
	RoboMovingState_LEFTJUMPKABE(){};
	~RoboMovingState_LEFTJUMPKABE(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboMovingState_RIGHTJUMPKABE : public RoboState {
private:
	float t;
public:
	RoboMovingState_RIGHTJUMPKABE(){};
	~RoboMovingState_RIGHTJUMPKABE(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};


class RoboSETTIState_SETTI : public RoboState {

	public:
	RoboSETTIState_SETTI(){};
	~RoboSETTIState_SETTI(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboSETTIState_KUUTYUU : public RoboState {

	public:
	RoboSETTIState_KUUTYUU(){};
	~RoboSETTIState_KUUTYUU(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboSETTIState_SETKABE : public RoboState { // 壁に接している場合（壁蹴りができる)

	public:
	RoboSETTIState_SETKABE(){};
	~RoboSETTIState_SETKABE(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboSETTIState_SETTENJYOU : public RoboState { // 天井に接している
	public:
	RoboSETTIState_SETTENJYOU(){};
	~RoboSETTIState_SETTENJYOU(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboBoosterState_OFF : public RoboState { 
	public:
	RoboBoosterState_OFF(){};
	~RoboBoosterState_OFF(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboBoosterState_ONTAIKI : public RoboState { 
	public:
	RoboBoosterState_ONTAIKI(){};
	~RoboBoosterState_ONTAIKI(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};


class RoboBoosterCalc {
private:
	Robo* robo;
	RoboBooster* booster;
public:
	RoboBoosterCalc();
	~RoboBoosterCalc();
private:
	float time_maxspeed_made;
	float maxspeed;

	float time_maxspeed_jizoku_made;

	float time_maxato_made;
	float maxatospeed;

	float time_backto_fudan;
	float fudanspeed;

	float time_to_reload;
	float time_to_canmove;

	float energy_drain;
public:
	void Init(Robo* robo, RoboBooster* booster);
	float getFudanSpeed() {return fudanspeed;}
	float getSpeed(float dsecond);
	bool isCanReload(float dsecond);
	bool isCanMove(float dsecond) {
		if (dsecond > time_to_canmove) {
			return true;
		}
		return false;
	}
};

class RoboBoosterState_BOOSTUP : public RoboState { 
private:
	float t;

	public:
	RoboBoosterState_BOOSTUP(){};
	~RoboBoosterState_BOOSTUP(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
	bool isCanBoost(Robo* robo);
	bool isCanMoveWhenBoost(Robo* robo);
};

class RoboBoosterState_BOOSTFORWARD : public RoboState {
private:
	float t;

	public:
	RoboBoosterState_BOOSTFORWARD(){};
	~RoboBoosterState_BOOSTFORWARD(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
	bool isCanBoost(Robo* robo);
	bool isCanMoveWhenBoost(Robo* robo);
};

class RoboBoosterState_BOOSTBACK : public RoboState {
private:
	float t;
	public:
	RoboBoosterState_BOOSTBACK(){};
	~RoboBoosterState_BOOSTBACK(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
	bool isCanBoost(Robo* robo);
	bool isCanMoveWhenBoost(Robo* robo);
};

class RoboBoosterState_BOOSTLEFT : public RoboState {
private:
	float t;
	public:
	RoboBoosterState_BOOSTLEFT(){};
	~RoboBoosterState_BOOSTLEFT(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
	bool isCanBoost(Robo* robo);
	bool isCanMoveWhenBoost(Robo* robo);
};

class RoboBoosterState_BOOSTRIGHT : public RoboState {
private:
	float t;
	public:
	RoboBoosterState_BOOSTRIGHT(){};
	~RoboBoosterState_BOOSTRIGHT(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
	bool isCanBoost(Robo* robo);
	bool isCanMoveWhenBoost(Robo* robo);
};





class RoboAnimeLoop {
private:
	float speed;
	float firstsecond;
	float nowsecond;
	float first;
	float last;
	float now;
	bool is_loop;
	bool is_lock;
public:

	RoboAnimeLoop() {
		speed = 0;
		firstsecond = 0;
		first = 0;
		last = 0;
		now = 0;
		is_loop = 0;
		nowsecond = 0;
		is_lock = false;
	};

	~RoboAnimeLoop() {};

	bool isFinished() {
		if (now >= last) {
			return true;
		}
	}
	void setLock(bool t) {
		is_lock = t;
	}

	void setAnime(float first, float last, bool is_loop) {
		if (is_lock) return;

		this->first = first;
		this->last = last;
		this->is_loop = is_loop;
		this->now = first;
	};
	void setTimeAndSpeed(float speed, float firstsecond) {
		this->speed = speed;
		this->firstsecond = firstsecond;
		nowsecond = firstsecond;
	};

	void reset(float second) {
		now = first;
		firstsecond = second;
	};
	void animateTime(float dsecond) {
		nowsecond += dsecond;
		now += speed * (nowsecond - firstsecond) / (last - first);

		if (now >= last) {
			if (is_loop) {
				reset(nowsecond);
			} else {
				now = last;
			}
		}
	}


	void animate(UMesh* umesh, bool calculateoffsetmatrix);
};

class ArmPositioner;
class ArmPositionerHelper;
class ArmPointIndexInfo;
class Game;
class Texture;
class WeaponFireRifle;

class RoboParam {
private:
	Robo* robo;
	int maxap;
	int allweight;
	int canweight;
	int def;
	int edef;
	int amari_energy;
	int energyshuturyoku;
	int energy_pool;



	int nowap;
	int now_energy;

public:
	RoboBoosterCalc boostercalc;
	
public:
	RoboParam();
	~RoboParam();
	void Init(Robo* robo) {
		this->robo = robo;
	}

	void calcParam();
	int getKoumokuSize() {
		return 8;
	}

	int getMaxAP();
	int getAllWeight();
	int getCanWeight();
	int getDef();
	int getEDef();
	int getAmariEnergy();
	int getEnergyShuturyoku();
	int getEnergyPool();
	char* getNameOfHead();
	char* getNameOfBody();
	char* getNameOfArm();
	char* getNameOfLeg();
	char* getNameOfInside();
	char* getNameOfRArmWeapon();
	char* getNameOfLArmWeapon();
	char* getNameOfRShoulderWeapon();
	char* getNameOfLShoulderWeapon();
	char* getNameOfBooster();
	char* getNameOfFCS();
	char* getNameOfEngine();

};


class Robo : public INPUTSHORICLASS, public AtariBase
{
private:
public:
	static const int KTROBO_ROBO_JUMPKABE_COUNT = 20;
	ArmPositioner* ap;
	ArmPositionerHelper* aphelper;

	ArmPositioner* ap_hidari;
	ArmPositionerHelper* aphelper_hidari;

	ArmPointIndexInfo* apinfo;
public:
	RoboHead* head;
	RoboBody* body;
	RoboLeg* leg;
public:
	RoboArm* arm;
public:
	RoboBooster* booster;
	RoboFCS* fcs;
	RoboEngine* engine;

	RArmWeapon* raweapon;
	LArmWeapon* laweapon;
	RShoulderWeapon* rsweapon;
	LShoulderWeapon* lsweapon;
	InsideWeapon* iweapon;
public:
	RoboParam roboparam;

public:
	RoboState* move_state;
	RoboState* moveturn_state;

	RoboState* setti_state;
	RoboState* setkabe_state;
	RoboState* settenjyou_state;
	RoboState* booster_state;
	int kuutyuu_count;
	int pressed_space_count;
	int setti_count;
	int setkabe_count;
	int setti_jizoku_count;
	int setno_jizoku_count;
	MYVECTOR3 kabe_housen;

	float jump_f_z;
	float jump_f_z_kabe;
	void incPressedSpaceCount() {
		pressed_space_count++;
	}
	void resetPressedSpaceCount() {
		pressed_space_count = 0;
	}
	int getPressedSpaceCount() {
		return pressed_space_count;
	}

	void resetCount() {
		kuutyuu_count = 0;
		setti_count = 0;
	};
	void incKuutyuu() {
		kuutyuu_count++;
	};
	void incSetti() {
		setti_count++;
	};

	void resetSetKabe() {
		setkabe_count = 0;
		//setkabe_state = &kuutyuu;
	}

	void incSetKabe() {
		setkabe_count++;
	}

	RoboBoosterState_BOOSTBACK boostback;
	RoboBoosterState_BOOSTFORWARD boostforward;
	RoboBoosterState_BOOSTLEFT boostleft;
	RoboBoosterState_BOOSTRIGHT boostright;
	RoboBoosterState_BOOSTUP boostup;
	RoboBoosterState_OFF boostoff;
	RoboBoosterState_ONTAIKI boostontaiki;


	RoboSETTIState_KUUTYUU kuutyuu;
	RoboSETTIState_SETKABE setkabe;
	RoboSETTIState_SETTENJYOU settenjyou;
	RoboSETTIState_SETTI setti;

	RoboMovingState_BACK moveback;
	RoboMovingState_FORWARD moveforward;
	RoboMovingState_JUMP movejump;
	RoboMovingState_FORWARDJUMP movejumpforward;
	RoboMovingState_BACKJUMP movejumpback;
	RoboMovingState_LEFTJUMP movejumpleft;
	RoboMovingState_RIGHTJUMP movejumpright;

	RoboMovingState_JUMPKABE movejumpkabe;
	RoboMovingState_FORWARDJUMPKABE movejumpforwardkabe;
	RoboMovingState_BACKJUMPKABE movejumpbackkabe;
	RoboMovingState_LEFTJUMPKABE movejumpleftkabe;
	RoboMovingState_RIGHTJUMPKABE movejumprightkabe;



	RoboMovingState_LEFT moveleft;
	RoboMovingState_RIGHT moveright;
	RoboMovingState_LEFTBACK moveleftback;
	RoboMovingState_RIGHTBACK moverightback;
	RoboMovingState_LEFTFORWARD moveleftforward;
	RoboMovingState_RIGHTFORWARD moverightforward;


	RoboMovingState_STOP movestop;
	RoboMovingState_LEFTTURN moveleftturn;
	RoboMovingState_RIGHTTURN moverightturn;
	float updown_muki;
	int screen_height;
	MYMATRIX world_without_rotx;

public:
	MYMATRIX* getWorldWithoutRotX() {
		return &world_without_rotx;
	}

	void calcWorldWithoutRotX() {
		MYMATRIX world;
		MYMATRIX temp;
		MYMATRIX temp2;
		MYMATRIX temp3;
		MYMATRIX temp4;
		MYMATRIX temp5;	
		MyMatrixRotationX(temp,0);
		MyMatrixRotationY(temp2,atarihan->roty);
		MyMatrixRotationZ(temp3,atarihan->rotz);
		MyMatrixTranslation(temp4,atarihan->x,atarihan->y,atarihan->z);
		MyMatrixScaling(temp5,atarihan->scalex,atarihan->scaley,atarihan->scalez);
		MyMatrixMultiply(world,temp5,temp);
		MyMatrixMultiply(world,world,temp3);
		MyMatrixMultiply(world,world,temp2);
		MyMatrixMultiply(world,world,temp4);	


		world_without_rotx = world;
	}

	//UMeshUnit* atarihan;
	UMesh* atari_leg;
	UMesh* atari_head;
	UMesh* atari_head2;
	UMesh* atari_head3;
	UMesh* atari_core;
	UMesh* atari_rarm;
	UMesh* atari_larm;
	
	RoboAnimeLoop anime_loop_leg;

	MYVECTOR3 target;
	bool is_fireraweapon;

	void remakeUMesh(Graphics* g, MyTextureLoader* tex_loader);
	void settyakuRArmWeaponWithArm();
	void settyakuLArmWeaponWithArm();
	void settyakuBoosterWithLeg();
	void settyakuRShoulderWeaponWithBody();
	void settyakuLShoulderWeaponWithBody();




	void upDownMuki(float mouse_y, float dmouse_y);

	//MYMATRIX world;
	void aim(Graphics* g, MYMATRIX* view);
	void setTarget(MYVECTOR3* t) {
		target = *t;
	}

	bool handleMessage(int msg, void* data, DWORD time);
public:
	Robo(void);
	~Robo(void);

	void boosterEffect(Game* game, Graphics* g, float dt, int stamp);
	void byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	void byougaRay(Graphics* g, MYMATRIX* view, MYMATRIX* proj);

	void init(Graphics* g, MyTextureLoader* tex_loader, AtariHantei* hantei);
	void release();
	void atarishori(Graphics* g , MYMATRIX* view, AtariHantei* hantei, float dt, int stamp);
	void fireUpdate(Graphics* g, Game* game, Scene* scene,BulletController* bullet_c, AtariHantei* hantei, float dt, int stamp);
	void atariAim(Graphics* g, MYMATRIX* view, float dt, int stamp);
	void calcAim(Graphics* g, MYMATRIX* view, float dt, int stamp);
};

}
#endif