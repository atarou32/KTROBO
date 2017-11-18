#ifndef KTROBOEFFECT_H
#define KTROBOEFFECT_H

#pragma once
#include "KTRoboTexture.h"
#include "MyButukari.h"
#include "MyDefine.h"
#include "tolua_glue/MyLuaGlueMakeCommon.h"

namespace KTROBO {

// billboard

#define KTROBO_EFFECT_DUMMY_INDEX 0
#define KTROBO_EFFECT_DUMMY_ID 0
#define KTROBO_EFFECT_IMPL_DUMMY_INDEX 0
#define KTROBO_EFFECT_IMPL_DUMMY_ID 0
#define KTROBO_EFFECT_NONE 0



interface IEffectManager {
public:
	TO_LUA virtual int getEffect(char* effect_name, char* texture_name, char* lua_file_name)=0;
	TO_LUA virtual void makeEffectPart(int effect_id,int index, float endtime, bool is_loop, float time_when_loop, float plustime_loop)=0;
	TO_LUA virtual void setEffectPartPos(int effect_id, int index, float dtime_start, float dtime_end, float start_x, float start_y, float start_z, float end_x, float end_y, float end_z)=0;
	TO_LUA virtual void setEffectPartTexPos(int effect_id, int index, float dtime_start, float dtime_end, int tex_x,int tex_y, int tex_width, int tex_height)=0;
	TO_LUA virtual void setEffectPartWH(int effect_id, int index, float dtime_start, float dtime_end,float start_width, float start_height, float end_width, float end_height)=0;
	TO_LUA virtual void setEffectPartRot(int effect_id, int index, float dtime_start, float dtime_end, float start_rotx, float start_roty, float start_rotz, float end_rotx, float end_roty, float end_rotz)=0;
	TO_LUA virtual void setEffectPartColor(int effect_id, int index, float dtime_start, float dtime_end, unsigned int start_color, unsigned int end_color)=0;

	TO_LUA virtual void deleteEffect(int effect_id)=0;
	TO_LUA virtual int getEffectImpl(int effect_id)=0;
	TO_LUA virtual void setEffectImplWorld(int effect_impl_id, YARITORI MYMATRIX* world)=0;
	TO_LUA virtual void setEffectImplTime(int effect_impl_id, float time)=0;
	TO_LUA virtual void setEffectImplIsRender(int effect_impl_id, bool t)=0;
	TO_LUA virtual void setEffectImplIsStart(int effect_impl_id, bool t)=0;
	TO_LUA virtual void lightdeleteEffectImpl(int effect_impl_id)=0;
	TO_LUA virtual void deleteEffectImpl()=0;
};

class Effect;

class EffectImpl {
private:
	bool is_activated; // updateの際に対象とするかどうか
	int activate_jyunbi_count;
	int effect_id;
	MYMATRIX world; // 初期位置姿勢
	float time; // 発生してからのの再生時間
	int effect_impl_id;

public:
	vector<int> effectpart_billboard_ids; // effectpartの数だけ入る int は　billboard_id
	vector<float> looptime_effectpart;
public:
	int getEffectImplID() {return effect_impl_id;}
	int getEffectID() {return effect_id;}
/*	EffectImpl(int effect_impl_id) {
		this->effect_impl_id = effect_impl_id;
		MyMatrixIdentity(world);
		time =0;
		effect_id = 0;
		is_activated = false;
	}
*/
	EffectImpl(int effect_impl_id, int effect_id) {
		time = 0;
		this->effect_impl_id = effect_impl_id;
		is_activated = false;
		activate_jyunbi_count = 0;
		this->effect_id = effect_id;
		MyMatrixIdentity(this->world);
	}
	void release(Texture* t) {
		int siz = effectpart_billboard_ids.size();
		for (int i=0;i<siz;i++) {
			t->setRenderBillBoardIsRender(effectpart_billboard_ids[i],false);
			t->lightdeleteRenderBillBoard(effectpart_billboard_ids[i]);
		}
	}

	float getTime() {return time;}
	void setTime(float t) {time = t;}
	MYMATRIX* getWorld() {return &world;}
	void setWorld(MYMATRIX* world) {
		this->world = *world;
	}

	bool getIsActivated() {
		
		
		return is_activated;
	
	}

	bool isActivateJyunbiOK() {
		if (activate_jyunbi_count > 2) {
			return true;
		}
		return false;
	}

	void incActivateJyunbi() {
		if (activate_jyunbi_count <= 2) {
			activate_jyunbi_count ++;
		}
	}

	void setIsActivated(bool t) {
		if (t) {
			activate_jyunbi_count = 0;
			
		}

		is_activated = t;
	
	
	
	}

	

	void changeEffectID(Texture* texture, Effect* mae_effect, Effect* new_effect);
};

class EffectPartPart {
protected:
	float dtime_start;
	float dtime_end;



public:
	bool isInTime(float time);
	bool isStartedTime(float time);

