#ifndef KTROBOEFFECT_H
#define KTROBOEFFECT_H

#pragma once
#include "KTRoboTexture.h"
#include "MyButukari.h"

namespace KTROBO {

// billboard

class EffectImpl {
private:
	bool is_activated; // 適用したかどうか
	int effect_id;
	MYMATRIX world; // 初期位置姿勢
	float time; // 発生したときの時間

	vector<int> effectpart_billboard_ids; // effectpartの数だけ入る int は　billboard_id
	vector<float> looptime_effectpart;

	bool getIsActivated() {return is_activated;}
	void setIsActivated(bool t) {is_activated = t;}
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
	int texture_id;
	char texture_name[256];
	bool is_loop;
	float end_time;
	float time_when_loop; 
	float plustime_loop;
	vector<EffectPartPos*> poss;
	vector<EffectPartTexPos*> tex_poss; // テクスチャ名も入る
	vector<EffectPartWH*> whs;
	vector<EffectPartRot*> rots;
	vector<EffectPartColor*> colors; // カラーの調整　（透明度など）


	void update(EffectImpl* effect_impl, float loop_time, int billboard_id, float dtime, int stamp); // 該当するエフェクトパートの状態を更新する
};

class Effect
{
private:
	char effect_name[64];
	char file_name[256]; // Luafilename
	vector<EffectPart*> parts;

public:
	Effect(void);
	~Effect(void);
	void setName(char* name);
	void loadFileFromLua(char* filename);
	void update(EffectImpl* effect_impl, float dtime, int stamp); // 該当するエフェクトの実体の状態を更新する
};

class EffectManager {
private:
	Texture* tex;
public:

	EffectManager(Texture* tex);

	int getEffect(char* effect_name); // 同じnameを指定した場合は同じIDが返る
	int getEffectFromFile(char* file_name); // effect_nameはファイル中の設定の値が入る　基本的に１エフェクト１ファイル
	int getEffectPart(int effect_id,int index,int texture_id, float endtime, bool is_loop, float time_when_loop, float plustime_loop); // indexは　0 1 2 3 4 5 の順にゲットすること　急に１０とかゲットしようとしてもindexは増えない
	void setEffectPartPos(int effect_part_id, float dtime_start, float dtime_end, float start_x, float start_y, float start_z, float end_x, float end_y, float end_z);
	void setEffectPartTexPos(int effect_part_id, float dtime_start, float dtime_end, int tex_x,int tex_y, int tex_width, int tex_height);
	void setEffectPartWH(int effect_part_id, float dtime_start, float dtime_end,float start_width, float start_height, float end_width, float height);
	void setEffectPartRot(int effect_part_id, float dtime_start, float dtime_end, float start_rotx, float start_roty, float start_rotz, float end_rotx, float end_roty, float end_rotz);
	void setEffectPartColor(int effect_part_id, float dtime_start, float dtime_end, unsigned int start_color, unsigned int end_color);

	void deleteEffect(int effect_id); // effect_part_idを開放する　effect自体は残す

	void release(); // すべてを開放する

	int getEffectImpl(int effect_id); // effectの実体を作る
	int setEffectImplWorld(int effect_impl_id, MYMATRIX* world);
	int setEffectImplTime(int effect_impl_id, float time);
	void setEffectImplIsRender(int effect_impl_id, bool t);
	void setEffectImplIsStart(int effect_impl_id, bool t);
	void lightdeleteEffectImpl(int effect_impl_id); // effect_impl を未使用にする 消しはしない（再利用する）
	void deleteEffectImpl(int effect_impl_id); // effect_implを消去する

	void update(float dtime, int stamp); // 管理しているエフェクトの実体の状態を更新する
	

};

class EffectManagers {


	// getinstance getinterface を作る




};

}


#endif