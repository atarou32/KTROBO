#ifndef KTROBOANIMATIONBUILDER_H
#define KTROBOANIMATIONBUILDER_H

#pragma once
#include "KTRoboMesh.h"
#include "KTRoboScene.h"

namespace KTROBO {



interface IABuilder {
	int createAnimationBuilderImpl(char* hon_filepath);
	void setOnajiMesh(int impl_id, char* onaji_filepath);
	void setKoMesh(int impl_id, char* ko_filepath, char* parent_bone_name);
	int getHonMeshBoneNum(int impl_id);
	char* getHonMeshBoneName(int impl_id, int bone_index);

	void setHonMeshBoneRotX(int impl_id, int bone_index, float rotx);
	void setHonMeshBoneRotY(int impl_id, int bone_index, float roty);
	void setHonMeshBoneRotZ(int impl_id, int bone_index, float rotz);
	void setHonMeshBoneRotXIsChange(int impl_id, int bone_index, bool t); // falseÇ…Ç∑ÇÈÇ∆âÒì]Ç™0Ç…Ç»ÇËIKÇ≈Ç‡âÒì]ÇµÇ»Ç≠Ç»ÇÈ
	void setHonMeshBoneRotYIsChange(int impl_id, int bone_index, bool t);
	void setHonMeshBoneRotZIsChange(int impl_id, int bone_index, bool t);

	void setAnimePoseFrame(int impl_id, int frame);// åªç›ÇÃÇ∆Ç¡ÇƒÇ¢ÇÈépê®ÇéwíËÇµÇΩÉAÉjÉÅÉtÉåÅ[ÉÄÇ∆ÇµÇƒï€ë∂Ç∑ÇÈ
	
	int createFrameExe(int impl_id, char* frameexe_name, bool is_loop);
	void setFrameToExe(int impl_id, int frameexe_id, int frame, float time);


};


class AnimationBuilderMesh {
	Mesh* mesh;
	bool mesh_loaded;
	char mesh_meshpath[128];
	char mesh_animepath[128];

};

class AnimationBuilderImpl {
	AnimationBuilderMesh* hon_mesh;
	vector<AnimationBuilderMesh*> onaji_mesh;
	vector<AnimationBuilderMesh*> ko_mesh;



};

class AnimationBuilder : Scene
{
private:

public:
	AnimationBuilder(char* c, int len);
	virtual ~AnimationBuilder(void) {
		Scene::~Scene();
	}
	void enter() {
		Scene::enter();
	}
	void leave() {
		Scene::leave();
	}

	void mainrenderIMPL(bool is_focused, Graphics* g, Game* game) {};
	void renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};
	void aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};
	void posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};
	void loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {};

};


}






#endif