	EffectPartPart(float dtime_start, float dtime_end) {
		this->dtime_start = dtime_start;
		this->dtime_end = dtime_end;
	
	}
};

class EffectPartPos : public EffectPartPart {
private:
	float start_x;
	float start_y;
	float start_z;
	float end_x;
	float end_y;
	float end_z;
public:
	EffectPartPos(float dtime_start, float dtime_end, float start_x, float start_y, float start_z, float end_x, float end_y, float end_z) : EffectPartPart(dtime_start, dtime_end){
		this->start_x = start_x;
		this->start_y = start_y;
		this->start_z = start_z;
		this->end_x = end_x;
		this->end_y = end_y;
		this->end_z = end_z;
	}

	float getXOfT(float time);
	float getYOfT(float time);
	float getZOfT(float time);
};

class EffectPartTexPos : public EffectPartPart {
private:
	int tex_x;
	int tex_y;
	int tex_width;
	int tex_height;
public:
	EffectPartTexPos ( float dtime_start, float dtime_end, int tex_x, int tex_y, int tex_width, int tex_height) : EffectPartPart(dtime_start, dtime_end) {
		this->tex_x = tex_x;
		this->tex_y = tex_y;
		this->tex_width = tex_width;
		this->tex_height = tex_height;
	}

	void setTexPos(Texture* t, int billboard_id);


};

class EffectPartWH : public EffectPartPart {
private:
	float start_width;
	float start_height;
	float end_width;
	float end_height;
public:
	EffectPartWH(float dtime_start, float dtime_end, float start_width, float start_height, float end_width, float end_height) : EffectPartPart ( dtime_start, dtime_end) {
		this->start_width = start_width;
		this->start_height = start_height;
		this->end_width = end_width;
		this->end_height = end_height;
	}

	float getWidthOfT(float time);
	float getHeightOfT(float time);
};

class EffectPartRot : public EffectPartPart {
private:
	float start_rotx;
	float start_roty;
	float start_rotz;
	float end_rotx;
	float end_roty;
	float end_rotz;
public:
	EffectPartRot(float dtime_start, float dtime_end, float start_rotx, float start_roty, float start_rotz, float end_rotx, float end_roty, float end_rotz) : EffectPartPart ( dtime_start, dtime_end) {
		this->start_rotx = start_rotx;
		this->start_roty = start_roty;
		this->start_rotz = start_rotz;
		this->end_rotx = end_rotx;
		this->end_roty = end_roty;
		this->end_rotz = end_rotz;
	}

	float getRotXOfT(float time);
	float getRotYOfT(float time);
	float getRotZOfT(float time);
};

class EffectPartColor : public EffectPartPart {
private:
	unsigned int start_color;
	unsigned int end_color;
public:
	EffectPartColor(float dtime_start, float dtime_end, unsigned int start_color, unsigned int end_color) : EffectPartPart(dtime_start, dtime_end) {
		this->start_color = start_color;
		this->end_color = end_color;
	};

	unsigned int getColorOfT(float time);


};

class EffectPart {
private:
	//int billboard_id; // billboard_id は　effectpartimpl に入れる
	
	bool is_loop;
	float end_time;
	float time_when_loop; 
	float plustime_loop;
public:
	vector<EffectPartPos*> poss;
	vector<EffectPartTexPos*> tex_poss;
	vector<EffectPartWH*> whs;
	vector<EffectPartRot*> rots;
	vector<EffectPartColor*> colors; // カラーの調整　（透明度など）

public:
	EffectPart(bool is_loop, float end_time, float time_when_loop, float plustime_loop) {
		this->is_loop = is_loop;
		this->end_time = end_time;
		this->time_when_loop = time_when_loop;
		this->plustime_loop = plustime_loop;
	};
	~EffectPart() {
		Release();
	}

	void Release() {

		vector<EffectPartPos*>::iterator it_pos = poss.begin();
		while(it_pos != poss.end()) {
			EffectPartPos* posp = *it_pos;
			delete posp;
			posp = 0;
			it_pos++;
		}
		poss.clear();

		vector<EffectPartTexPos*>::iterator it_texpos = tex_poss.begin();
		while(it_texpos != tex_poss.end()) {
			EffectPartTexPos* texposp = *it_texpos;
			delete texposp;
			texposp = 0;
			it_texpos++;
		}
		tex_poss.clear();

		vector<EffectPartWH*>::iterator it_wh = whs.begin();
		while(it_wh != whs.end()) {
			EffectPartWH* whp = *it_wh;
			delete whp;
			whp = 0;
			it_wh++;
		}
		whs.clear();

		vector<EffectPartRot*>::iterator it_rot = rots.begin();
		while(it_rot != rots.end()) {
			EffectPartRot* rotp= *it_rot;
			delete rotp;
			rotp = 0;
			it_rot++;
		}
		rots.clear();

		vector<EffectPartColor*>::iterator it_color = colors.begin();
		while(it_color != colors.end()) {
			EffectPartColor* colorp = *it_color;
			delete colorp;
			colorp = 0;
			it_color++;
		}
		colors.clear();
	};


