#ifndef KTROBOLUACMESH_H
#define KTROBOLUACMESH_H

#pragma once
#include "KTRoboMesh.h"
#include "KTRoboLuaCollection.h"
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include <vector>
#include <map>
using namespace std;

namespace KTROBO {
class CMesh;

interface ICMesh {
public:
	TO_LUA virtual int getMesh(char* path_without_dot)=0;
	TO_LUA virtual int getMeshWithAnime(char* path_to_mesh, char* path_to_anime)=0;
	TO_LUA virtual bool getIsLoad()=0;
	TO_LUA virtual void setIsLoad(bool t)=0;
	TO_LUA virtual void deleteMesh(char* path_without_dot)=0;
	TO_LUA virtual void deleteALL()=0;
	TO_LUA virtual void copyDAYO(COLLECTED CMesh* src)=0;
};

struct Meshdayo {
	Mesh* mesh;
	bool has_responsibility; // ÉfÉäÅ[ÉgÇ∑ÇÈå†óòÇÇ‡Ç¡ÇƒÇ¢ÇÈÇ©
};

class CMesh : public ICMesh, public Loadable {
private:
	vector<Meshdayo*> meshs;
	map<string, int> meshname_index;
	
	Graphics* g;
	MyTextureLoader* tex_loader;
public:
	int getMesh(char* path_without_dot);
	int getMeshWithAnime(char* path_to_mesh, char* path_to_anime);
	bool getIsLoad();
	void setIsLoad(bool t);
	void deleteMesh(char* path_without_dot);
	void deleteALL();
	Mesh* getCollectedMesh(int index);
	CMesh(Graphics* g, MyTextureLoader* tex_loader);
	~CMesh();
	void copyDAYO(COLLECTED CMesh* src);
};

class CMeshs {
private:
	vector<CMesh*> cmeshs;
	Graphics* g;
	MyTextureLoader* tex_loader;
public:
	CMeshs(Graphics* g, MyTextureLoader* tex_loader);
	~CMeshs();
	int makeInstance();
	CMesh* getInstance(int index);
	ICMesh* getInterface(int index);

};
}
#endif