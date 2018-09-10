#include "KTRoboMap.h"
#include "KTRoboInput.h"
#include "KTRoboRobo.h"
#include "KTRoboGame.h"

using namespace KTROBO;



void RMap::registerMesh(string name, Mesh* m) {
	if (map_mesh_names.find(name) != map_mesh_names.end()) {


	}
	else {
		// register
		int index = map_meshs.size();
		map_meshs.push_back(m);
		map_mesh_names.insert(std::pair<string, int>(name, index));
	}

}

void RMap::registerTikei(UMeshUnit* um, MeshInstanced* mi) {
	MAP_ENT_TIKEI* ti = new MAP_ENT_TIKEI(um,mi);
	MAP_STRUCT* ms = new MAP_STRUCT();
	ms->struct_type = MAP_ENTITY_TYPE::MAP_ENTITY_TIKEI;
	ms->struct_id = max_struct_id;
	max_struct_id++;
	ms->ent = ti;
	map_structs.push_back(ms);
	map_meshinstanceds.push_back(mi);
	map_umeshunits.push_back(um);

}

void RMap::registerChara(UMeshUnit* um) {
	MAP_ENT_CHARA* ch = new MAP_ENT_CHARA(um);
	MAP_STRUCT* ms = new MAP_STRUCT();
	ms->struct_type = MAP_ENTITY_TYPE::MAP_ENTITY_CHARA;
	ms->struct_id = max_struct_id;
	max_struct_id++;
	ms->ent = ch;
	map_structs.push_back(ms);
	map_umeshunits.push_back(um);
}


void RMap::registerRobo(Robo* r) {
	MAP_ENT_ROBO* tr = new MAP_ENT_ROBO(r);
	MAP_STRUCT* ms = new MAP_STRUCT();
	ms->struct_type = MAP_ENTITY_TYPE::MAP_ENTITY_ROBO;
	ms->struct_id = max_struct_id;
	max_struct_id++;
	ms->ent = tr;
	map_structs.push_back(ms);
}

void RMap::registerObject() {
	MAP_ENT_OBJECT* ob = new MAP_ENT_OBJECT();
	MAP_STRUCT* ms = new MAP_STRUCT();
	ms->struct_id = max_struct_id;
	max_struct_id++;
	ms->struct_type = MAP_ENTITY_TYPE::MAP_ENTITY_OBJECT;
	ms->ent = ob;
	map_structs.push_back(ms);
}


void RMap::changePlayer(Robo* r) {
	// player_roboだけを置き換える
	if (player_robo) {
		InputMessageDispatcher::unregisterImpl(player_robo);
	}
	player_robo = r;
	InputMessageDispatcher::registerImpl(player_robo, NULL, NULL);

}


MAP_ENT_ROBO::~MAP_ENT_ROBO()
{
	if (rob) {
		rob->release();
		delete rob;
		rob = 0;
	}

};