	bool isLoop() {return is_loop;}
	float getEndTime() {return end_time;}
	float getTimeWhenLoop() {return time_when_loop;}
	float getPlusTimeLoop() {return plustime_loop;}

	void update(EffectImpl* effect_impl, Texture* texture,  float loop_time, int billboard_id, float dtime, int stamp); // 該当するエフェクトパートの状態を更新する
};

class Effect
{
private:
	int effect_id;
	char effect_name[64];
	char file_name[256]; // Luafilename
	vector<EffectPart*> parts;
	int texture_id;
	char texture_name[256];
public:

	int getPartsSize() {
		return parts.size();
	}

	EffectPart* getPart(int index) {
		return parts[index];
	}

	void setParts(EffectPart* p) {
		parts.push_back(p);
	};

	Effect(int effect_id);
	~Effect(void);
	void Release();
	void setName(char* name);
	void setFileName(char* filename);
	int getEffectID() {
		return effect_id;
	};
	int getTextureID() {
		return texture_id;
	}

	void setTextureIDAndName(int texture_id, char* texture_name);
	void update(EffectImpl* effect_impl, Texture* texture, float dtime, int stamp); // 該当するエフェクトの実体の状態を更新する
};

class EffectManager : public IEffectManager {
private:
	Texture* tex;
	vector<Effect*> effects;
	vector<EffectImpl*> effect_impls;
	map<string, int> effect_name_indexs;
	map<int,int> effect_id_indexs;
	map<int,int> effect_impl_id_indexs;
	set<int> unuse_effect_impl_indexs;
	set<int> used_unuse_effect_impl_indexs;

	Effect* dummy_effect;
	EffectImpl* dummy_effect_impl;

	bool hasEffectPartIndex(int effect_id, int index);
	EffectImpl* getUnUsedEffectImpl(Effect* efe);
public:
	void loadFileFromLua(int task_index, char* filename); // どのスレッドから呼んでいるか指定する
	EffectManager(Texture* tex);
	int getEffectFromName(char* effect_name);

	int getEffect(char* effect_name, char* texture_name, char* lua_file_name); // 同じnameを指定した場合は同じIDが返る　ない場合は作られる
	//int getEffectFromFileName(char* file_name); // effect_nameはファイル中の設定の値が入る　基本的に１エフェクト１ファイル ない場合は0が返る posに関してはループしない
	void makeEffectPart(int effect_id,int index, float endtime, bool is_loop, float time_when_loop, float plustime_loop); // indexは　0 1 2 3 4 5 の順にゲットすること　急に１０とかゲットしようとしてもindexは増えない すでに作られていたindexの場合は作られない
	void setEffectPartPos(int effect_id, int index, float dtime_start, float dtime_end, float start_x, float start_y, float start_z, float end_x, float end_y, float end_z);
	void setEffectPartTexPos(int effect_id, int index, float dtime_start, float dtime_end, int tex_x,int tex_y, int tex_width, int tex_height);
	void setEffectPartWH(int effect_id, int index, float dtime_start, float dtime_end,float start_width, float start_height, float end_width, float height);
	void setEffectPartRot(int effect_id, int index, float dtime_start, float dtime_end, float start_rotx, float start_roty, float start_rotz, float end_rotx, float end_roty, float end_rotz);
	void setEffectPartColor(int effect_id, int index, float dtime_start, float dtime_end, unsigned int start_color, unsigned int end_color);

	void deleteEffect(int effect_id); // effect_part_idを開放する　effect自体は残す

	void release(); // すべてを開放する dummyも消す

	int getEffectImpl(int effect_id); // effectの実体を作る
	void setEffectImplWorld(int effect_impl_id, MYMATRIX* world);
	void setEffectImplTime(int effect_impl_id, float time);
	void setEffectImplIsRender(int effect_impl_id, bool t);
	void setEffectImplIsStart(int effect_impl_id, bool t);
	void lightdeleteEffectImpl(int effect_impl_id); // effect_impl を未使用にする 消しはしない（再利用する）
	void deleteEffectImpl(); // effect_implを消去する
	bool getEffectImplIsActivated(int effect_impl_id); // CS_RENDERDATA_CS をロックしてから呼ぶこと

	void update(float dtime, int stamp); // 管理しているエフェクトの実体の状態を更新する
	

};

class EffectManagers {
private:
	Texture* tex;
	vector<EffectManager*> managers;
public:

	EffectManagers(Texture* tex);
	~EffectManagers();


	// getinstance getinterface を作る
	IEffectManager* getInterface(int index) {

		return (IEffectManager*)managers[0];
	}
	EffectManager* getInstance(int index) {
		return managers[0];
	}

	void update(float dtime, int stamp);
	int makeInst() {
		return 0;
	}

};

}


#endif