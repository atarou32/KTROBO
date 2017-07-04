#ifndef KTROBO_ROBO_H
#define KTROBO_ROBO_H

#pragma once
#include "KTRoboMesh.h"
#include "MyTokenAnalyzer.h"
#include "KTRoboAtari.h"
#include "KTRoboInput.h"
//#include "KTROBOArmPositioner.h"
#include "KTRoboWeapon.h"

namespace KTROBO {

struct RoboDataPart {
public:
	int int_data;
	char data_name[16];
	char data_name2[32];
	char string_data[96];
	float float_data;
	RoboDataPart() {
		int_data = 0;
		memset(data_name,0,16);
		memset(data_name2,0,32);
		memset(string_data,0,96);
		float_data = 0;
	}
};

class RoboData {
	vector<RoboDataPart*> datas;
public:
	void setData(int int_data, char* data_name, char* data_name2, char* string_data, float float_data) {
		RoboDataPart* p = new RoboDataPart();
		p->int_data = int_data;
		strcpy_s(p->data_name,16,data_name);
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

		return 0;
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
	char data_name[16];
	char data_name2[32];
	char data_type[32];
	char data_sentence[32];
	char data_compare[32];
	void clear() {
		memset(data_name,0,16);
		memset(data_name2,0,32);
		memset(data_type,0,32);
		memset(data_sentence,0,32);
		memset(data_compare,0,32);
	}
	void readline(MyTokenAnalyzer* ma) {
		ma->GetToken();
		strcpy_s(data_name,16,ma->Toke());
		ma->GetToken();
		strcpy_s(data_name2,32,ma->Toke());
		ma->GetToken();
		strcpy_s(data_type,32,ma->Toke());
		ma->GetToken();
		strcpy_s(data_sentence,32,ma->Toke());
		ma->GetToken();
		strcpy_s(data_compare,32,ma->Toke());
	}

	RoboMetaDataPart() {
		memset(data_name,0,16);
		memset(data_name2,0,32);
		memset(data_type,0,32);
		memset(data_sentence,0,32);
		memset(data_compare,0,32);
	}
};

class RoboDataMetaData {
	vector<RoboMetaDataPart*> metadatas;
public:
	void setData(char* data_name, char* data_name2, char* data_type, char* data_sentence, char* data_compare) {
		RoboMetaDataPart* p = new RoboMetaDataPart();
		strcpy_s(p->data_name,16,data_name);
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


class RoboHead {
public:
	Mesh* head;
	Mesh* head2;
	Mesh* head3;

	RoboData* data;

public:
	RoboHead() {
		
		head = 0;
		head2 = 0;
		head3 = 0;
		data = 0;
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

};

class RoboArm {
public:
	Mesh* rarm;
	Mesh* larm;
	RoboData* data;
public:
	RoboArm() {
		rarm = 0;
		larm = 0;
		data = 0;
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

};

class RoboLeg {
public:
	Mesh* leg;
	RoboData* data;
public:
	RoboLeg() {
		leg = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RoboLeg* sleg) {
		if (sleg->leg) {
			leg = sleg->leg->clone();
		}
		if (sleg->data) {
			data = sleg->data->clone();
		}
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
};

class RoboBody {
public:
	Mesh* body;
	RoboData* data;
public:
	RoboBody() {
		body = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RoboBody* sbody) {
		if (sbody->body) {
		body = sbody->body->clone();
		}
		if (sbody->data) {
			data = sbody->data->clone();
		}
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
};


class RArmWeapon {
public:
	Mesh* weapon;
	RoboData* data;

	WeaponFireRifle wf_rifle;
public:
	RArmWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(RArmWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
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


};

class LArmWeapon {
public:
	Mesh* weapon;
	RoboData* data;
public:
	LArmWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader);
	void init(LArmWeapon* sweapon) {
		if (sweapon->weapon) {
			weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
			data = sweapon->data->clone();
		}
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

};

class RShoulderWeapon {
public:
		Mesh* weapon;
		RoboData* data;
public:
	RShoulderWeapon() {
		weapon = 0;
		data = 0;
	}

	void init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader); 
	void init(RShoulderWeapon* sweapon) {
		if (sweapon->weapon) {
		weapon = sweapon->weapon->clone();
		}
		if (sweapon->data) {
		data = sweapon->data->clone();
		}
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

};

class LShoulderWeapon {
public:
	Mesh* weapon;
	RoboData* data;
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
};

class InsideWeapon {
public:
	Mesh* weapon;
	RoboData* data;
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
};

class RoboEngine {

public:
	Mesh* mesh;
	RoboData* data;
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






};

class RoboBooster {


public:
	Mesh* mesh;
	RoboData* data;
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





};

class RoboFCS {


public:
	Mesh* mesh;
	RoboData* data;
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






};


class Robo;

class RoboState {
public:
	bool isJump();
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

class RoboSETTIState_SETKABE : public RoboState { // •Ç‚ÉÚ‚µ‚Ä‚¢‚éê‡i•ÇR‚è‚ª‚Å‚«‚é)

	public:
	RoboSETTIState_SETKABE(){};
	~RoboSETTIState_SETKABE(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboSETTIState_SETTENJYOU : public RoboState { // “Vˆä‚ÉÚ‚µ‚Ä‚¢‚é
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

class RoboBoosterState_BOOSTUP : public RoboState { 
	public:
	RoboBoosterState_BOOSTUP(){};
	~RoboBoosterState_BOOSTUP(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboBoosterState_BOOSTFORWARD : public RoboState { 
	public:
	RoboBoosterState_BOOSTFORWARD(){};
	~RoboBoosterState_BOOSTFORWARD(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboBoosterState_BOOSTBACK : public RoboState { 
	public:
	RoboBoosterState_BOOSTBACK(){};
	~RoboBoosterState_BOOSTBACK(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboBoosterState_BOOSTLEFT : public RoboState { 
	public:
	RoboBoosterState_BOOSTLEFT(){};
	~RoboBoosterState_BOOSTLEFT(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
};

class RoboBoosterState_BOOSTRIGHT : public RoboState { 
	public:
	RoboBoosterState_BOOSTRIGHT(){};
	~RoboBoosterState_BOOSTRIGHT(){};
	void enter(Robo* robo, RoboState* now_state, RoboState* before_state);
	void leave(Robo* robo, RoboState* now_state, RoboState* before_state);
	int getStateID();
	void exec(Graphics* g, Robo* robo, float dsecond, int stamp);
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
public:

	RoboAnimeLoop() {
		speed = 0;
		firstsecond = 0;
		first = 0;
		last = 0;
		now = 0;
		is_loop = 0;
		nowsecond = 0;
	};

	~RoboAnimeLoop() {};

	bool isFinished() {
		if (now >= last) {
			return true;
		}
	}
	void setAnime(float first, float last, bool is_loop) {
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
class Robo : public INPUTSHORICLASS
{
private:
public:
	ArmPositioner* ap;
	ArmPositionerHelper* aphelper;

	ArmPositioner* ap_hidari;
	ArmPositionerHelper* aphelper_hidari;

	ArmPointIndexInfo* apinfo;
private:
	RoboHead* head;
	RoboBody* body;
	RoboLeg* leg;
public:
	RoboArm* arm;
private:
	RoboBooster* booster;
	RoboFCS* fcs;
	RoboEngine* engine;

	RArmWeapon* raweapon;
	LArmWeapon* laweapon;
	RShoulderWeapon* rsweapon;
	LShoulderWeapon* lsweapon;
	InsideWeapon* iweapon;


public:
	RoboState* move_state;
	RoboState* setti_state;
	RoboState* setkabe_state;
	RoboState* settenjyou_state;
	RoboState* booster_state;
	int kuutyuu_count;
	int setti_count;
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
	RoboMovingState_LEFT moveleft;
	RoboMovingState_RIGHT moveright;
	RoboMovingState_STOP movestop;
	RoboMovingState_LEFTTURN moveleftturn;
	RoboMovingState_RIGHTTURN moverightturn;

public:
	UMeshUnit* atarihan;
	UMesh* atari_leg;
	RoboAnimeLoop anime_loop_leg;

	MYVECTOR3 target;
	bool is_fireraweapon;



	//MYMATRIX world;
	void aim(Graphics* g, MYMATRIX* view);
;
	bool handleMessage(int msg, void* data, DWORD time);
public:
	Robo(void);
	~Robo(void);

	void byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj);
	void init(Graphics* g, MyTextureLoader* tex_loader, AtariHantei* hantei);
	void release();
	void atarishori(Graphics* g , MYMATRIX* view, AtariHantei* hantei, float dt, int stamp);
	void fireUpdate(Graphics* g, MyTextureLoader* tex_loader, MYMATRIX* view, AtariHantei* hantei, float dt, int stamp, Game* game, Texture* tex);

};

}
#endif