RMapSetterExample::RMapSetterExample(Graphics* g, Game* game, AtariHantei* hantei,MeshInstanceds* mids, MyTextureLoader* loader, RMap* m) {
	Mesh* mesh;
	Mesh* mesh2;
	Mesh* mesh3[11];
	MeshInstanced* mesh_is[100];
	MeshInstanced* mesh_is2[30];

	mesh = new Mesh();
	mesh->readMesh(g, "resrc/model/tikei/tikei_douro.MESH", loader);
	mesh->readAnime("resrc/model/tikei/tikei_douro.ANIME");
	m->registerMesh("resrc/model/tikei/tikei_douro.MESH", mesh);

	mesh2 = new Mesh();
	mesh2->readMesh(g, "resrc/model/tikei/tikei_densin.MESH", loader);
	mesh2->readAnime("resrc/model/tikei/tikei_densin.ANIME");
	m->registerMesh("resrc/model/tikei/tikei_densin.MESH", mesh2);

	mesh3[0] = new Mesh();
	mesh3[0]->readMesh(g, "resrc/model/tikei/tikei_house.MESH", loader);
	mesh3[0]->readAnime("resrc/model/tikei/tikei_house.ANIME");
	m->registerMesh("resrc/model/tikei/tikei_house.MESH", mesh3[0]);

	mesh3[1] = new Mesh();
	//mesh3[1]->readMesh(g, "resrc/model/ponko2-3/pk2hidarime.MESH", demo->tex_loader);
	//mesh3[1]->readAnime("resrc/model/ponko2-3/pk2hidarime.ANIME");
	mesh3[1]->readMesh(g, "resrc/model/poriface/poriface.MESH", loader);
	mesh3[1]->readAnime("resrc/model/poriface/poriface.ANIME");
	m->registerMesh("resrc/model/poriface/poriface.MESH", mesh3[1]);



	mesh3[2] = new Mesh();
	mesh3[2]->readMesh(g, "resrc/model/ponko2-3/pk2hidarimedama.MESH", loader);
	mesh3[2]->readAnime("resrc/model/ponko2-3/pk2hidarimedama.ANIME");
	m->registerMesh("resrc/model/ponko2-3/pk2hidarimedama.MESH", mesh3[2]);


	mesh3[3] = new Mesh();
	mesh3[3]->readMesh(g, "resrc/model/ponko2-3/pk2kami.MESH",loader);
	mesh3[3]->readAnime("resrc/model/ponko2-3/pk2kami.ANIME");
	m->registerMesh("resrc/model/ponko2-3/pk2kami.MESH", mesh3[3]);

	mesh3[4] = new Mesh();
	mesh3[4]->readMesh(g, "resrc/model/ponko2-3/pk2kuti.MESH", loader);
	mesh3[4]->readAnime("resrc/model/ponko2-3/pk2kuti.ANIME");
	m->registerMesh("resrc/model/ponko2-3/pk2kuti.MESH", mesh3[4]);

	mesh3[5] = new Mesh();
	mesh3[5]->readMesh(g, "resrc/model/ponko2-3/pk2kubi.MESH", loader);
	mesh3[5]->readAnime("resrc/model/ponko2-3/pk2kubi.ANIME");
	m->registerMesh("resrc/model/ponko2-3/pk2kubi.MESH", mesh3[5]);

	mesh3[6] = new Mesh();
	mesh3[6]->readMesh(g, "resrc/model/ponko2-3/pk2mayuge.MESH", loader);
	mesh3[6]->readAnime("resrc/model/ponko2-3/pk2mayuge.ANIME");
	m->registerMesh("resrc/model/ponko2-3/pk2mayuge.MESH", mesh3[6]);

	/*
	mesh3[7] = new Mesh();
	mesh3[7]->readMesh(g, "resrc/model/ponko2-3/pk2midarime.MESH", demo->tex_loader);
	mesh3[7]->readAnime("resrc/model/ponko2-3/pk2midarime.ANIME");
	*/

	mesh3[8] = new Mesh();
	mesh3[8]->readMesh(g, "resrc/model/ponko2-3/pk2migimedama.MESH", loader);
	mesh3[8]->readAnime("resrc/model/ponko2-3/pk2migimedama.ANIME");
	m->registerMesh("resrc/model/ponko2-3/pk2migimedama.MESH", mesh3[8]);


	mesh3[7] = new Mesh();
	mesh3[7]->readMesh(g, "resrc/model/ponko2-4/pk2migihanddayo.MESH", loader);
	mesh3[7]->readAnime("resrc/model/ponko2-4/pk2migihanddayo.ANIME");
	mesh3[7]->RootBone->parent_bone = mesh2->Bones[mesh2->BoneIndexes["migiArmTekubiBone"]];
	mesh3[7]->RootBone->parent_bone_index = mesh2->BoneIndexes["migiArmTekubiBone"];
	mesh3[7]->RootBone_connect_without_material_local = true;
	m->registerMesh("resrc/model/ponko2-4/pk2migihanddayo.MESH", mesh3[7]);


	mesh3[10] = new Mesh();
	mesh3[10]->readMesh(g, "resrc/model/ponko2-4/pk2hidarihand.MESH", loader);
	mesh3[10]->readAnime("resrc/model/ponko2-4/pk2hidarihand.ANIME");
	mesh3[10]->RootBone->parent_bone = mesh2->Bones[mesh2->BoneIndexes["hidariArmTekubiBone"]];
	mesh3[10]->RootBone->parent_bone_index = mesh2->BoneIndexes["hidariArmTekubiBone"];// = mesh3[9]->RootBone->parent_bone->bone_index;
	mesh3[10]->RootBone_connect_without_material_local = true;
	m->registerMesh("resrc/model/ponko2-4/pk2hidarihand.MESH", mesh3[10]);

	mesh3[9] = new Mesh();
	mesh3[9]->readMesh(g, "resrc/model/bazooka/rweaponbazookaguna.MESH", loader);
	mesh3[9]->readAnime("resrc/model/bazooka/rweaponbazookaguna.ANIME");
	mesh3[9]->RootBone->parent_bone = mesh2->Bones[mesh2->BoneIndexes["migiArmTekubiBone"]];//mesh3[9]->Bones[mesh3[9]->BoneIndexes["MigiHandMotiBone"]];
	mesh3[9]->RootBone->parent_bone_index = mesh3[10]->RootBone->parent_bone->bone_index;
	mesh3[9]->RootBone_connect_without_material_local = false;
	m->registerMesh("resrc/model/bazooka/rweaponbazookaguna.MESH", mesh3[9]);

	/*
	mesh3[8] = new Mesh();
	mesh3[8]->readMesh(g, "resrc/model/bazooka/rweaponbazookaguna.MESH", demo->tex_loader);
	mesh3[8]->readAnime("resrc/model/bazooka/rweaponbazookaguna.ANIME");
	mesh3[8]->RootBone->parent_bone = mesh3[7]->Bones[mesh3[7]->BoneIndexes["MigiHandMotiBone"]];
	mesh3[8]->RootBone->parent_bone_index = mesh3[7]->BoneIndexes["MigiHandMotiBone"];
	mesh3[8]->RootBone_connect_without_material_local = false;
	*/

	//throw new GameError(FATAL_ERROR,"test");



	MYMATRIX mat;
	MyMatrixIdentity(mat);
	MYMATRIX worldforg;
	MyMatrixRotationZ(worldforg, 3.14 / 2);
	MyMatrixMultiply(mat, mat, worldforg);

	MyMatrixScaling(worldforg, 1 / 3.0, 1 / 3.0, 1 / 3.0);
	MyMatrixMultiply(mat, mat, worldforg);

	MyMatrixRotationY(worldforg, -3.14 / 2);
	MyMatrixMultiply(mat, mat, worldforg);


	//	MYMATRIX worldforg;
	MyMatrixRotationZ(worldforg, 3.14 / 2);
	MyMatrixMultiply(mesh3[9]->rootbone_matrix_local_kakeru, mesh3[9]->rootbone_matrix_local_kakeru, worldforg);

	MyMatrixScaling(worldforg, 1 / 3.0, 1 / 3.0, 1 / 3.0);
	MyMatrixMultiply(mesh3[9]->rootbone_matrix_local_kakeru, mesh3[9]->rootbone_matrix_local_kakeru, worldforg);

	MyMatrixRotationY(worldforg, -3.14 / 2);
	MyMatrixMultiply(mesh3[9]->rootbone_matrix_local_kakeru, mesh3[9]->rootbone_matrix_local_kakeru, worldforg);

	Robo* robodayo;
	Robo* roboaitedayo;
	robodayo = new Robo();
	robodayo->init(g, loader, hantei);
	InputMessageDispatcher::registerImpl(robodayo, NULL, NULL);
	robodayo->atarihan->setXYZ(0, 3, 5);
	roboaitedayo = new Robo();
	roboaitedayo->init(g, loader, hantei);

	game->robodayo = robodayo;
	game->roboaitedayo = roboaitedayo;


	/*
	for (int i = 0; i<1; i++) {
		{
			MYMATRIX idenmat;
			MyMatrixIdentity(idenmat);
			UMeshUnit* umesh_unit = new UMeshUnit();
			UMesh* um = new UMesh(g, "resrc/model/ponko2-4/pk2sailor.MESH", loader, mesh2, false, &idenmat,
				0, KTROBO_MESH_BONE_NULL, false);
			umesh_unit->setUMesh(um);
			m->registerChara(umesh_unit);
			//umesh_units.push_back(umesh_unit);
			bool tyo_unko = true;
			float frame_anime = 0;

			umesh_unit->setSCALEXYZ(1, 1, 1);
			umesh_unit->setXYZ(0, 0, 10);
			umesh_unit->calcAnimeFrame(1, &frame_anime, &tyo_unko);

			umesh_unit->calcJyusinAndR();
			hantei->setUMeshUnit(umesh_unit, AtariUnit::AtariType::ATARI_CHARA);
		}

	}
	UMeshUnit* uum;
	{
		MYMATRIX idenmat;
		MyMatrixIdentity(idenmat);
		UMeshUnit* umesh_unit = new UMeshUnit();
		uum = umesh_unit;

		UMesh* um = new UMesh(g, "resrc/model/ponko2-4/pk2sailordayo.MESH"/*"resrc/model/cube/pkcube.MESH"*/
	/*
			, loader, mesh2, false, &idenmat,
			0, KTROBO_MESH_BONE_NULL, false);
		umesh_unit->setUMesh(um);
		umesh_units.push_back(umesh_unit);
		bool tyo_unko = true;
		float frame_anime = 0;

		umesh_unit->setSCALEXYZ(1, 1, 1);
		umesh_unit->setXYZ(0, 3, -5);
		umesh_unit->calcAnimeFrame(1, &frame_anime, &tyo_unko);

		umesh_unit->calcJyusinAndR();
		hantei->setUMeshUnit(umesh_unit, AtariUnit::AtariType::ATARI_CHARA);
*/
		/*sinai = new Sinai();
		sinai->init(g, demo->tex_loader,hantei);
		ss = new SinaiNigiru(sinai,umesh_unit);

		sfuru = new SinaiFuru(sinai,umesh_unit);
		*/
		/*
		SinaiFuruPart p;
		p.dt =0.1;
		p.rotx = 0;
		p.rotz = 0;
		p.tuka_dpos = MYVECTOR3(0,0,0);
		sfuru->setMenParts(&p);
		*/
		/*
		p.dt = 10;
		p.rotx = -1.57;
		p.rotz = 0;
		p.tuka_dpos = MYVECTOR3(0,0.7f,1.1f);
		sfuru->setMenParts(&p);

		p.dt = 20;
		p.rotx = 0.05f;
		p.rotz = 0;
		p.tuka_dpos = MYVECTOR3(0,0,1);
		sfuru->setMenParts(&p);
		*/
		//	sfuru->setKAMAE(-0.12,0,&MYVECTOR3(0,1.9,-0.20));
	/*
	}
	*/

	/*
	{
	MYMATRIX idenmat;
	MyMatrixIdentity(idenmat);
	UMeshUnit* umesh_unit = new UMeshUnit();
	UMesh* um = new UMesh(g,"resrc/model/ponko2-4/pk2sailordayo.MESH", demo->tex_loader,mesh2,false,&idenmat,
	0,KTROBO_MESH_BONE_NULL,false);
	umesh_unit->setUMesh(um);
	umesh_units.push_back(umesh_unit);
	umesh_unit->calcJyusinAndR();
	hantei->setUMeshUnit(umesh_unit, AtariUnit::AtariType::ATARI_CHARA);
	}*/


	unsigned short animf[KTROBO_MESH_INSTANCED_BONE_MAX];
	unsigned short animl[KTROBO_MESH_INSTANCED_BONE_MAX];
	float animw[KTROBO_MESH_INSTANCED_BONE_MAX];

	memset(animf, 0, sizeof(animf));
	memset(animl, 0, sizeof(animl));
	memset(animw, 0, sizeof(animw));


	for (int i = 0; i<5; i++) {
		for (int k = 0; k<5; k++) {
			{
				MYMATRIX idenmat;
				MyMatrixIdentity(idenmat);
				UMeshUnit* umesh_unit = new UMeshUnit();
				UMesh* um = new UMesh(g, "resrc/model/tikei/tikei_douro.MESH", loader, mesh, false, &idenmat,
					0, KTROBO_MESH_BONE_NULL, true); // doBoneのobbがtrueになるので大丈夫

				mesh_is[i + 5 * k] = mids->makeInstanced(mesh, mesh, NULL, NULL, false, &idenmat);
				umesh_unit->setUMesh(um);
				m->registerTikei(umesh_unit, mesh_is[i + 5 * k]);

				//umesh_units.push_back(umesh_unit);
				umesh_unit->setSCALEXYZ(100, 100, 130);
				umesh_unit->setXYZ((i - 2) * 200, (k - 2) * 200, -4);
				umesh_unit->calcJyusinAndR();
				mesh_is[i + 5 * k]->setWorld(&umesh_unit->world);
				mesh_is[i + 5 * k]->setIsRender(true);
				mesh_is[i + 5 * k]->setBoneIndexInfo(animf, animl, animw);
				bool tyo_unko = true;
				float frame_anime = 0;

				//umesh_unit->calcAnimeFrame(1,&frame_anime,&tyo_unko);
				//umesh_unit->calcJyusinAndR();
				hantei->setUMeshUnit(umesh_unit, AtariUnit::AtariType::ATARI_TIKEI);
			}
		}
	}

	for (int i = 0; i<5; i++) {
		for (int k = 0; k<5; k++) {
			{
				MYMATRIX idenmat;
				MyMatrixIdentity(idenmat);
				UMeshUnit* umesh_unit = new UMeshUnit();
				UMesh* um = new UMesh(g, "resrc/model/tikei/tikei_densin.MESH", loader, mesh2, false, &idenmat,
					0, KTROBO_MESH_BONE_NULL, true); // doBoneのobbがtrueになるので大丈夫

				mesh_is[i + 5 * k + 25] = mids->makeInstanced(mesh2, mesh2, NULL, NULL, false, &idenmat);
				umesh_unit->setUMesh(um);
				m->registerTikei(umesh_unit, mesh_is[i + 5 * k+25]);

//				umesh_units.push_back(umesh_unit);
				umesh_unit->setSCALEXYZ(100, 100, 130);
				umesh_unit->setXYZ((i - 2) * 200, (k - 2) * 200, 0);
				umesh_unit->calcJyusinAndR();
				mesh_is[i + 5 * k + 25]->setWorld(&umesh_unit->world);
				mesh_is[i + 5 * k + 25]->setIsRender(true);
				mesh_is[i + 5 * k + 25]->setBoneIndexInfo(animf, animl, animw);
				bool tyo_unko = true;
				float frame_anime = 0;

				//umesh_unit->calcAnimeFrame(1,&frame_anime,&tyo_unko);
				//umesh_unit->calcJyusinAndR();
				hantei->setUMeshUnit(umesh_unit, AtariUnit::AtariType::ATARI_TIKEI);
			}
		}
	}

	for (int i = 0; i<5; i++) {
		for (int k = 0; k<5; k++) {
			{
				MYMATRIX idenmat;
				MyMatrixIdentity(idenmat);
				UMeshUnit* umesh_unit = new UMeshUnit();
				UMesh* um = new UMesh(g, "resrc/model/tikei/tikei_house.MESH", loader, mesh3[0], false, &idenmat,
					0, KTROBO_MESH_BONE_NULL, true); // doBoneのobbがtrueになるので大丈夫

				mesh_is[i + 5 * k + 50] = mids->makeInstanced(mesh3[0], mesh3[0], NULL, NULL, false, &idenmat);
				umesh_unit->setUMesh(um);
				m->registerTikei(umesh_unit, mesh_is[i+5*k+50]);

//				umesh_units.push_back(umesh_unit);
				umesh_unit->setSCALEXYZ(50, 50, 50);
				umesh_unit->setXYZ((i - 2) * 200 - 100, (k - 2) * 200 - 100, 0);
				umesh_unit->calcJyusinAndR();
				mesh_is[i + 5 * k + 50]->setWorld(&umesh_unit->world);
				mesh_is[i + 5 * k + 50]->setIsRender(true);
				mesh_is[i + 5 * k + 50]->setBoneIndexInfo(animf, animl, animw);
				bool tyo_unko = true;
				float frame_anime = 0;

				//umesh_unit->calcAnimeFrame(1,&frame_anime,&tyo_unko);
				//umesh_unit->calcJyusinAndR();
				hantei->setUMeshUnit(umesh_unit, AtariUnit::AtariType::ATARI_TIKEI);
			}
		}
	}



	//*//

	/*
	MyMatrixRotationX(worldforg, -3.14/4*2);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);
	MyMatrixRotationZ(worldforg, 3.14/2);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);

	MyMatrixRotationY(worldforg, 3.14/2);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);
	MyMatrixRotationZ(worldforg, 3.14);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);
	MyMatrixRotationY(worldforg, 3.14/2);
	MyMatrixMultiply(mesh3[10]->RootBone->matrix_local, mesh3[10]->RootBone->matrix_local, worldforg);

	*/

	float frame = 0;

	mesh->animate(frame, true);
	mesh2->animate(frame, true);
	for (int i = 0; i <= 10; i++) {
		mesh3[i]->animate(frame, true);
		if (i == 9) {
			mesh3[i]->animate(0, true);
		}
	}
	mesh3[7]->animate(frame, true);
	mesh3[8]->animate(frame, true);

}