#ifndef KTROBO_MAP_H
#define KTROBO_MAP_H


#pragma once
#include <vector>
#include <map>

#include "KTRoboMeshInstanced.h"
#include "KTRoboAtari.h"

namespace KTROBO {
enum MAP_ENTITY_TYPE {
	MAP_ENTITY_UNKNOWN =0,
	MAP_ENTITY_TIKEI=1,
	MAP_ENTITY_OBJECT=2,
	MAP_ENTITY_ROBO=3,
	MAP_ENTITY_CHARA=4,
};

class MAP_ENT {
public:
	MAP_ENT() {};
	virtual ~MAP_ENT() {};

	

};
class MAP_ENT_CHARA : public MAP_ENT {
private:
	UMeshUnit * unit;
public:
	MAP_ENT_CHARA(UMeshUnit* u) {
		unit = u;
	};
	virtual ~MAP_ENT_CHARA() {};


};
class MAP_ENT_TIKEI : public MAP_ENT {
private:
	UMeshUnit * unit;
	MeshInstanced* mesh_inst;
	
public:
	MAP_ENT_TIKEI(UMeshUnit* u , MeshInstanced* mi) {
		unit = u;
		mesh_inst = mi;
	};
	virtual ~MAP_ENT_TIKEI() {};

};

class MAP_ENT_OBJECT : public MAP_ENT {

public:
	MAP_ENT_OBJECT() {};
	virtual ~MAP_ENT_OBJECT() {};
};

class Robo;

class MAP_ENT_ROBO : public MAP_ENT {
private:
	Robo* rob;
public: 
	// robo をデストラクトする
	MAP_ENT_ROBO(Robo* r) { rob = r; };
	virtual ~MAP_ENT_ROBO();

};

struct MAP_STRUCT {
	int struct_id; // map毎
	MAP_ENTITY_TYPE struct_type;
	MAP_ENT* ent;
};



class RMap
{
private:
	AtariHantei * hantei;
	MeshInstanceds* mesh_instanceds;

	private:
		map <string, int> map_mesh_names;
		vector<Mesh*> map_meshs;
		vector<UMeshUnit*> map_umeshunits;
		vector<MeshInstanced*> map_meshinstanceds;
		vector<MAP_STRUCT*> map_structs;
		int max_struct_id;
		Robo* player_robo;
	
	public:
		RMap() { max_struct_id = 0; hantei = 0; mesh_instanceds = 0; player_robo = 0; };
		~RMap() {};
		
		void registerMesh(string name, Mesh* m);
		void registerTikei(UMeshUnit* um, MeshInstanced* mi);
		void registerRobo(Robo* r);
		void registerObject();
		void registerChara(UMeshUnit* um);
		void changePlayer(Robo* r); // player_roboだけを置き換える


		void Init(AtariHantei* h, MeshInstanceds* mids) {
			this->hantei = h;
			this->mesh_instanceds = mids;
		}
		void Del() {
			// mesh と　map_struct と　umesh_unitのみ　消す　他は他で消す
			vector<UMeshUnit*>::iterator itt = map_umeshunits.begin();
			while (itt != map_umeshunits.end()) {
				UMeshUnit* mm = *itt;
				if (mm) {
					delete mm;
					mm = 0;
				}
				itt++;
			}
			map_umeshunits.clear();
			
			vector<Mesh*>::iterator it = map_meshs.begin();
			while (it != map_meshs.end()) {
				Mesh* m = *it;
				if (m) {
					delete m;
					m = 0;
				}

				it++;
			}
			map_meshs.clear();



			vector<MAP_STRUCT*>::iterator it_m = map_structs.begin();
			while (it_m != map_structs.end()) {
				MAP_STRUCT* ms = *it_m;
				if (ms) {
					if (ms->ent) {
						delete ms->ent;
						ms->ent = 0;
					}

					delete ms;
					ms = 0;

				}


				it_m++;
			}
			map_structs.clear();


		}

};
class Game;
class RMapSetterExample {



public:

	RMapSetterExample(Graphics* g, Game* game, AtariHantei* hantei, MeshInstanceds* mids, MyTextureLoader* loader, RMap* m);

};


}

#endif