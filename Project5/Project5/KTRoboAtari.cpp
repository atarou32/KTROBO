#include "KTRoboAtari.h"
#include "KTRoboGameError.h"
#include <set>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include "KTRoboLog.h"
#include "KTRoboClock.h"
#include "KTRoboGamen_GARAGE.h"

using namespace std;

void UMeshUnit::setXYZ(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	is_updated = true;
}

void UMeshUnit::setWorld(MYMATRIX* nworld) {
	if (!nworld) return;
	
	/*
	MYMATRIX temp;
	MYMATRIX temp2;
	MYMATRIX temp3;
	MYMATRIX temp4;
	MYMATRIX temp5;
	MyMatrixRotationX(temp,rotx);
	MyMatrixRotationY(temp2,roty);
	MyMatrixRotationZ(temp3,rotz);
	MyMatrixTranslation(temp4,x,y,z);
	MyMatrixScaling(temp5,scalex,scaley,scalez);
	MyMatrixMultiply(world,temp5,temp);
	MyMatrixMultiply(world,world,temp3);
	MyMatrixMultiply(world,world,temp2);
	MyMatrixMultiply(world,world,temp4);	
	*/





	world = *nworld;

	is_updated = true;
}
void UMeshUnit::setXYZD(float x, float y, float z, float ddmax) {
	
	MYVECTOR3 tt = MYVECTOR3(this->x-x,this->y-y, this->z-z);
	float td = MyVec3Dot(tt,tt);
	if (td < ddmax) {
		this->x = x;
		this->y = y;
		this->z = z;
		is_updated = true;
	} else {
		// ddmax にして移動させる
		tt = -tt;
		MyVec3Normalize(tt,tt);
		this->x = this->x + tt.float3.x * ddmax;
		this->y = this->y + tt.float3.y * ddmax;
		this->z = this->z + tt.float3.z * ddmax;
		is_updated = true;
	}

}

void UMeshUnit::setROTXYZ(float rotx, float roty, float rotz) {
	this->rotx = rotx;
	this->roty = roty;
	this->rotz = rotz;
	is_updated = true;
}
void UMeshUnit::setV(MYVECTOR3* v) {
	this->v = *v;
	is_updated = true;

}

void UMesh::setInfo() {
	this->mesh->rootbone_matrix_local_kakeru = RootBone_matrix_local_kakeru;
	this->mesh->RootBone_connect_without_material_local = this->RootBone_connect_without_material_local;
	this->mesh->RootBone->parent_bone = this->rootbone_parent;
	this->mesh->RootBone->parent_bone_index = this->rootbone_parent_index;
	
}

void UMeshUnit::calcAnimeFrame(int meshnum, float* frames, bool* calculateoffsetmatrixs) {
	if (meshnum != meshs.size()) return;

	for (int i=0;i<meshnum;i++) {
		meshs[i]->setAnimeFrame(frames[i]);
	}
	for (int i=0;i<meshnum;i++) {
		meshs[i]->setInfo();
		meshs[i]->Animate(calculateoffsetmatrixs[i]);
	}
	is_updated = true;
}


void UMeshUnit::calcJyusinAndR(bool calcWorld) {
	// UMeshUnitを包括する重心座標とRを求める
	/*if (is_updated == false) {
		return;
	}*/


	is_updated = false;
	vector<UMesh*>::iterator it = meshs.begin();
	int size = meshs.size();
	MYVECTOR3 ans_jyusin(0,0,0);
	float ans_r = 0;
	MYMATRIX temp;
	MYMATRIX temp2;
	MYMATRIX temp3;
	MYMATRIX temp4;
	MYMATRIX temp5;

	if (calcWorld) {
	MyMatrixRotationX(temp,rotx);
	MyMatrixRotationY(temp2,roty);
	MyMatrixRotationZ(temp3,rotz);
	MyMatrixTranslation(temp4,x,y,z);
	MyMatrixScaling(temp5,scalex,scaley,scalez);
	MyMatrixMultiply(world,temp5,temp);
	MyMatrixMultiply(world,world,temp3);
	MyMatrixMultiply(world,world,temp2);
	MyMatrixMultiply(world,world,temp4);	
	}
	// 現在のアニメフレームと位置と姿勢のときの各umesh のbone_obbs の計算を行う
	// combined_matrix はすでにcalcanimeframe で計算されている
	it = meshs.begin();
	while(it != meshs.end()) {
		UMesh* em = *it;

		if (!em->mesh) {
			// 専用のcalcJyusinRを呼ぶ
			this->calcJyusinAndRForEmptyMesh();
			it++;
			continue;
		}


		if (em->mesh->Bones.size() >= KTROBO_MESH_BONE_MAX) {
			it++;
			continue;
		}

		int size = em->mesh->Bones.size();
		for (int i=0;i<size;i++) {

			if (em->mesh->Bones[i]->houkatuobbuse) {

				MYMATRIX cm = em->mesh->Bones[i]->combined_matrix;
				MYMATRIX henkanmat;
				OBB henkanobb = em->mesh->Bones[i]->houkatuobb;
				MyMatrixMultiply(henkanmat,cm,world);
				//MyMatrixMultiply(henkanmat,henkanmat
				MYVECTOR3 ux= henkanobb.u[0] + henkanobb.c;
				MYVECTOR3 uy = henkanobb.u[1] + henkanobb.c;
				MYVECTOR3 uz = henkanobb.u[2] + henkanobb.c;

				MyVec3TransformCoord(ux, ux, henkanmat);
				MyVec3TransformCoord(uy, uy,henkanmat);
				MyVec3TransformCoord(uz, uz, henkanmat);



				MyVec3TransformCoord(henkanobb.c,henkanobb.c,henkanmat);
				MyVec3TransformNormal(henkanobb.u[0],henkanobb.u[0],henkanmat);
			//	henkanobb.e.float3.x = henkanobb.e.float3.x * MyVec3Length(henkanobb.u[0]);
				MyVec3Normalize(henkanobb.u[0],henkanobb.u[0]);
				MyVec3TransformNormal(henkanobb.u[1],henkanobb.u[1],henkanmat);
			//	henkanobb.e.float3.y = henkanobb.e.float3.y * MyVec3Length(henkanobb.u[1]);
				MyVec3Normalize(henkanobb.u[1],henkanobb.u[1]);
				MyVec3TransformNormal(henkanobb.u[2],henkanobb.u[2],henkanmat);
			//	henkanobb.e.float3.z = henkanobb.e.float3.z * MyVec3Length(henkanobb.u[2]);
				MyVec3Normalize(henkanobb.u[2],henkanobb.u[2]);
				

			
				MyVec3Subtract(ux, ux, henkanobb.c);
				MyVec3Subtract(uy, uy, henkanobb.c);
				MyVec3Subtract(uz, uz, henkanobb.c);

				henkanobb.e.float3.x = 2*MyVec3Length(ux)*henkanobb.e.float3.x;
				henkanobb.e.float3.y = 2*MyVec3Length(uy)*henkanobb.e.float3.y;
				henkanobb.e.float3.z = 2*MyVec3Length(uz)*henkanobb.e.float3.z;



				// henkanobb の大きさをｖ＊ｄｔに応じて大きくする
			/*	henkanobb.e.float3.x += abs(v.float3.x) * dt;
				henkanobb.e.float3.y += abs(v.float3.y) * dt;
				henkanobb.e.float3.z += abs(v.float3.z) * dt;
				pp.c -= v * dt/2;
				pp.e[0] += abs(MyVec3Dot(&pp.u[0], &v)) * dt;
				pp.e[1] += abs(MyVec3Dot(&pp.u[1], &v)) * dt;
				pp.e[2] += abs(MyVec3Dot(&pp.u[2], &v)) * dt;
				*/
//				henkanobb.c = henkanobb.c - v * dt/2;
//				henkanobb.e.float3.x += henkanobb.e[0] * abs(MyVec3Dot(henkanobb.u[0], v)) * dt;
//				henkanobb.e.float3.y += henkanobb.e[1] * abs(MyVec3Dot(henkanobb.u[1], v)) * dt;
//				henkanobb.e.float3.z += henkanobb.e[2] * abs(MyVec3Dot(henkanobb.u[2], v)) * dt;


				if ((MyVec3Length(henkanobb.e) < 0.00001f) || (MyVec3Length(henkanobb.u[0]) < 0.000001) ||
					(MyVec3Length(henkanobb.u[1]) < 0.000001) ||(MyVec3Length(henkanobb.u[2]) < 0.000001)) {
						em->is_bone_obbs_use[i] = false;
				} else {

					em->is_bone_obbs_use[i] = true;
				}
				em->bone_obbs[i] = henkanobb;
		
			}
		}

		it++;
	}
	


	// bone_obbs を使用して包括する重心とRを求める

	int cc=0;
	it = meshs.begin();
	while(it != meshs.end()) {
		UMesh* em = *it;

		for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
			if (em->is_bone_obbs_use[i]) {
				ans_jyusin = ans_jyusin + em->bone_obbs[i].c;
				cc++;
			}
		}

		it++;
	}
	if (cc) {
		ans_jyusin = ans_jyusin / cc;
	}
	// 重心が求まったので次はｒ
	it = meshs.begin();
	while(it != meshs.end()) {
		UMesh* em = *it;

		for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {

			if (em->is_bone_obbs_use[i]) {
				MYVECTOR3 tmp;
				float tempp=0;
				tmp = em->bone_obbs[i].c - ans_jyusin;
				MYVECTOR3 tt = em->bone_obbs[i].u[0] * em->bone_obbs[i].e[0];
				MYVECTOR3 tt2 = em->bone_obbs[i].u[1]*em->bone_obbs[i].e[1] ;
				float ppp = abs(MyVec3Length(tt ));
				float ooo =	abs(MyVec3Length(tt2));
				float ttt = max(ppp, ooo);
				MYVECTOR3 tt3 = em->bone_obbs[i].u[2] * em->bone_obbs[i].e[2];
				float ppp2 = MyVec3Length(tt3);				
				ttt = max(ttt, ppp2);

				tempp = MyVec3Length(tmp) + 1*MyVec3Dot(em->bone_obbs[i].e, em->bone_obbs[i].e);
				if (tempp > ans_r) {
					ans_r = tempp;
				}

			}
		}

		it++;
	}



	r = ans_r;// +MyVec3Length(v) * dt; // ｖの長さの分だけ半径を大きくする
	jyusin = ans_jyusin;
}


void UMeshUnit::setIsEnabled(AtariHantei* hantei, bool t){ 
		
		hantei->resetIsUnitUpdated();

		//hantei->setUnitStateChanged(this);
		is_enabled = t;
}
void UMeshUnit::draw(Graphics* g, MYMATRIX* view, MYMATRIX* proj, int meshnum, float* frames, bool* calculateoffsetmatrixs,
					 bool is_draw_mesh, bool is_calc_animeframe, bool is_draw_jyusinR, bool is_draw_boneobb) {
	if (is_calc_animeframe) {
	calcAnimeFrame(meshnum,frames,calculateoffsetmatrixs);
	}
	calcJyusinAndR();
	MYMATRIX idenmat;
	MyMatrixIdentity(idenmat);
	int size = meshs.size();
	for (int i=0;i<size;i++) {
	// mesh の描画
		if (is_draw_mesh && meshs[i]->mesh) {
			meshs[i]->mesh->draw(g,&world,view,proj);
		}
		// mesh_obb の描画
		for (int j=0;j<KTROBO_MESH_BONE_MAX;j++) {
			if (meshs[i]->is_bone_obbs_use[j] && is_draw_boneobb) {
				g->drawOBB(g,0xFFFFFFFF,&idenmat,view,proj,&meshs[i]->bone_obbs[j]);
			}
		}
	}

	// 包括球（描画ではOBBの描画)
	if (is_draw_jyusinR) {
	OBB houkatukyuuobb;
	houkatukyuuobb.c = jyusin;
	houkatukyuuobb.e = MYVECTOR3(r,r,r);
	g->drawOBB(g,0xFFFFFFFF,&idenmat,view,proj,&houkatukyuuobb);
	}
}

int UMesh::id_max = 0;

const int AtariHantei::kakuho_counts[] = {
		128,1024,8192,32768,65535,131070,393210};

int AtariHantei::getKakuhoCountsFromCount(int count) {
	for (int i=0;i<=kakuho_max;i++) {
		if (kakuho_counts[i] > count) {
			return kakuho_counts[i];
		}
	}

	throw new GameError(KTROBO::FATAL_ERROR,"no more countdatyo");
}

void AtariHantei::maecalcdayo(Graphics* g) {
// maecalcdayoでは登録されたUMeshUnitからatariunitの計算を行う
	//if (!atari_start) return;
	if (!is_unit_updated) return;

	if (!dummy_umeshunit) {
		dummy_umeshunit = new UMeshUnit();
		dummy_umesh = new UMesh();
		dummy_umesh->is_bone_obbs_use[0] = true;
		dummy_umesh->bone_obbs_idx[0] = KTROBO_ATARI_CALC_OBBS_IDX_DUMMY;
		dummy_umesh->bone_obbs[0].c = MYVECTOR3(0,0,0);
		dummy_umesh->bone_obbs[0].e = MYVECTOR3(1,1,1);
		dummy_umesh->bone_obbs[0].u[0] = MYVECTOR3(1,0,0);
		dummy_umesh->bone_obbs[0].u[1] = MYVECTOR3(0,1,0);
		dummy_umesh->bone_obbs[0].u[2] = MYVECTOR3(0,0,1);
		dummy_umeshunit->setUMesh(dummy_umesh);
	}



	int atari_unit_count = 0;
	int size = umesh_units.size();
	for (int i=0;i<size;i++) {
		UMeshUnit* um = umesh_units[i];
		AtariUnit::AtariType type = umesh_unit_types[i];
		int umsize = um->meshs.size();
		if (umsize && um->getIsEnabled()) {
		for (int k=0;k<umsize;k++) {
			UMesh* umm = um->meshs[k];
			if (atari_unit_count >= KTROBO_MAX_ATARI_HANTEI_UNIT_NUM) {
				throw new GameError(KTROBO::FATAL_ERROR, "no more atari");
			}
			units[atari_unit_count].atariidx = atari_unit_count; // このインデックス　とあたりいｄｘの関係を変えないこと
			units[atari_unit_count].type = type;
			units[atari_unit_count].umesh = umm;
			units[atari_unit_count].umesh_unit = um;
			atari_unit_count++;
		}
		}
	}
	au_count = atari_unit_count;
	// atariunit の簡易計算が終わったので
	// 以下の変数のサイズは足りているか確認をする

	/*
	MESH_VERTEX* max_tikei_vertexs;
	UINT* max_tikei_indexs;
	AtariUnitInfo* au_info;
	AtariUnitKumi* kumi;
	AtariUnitKumi* ans;
	AtariUnitOBB* obbs;
	AtariUnitTikeiIgaiDousi* autid;
	AtariUnitTikeiToSoreigai* autts;
	*/


	// max_tikei_vertexs の値を調べる
	int temp_vertexs_count = 0;

	for (int i=0;i<atari_unit_count;i++) {
		if (units[i].type == AtariUnit::AtariType::ATARI_TIKEI
			&& units[i].umesh && units[i].umesh->mesh) {
				if (units[i].umesh->vertexs) {
					temp_vertexs_count += units[i].umesh->mesh->VertexCount;
				}
		}
	}
	int temp_c = this->getKakuhoCountsFromCount(temp_vertexs_count);
	if (temp_c > max_count.vertexs_count) {
		max_count.vertexs_count = temp_c;
		temp_count.vertexs_count = temp_vertexs_count;
		delete[] max_tikei_vertexs;
		max_tikei_vertexs = new AtariUnitVertexs[temp_c];
		for (int i=0;i<temp_c;i++) {
			max_tikei_vertexs[i].normal = MYVECTOR4(0,0,1,0);
			max_tikei_vertexs[i].pos = MYVECTOR4(0,0,0,1);
		}

		//memset(max_tikei_vertexs,0,sizeof(AtariUnitVertexs)*temp_c);
		if (buffer_vertexs) {
			buffer_vertexs->Release();
			buffer_vertexs  =0;
		}
		if (buffer_vertexs_view) {
			buffer_vertexs_view->Release();
			buffer_vertexs_view = 0;
		}
		HRESULT hr;
		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitVertexs),temp_c,max_tikei_vertexs,&buffer_vertexs);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createShaderResourceView(g,buffer_vertexs,&buffer_vertexs_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}
	}

	int temp_indexs_count = 0;
	for (int i=0;i<atari_unit_count;i++) {
		if (units[i].type == AtariUnit::AtariType::ATARI_TIKEI
			&& units[i].umesh && units[i].umesh->mesh) {
				if (units[i].umesh->indexs) {
					temp_indexs_count += units[i].umesh->mesh->FaceCount * 3;
				}
		}
	}
	int temp_c2 = this->getKakuhoCountsFromCount(temp_indexs_count);
	if (temp_c2 > max_count.indexs_count) {
		max_count.indexs_count = temp_c2;
		temp_count.indexs_count = temp_indexs_count;
		delete[] max_tikei_indexs;
		max_tikei_indexs = new AtariUnitIndexs[temp_c2];
		//memset(max_tikei_indexs,0,sizeof(AtariUnitIndexs)*temp_c2);
		for (int i=0;i<temp_c2;i++) {
			max_tikei_indexs[i].index = 0;
		}
		if (buffer_indexs) {
			buffer_indexs->Release();
			buffer_indexs  =0;
		}
		if (buffer_indexs_view) {
			buffer_indexs_view->Release();
			buffer_indexs_view = 0;
		}
		HRESULT hr;
		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitIndexs),temp_c2,max_tikei_indexs,&buffer_indexs);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createShaderResourceView(g,buffer_indexs,&buffer_indexs_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}
	}

	// tugiha auinfo
	int temp_c3 = this->getKakuhoCountsFromCount(atari_unit_count);
	if (temp_c3 > max_count.auinfo_count) {
		max_count.auinfo_count = temp_c3;
		temp_count.auinfo_count = atari_unit_count;
		delete[] au_info;
		au_info = new AtariUnitInfo[temp_c3];
		//memset(au_info,0,sizeof(AtariUnitInfo)*temp_c3);
		
		MYMATRIX idenmat;
		MyMatrixIdentity(idenmat);
		for (int i=0;i<temp_c3;i++) {
			au_info[i].atari_idx = 0;
			au_info[i].dt = 0;
			au_info[i].indexs_place = 0;
			au_info[i].index_count = 0;
			au_info[i].jyusin = MYVECTOR3(0,0,0);
			au_info[i].r = 1;
			au_info[i].v = MYVECTOR3(0,0,0);
			au_info[i].vertexs_place = 0;
			au_info[i].vertex_count = 0;
			au_info[i].world = idenmat;
		}
		if (buffer_au_info) {
			buffer_au_info->Release();
			buffer_au_info  =0;
		}
		if (buffer_au_info_view) {
			buffer_au_info_view->Release();
			buffer_au_info_view = 0;
		}
		HRESULT hr;
		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitInfo),temp_c3,au_info,&buffer_au_info);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createShaderResourceView(g,buffer_au_info,&buffer_au_info_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}
	}

	

	// obbs のカウントを出す
	// obbsをカウントするのは地形以外
	int temp_obbs_count = KTROBO_ATARI_CALC_OBBS_IDX_DUMMY+1; //これから始める
	for (int i=0;i<atari_unit_count;i++) {
		if ((units[i].type != AtariUnit::AtariType::ATARI_TIKEI) // 地形はカウントされないのだがatariunitの計上にはobbs_useがtrueとなったumesh が地形には必要
			&& units[i].umesh /*&& units[i].umesh->mesh*/) {
				UMesh* um = units[i].umesh;
				for (int k=0;k<KTROBO_MESH_BONE_MAX;k++) {
					if (um->is_bone_obbs_use[k]) {
						um->bone_obbs_idx[k] = temp_obbs_count;
						temp_obbs_count++;
					} else {
						um->bone_obbs_idx[k] = 0; // 0でクリアする
					}
				}
		}
	}


//	au_obbs_count = temp_obbs_count;
	int temp_c5 = this->getKakuhoCountsFromCount(temp_obbs_count);
	if (temp_c5 > max_count.obbs_count) {
		max_count.obbs_count = temp_c5;
		temp_count.obbs_count = temp_obbs_count;
		delete[] obbs;
		obbs = new AtariUnitOBB[temp_c5];
		//memset(obbs, 0, sizeof(AtariUnitOBB)*temp_c5);
		for (int i=0;i<temp_c5;i++) {
			obbs[i].c = MYVECTOR3(0,0,0);
			obbs[i].e = MYVECTOR3(1,1,1);
			obbs[i].u[0] = MYVECTOR3(1,0,0);
			obbs[i].u[1] = MYVECTOR3(0,1,0);
			obbs[i].u[2] = MYVECTOR3(0,0,1);
		}
		if (buffer_obbs) {
			buffer_obbs->Release();
			buffer_obbs  =0;
		}
		if (buffer_obbs_view) {
			buffer_obbs_view->Release();
			buffer_obbs_view = 0;
		}
		HRESULT hr;
		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitOBB),temp_c5,obbs,&buffer_obbs);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createShaderResourceView(g,buffer_obbs,&buffer_obbs_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}




	}

	// autid と　autts のカウントを出す
	// まず各typeのカウントを出す
	this->au_waza_count = 0;
	this->au_chara_count = 0;
	this->au_object_count = 0;
	this->au_tikei_count = 0;

	for (int i=0;i<atari_unit_count;i++) {
		if (units[i].type == AtariUnit::AtariType::ATARI_TIKEI) {
			au_tikei_count++;
		}
		if (units[i].type == AtariUnit::AtariType::ATARI_CHARA) {
			au_chara_count++;
		}
		if (units[i].type == AtariUnit::AtariType::ATARI_OBJECT) {
			au_object_count++;
		}
		if (units[i].type == AtariUnit::AtariType::ATARI_WAZA) {
			au_waza_count++;
		}
	}

	int autid_count = temp_obbs_count*(temp_obbs_count-1)/2;
	int temp_c6 = this->getKakuhoCountsFromCount(autid_count);
	if (temp_c6 > max_count.igaidousi_count) {
		max_count.igaidousi_count = temp_c6;
		temp_count.igaidousi_count = autid_count;
		delete[] autid;
		autid = new AtariUnitTikeiIgaiDousi[temp_c6];
		//memset(autid,0,sizeof(AtariUnitTikeiIgaiDousi)*temp_c6);
		for (int i=0;i<temp_c6;i++) {
			autid[i].atariidx = 0;
			autid[i].atariidx2 = 0;
			autid[i].obbidx = 0;
			autid[i].obbidx2 = 0;
		}
		if (buffer_autid) {
			buffer_autid->Release();
			buffer_autid  =0;
		}
		if (buffer_autid_view) {
			buffer_autid_view->Release();
			buffer_autid_view = 0;
		}
		HRESULT hr;
		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitTikeiIgaiDousi),temp_c6,autid,&buffer_autid);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createShaderResourceView(g,buffer_autid,&buffer_autid_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}


	}

	int autts_count = temp_obbs_count * au_tikei_count;//(au_waza_count + au_chara_count+au_object_count) * KTROBO_MESH_BONE_MAX * (au_tikei_count);
	int temp_c7 = this->getKakuhoCountsFromCount(autts_count);
	if (temp_c7 > max_count.soreigai_count) {
		max_count.soreigai_count = temp_c7;
		temp_count.soreigai_count = autts_count;
		delete[] autts;
		autts = new AtariUnitTikeiToSoreigai[temp_c7];
		//memset(autts,0,sizeof(AtariUnitTikeiToSoreigai)*temp_c7);
		for (int i=0;i<temp_c7;i++) {
			autts[i].atariidx = 0;
			autts[i].atariidx2 = 0;
			autts[i].obbidx = 0;
			autts[i].offset = 0;
		}

		if (buffer_autts) {
			buffer_autts->Release();
			buffer_autts  =0;
		}
		if (buffer_autts_view) {
			buffer_autts_view->Release();
			buffer_autts_view = 0;
		}
		HRESULT hr;
		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitTikeiToSoreigai),temp_c7,autts,&buffer_autts);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createShaderResourceView(g,buffer_autts,&buffer_autts_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}


	}

	// kumi ans
	int temp_c4 = this->getKakuhoCountsFromCount(atari_unit_count*(atari_unit_count-1)/2);
	if (temp_c4 > max_count.kumi_count) {
	


		max_count.kumi_count = temp_c4;
		temp_count.kumi_count = atari_unit_count*(atari_unit_count-1)/2;
		delete[] kumi;
		kumi = new AtariUnitKumi[temp_c4];
		//memset(kumi,0,sizeof(AtariUnitKumi)*temp_c4);
		for (int i=0;i<temp_c4;i++) {
			kumi[i].atari_idx = 0;
			kumi[i].atari_idx2 = 0;
		}

		if (buffer_kumi) {
			buffer_kumi->Release();
			buffer_kumi  =0;
		}
		if (buffer_kumi_view) {
			buffer_kumi_view->Release();
			buffer_kumi_view = 0;
		}
		HRESULT hr;
		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitKumi),temp_c4,kumi,&buffer_kumi);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createShaderResourceView(g,buffer_kumi,&buffer_kumi_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}

	}


	int a_c  = autid_count + autts_count;
	int temp_c8 = this->getKakuhoCountsFromCount(a_c);

	if (temp_c8 > max_count.ans_count) {

		max_count.ans_count = temp_c8;
		temp_count.ans_count = a_c;
		delete[] ans;
		ans = new AtariUnitAns[temp_c8];
	//	memset(ans,0,sizeof(AtariUnitAns)*temp_c8);
		for (int i=0;i<temp_c8;i++) {
			ans[i].atari_idx = 0;
			ans[i].atari_idx2 = 0;
			ans[i].is_use = 0;
			ans[i].kouten_housen = MYVECTOR3(0,0,1);
			ans[i].kouten_jyusin = MYVECTOR3(0,0,0);
			ans[i].obbidx = 0;
			ans[i].obbidx2 = 0;
		}

		if (buffer_ans) {
			buffer_ans->Release();
			buffer_ans  =0;
		}
		if (buffer_ans_view) {
			buffer_ans_view->Release();
			buffer_ans_view = 0;
		}
		HRESULT hr;
		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitAns),temp_c8,ans,&buffer_ans);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createBufferUnorderedAccessView(g,buffer_ans,&buffer_ans_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}
		if (buffer_ans_copy) {
			buffer_ans_copy->Release();
			buffer_ans_copy = 0;
		}

		hr = AtariHantei::createBufferForCopy(g,buffer_ans,&buffer_ans_copy);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer copy");
		}

		if (buffer_ans2) {
			buffer_ans2->Release();
			buffer_ans2  =0;
		}
		if (buffer_ans2_view) {
			buffer_ans2_view->Release();
			buffer_ans2_view = 0;
		}
		hr;
		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitAns),temp_c8,ans,&buffer_ans2);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createBufferUnorderedAccessView(g,buffer_ans2,&buffer_ans2_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}

		if (buffer_ans2_aida) {
			buffer_ans2_aida->Release();
			buffer_ans2_aida = 0;
		}
		if (buffer_ans2_aida_view) {
			buffer_ans2_aida_view->Release();
			buffer_ans2_aida_view = 0;
		}

		hr = AtariHantei::createStructuredBuffer(g,sizeof(AtariUnitAns),KTROBO_ATARI_OFFSET,NULL,&buffer_ans2_aida);
		if(FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer");
		}
		hr = AtariHantei::createBufferUnorderedAccessView(g,buffer_ans2_aida,&buffer_ans2_aida_view);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer view");
		}



		if (buffer_ans2_copy) {
			buffer_ans2_copy->Release();
			buffer_ans2_copy = 0;
		}
		if (buffer_ans2_copy_aida) {
			buffer_ans2_copy_aida->Release();
			buffer_ans2_copy_aida  =0;
		}

		hr = AtariHantei::createBufferForCopy(g,buffer_ans2,&buffer_ans2_copy);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in create buffer copy");
		}
		hr = AtariHantei::createBufferForCopy(g, buffer_ans2_aida,&buffer_ans2_copy_aida);
		if (FAILED(hr)) {
			throw new  GameError(KTROBO::FATAL_ERROR, "failed in create buffer copy");
		}

	}


	
	need_calc_kumi = true;

	is_unit_updated = false;

	// dummy の値がなくなるので関連の情報をクリアする
	clearDummyInfo(false);
}

void AtariHantei::clearDummyInfo(bool is_use_in_mae_dummy) {

	state_changed_plus_count.ans_count = 0;
	state_changed_plus_count.auinfo_count = 0;
	state_changed_plus_count.igaidousi_count = 0;
	state_changed_plus_count.indexs_count = 0;
	state_changed_plus_count.kumi_count = 0;
	state_changed_plus_count.obbs_count = 0;
	state_changed_plus_count.soreigai_count = 0;
	state_changed_plus_count.vertexs_count = 0;

	if (false == is_use_in_mae_dummy) {
		// 全てクリアする
		state_changed_umeshunit.clear();
		auinfo_dummy_index.clear();
		
		autts_dummy_index.clear();
	
		autid_dummy_index.clear();
		
		kumi_dummy_index.clear();
		
		obbs_dummy_index.clear();
		au_dummy_index.clear();

	}

	auinfo_dummy_index_used.clear();
	autts_dummy_index_used.clear();
	autid_dummy_index_used.clear();
	kumi_dummy_index_used.clear();
	obbs_dummy_index_used.clear();
	au_dummy_index_used.clear();

}

void AtariHantei::calcKumi(Graphics* g) {
	// 組の変数に値を入れる
	if (!atari_start) return;
	if (!need_calc_kumi) {
		if (g->isCopied()) {
			ID3D11DeviceContext* context;
			context = g->getImmediateContext();
			context->UpdateSubresource(buffer_kumi, 0, 0, kumi, 0, 0);
			context->UpdateSubresource(buffer_autts, 0, 0, autts, 0, 0);
			context->UpdateSubresource(buffer_autid, 0, 0, autid, 0, 0);
		}
		else {
			g->getDeviceContext()->UpdateSubresource(buffer_kumi, 0, 0, kumi, 0, 0);
			//	g->getDeviceContext()->Map(buffer_kumi,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
			//	memcpy(subresource.pData, kumi, sizeof(AtariUnitKumi) * temp);
			//	g->getDeviceContext()->Unmap(buffer_kumi,0);
			g->getDeviceContext()->UpdateSubresource(buffer_autts, 0, 0, autts, 0, 0);
			//	subresource.pData = autts;
			//	g->getDeviceContext()->Map(buffer_autts,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
			//	memcpy(subresource.pData, autts, sizeof(AtariUnitTikeiToSoreigai) * temp_tosoreigai);
			//	g->getDeviceContext()->Unmap(buffer_autts,0);
			g->getDeviceContext()->UpdateSubresource(buffer_autid, 0, 0, autid, 0, 0);
		}

		return;
	}
	int temp = 0;
	int temp_igaidousi = 0;
	int temp_tosoreigai = 0;
	for (int i = 0; i < au_count; i++) {
		for (int k = 0; k < au_count; k++) {
			if (i >= k) continue;

			AtariUnit* aui = &units[i];
			AtariUnit* auk = &units[k];

			if (aui->umesh_unit == auk->umesh_unit) continue;


			if ((aui->type == AtariUnit::AtariType::ATARI_TIKEI)
				&& (auk->type == AtariUnit::AtariType::ATARI_TIKEI)) {
				continue;
			}

			if ((aui->type != AtariUnit::AtariType::ATARI_TIKEI)
				&& auk->type != AtariUnit::AtariType::ATARI_TIKEI) {
				// igaidousi
				autid[temp_igaidousi].atariidx = aui->atariidx;
				autid[temp_igaidousi].atariidx2 = auk->atariidx;
				for (int t = 0; t < KTROBO_MESH_BONE_MAX; t++) {
					for (int h = 0; h < KTROBO_MESH_BONE_MAX; h++) {
						if (aui->umesh->is_bone_obbs_use[t] && auk->umesh->is_bone_obbs_use[h]) {
							autid[temp_igaidousi].obbidx = aui->umesh->bone_obbs_idx[t];
							autid[temp_igaidousi].obbidx2 = auk->umesh->bone_obbs_idx[h];
							temp_igaidousi++;
							if (temp_igaidousi >= max_count.igaidousi_count) {
								throw new GameError(KTROBO::FATAL_ERROR, "igaidousicount over no nono ");
							}

							autid[temp_igaidousi].atariidx = aui->atariidx;
							autid[temp_igaidousi].atariidx2 = auk->atariidx;
						}
					}
				}
			}
			if ((aui->type == AtariUnit::AtariType::ATARI_TIKEI)
				&& auk->type != AtariUnit::AtariType::ATARI_TIKEI) {
				// tosoreigai
				autts[temp_tosoreigai].atariidx = auk->atariidx;
				autts[temp_tosoreigai].atariidx2 = aui->atariidx;
				for (int h = 0; h < KTROBO_MESH_BONE_MAX; h++) {
					if (auk->umesh->is_bone_obbs_use[h]) {
						autts[temp_tosoreigai].obbidx = auk->umesh->bone_obbs_idx[h];
						temp_tosoreigai++;
						if (temp_tosoreigai >= max_count.soreigai_count) {
							throw new GameError(KTROBO::FATAL_ERROR, "tosoreigai count over no nono");
						}

						autts[temp_tosoreigai].atariidx = auk->atariidx;
						autts[temp_tosoreigai].atariidx2 = aui->atariidx;
					}
				}
			}
			if ((aui->type != AtariUnit::AtariType::ATARI_TIKEI)
				&& auk->type == AtariUnit::AtariType::ATARI_TIKEI) {
				// tosoreigai
				autts[temp_tosoreigai].atariidx = aui->atariidx;
				autts[temp_tosoreigai].atariidx2 = auk->atariidx;
				for (int t = 0; t < KTROBO_MESH_BONE_MAX; t++) {
					if (aui->umesh->is_bone_obbs_use[t]) {
						autts[temp_tosoreigai].obbidx = aui->umesh->bone_obbs_idx[t];
						temp_tosoreigai++;
						if (temp_tosoreigai >= max_count.soreigai_count) {
							throw new GameError(KTROBO::FATAL_ERROR, " no no no ");
						}

						autts[temp_tosoreigai].atariidx = aui->atariidx;
						autts[temp_tosoreigai].atariidx2 = auk->atariidx;
					}
				}
			}

			kumi[temp].atari_idx = aui->atariidx;
			kumi[temp].atari_idx2 = auk->atariidx;
			temp++;
			if (temp >= max_count.kumi_count) {
				throw new GameError(KTROBO::FATAL_ERROR, " no noooo");
			}
		}
	}

	temp_count.kumi_count = temp;
	temp_count.igaidousi_count = temp_igaidousi;
	temp_count.soreigai_count = temp_tosoreigai;
	// buffer に値を入れる
	D3D11_MAPPED_SUBRESOURCE subresource;
	subresource.pData = kumi;
	subresource.DepthPitch = 0;
	subresource.RowPitch = 0;


	if (g->isCopied()) {
		ID3D11DeviceContext* context;
		context = g->getImmediateContext();
		context->UpdateSubresource(buffer_kumi, 0, 0, kumi, 0, 0);
		context->UpdateSubresource(buffer_autts, 0, 0, autts, 0, 0);
		context->UpdateSubresource(buffer_autid, 0, 0, autid, 0, 0);
	}
	else {

	g->getDeviceContext()->UpdateSubresource(buffer_kumi, 0, 0, kumi, 0, 0);
	//	g->getDeviceContext()->Map(buffer_kumi,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	//	memcpy(subresource.pData, kumi, sizeof(AtariUnitKumi) * temp);
	//	g->getDeviceContext()->Unmap(buffer_kumi,0);
	g->getDeviceContext()->UpdateSubresource(buffer_autts, 0, 0, autts, 0, 0);
	//	subresource.pData = autts;
	//	g->getDeviceContext()->Map(buffer_autts,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	//	memcpy(subresource.pData, autts, sizeof(AtariUnitTikeiToSoreigai) * temp_tosoreigai);
	//	g->getDeviceContext()->Unmap(buffer_autts,0);
	g->getDeviceContext()->UpdateSubresource(buffer_autid, 0, 0, autid, 0, 0);
	//	subresource.pData = autid;
	//	g->getDeviceContext()->Map(buffer_autid,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	//	memcpy(subresource.pData, autid, sizeof(AtariUnitTikeiIgaiDousi) * temp_igaidousi);
	//	g->getDeviceContext()->Unmap(buffer_autid,0);
	}
	need_calc_kumi = false;
}


void AtariHantei::calcKumiKuwasiku(Graphics* g) {
		// 組の変数に値を入れる
	if (!atari_start) return;
	int temp = 0;
	int temp_igaidousi = 0;
	int temp_tosoreigai = 0;
	for (int i=0;i<atatta_count;i++) {
		if(ans[i].is_use) {

			AtariUnit* aui = &units[ans[i].atari_idx];
			AtariUnit* auk = &units[ans[i].atari_idx2];
			if (ans[i].atari_idx == ans[i].atari_idx2) continue;

			if ((aui->type == AtariUnit::AtariType::ATARI_TIKEI)
				&& (auk->type == AtariUnit::AtariType::ATARI_TIKEI)) {
					continue;
			}

			if ((aui->type != AtariUnit::AtariType::ATARI_TIKEI)
				&& auk->type != AtariUnit::AtariType::ATARI_TIKEI) {
					// igaidousi
					autid[temp_igaidousi].atariidx = aui->atariidx;
					autid[temp_igaidousi].atariidx2 = auk->atariidx;
					for (int t=0;t<KTROBO_MESH_BONE_MAX;t++) {
						for (int h=0;h<KTROBO_MESH_BONE_MAX;h++) {
							if (aui->umesh->is_bone_obbs_use[t] && auk->umesh->is_bone_obbs_use[h]) {
								autid[temp_igaidousi].obbidx = aui->umesh->bone_obbs_idx[t];
								autid[temp_igaidousi].obbidx2 = auk->umesh->bone_obbs_idx[h];
								temp_igaidousi++;
								if (temp_igaidousi >= max_count.igaidousi_count) {
									throw new GameError(KTROBO::FATAL_ERROR, "\nkuwasiku igaidousi count no nono \n");
								}

								autid[temp_igaidousi].atariidx = aui->atariidx;
								autid[temp_igaidousi].atariidx2 = auk->atariidx;
							}
						}
					}
			}
			if ((aui->type == AtariUnit::AtariType::ATARI_TIKEI)
				&& auk->type != AtariUnit::AtariType::ATARI_TIKEI) {
					// tosoreigai
					autts[temp_tosoreigai].atariidx = auk->atariidx;
					autts[temp_tosoreigai].atariidx2 = aui->atariidx;
					for (int h=0;h<KTROBO_MESH_BONE_MAX;h++) {
						if (auk->umesh->is_bone_obbs_use[h]) {
							autts[temp_tosoreigai].obbidx = auk->umesh->bone_obbs_idx[h];
							temp_tosoreigai++;
							if (temp_tosoreigai >= max_count.soreigai_count) {
								throw new GameError(KTROBO::FATAL_ERROR, "\n soreigai count over no nono\n");
							}

							autts[temp_tosoreigai].atariidx = auk->atariidx;
							autts[temp_tosoreigai].atariidx2 = aui->atariidx;
						}
					}
			}
			if ((aui->type != AtariUnit::AtariType::ATARI_TIKEI)
				&& auk->type == AtariUnit::AtariType::ATARI_TIKEI) {
					// tosoreigai
					autts[temp_tosoreigai].atariidx = aui->atariidx;
					autts[temp_tosoreigai].atariidx2 = auk->atariidx;
					for (int t=0;t<KTROBO_MESH_BONE_MAX;t++) {
						if (aui->umesh->is_bone_obbs_use[t]) {
							autts[temp_tosoreigai].obbidx = aui->umesh->bone_obbs_idx[t];
							temp_tosoreigai++;
							if (temp_tosoreigai >= max_count.soreigai_count) {
								throw new GameError(KTROBO::FATAL_ERROR, " no no no ");
							}

							autts[temp_tosoreigai].atariidx = aui->atariidx;
							autts[temp_tosoreigai].atariidx2 = auk->atariidx;
						}
					}
			}

			
		}
	}


	temp_count.igaidousi_count = temp_igaidousi;
	temp_count.soreigai_count = temp_tosoreigai;

	// buffer に値を入れる
	if (buffer_autts) {
		if (g->isCopied()) {
			ID3D11DeviceContext* context;
			context = g->getImmediateContext();
			context->UpdateSubresource(buffer_autts, 0, 0, autts, 0, 0);
//			g->getDeviceContext()->UpdateSubresource(buffer_autts, 0, 0, autts, 0, 0);

		}
		else {
			g->getDeviceContext()->UpdateSubresource(buffer_autts, 0, 0, autts, 0, 0);
		}
	}
	if (buffer_autid) {

		if (g->isCopied()) {
			ID3D11DeviceContext* context;
			context = g->getImmediateContext();
			context->UpdateSubresource(buffer_autid, 0, 0, autid, 0, 0);
		}
		else {
		g->getDeviceContext()->UpdateSubresource(buffer_autid, 0, 0, autid, 0, 0);
		}
	}
}


void AtariHantei::calcAuInfo(Graphics* g, bool calc_vertex_and_index) {
	if (!atari_start) return;
	
	int temp_index_place=0;
	int temp_vertex_place=0;

	for (int i = 0;i<au_count;i++) {
		AtariUnit* au = &units[i];
		if( au->umesh && au->umesh_unit) {
			au_info[au->atariidx].world = au->umesh_unit->world;
			au_info[au->atariidx].r = au->umesh_unit->r;
			au_info[au->atariidx].unit_type = au->umesh_unit->getType();
			au_info[au->atariidx].jyusin = au->umesh_unit->jyusin;
			au_info[au->atariidx].v = au->umesh_unit->v;
			au_info[au->atariidx].atari_idx = au->atariidx;
			au_info[au->atariidx].dt = au->umesh_unit->dt;
			if (calc_vertex_and_index && au->umesh->vertexs && au->umesh->mesh) {
				au_info[au->atariidx].vertexs_place = temp_vertex_place;
				au_info[au->atariidx].vertex_count = au->umesh->mesh->VertexCount;
				int vc = au->umesh->mesh->VertexCount;
				if (vc + temp_vertex_place >= max_count.vertexs_count) {
					throw new GameError(KTROBO::FATAL_ERROR, "no more vertex");
				}

				for (int k=0;k<vc;k++) {
					max_tikei_vertexs[k+temp_vertex_place].pos = MYVECTOR4(au->umesh->vertexs[k].pos.float3.x,
						au->umesh->vertexs[k].pos.float3.y,au->umesh->vertexs[k].pos.float3.z, 1);
					max_tikei_vertexs[k+temp_vertex_place].normal = MYVECTOR4(au->umesh->vertexs[k].normal.float3.x,
						au->umesh->vertexs[k].normal.float3.y, au->umesh->vertexs[k].normal.float3.z,0);
//					max_tikei_vertexs[k+temp_vertex_place].UV.x = au->umesh->vertexs[k].UV.x;
	//				max_tikei_vertexs[k+temp_vertex_place].UV.y = au->umesh->vertexs[k].UV.y;
					for (int t=0;t<MODEL_BLEND_COUNT+3;t++) {
	//					max_tikei_vertexs[k+temp_vertex_place].Index[t] = au->umesh->vertexs[k].Index[t];
					}
					for (int t=0;t<MODEL_BLEND_COUNT;t++) {
	//					max_tikei_vertexs[k+temp_vertex_place].weight[t] = au->umesh->vertexs[k].weight[t];
					}
				}

				temp_vertex_place += au->umesh->mesh->VertexCount;
			}
			if (calc_vertex_and_index && au->umesh->indexs && au->umesh->mesh) {
				au_info[au->atariidx].indexs_place = temp_index_place;
				au_info[au->atariidx].index_count = au->umesh->mesh->FaceCount * 3;
				
				int ic = au->umesh->mesh->FaceCount* 3;
				if (ic + temp_index_place >= max_count.indexs_count) {
					throw new GameError(KTROBO::FATAL_ERROR, "no more index in atarihantei");
				}
				for (int k=0;k<ic;k++) {
					max_tikei_indexs[k+temp_index_place].index = au->umesh->indexs[k];
				}
				temp_index_place += au->umesh->mesh->FaceCount * 3;
			}
		}

	}

	if (calc_vertex_and_index) {
		temp_count.vertexs_count = temp_vertex_place;
		temp_count.indexs_count = temp_index_place;
	}
	//buffer に値を入れる
	// vertex index 

	// calc_vertex_and_index が　真のときは
	// buffer_au_info buffer_vertex buffer_index を書き換える
	// 偽のときは
	// buffer_au_info のみ
	if (calc_vertex_and_index) {
/*		D3D11_MAPPED_SUBRESOURCE subresource;
		subresource.pData = au_info;
		subresource.DepthPitch = 0;
		subresource.RowPitch = 0;
		*/
		if (g->isCopied()) {
		ID3D11DeviceContext* context;
		context = g->getImmediateContext();
		context->UpdateSubresource(buffer_au_info, 0, 0, au_info, 0, 0);
		context->UpdateSubresource(buffer_vertexs, 0, 0, max_tikei_vertexs, 0, 0);
		context->UpdateSubresource(buffer_indexs, 0, 0, max_tikei_indexs, 0, 0);
		
		}
		else {
			g->getDeviceContext()->UpdateSubresource(buffer_au_info, 0, 0, au_info, 0, 0);
			g->getDeviceContext()->UpdateSubresource(buffer_vertexs, 0, 0, max_tikei_vertexs, 0, 0);
			g->getDeviceContext()->UpdateSubresource(buffer_indexs, 0, 0, max_tikei_indexs, 0, 0);
		}
/*
		g->getDeviceContext()->Map(buffer_au_info,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, au_info, sizeof(AtariUnitInfo) * au_count);
		g->getDeviceContext()->Unmap(buffer_au_info,0);

		subresource.pData = max_tikei_vertexs;
		g->getDeviceContext()->Map(buffer_vertexs,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, max_tikei_vertexs, sizeof(AtariUnitVertexs) * temp_vertex_place);
		g->getDeviceContext()->Unmap(buffer_vertexs,0);

		subresource.pData = max_tikei_indexs;
		g->getDeviceContext()->Map(buffer_indexs,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, max_tikei_indexs, sizeof(UINT) * temp_index_place);
		g->getDeviceContext()->Unmap(buffer_indexs,0);
*/
	}
	else {
		/*		D3D11_MAPPED_SUBRESOURCE subresource;
				subresource.pData = au_info;
				subresource.DepthPitch = 0;
				subresource.RowPitch = 0;
		*/		if (g->isCopied()) {
				ID3D11DeviceContext* context;
				context = g->getImmediateContext();
				context->UpdateSubresource(buffer_au_info, 0, 0, au_info, 0, 0);
				} else {
				g->getDeviceContext()->UpdateSubresource(buffer_au_info, 0, 0, au_info, 0, 0);
				}

/*
		g->getDeviceContext()->Map(buffer_au_info,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
		memcpy(subresource.pData, au_info, sizeof(AtariUnitInfo) * au_count);
		g->getDeviceContext()->Unmap(buffer_au_info,0);
*/
	}
	
}


void AtariHantei::calcObb(Graphics* g) {
	if (!atari_start) return;
	
	for (int i = 0;i<au_count;i++) {
		AtariUnit* au = &units[i];
		for (int k=0;k<KTROBO_MESH_BONE_MAX;k++) {
			if (au->umesh_unit && au->umesh_unit->getIsEnabled() && au->umesh && au->umesh->is_bone_obbs_use[k] && (au->type != AtariUnit::AtariType::ATARI_TIKEI)) {
				obbs[au->umesh->bone_obbs_idx[k]].c = au->umesh->bone_obbs[k].c;
				obbs[au->umesh->bone_obbs_idx[k]].e = au->umesh->bone_obbs[k].e;
				obbs[au->umesh->bone_obbs_idx[k]].u[0] =au->umesh->bone_obbs[k].u[0];
				obbs[au->umesh->bone_obbs_idx[k]].u[1] = au->umesh->bone_obbs[k].u[1];
				obbs[au->umesh->bone_obbs_idx[k]].u[2] = au->umesh->bone_obbs[k].u[2];
			}
		}
	}

	obbs[KTROBO_ATARI_CALC_OBBS_IDX_TIKEI].c = MYVECTOR3(0,0,0);
	obbs[KTROBO_ATARI_CALC_OBBS_IDX_TIKEI].e = MYVECTOR3(1,1,1);
	obbs[KTROBO_ATARI_CALC_OBBS_IDX_TIKEI].u[0] = MYVECTOR3(1,0,0);
	obbs[KTROBO_ATARI_CALC_OBBS_IDX_TIKEI].u[1] = MYVECTOR3(0,1,0);
	obbs[KTROBO_ATARI_CALC_OBBS_IDX_TIKEI].u[2] = MYVECTOR3(0,0,1);

	obbs[KTROBO_ATARI_CALC_OBBS_IDX_DUMMY].c = MYVECTOR3(0,0,0);
	obbs[KTROBO_ATARI_CALC_OBBS_IDX_DUMMY].e = MYVECTOR3(1,1,1);
	obbs[KTROBO_ATARI_CALC_OBBS_IDX_DUMMY].u[0] = MYVECTOR3(1,0,0);
	obbs[KTROBO_ATARI_CALC_OBBS_IDX_DUMMY].u[1] = MYVECTOR3(0,1,0);
	obbs[KTROBO_ATARI_CALC_OBBS_IDX_DUMMY].u[2] = MYVECTOR3(0,0,1);





	// buffer に値を入れる
/*	D3D11_MAPPED_SUBRESOURCE subresource;
	subresource.pData = obbs;
	subresource.DepthPitch = 0;
	subresource.RowPitch = 0;
*/
	if (g->isCopied()) {
		ID3D11DeviceContext* context;
		context = g->getImmediateContext();
		context->UpdateSubresource(buffer_obbs, 0, 0, obbs, 0, 0);
	} else {
		g->getDeviceContext()->UpdateSubresource(buffer_obbs, 0, 0, obbs, 0, 0);
	}
	
/*
	g->getDeviceContext()->Map(buffer_obbs,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, obbs, sizeof(AtariUnitKumi) * au_obbs_count);
	g->getDeviceContext()->Unmap(buffer_obbs,0);
*/
}


HRESULT AtariHantei::createStructuredBuffer(Graphics* g, UINT element_size, UINT count, void* pInitData, ID3D11Buffer** ppBufferOut) {

	*ppBufferOut = 0;
	D3D11_BUFFER_DESC desc;
	memset(&desc,0,sizeof(desc));
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;//D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.ByteWidth = element_size * count;
	desc.StructureByteStride = element_size;
	
	if (pInitData) {
		D3D11_SUBRESOURCE_DATA initData;
		memset(&initData,0,sizeof(D3D11_SUBRESOURCE_DATA));
		initData.pSysMem = pInitData;
		return g->getDevice()->CreateBuffer(&desc,&initData,ppBufferOut);
	}
	return g->getDevice()->CreateBuffer(&desc,0,ppBufferOut);
}


HRESULT AtariHantei::createShaderResourceView(Graphics* g, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSrvOut) {

	D3D11_BUFFER_DESC desc;
	memset(&desc,0,sizeof(D3D11_BUFFER_DESC));
	pBuffer->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc,0,sizeof(srvDesc));

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	if (desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) {
		srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		srvDesc.BufferEx.NumElements = desc.ByteWidth / 4;


	} else if( desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED) {
		srvDesc.Format = DXGI_FORMAT_UNKNOWN; // DXGI_FORMAT_UNKNOWN
	
		srvDesc.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
	} else {
		return E_INVALIDARG;
	}
	return g->getDevice()->CreateShaderResourceView(pBuffer, &srvDesc,ppSrvOut);
}

HRESULT AtariHantei::createBufferUnorderedAccessView(Graphics* g, ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUavOut) {

	D3D11_BUFFER_DESC desc;
	memset(&desc,0, sizeof(desc));
	pBuffer->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	memset(&uavDesc, 0, sizeof(uavDesc));

	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;

	if (desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) {

		uavDesc.Format              = DXGI_FORMAT_R32_TYPELESS;
		uavDesc.Buffer.Flags        = D3D11_BUFFER_UAV_FLAG_RAW;
		uavDesc.Buffer.NumElements  = desc.ByteWidth / 4;
	} else if ( desc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED ) {
	          uavDesc.Format              = DXGI_FORMAT_UNKNOWN;
			  uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
	          uavDesc.Buffer.NumElements  = desc.ByteWidth / desc.StructureByteStride;
	} else {
	          return E_INVALIDARG;
	}

	return g->getDevice()->CreateUnorderedAccessView( pBuffer, &uavDesc, ppUavOut );
}
HRESULT AtariHantei::createBufferForCopy(Graphics* g, ID3D11Buffer* pBuffer, ID3D11Buffer** ppBufOut) {

	D3D11_BUFFER_DESC desc;
	memset(&desc,0,sizeof(desc));
	pBuffer->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;

	return g->getDevice()->CreateBuffer(&desc,0,ppBufOut);
}

HRESULT AtariHantei::copyKekkaToBufferForCopy(Graphics* g,bool isans1) {
	if (!is_updated) return S_OK;
	if (!buffer_ans || !buffer_ans2 || !buffer_ans_copy || !buffer_ans2_copy ) return S_OK;
	if (isans1) {

		D3D11_MAPPED_SUBRESOURCE subRes;
		AtariUnitAns* anspo;

		// ans のis_use を0で初期化する
		for (int i=0;i<max_count.ans_count;i++) {
			ans[i].is_use = 0;
		}

		g->getDeviceContext()->CopyResource(buffer_ans_copy , buffer_ans);
		// マップ.
		int i_count=0;
		
		g->getDeviceContext()->Map(buffer_ans_copy, 0, D3D11_MAP_READ, 0, &subRes );
		anspo = (AtariUnitAns*)subRes.pData;
		if (anspo) {

			AtariUnitAns temp_ans[512];
			for (int i=0;i<512;i++) {
				temp_ans[i].is_use = 0;
				//memset
			}
			if (temp_count.kumi_count > 512 && false) {
		
			if (anspo) {
				memcpy_s(temp_ans,sizeof(AtariUnitAns)*512,anspo,sizeof(AtariUnitAns)*512);
				for (int i=0;i<128;i++) {
					if (temp_ans[i].is_use) {
						ans[i_count].atari_idx = temp_ans[i].atari_idx;
						ans[i_count].atari_idx2 = temp_ans[i].atari_idx2;
						ans[i_count].kouten_housen = temp_ans[i].kouten_housen;
						ans[i_count].kouten_jyusin = temp_ans[i].kouten_jyusin;
						ans[i_count].obbidx = temp_ans[i].obbidx;
						ans[i_count].obbidx2 = temp_ans[i].obbidx2;
						ans[i_count].is_use = 1;
						i_count++;
					}
				}
			}
			g->getDeviceContext()->Unmap(buffer_ans_copy,0);	
			atatta_count = i_count;
		} else {

		for (int i=0;i<temp_count.kumi_count;i++) {
			if (anspo[i].is_use) {
			ans[i_count].atari_idx = anspo[i].atari_idx;
			ans[i_count].atari_idx2 = anspo[i].atari_idx2;
			ans[i_count].kouten_housen = anspo[i].kouten_housen;
			ans[i_count].kouten_jyusin = anspo[i].kouten_jyusin;
			ans[i_count].obbidx = anspo[i].obbidx;
			ans[i_count].obbidx2 = anspo[i].obbidx2;
			ans[i_count].is_use = 1;

			i_count++;
			} else {
				break;
			}
		}
		
		g->getDeviceContext()->Unmap(buffer_ans_copy,0);
		atatta_count = i_count;
		}
		
		}
	} else {
		D3D11_MAPPED_SUBRESOURCE subRes;
		AtariUnitAns* anspo;

		// ans のis_use を0で初期化する
		for (int i=0;i<max_count.ans_count;i++) {
			ans[i].is_use = 0;
		}
		if (g->isCopied()) {
			ID3D11DeviceContext* context;
			//g->getDevice()->GetImmediateContext(&context);
			context = g->getImmediateContext();
			context->CopyResource(buffer_ans2_copy, buffer_ans2);
		}
		else {
			g->getDeviceContext()->CopyResource(buffer_ans2_copy, buffer_ans2);
		}
		
		// マップ.
		int i_count=0;
		AtariUnitAns temp_ans[128];
		for (int i=0;i<128;i++) {
			temp_ans[i].is_use = 0;
			//memset
		}
		if (g->isCopied()) {
			ID3D11DeviceContext* context;
			//g->getDevice()->GetImmediateContext(&context);
			context = g->getImmediateContext();
			context->Map(buffer_ans2_copy, 0, D3D11_MAP_READ, 0, &subRes);
		}
		else {
			g->getDeviceContext()->Map(buffer_ans2_copy, 0, D3D11_MAP_READ, 0, &subRes);
		}
		anspo = (AtariUnitAns*)subRes.pData;
		if (temp_count.ans_count > 256) {
		
			if (anspo) {
				bool is_loop = true;

				while(is_loop && temp_count.ans_count > i_count + 256) {
					int temp_c = i_count;
				memcpy_s(&ans[i_count],sizeof(AtariUnitAns)*256,anspo,sizeof(AtariUnitAns)*256);
				for (int i=0;i<256;i++) {
					if (ans[i+temp_c].is_use) {
						
						ans[i_count].atari_idx = anspo[i].atari_idx;
						ans[i_count].atari_idx2 = anspo[i].atari_idx2;
						ans[i_count].kouten_housen = anspo[i].kouten_housen;
						ans[i_count].kouten_jyusin = anspo[i].kouten_jyusin;
						ans[i_count].obbidx = anspo[i].obbidx;
						ans[i_count].obbidx2 = anspo[i].obbidx2;
						ans[i_count].is_use = 1;
						


						i_count++;
					} else {
						is_loop = false;
						break;
					}
				}
				}
			}
			if (g->isCopied()) {
				ID3D11DeviceContext* context;
				//g->getDevice()->GetImmediateContext(&context);
				context = g->getImmediateContext();
				context->Unmap(buffer_ans2_copy, 0);
			}
			else {
				g->getDeviceContext()->Unmap(buffer_ans2_copy, 0);
			}
			atatta_count = i_count;
		}
		else {
			if (anspo) {
				for (int i = 0; i < temp_count.ans_count; i++) {
					if (anspo[i].is_use) {
						ans[i_count].atari_idx = anspo[i].atari_idx;
						ans[i_count].atari_idx2 = anspo[i].atari_idx2;
						ans[i_count].kouten_housen = anspo[i].kouten_housen;
						ans[i_count].kouten_jyusin = anspo[i].kouten_jyusin;
						ans[i_count].obbidx = anspo[i].obbidx;
						ans[i_count].obbidx2 = anspo[i].obbidx2;
						ans[i_count].is_use = 1;

						i_count++;
					}
					else {
						break;
					}
				}
			}
			if (g->isCopied()) {
				ID3D11DeviceContext* context;
				//g->getDevice()->GetImmediateContext(&context);
				context = g->getImmediateContext();
				context->Unmap(buffer_ans2_copy, 0);
			}
			else {
			g->getDeviceContext()->Unmap(buffer_ans2_copy, 0);
			}
			atatta_count = i_count;
		}
	}
	is_updated = false;
/*
	for (int i = 0; i < 512; i++) {
		char test[8192];
		memset(test, 0, 8192);
		sprintf_s(test,"adx=%d adx2=%d oidx=%d,oidx2=%d, hs=(%f, %f,%f), js=%3.3f,%3.3f,%3.3f\n", 
			ans[i].atari_idx, ans[i].atari_idx2, ans[i].obbidx, ans[i].obbidx2,
			ans[i].kouten_housen.float3.x, ans[i].kouten_housen.float3.y, ans[i].kouten_housen.float3.z,
			ans[i].kouten_jyusin.float3.x, ans[i].kouten_jyusin.float3.y, ans[i].kouten_jyusin.float3.z);
		mylog::writelog("testfile.txt", test);

	}

	Sleep(1000);
	//throw new GameError(KTROBO::FATAL_ERROR, "testdesu");
	*/
	return S_OK;

}


HRESULT AtariHantei::copyKekkaToBufferForCopy2(Graphics* g) {
	if (!is_updated) return E_FAIL;
	if (!buffer_ans || !buffer_ans2 || !buffer_ans_copy || !buffer_ans2_copy|| !buffer_ans2_aida || !buffer_ans2_copy_aida) return E_FAIL;



	// ans のis_use を0で初期化する
	for (int i=0;i<max_count.ans_count;i++) {
		ans[i].is_use = 0;
	}


	if (temp_count.ans_count > KTROBO_ATARI_OFFSET){
		D3D11_MAPPED_SUBRESOURCE subRes;
		AtariUnitAns* anspo;

		
		// マップ.
		int i_count=0;
		int max_count = icounter.counter;
		int cc = 0;
		bool is_loop = true;

		while (is_loop && (i_count+KTROBO_ATARI_OFFSET < temp_count.ans_count) && (cc*KTROBO_ATARI_OFFSET < temp_count.ans_count)) {
			if (g->isCopied()) {
				ID3D11DeviceContext* context;
				//g->getDevice()->GetImmediateContext(&context);
				context = g->getImmediateContext();
				this->sendAtariWithOffset(context,cc*KTROBO_ATARI_OFFSET);
			} else {

				this->sendAtariWithOffset(g->getDeviceContext(),cc*KTROBO_ATARI_OFFSET);
			}
			if (g->isCopied()) {
				ID3D11DeviceContext* context;
				context = g->getImmediateContext();
				context->CopyResource(buffer_ans2_copy_aida, buffer_ans2_aida);
				HRESULT hr = context->Map(buffer_ans2_copy_aida, 0, D3D11_MAP_READ, 0, &subRes );
			} else {
			g->getDeviceContext()->CopyResource(buffer_ans2_copy_aida, buffer_ans2_aida);
			HRESULT hr = g->getDeviceContext()->Map(buffer_ans2_copy_aida, 0, D3D11_MAP_READ, 0, &subRes );
			}
			anspo = (AtariUnitAns*)subRes.pData;
		
			if (anspo) {
				

			
				int temp_c = i_count;
				//memcpy_s(&ans[i_count],sizeof(AtariUnitAns)*KTROBO_ATARI_OFFSET,anspo,sizeof(AtariUnitAns)*KTROBO_ATARI_OFFSET);
				for (int i=0;i<KTROBO_ATARI_OFFSET;i++) {
					if (anspo[i].is_use) {
					//	ans[i].is_use = 1;
						ans[i_count].atari_idx = anspo[i].atari_idx;
						ans[i_count].atari_idx2 = anspo[i].atari_idx2;
						ans[i_count].kouten_housen = anspo[i].kouten_housen;
						ans[i_count].kouten_jyusin = anspo[i].kouten_jyusin;
						ans[i_count].obbidx = anspo[i].obbidx;
						ans[i_count].obbidx2 = anspo[i].obbidx2;
						ans[i_count].is_use = 1;
						i_count++;
						if (max_count <= i_count) {
							is_loop = false;
							break;
						}
					} else {
						is_loop = false;
						//break;
					}
				}
				
			}

			if (g->isCopied()) {
				ID3D11DeviceContext* context;
				context = g->getImmediateContext();
				context->Unmap(buffer_ans2_copy_aida, 0);
			} else {


				g->getDeviceContext()->Unmap(buffer_ans2_copy_aida,0);
			}
			cc++;
		}
		
		atatta_count = i_count;
	} else {
		D3D11_MAPPED_SUBRESOURCE subRes;
		AtariUnitAns* anspo;

		
		// マップ.
		int i_count=0;
		int cc = 0;
			int max_count = icounter.counter;
		//this->sendAtariWithOffset(g,KTROBO_ATARI_OFFSET*cc);

		if (g->isCopied()) {
				ID3D11DeviceContext* context;
				context = g->getImmediateContext();
				context->CopyResource(buffer_ans2_copy, buffer_ans2);
				context->Map(buffer_ans2_copy,0,D3D11_MAP_READ,0, &subRes);
		} else {

			g->getDeviceContext()->CopyResource(buffer_ans2_copy, buffer_ans2);
			g->getDeviceContext()->Map(buffer_ans2_copy, 0, D3D11_MAP_READ, 0, &subRes );
		}
		anspo = (AtariUnitAns*)subRes.pData;
		if (anspo) {
			for (int i=0;i<temp_count.ans_count;i++) {
				if (anspo[i].is_use) {
				ans[i_count].atari_idx = anspo[i].atari_idx;
				ans[i_count].atari_idx2 = anspo[i].atari_idx2;
				ans[i_count].kouten_housen = anspo[i].kouten_housen;
				ans[i_count].kouten_jyusin = anspo[i].kouten_jyusin;
				ans[i_count].obbidx = anspo[i].obbidx;
				ans[i_count].obbidx2 = anspo[i].obbidx2;
				ans[i_count].is_use = 1;

				i_count++;
				if (i_count >= max_count) {
					break;
				}
				} else {
					break;
				}
			}
			}
		if (g->isCopied()) {
				ID3D11DeviceContext* context;
				context = g->getImmediateContext();
				context->Unmap(buffer_ans2_copy,0);
		} else {
			g->getDeviceContext()->Unmap(buffer_ans2_copy,0);
		}
		atatta_count = i_count;
	}

	is_updated = false;
	is_calc_kuwasiku = KTROBO_ATARI_CALC_KUWASIKU_CALCED;

	return S_OK;

}
void AtariHantei::sendAtariWithOffset(ID3D11DeviceContext* context, int offset) {
		context->CSSetShader(mss_aida_offset.cs, nullptr, 0 );
		ID3D11UnorderedAccessView* pUAV[3] = { buffer_ans_view, buffer_ans2_view, buffer_ans2_aida_view};
	ID3D11ShaderResourceView* pSRVs[8] = { buffer_vertexs_view, buffer_indexs_view, buffer_obbs_view, buffer_au_info_view,
		buffer_kumi_view, buffer_autts_view, buffer_autid_view,0};

	context->CSSetShaderResources(0, 1, &buffer_vertexs_view);
	context->CSSetShaderResources(1, 1, &buffer_indexs_view);
	context->CSSetShaderResources(2, 1, &buffer_obbs_view);
	context->CSSetShaderResources(3, 1, &buffer_au_info_view);
	context->CSSetShaderResources(4, 1, &buffer_kumi_view);
	context->CSSetShaderResources(5, 1, &buffer_autts_view);
	context->CSSetShaderResources(6, 1, &buffer_autid_view);
	//g->getDeviceContext()->CSSetShaderResources(7, 1, pSRVs);


	//g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 2, pUAV, 0 );
	UINT counter = 0;
	context->CSSetUnorderedAccessViews( 0, 1, &buffer_ans_view, &counter );
	counter = offset;
	context->CSSetUnorderedAccessViews( 1, 1, &buffer_ans2_view, &counter );
	counter = 0;
	context->CSSetUnorderedAccessViews( 2, 1, &buffer_ans2_aida_view, &counter );
    D3D11_MAPPED_SUBRESOURCE res;

	context->Map(buffer_count, 0, D3D11_MAP_WRITE_DISCARD, 0, &res );
	memcpy( res.pData, &temp_count, sizeof(AtariHanteiTempCount) );
	context->Unmap(buffer_count, 0 );

	temp_offset.atari_offset = KTROBO_ATARI_OFFSET;
	temp_offset.offset_atari = offset;
	context->Map(buffer_offset, 0, D3D11_MAP_WRITE_DISCARD, 0, &res );
	memcpy( res.pData, &temp_offset, sizeof(AtariHanteiTempOffset) );
	context->Unmap(buffer_offset, 0 );

    ID3D11Buffer* ppCB[ 2 ] = { buffer_count,buffer_offset };
//	ID3D11Buffer* ppCB2[1] = { buffer_offset};

	context->CSSetConstantBuffers( 0, 2, ppCB );
//	g->getDeviceContext()->CSSetConstantBuffers( 1, 1, ppCB2);
	int zenbuoki = temp_count.kumi_count + temp_count.igaidousi_count + temp_count.soreigai_count;
	int x = 32;
	int y = 32;
	int z = 32;

	context->Dispatch( x, y, z );

	ID3D11UnorderedAccessView*  pNullUAVs[ 2 ] = { 0,0 };
	ID3D11ShaderResourceView*   pNullSRVs[ 8 ] = { 0, 0,0,0,0,0,0,0 };
	ID3D11Buffer*               pNullCBs [ 2 ] = { 0,0 };

	context->CSSetShader(0, 0, 0 );
	context->CSSetUnorderedAccessViews( 0, 2, pNullUAVs, 0 );
	context->CSSetShaderResources( 0, 8, pNullSRVs );
	context->CSSetConstantBuffers( 0, 2, pNullCBs );
//	g->getDeviceContext()->CSSetConstantBuffers( 1, 1, pNullCBs );
}	


void AtariHantei::runComputeShaderAida(Graphics* g) {


	g->getDeviceContext()->CSSetShader(mss_aida.cs, nullptr, 0 );
	ID3D11UnorderedAccessView* pUAV[2] = { buffer_ans_view, buffer_ans2_view};
	ID3D11ShaderResourceView* pSRVs[8] = { buffer_vertexs_view, buffer_indexs_view, buffer_obbs_view, buffer_au_info_view,
		buffer_kumi_view, buffer_autts_view, buffer_autid_view,0};

	/*g->getDeviceContext()->CSSetShaderResources(0, 1, &buffer_vertexs_view);
	g->getDeviceContext()->CSSetShaderResources(1, 1, &buffer_indexs_view);
	g->getDeviceContext()->CSSetShaderResources(2, 1, &buffer_obbs_view);
	g->getDeviceContext()->CSSetShaderResources(3, 1, &buffer_au_info_view);
	g->getDeviceContext()->CSSetShaderResources(4, 1, &buffer_kumi_view);
	g->getDeviceContext()->CSSetShaderResources(5, 1, &buffer_autts_view);
	g->getDeviceContext()->CSSetShaderResources(6, 1, &buffer_autid_view);*/
	g->getDeviceContext()->CSSetShaderResources(0, 7, pSRVs);


	//g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 2, pUAV, 0 );
	UINT counter = 0;
	g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 1, &buffer_ans_view, &counter );
	g->getDeviceContext()->CSSetUnorderedAccessViews( 1, 1, &buffer_ans2_view, &counter );
    D3D11_MAPPED_SUBRESOURCE res;

	g->getDeviceContext()->Map(buffer_count, 0, D3D11_MAP_WRITE_DISCARD, 0, &res );
	memcpy( res.pData, &temp_count, sizeof(AtariHanteiTempCount) );
	g->getDeviceContext()->Unmap(buffer_count, 0 );

    ID3D11Buffer* ppCB[ 1 ] = { buffer_count };
	g->getDeviceContext()->CSSetConstantBuffers( 0, 1, ppCB );
 
	int zenbuoki = temp_count.kumi_count + temp_count.igaidousi_count + temp_count.soreigai_count;
	int x = 32*16; // 32*16 32 32
	int y = 32;
	int z = 32;

	g->getDeviceContext()->Dispatch( x, y, z );

	ID3D11UnorderedAccessView*  pNullUAVs[ 2 ] = { 0,0 };
	ID3D11ShaderResourceView*   pNullSRVs[ 8 ] = { 0, 0,0,0,0,0,0,0 };
	ID3D11Buffer*               pNullCBs [ 1 ] = { 0 };

	g->getDeviceContext()->CSSetShader(0, 0, 0 );
	g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 2, pNullUAVs, 0 );
	g->getDeviceContext()->CSSetShaderResources( 0, 8, pNullSRVs );
	g->getDeviceContext()->CSSetConstantBuffers( 0, 1, pNullCBs );

	is_updated = true;















}


void AtariHantei::runComputeShader(Graphics* g) {
	g->getDeviceContext()->CSSetShader(mss.cs, nullptr, 0 );
	ID3D11UnorderedAccessView* pUAV[2] = { buffer_ans_view, buffer_ans2_view};
	ID3D11ShaderResourceView* pSRVs[8] = { buffer_vertexs_view, buffer_indexs_view, buffer_obbs_view, buffer_au_info_view,
		buffer_kumi_view, buffer_autts_view, buffer_autid_view,0};

	g->getDeviceContext()->CSSetShaderResources(0, 1, &buffer_vertexs_view);
	g->getDeviceContext()->CSSetShaderResources(1, 1, &buffer_indexs_view);
	g->getDeviceContext()->CSSetShaderResources(2, 1, &buffer_obbs_view);
	g->getDeviceContext()->CSSetShaderResources(3, 1, &buffer_au_info_view);
	g->getDeviceContext()->CSSetShaderResources(4, 1, &buffer_kumi_view);
	g->getDeviceContext()->CSSetShaderResources(5, 1, &buffer_autts_view);
	g->getDeviceContext()->CSSetShaderResources(6, 1, &buffer_autid_view);
	//g->getDeviceContext()->CSSetShaderResources(7, 1, pSRVs);


	//g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 2, pUAV, 0 );
	UINT counter = 0;
	g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 1, &buffer_ans_view, &counter );
	g->getDeviceContext()->CSSetUnorderedAccessViews( 1, 1, &buffer_ans2_view, &counter );
    D3D11_MAPPED_SUBRESOURCE res;

	g->getDeviceContext()->Map(buffer_count, 0, D3D11_MAP_WRITE_DISCARD, 0, &res );
	memcpy( res.pData, &temp_count, sizeof(AtariHanteiTempCount) );
	g->getDeviceContext()->Unmap(buffer_count, 0 );

    ID3D11Buffer* ppCB[ 1 ] = { buffer_count };
	g->getDeviceContext()->CSSetConstantBuffers( 0, 1, ppCB );
 
	int zenbuoki = temp_count.kumi_count + temp_count.igaidousi_count + temp_count.soreigai_count;
	int x = 32*16;
	int y = 32;
	int z = 32;

	g->getDeviceContext()->Dispatch( x, y, z );

	ID3D11UnorderedAccessView*  pNullUAVs[ 2 ] = { 0,0 };
	ID3D11ShaderResourceView*   pNullSRVs[ 8 ] = { 0, 0,0,0,0,0,0,0 };
	ID3D11Buffer*               pNullCBs [ 1 ] = { 0 };

	g->getDeviceContext()->CSSetShader(0, 0, 0 );
	g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 2, pNullUAVs, 0 );
	g->getDeviceContext()->CSSetShaderResources( 0, 8, pNullSRVs );
	g->getDeviceContext()->CSSetConstantBuffers( 0, 1, pNullCBs );

	is_updated = true;



}


void AtariHantei::runComputeShaderKuwasiku(Graphics* g) {
	g->getDeviceContext()->CSSetShader(mss2.cs, nullptr, 0 );
	ID3D11UnorderedAccessView* pUAV[2] = { buffer_ans_view, buffer_ans2_view};
	ID3D11ShaderResourceView* pSRVs[8] = { buffer_vertexs_view, buffer_indexs_view, buffer_obbs_view, buffer_au_info_view,
		buffer_kumi_view, buffer_autts_view, buffer_autid_view,0};

	g->getDeviceContext()->CSSetShaderResources(0, 1, &buffer_vertexs_view);
	g->getDeviceContext()->CSSetShaderResources(1, 1, &buffer_indexs_view);
	g->getDeviceContext()->CSSetShaderResources(2, 1, &buffer_obbs_view);
	g->getDeviceContext()->CSSetShaderResources(3, 1, &buffer_au_info_view);
	g->getDeviceContext()->CSSetShaderResources(4, 1, &buffer_kumi_view);
	g->getDeviceContext()->CSSetShaderResources(5, 1, &buffer_autts_view);
	g->getDeviceContext()->CSSetShaderResources(6, 1, &buffer_autid_view);
	//g->getDeviceContext()->CSSetShaderResources(7, 1, pSRVs);


//	g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 2, pUAV, 0 );
	//g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 2, pUAV, 0 );

	UINT counter=0;
	g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 1, &buffer_ans_view, &counter );
	g->getDeviceContext()->CSSetUnorderedAccessViews( 1, 1, &buffer_ans2_view, &counter );
    D3D11_MAPPED_SUBRESOURCE res;

	g->getDeviceContext()->Map(buffer_count, 0, D3D11_MAP_WRITE_DISCARD, 0, &res );
	memcpy( res.pData, &temp_count, sizeof(AtariHanteiTempCount) );
	g->getDeviceContext()->Unmap(buffer_count, 0 );

    ID3D11Buffer* ppCB[ 1 ] = { buffer_count };
	g->getDeviceContext()->CSSetConstantBuffers( 0, 1, ppCB );
 
	int zenbuoki = temp_count.kumi_count + temp_count.igaidousi_count + temp_count.soreigai_count;
	int x = 32*16;
	int y = 32;
	int z = 1;

	g->getDeviceContext()->Dispatch( x, y, z );

	ID3D11UnorderedAccessView*  pNullUAVs[ 2 ] = { 0,0 };
	ID3D11ShaderResourceView*   pNullSRVs[ 8 ] = { 0, 0,0,0,0,0,0,0 };
	ID3D11Buffer*               pNullCBs [ 1 ] = { 0 };

	g->getDeviceContext()->CSSetShader(0, 0, 0 );
	g->getDeviceContext()->CSSetUnorderedAccessViews( 0, 2, pNullUAVs, 0 );
	g->getDeviceContext()->CSSetShaderResources( 0, 8, pNullSRVs );
	g->getDeviceContext()->CSSetConstantBuffers( 0, 1, pNullCBs );

	is_updated = true;
}

void AtariHantei::compileShader(Graphics* g) {

	

	HRESULT hr;
	ID3DBlob* pblob = 0;
	try {
		CompileShaderFromFile(KTROBO_ATARI_SHADER_COMPUTE, "CalcCS", "cs_5_0",&pblob,true);
		if (!pblob) {
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
		}
		hr = g->getDevice()->CreateComputeShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&mss.cs);
		if (FAILED(hr)) {
			pblob->Release();
			delete pblob;
			pblob = 0;
			mss.cs = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
		}
		if (pblob) {
			pblob->Release();
			delete pblob;
			pblob = 0;
		}

	//delete[] ss;
	}catch (GameError* err) {
		del();
		if (pblob) {
			pblob->Release();
			pblob = 0;
			
		}
		throw err;
	}

	
	try {
		CompileShaderFromFile(KTROBO_ATARI_SHADER_COMPUTE2, "CalcCS", "cs_5_0",&pblob,true);
			if (!pblob) {
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
		}
		hr = g->getDevice()->CreateComputeShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&mss2.cs);

		if (FAILED(hr)) {
			pblob->Release();
			delete pblob;
			pblob = 0;
			mss2.cs = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
		}
		if (pblob) {
			pblob->Release();
			delete pblob;
			pblob = 0;
		}

	//delete[] ss;
	}catch (GameError* err) {
		del();
		if (pblob) {
			pblob->Release();
			pblob = 0;
			
		}
		throw err;
	}

		try {
		CompileShaderFromFile(KTROBO_ATARI_SHADER_COMPUTE_AIDA, "CalcCS", "cs_5_0",&pblob,true);
			if (!pblob) {
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
		}
		hr = g->getDevice()->CreateComputeShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&mss_aida.cs);

		if (FAILED(hr)) {
			pblob->Release();
			delete pblob;
			pblob = 0;
			mss_aida.cs = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
		}
		if (pblob) {
			pblob->Release();
			delete pblob;
			pblob = 0;
		}

	//delete[] ss;
	}catch (GameError* err) {
		del();
		if (pblob) {
			pblob->Release();
			pblob = 0;
			
		}
		throw err;
	}


	try {
		CompileShaderFromFile(KTROBO_ATARI_SHADER_COMPUTE_AIDA_OFFSET, "CalcCS", "cs_5_0",&pblob,true);
			if (!pblob) {
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
		}
		hr = g->getDevice()->CreateComputeShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&mss_aida_offset.cs);

		if (FAILED(hr)) {
			pblob->Release();
			delete pblob;
			pblob = 0;
			mss_aida_offset.cs = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cs make error");;
		}
		if (pblob) {
			pblob->Release();
			delete pblob;
			pblob = 0;
		}

	//delete[] ss;
	}catch (GameError* err) {
		del();
		if (pblob) {
			pblob->Release();
			pblob = 0;
			
		}
		throw err;
	}

	

	D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(AtariHanteiTempCount);
	des.Usage = D3D11_USAGE_DYNAMIC;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	hr = g->getDevice()->CreateBuffer(&des, 0, &buffer_count);
	if (FAILED(hr)) {
		del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}


	//D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(AtariHanteiTempOffset);
	des.Usage = D3D11_USAGE_DYNAMIC;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	hr = g->getDevice()->CreateBuffer(&des, 0, &buffer_offset);
	if (FAILED(hr)) {
		del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

	//D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(AtariUnitIncrementCounter);
	des.Usage =  D3D11_USAGE_STAGING;
	des.BindFlags = 0;
	des.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	des.MiscFlags = 0;
	des.StructureByteStride = sizeof(AtariUnitIncrementCounter);

	hr = g->getDevice()->CreateBuffer(&des, 0, &buffer_counter);
	if (FAILED(hr)) {
		del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

}


MYSHADERSTRUCT AtariHantei::mss;
MYSHADERSTRUCT AtariHantei::mss2;
MYSHADERSTRUCT AtariHantei::mss_aida;
MYSHADERSTRUCT AtariHantei::mss_aida_offset;

ID3D11Buffer* AtariHantei::buffer_count = 0;
ID3D11Buffer* AtariHantei::buffer_offset = 0;
ID3D11Buffer* AtariHantei::buffer_counter = 0;

void AtariHantei::releaseBufferAndView() {

	if (buffer_vertexs) {
		buffer_vertexs->Release();
		buffer_vertexs = 0;
	}

	if (buffer_indexs) {
		buffer_indexs->Release();
		buffer_indexs = 0;
	}

	if (buffer_au_info) {
		buffer_au_info->Release();
		buffer_au_info = 0;
	}

	if (buffer_obbs) {
		buffer_obbs->Release();
		buffer_obbs = 0;
	}
	if (buffer_kumi) {
		buffer_kumi->Release();
		buffer_kumi = 0;
	}

	if (buffer_ans) {
		buffer_ans->Release();
		buffer_ans = 0;
	}
	if (buffer_ans2) {
		buffer_ans2->Release();
		buffer_ans2 = 0;
	}
	if (buffer_ans2_aida) {
		buffer_ans2_aida->Release();
		buffer_ans2_aida = 0;
	}

	if (buffer_ans_copy) {
		buffer_ans_copy->Release();
		buffer_ans_copy = 0;
	}
	if (buffer_ans2_copy) {
		buffer_ans2_copy->Release();
		buffer_ans2_copy = 0;
	}
	if (buffer_ans2_copy_aida) {
		buffer_ans2_copy_aida->Release();
		buffer_ans2_copy_aida = 0;
	}

	if (buffer_autid) {
		buffer_autid->Release();
		buffer_autid = 0;
	}
	if (buffer_autts) {
		buffer_autts->Release();
		buffer_autts = 0;
	}



	if (buffer_vertexs_view) {
		buffer_vertexs_view->Release();
		buffer_vertexs_view = 0;
	}

	if (buffer_indexs_view) {
		buffer_indexs_view->Release();
		buffer_indexs_view = 0;
	}

	if (buffer_au_info_view) {
		buffer_au_info_view->Release();
		buffer_au_info_view = 0;
	}

	if (buffer_obbs_view) {
		buffer_obbs_view->Release();
		buffer_obbs_view = 0;
	}
	if (buffer_kumi_view) {
		buffer_kumi_view->Release();
		buffer_kumi_view = 0;
	}

	if (buffer_ans_view) {
		buffer_ans_view->Release();
		buffer_ans_view = 0;
	}
	if (buffer_ans2_view) {
		buffer_ans2_view->Release();
		buffer_ans2_view = 0;
	}
	if (buffer_ans2_aida_view) {
		buffer_ans2_aida_view->Release();
		buffer_ans2_aida_view = 0;
	}

	if (buffer_autid_view) {
		buffer_autid_view->Release();
		buffer_autid_view = 0;
	}
	if (buffer_autts_view) {
		buffer_autts_view->Release();
		buffer_autts_view = 0;
	}
}

int AtariHantei::getAnsWaza(AtariUnitAnsKWSK* out_ans, int out_ans_num) {


	int tmp = 0;

	for (int k = 0; k < atatta_count;k++) {
		
		if ((units[ans[k].atari_idx].type == AtariUnit::AtariType::ATARI_WAZA)) {
				out_ans[tmp].ans = &ans[k];
				out_ans[tmp].aite = units[ans[k].atari_idx2].umesh_unit;
			out_ans[tmp].aite_umesh = units[ans[k].atari_idx2].umesh;
					out_ans[tmp].my_umesh = units[ans[k].atari_idx].umesh;
			out_ans[tmp].aite_type = units[ans[k].atari_idx2].type;
			out_ans[tmp].my_umeshunit = units[ans[k].atari_idx].umesh_unit;
			tmp++;
			if (tmp >= out_ans_num) {
				return tmp;
			}
			}
		if ((units[ans[k].atari_idx2].type == AtariUnit::AtariType::ATARI_WAZA)) {// && (units[ans[k].atari_idx].umesh_unit != oya)) {
			out_ans[tmp].ans = &ans[k];
				out_ans[tmp].aite = units[ans[k].atari_idx].umesh_unit;
				out_ans[tmp].aite_umesh = units[ans[k].atari_idx].umesh;
				out_ans[tmp].aite_type = units[ans[k].atari_idx].type;
				out_ans[tmp].my_umesh = units[ans[k].atari_idx2].umesh;
				out_ans[tmp].my_umeshunit = units[ans[k].atari_idx2].umesh_unit;
			tmp++;
			if (tmp >= out_ans_num) {
				return tmp;
			}
			}
		
	}
	return tmp;
}







int AtariHantei::getAns(AtariUnitAnsKWSK* out_ans, UMeshUnit* oya, UMesh* oya2, int out_ans_num) {
	int tmp = 0;

	for (int k = 0; k < atatta_count;k++) {
		if (oya2) {
		if ((units[ans[k].atari_idx].umesh == oya2) && (units[ans[k].atari_idx].umesh_unit == oya)
			&&(units[ans[k].atari_idx2].umesh_unit != oya)) {
			out_ans[tmp].ans = &ans[k];
			out_ans[tmp].aite = units[ans[k].atari_idx2].umesh_unit;
			out_ans[tmp].aite_umesh = units[ans[k].atari_idx2].umesh;
			out_ans[tmp].my_umesh = units[ans[k].atari_idx].umesh;
			out_ans[tmp].aite_type = units[ans[k].atari_idx2].type;
			out_ans[tmp].my_umeshunit = units[ans[k].atari_idx].umesh_unit;
			tmp++;
			if (tmp >= out_ans_num) {
				return tmp;
			}
		}
		if ((units[ans[k].atari_idx2].umesh == oya2) && (units[ans[k].atari_idx2].umesh_unit == oya) &&
			(units[ans[k].atari_idx].umesh_unit != oya)) {
				out_ans[tmp].ans = &ans[k];
				out_ans[tmp].aite = units[ans[k].atari_idx].umesh_unit;
				out_ans[tmp].aite_umesh = units[ans[k].atari_idx].umesh;
				out_ans[tmp].my_umesh = units[ans[k].atari_idx2].umesh;
				out_ans[tmp].aite_type = units[ans[k].atari_idx].type;
				out_ans[tmp].my_umeshunit = units[ans[k].atari_idx2].umesh_unit;
			tmp++;
			if (tmp >= out_ans_num) {
				return tmp;
			}
		}



		} else {
			if ((units[ans[k].atari_idx].umesh_unit == oya) && (units[ans[k].atari_idx2].umesh_unit != oya)) {
				out_ans[tmp].ans = &ans[k];
				out_ans[tmp].aite = units[ans[k].atari_idx2].umesh_unit;
			out_ans[tmp].aite_umesh = units[ans[k].atari_idx2].umesh;
					out_ans[tmp].my_umesh = units[ans[k].atari_idx].umesh;
			out_ans[tmp].aite_type = units[ans[k].atari_idx2].type;
			out_ans[tmp].my_umeshunit = units[ans[k].atari_idx].umesh_unit;
			tmp++;
			if (tmp >= out_ans_num) {
				return tmp;
			}
			}
			if ((units[ans[k].atari_idx2].umesh_unit == oya) && (units[ans[k].atari_idx].umesh_unit != oya)) {
			out_ans[tmp].ans = &ans[k];
				out_ans[tmp].aite = units[ans[k].atari_idx].umesh_unit;
				out_ans[tmp].aite_umesh = units[ans[k].atari_idx].umesh;
				out_ans[tmp].aite_type = units[ans[k].atari_idx].type;
				out_ans[tmp].my_umesh = units[ans[k].atari_idx2].umesh;
				out_ans[tmp].my_umeshunit = units[ans[k].atari_idx2].umesh_unit;
			tmp++;
			if (tmp >= out_ans_num) {
				return tmp;
			}
			}
		}
	}
	return tmp;
}



void AtariHantei::drawKekka(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX idenmat;
	MyMatrixIdentity(idenmat);
	for (int k=0;k<atatta_count;k++) {
		RAY ray;

		ray.org = ans[k].kouten_jyusin;
		ray.dir = ans[k].kouten_housen;
		

		if ((units[ans[k].atari_idx].type == AtariUnit::AtariType::ATARI_TIKEI) ||
			(units[ans[k].atari_idx2].type == AtariUnit::AtariType::ATARI_TIKEI)) {

			DWORD color = 0xFFFFFF00;
			if (MyVec3Length(ray.dir) < 0.5) {
				ray.dir = MYVECTOR3(0,0,1);

				g->drawRAY(g,color,&idenmat,view,proj,50,&ray);
			} else {

				g->drawRAY(g,0xFF2222FF,&idenmat,view,proj,50,&ray);
			}
		}else {
			// 地形以外なのでdrawobb
			OBB ob;
			for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
				if (ans[k].is_use && units[ans[k].atari_idx].umesh && units[ans[k].atari_idx].umesh->is_bone_obbs_use[i] &&
					(units[ans[k].atari_idx].umesh->bone_obbs_idx[i] == ans[k].obbidx)) {
						g->drawOBBFill(g,0xFFFFFFFF,&idenmat,view,proj,&units[ans[k].atari_idx].umesh->bone_obbs[i]);
				}
				if (ans[k].is_use && units[ans[k].atari_idx2].umesh && units[ans[k].atari_idx2].umesh->is_bone_obbs_use[i] &&
					(units[ans[k].atari_idx2].umesh->bone_obbs_idx[i] == ans[k].obbidx2)) {
						g->drawOBBFill(g,0xFF0000FF,&idenmat,view,proj,&units[ans[k].atari_idx2].umesh->bone_obbs[i]);
				}
			}
		}
	}
}

void AtariHantei::copyCounterOfKekkaBuffer(Graphics* g) {
/*
	for (int i=0;i<max_count.ans_count;i++) {
		ans[i].is_use = 0;
	}
//	g->getDeviceContext()->UpdateSubresource(buffer_ans,0,0,ans,0,0);
	g->getDeviceContext()->UpdateSubresource(buffer_ans2,0,0,ans,0,0);
	g->getDeviceContext()->UpdateSubresource(buffer_ans2_aida, 0, 0, ans,0,0);
*/
	D3D11_MAPPED_SUBRESOURCE subresource;
	subresource.pData = 0;
	subresource.DepthPitch = 0;
	subresource.RowPitch = 0;


	if (g->isCopied()) {
		ID3D11DeviceContext* context;
		context = g->getImmediateContext();
		context->CopyStructureCount(buffer_counter, 0,buffer_ans2_view);
		HRESULT hr = context->Map(buffer_counter, 0, D3D11_MAP_READ, 0, &subresource );
		if (!FAILED(hr)) {
			AtariUnitIncrementCounter* pp = (AtariUnitIncrementCounter*)subresource.pData;
			icounter.counter = pp->counter;
		}
		context->Unmap(buffer_counter,0);
	} else {
		g->getDeviceContext()->CopyStructureCount(buffer_counter, 0,buffer_ans2_view);
		HRESULT hr = g->getDeviceContext()->Map(buffer_counter, 0, D3D11_MAP_READ, 0, &subresource );
		if (!FAILED(hr)) {
			AtariUnitIncrementCounter* pp = (AtariUnitIncrementCounter*)subresource.pData;
			icounter.counter = pp->counter;
		}
		g->getDeviceContext()->Unmap(buffer_counter,0);
	}



}


bool AtariHantei::isNeedMaeCalcWhenMaeCalcDummy() {
	// maxcount を超えてしまう場合全ての再計算が必要になる
	// 増分は多く計算する分には問題がない

	// obbs_count の増分を出す
	int obbs_zoubun=0;
	set<UMeshUnit*>::iterator it = state_changed_umeshunit.begin();
	while(it != state_changed_umeshunit.end()) {

		UMeshUnit* umm = *it;
		if (umm->getIsEnabled()) {
			int si = umm->meshs.size();
			for (int f = 0 ; f < si;f++) {
				UMesh* um = umm->meshs[f];
				for (int k=0;k<KTROBO_MESH_BONE_MAX;k++) {
					if (um->is_bone_obbs_use[k]) {
						obbs_zoubun++;
					}
				}
			}
		}
				

		it++;
	}




	// vertexsの増分を出す
	int vertex_c=0;
	// indexsの増分を出す
	int indexs_c=0;



	// au の増分を出す
	int au_zoubun = state_changed_umeshunit.size();

	// auinfoの増分を出す
	int auinfo_zoubun = au_zoubun;

	// kumi の増分を出す
	int kumi_zoubun = (temp_count.auinfo_count + au_zoubun)* (temp_count.auinfo_count +au_zoubun-1)/2 - temp_count.kumi_count;

	int tikei_c = 0;
	int siz = umesh_unit_types.size();

	// vertexs indexsの再計算を行う
	// enabledされてない地形でも計算する　多く計算する分には問題ないので umesh_unitsにはstate_changed_umesh_unit が含まれている
	for (int i=0;i<siz;i++) {
		if (AtariUnit::AtariType::ATARI_TIKEI == umesh_unit_types[i]) {
			
			int si = umesh_units[i]->meshs.size();
			for (int k=0;k<si;k++) {
				if (umesh_units[i]->meshs[k]->vertexs && umesh_units[i]->meshs[k]->mesh) {
					vertex_c += umesh_units[i]->meshs[k]->mesh->VertexCount;
					tikei_c++;// atariunit は　umeshunit 単位ではなくて　umesh単位であるのでこのように計上する
				}
				if (umesh_units[i]->meshs[k]->indexs && umesh_units[i]->meshs[k]->mesh) {
					indexs_c += umesh_units[i]->meshs[k]->mesh->FaceCount*3;
				}
			}
		}
	}

	



	// autid の増分を出す
	int autid_zoubun = (temp_count.obbs_count + obbs_zoubun) * (temp_count.obbs_count + obbs_zoubun -1)/2 - temp_count.igaidousi_count ;

	// autts の増分を出す
	int autts_zoubun = (temp_count.obbs_count + obbs_zoubun) * tikei_c - temp_count.soreigai_count;
	// ansの増分を出す

	int ans_zoubun = autid_zoubun + autts_zoubun;

	

	// zoubun は出したので　一つずつmax_count を超えるかどうかを計算していく

	if (max_count.vertexs_count <= vertex_c) {
		return true;
	}

	if (max_count.indexs_count <= indexs_c) {
		return true;
	}

	if (max_count.ans_count <= temp_count.ans_count + ans_zoubun) {
		return true;
	}

	if (max_count.auinfo_count <= temp_count.auinfo_count + auinfo_zoubun - auinfo_dummy_index.size()) {
		return true;
	}

	if (max_count.igaidousi_count <= temp_count.igaidousi_count + autid_zoubun - autid_dummy_index.size()) {
		return true;
	}

	if (max_count.obbs_count <= temp_count.obbs_count + obbs_zoubun - obbs_dummy_index.size()) {
		return true;
	}
	if (max_count.soreigai_count <= temp_count.soreigai_count + autts_zoubun - autts_dummy_index.size()) {
		return true;
	}

	if (KTROBO_MAX_ATARI_HANTEI_UNIT_NUM <= au_count + au_zoubun -au_dummy_index.size()) {
		return true;
	}
	if (max_count.kumi_count <= temp_count.kumi_count + kumi_zoubun - kumi_dummy_index.size()) {
		return true;
	}

	return false;

}

void AtariHantei::setStateChangedUMeshUnitToDummy() {
	ClockWatches watches;
	//atariunit


	watches.startWatch(0);
	for (int i=0;i<au_count;i++) {
		if (!units[i].umesh_unit) continue;

		if (state_changed_umeshunit.find(units[i].umesh_unit) != state_changed_umeshunit.end()) {
			// dummyを入れる

			units[i].umesh_unit = dummy_umeshunit;
			units[i].umesh = dummy_umesh;
			au_dummy_index.insert(i);
		}
	}
	watches.stopWatch(0);



	// auinfo
	watches.startWatch(1);
	for (int i=0;i<au_count;i++) {
		if (units[au_info[i].atari_idx].umesh_unit == dummy_umeshunit) {
			auinfo_dummy_index.insert(i);
		}
	}
	watches.stopWatch(1);



	//obbs
	watches.startWatch(2);
	set<UMeshUnit*>::iterator it = state_changed_umeshunit.begin();
	while(it != state_changed_umeshunit.end()) {
		UMeshUnit* um = *it;
		int sie = um->meshs.size();
		for (int i=0;i<sie;i++) {
			UMesh* umm = um->meshs[i];
			for (int k=0;k<KTROBO_MESH_BONE_MAX;k++) {
				if ((umm->bone_obbs_idx[k] != KTROBO_ATARI_CALC_OBBS_IDX_DUMMY) && 
					(umm->bone_obbs_idx[k] != KTROBO_ATARI_CALC_OBBS_IDX_TIKEI)) {
						// 使われているのでクリアする
						obbs_dummy_index.insert(umm->bone_obbs_idx[k]);
						umm->bone_obbs_idx[k] = KTROBO_ATARI_CALC_OBBS_IDX_DUMMY;
				}
			}
		}
	



		it++;
	}
	watches.stopWatch(2);

	// kumi
	watches.startWatch(3);

	/*
	for (int i=0;i< temp_count.kumi_count;i++) {
		if (au_dummy_index.find(kumi[i].atari_idx) != au_dummy_index.end()) {
			kumi_dummy_index.insert(i);
		}

		if (au_dummy_index.find(kumi[i].atari_idx2) != au_dummy_index.end()) {
			kumi_dummy_index.insert(i);
		}
	}
	*/

	// calc構造体は使われていいないので
	// 基のままにしておく
	watches.stopWatch(3);



	// autts
	watches.startWatch(4);
	for (int i=0;i<temp_count.soreigai_count;i++) {
		if (au_dummy_index.find(autts[i].atariidx) != au_dummy_index.end()) {
			autts_dummy_index.insert(i);
		}

		if (au_dummy_index.find(autts[i].atariidx2) != au_dummy_index.end()) {
			autts_dummy_index.insert(i);
		}
	}
	watches.stopWatch(4);
	
	// autid
	watches.startWatch(5);
	for (int i=0;i<temp_count.igaidousi_count;i++) {
		if (au_dummy_index.find(autid[i].atariidx) != au_dummy_index.end()) {
			autid_dummy_index.insert(i);
		}

		if (au_dummy_index.find(autid[i].atariidx2) != au_dummy_index.end()) {
			autid_dummy_index.insert(i);
		}
	}
	watches.stopWatch(5);










	
{
		WCHAR buff[512];
		char buf[128];
		char str[128];
		memset(buf,0,128);

		for (int i=0;i<6;i++) {
		memset(str,0,128);
		GamenGARAGE_partsParam::getSuutiChara((int)(watches.times[i]*100),str);
		strcat_s(buf,str);
		strcat_s(buf,",");
	//	GamenGARAGE_partsParam::getSuutiChara((int)(sizeof(AtariUnitAns)),str);
	//	strcat_s(buf,str);
		}
	
		//DebugTexts::instance()->setText(g,wcslen(buff),buff);
	//	if (watches.times[5] > 10) {
//			mylog::writelog(KTROBO::INFO, "setdummydayo::");
//			mylog::writelog(KTROBO::INFO, buf);
//			mylog::writelog(KTROBO::INFO, "\n");
	//	}
	}




}

void AtariHantei::setInfoOfStateChangedUMeshUnit() {
	// atariunit
	set<int>::iterator it_for_dummy_au = au_dummy_index.begin();

	set<UMeshUnit*>::iterator it_for_meshs = state_changed_umeshunit.begin();
	int au_count_offset = 0;

	ClockWatches watches;


	watches.startWatch(0);
	while ( it_for_meshs != state_changed_umeshunit.end() ) {

		UMeshUnit* umm = *it_for_meshs;
		if (umm->getIsEnabled()) {
			// atariunit の情報を入れ込む
			int siz = umm->meshs.size();
			for (int i=0;i<siz;i++) {
				UMesh* um = umm->meshs[i];
				if (it_for_dummy_au != au_dummy_index.end()) {
					int inde = *it_for_dummy_au;
					units[inde].umesh_unit = umm;
					units[inde].umesh = um;
					units[inde].type = umm->getType();
					auinfo_dummy_index_used.insert(inde);
					it_for_dummy_au++;
				} else {
					units[au_count+au_count_offset].umesh_unit = umm;
					units[au_count+au_count_offset].umesh = um;
					units[au_count+au_count_offset].type = umm->getType();
					au_count_offset++;
					auinfo_dummy_index_used.insert(au_count+au_count_offset);
				}
			}
		}
		it_for_meshs++;

	}
	watches.stopWatch(0);

	// auinfo
	// auinfoはcalcauinfoで再計算されるので大丈夫


	// obbs
	// obbsは再計算される
	// 大事なのはau_count を更新すること



	set<int>::iterator it_for_dummy_autts = autts_dummy_index.begin();
	set<int>::iterator it_for_dummy_autid = autid_dummy_index.begin();
	set<int>::iterator it_for_dummy_kumi = kumi_dummy_index.begin();
	// 下記の三つは再計算されないのでここで計算させておく必要がある
	int temp_offset = 0;
	int temp_igaidousi_offset = 0;
	int temp_tosoreigai_offset = 0;

	it_for_dummy_au = au_dummy_index_used.begin();
	// au_count 同士のループだと重いので工夫する
	watches.startWatch(1);
	while (it_for_dummy_au != au_dummy_index_used.end()) {
		AtariUnit* auu = &units[*it_for_dummy_au];
		*it_for_dummy_au++;

	//for (int i = 0;i<au_count+au_count_offset;i++) {
		for (int k=0;k<au_count+au_count_offset;k++) {
			if ((au_dummy_index_used.find(k) != au_dummy_index_used.end()) && (auu->atariidx >= k)) continue;
			//if ((au_dummy_index_used.find(i) == au_dummy_index_used.end()) && (au_dummy_index_used.find(k) == au_dummy_index_used.end())) continue;

			AtariUnit* aui = auu;// &units[i];
			AtariUnit* auk = &units[k];
			if (aui->umesh_unit == auk->umesh_unit) continue;
			
			if ((aui->type == AtariUnit::AtariType::ATARI_TIKEI)
				&& (auk->type == AtariUnit::AtariType::ATARI_TIKEI)) {
					continue;
			}

			if ((aui->type != AtariUnit::AtariType::ATARI_TIKEI)
				&& auk->type != AtariUnit::AtariType::ATARI_TIKEI) {
					// igaidousi
					
					for (int t=0;t<KTROBO_MESH_BONE_MAX;t++) {
						for (int h=0;h<KTROBO_MESH_BONE_MAX;h++) {
							if (aui->umesh->is_bone_obbs_use[t] && auk->umesh->is_bone_obbs_use[h]) {
			
								int empty_index;
								if (it_for_dummy_autid != autid_dummy_index.end()) {
									empty_index = *it_for_dummy_autid;
									it_for_dummy_autid++;
									autid_dummy_index_used.insert(empty_index);
								} else {
									empty_index = temp_count.igaidousi_count + temp_igaidousi_offset;
									temp_igaidousi_offset++;
								}


								autid[empty_index].atariidx = aui->atariidx;
								autid[empty_index].atariidx2 = auk->atariidx;
								autid[empty_index].obbidx = aui->umesh->bone_obbs_idx[t];
								autid[empty_index].obbidx2 = auk->umesh->bone_obbs_idx[h];
								

								

								if (temp_count.igaidousi_count + temp_igaidousi_offset >= max_count.igaidousi_count) {
									throw new GameError(KTROBO::FATAL_ERROR, "igaidousicount over no nono ");
								}
							}
						}
					}
			}
			if ((aui->type == AtariUnit::AtariType::ATARI_TIKEI)
				&& auk->type != AtariUnit::AtariType::ATARI_TIKEI) {
					// tosoreigai

			
				
					for (int h=0;h<KTROBO_MESH_BONE_MAX;h++) {
						if (auk->umesh->is_bone_obbs_use[h]) {
							int empty_index;
							if (it_for_dummy_autts != autts_dummy_index.end()) {
								empty_index = *it_for_dummy_autts;
								it_for_dummy_autts++;
								autts_dummy_index_used.insert(empty_index);
							} else {
								empty_index = temp_count.soreigai_count + temp_tosoreigai_offset;
								temp_tosoreigai_offset++;
							}

							autts[empty_index].atariidx = auk->atariidx;
							autts[empty_index].atariidx2 = aui->atariidx;
							autts[empty_index].obbidx = auk->umesh->bone_obbs_idx[h];
							
							if (temp_count.soreigai_count + temp_tosoreigai_offset >= max_count.soreigai_count) {
								throw new GameError(KTROBO::FATAL_ERROR, "tosoreigai count over no nono");
							}
						}
					}
			}
			if ((aui->type != AtariUnit::AtariType::ATARI_TIKEI)
				&& auk->type == AtariUnit::AtariType::ATARI_TIKEI) {
					// tosoreigai

			
			
					for (int t=0;t<KTROBO_MESH_BONE_MAX;t++) {
						if (aui->umesh->is_bone_obbs_use[t]) {
							int empty_index;
							if (it_for_dummy_autts != autts_dummy_index.end()) {
								empty_index = *it_for_dummy_autts;
								it_for_dummy_autts++;
								autts_dummy_index_used.insert(empty_index);
							} else {
								empty_index = temp_count.soreigai_count + temp_tosoreigai_offset;
								temp_tosoreigai_offset++;
							}

							autts[empty_index].atariidx = aui->atariidx;
							autts[empty_index].atariidx2 = auk->atariidx;
							autts[empty_index].obbidx = aui->umesh->bone_obbs_idx[t];
							

							if (temp_count.soreigai_count + temp_tosoreigai_offset >= max_count.soreigai_count) {
								throw new GameError(KTROBO::FATAL_ERROR, " no no no ");
							}
						}
					}
			}
			
			// kumiをこれ以上増やさないためにそのままにしておく
			// また減らしもしない
			continue;

			int empty_index;
			if (it_for_dummy_kumi != kumi_dummy_index.end()) {
				empty_index = *it_for_dummy_kumi;
				it_for_dummy_kumi++;
				kumi_dummy_index_used.insert(empty_index);
			} else {
				empty_index = temp_count.kumi_count + temp_offset;
				temp_offset++;
			}
			kumi[empty_index].atari_idx = aui->atariidx;
			kumi[empty_index].atari_idx2 = auk->atariidx;
			//temp++;
			if (temp_count.kumi_count + temp_offset >= max_count.kumi_count) {
				throw new GameError(KTROBO::FATAL_ERROR, " no noooo");
			}
		}
	}
	watches.stopWatch(1);
	temp_count.kumi_count += temp_offset;
	temp_count.igaidousi_count += temp_igaidousi_offset;
	temp_count.soreigai_count += temp_tosoreigai_offset;
	// kumi
	
	au_count += au_count_offset;
/*
multiset<int> au_dummy_index;
multiset<int> auinfo_dummy_index; // ダミーが入っているインデックスを保存する
multiset<int> kumi_dummy_index;
multiset<int> obbs_dummy_index;
multiset<int> autid_dummy_index;
multiset<int> autts_dummy_index;

	// 下の関数の計算の際に必要となる
multiset<int> au_dummy_index_used;
multiset<int> auinfo_dummy_index_used;
multiset<int> kumi_dummy_index_used;
multiset<int> obbs_dummy_index_used;
multiset<int> autid_dummy_index_used;
multiset<int> autts_dummy_index_used;

	// autid

	// autts
*/

	watches.startWatch(2);
	auinfo_dummy_index.clear();
	auinfo_dummy_index_used.clear();
	obbs_dummy_index.clear();
	obbs_dummy_index_used.clear();

	// 計算する
	set<int> au_dummy_index2;
	set<int> kumi_dummy_index2;
	set<int> autid_dummy_index2;
	set<int> autts_dummy_index2;
	set<int>::iterator it1 = au_dummy_index.begin();
	set<int>::iterator it2 = au_dummy_index_used.begin();

	watches.startWatch(3);
	std::set_difference(it1, au_dummy_index.end(),
		it2, au_dummy_index_used.end(), std::inserter<set<int> >(au_dummy_index2, au_dummy_index2.end()));
	watches.stopWatch(3);

	watches.startWatch(4);
	au_dummy_index.clear();
	au_dummy_index_used.clear();
	watches.stopWatch(4);

	it1 = au_dummy_index2.begin();
	it2 = au_dummy_index.begin();
	watches.startWatch(5);
	std::copy(it1,au_dummy_index2.end(), std::inserter<set<int> >(au_dummy_index, au_dummy_index.end()));
	watches.stopWatch(5);
	watches.startWatch(6);
	std::set_difference(kumi_dummy_index.begin(), kumi_dummy_index.end(),
		kumi_dummy_index_used.begin(), kumi_dummy_index_used.end(),  std::inserter<set<int> >(kumi_dummy_index2, kumi_dummy_index2.end()));

	watches.stopWatch(6);
	kumi_dummy_index.clear();
	kumi_dummy_index_used.clear();
	watches.startWatch(7);
	std::copy(kumi_dummy_index2.begin(),kumi_dummy_index2.end(),std::inserter<set<int> >(kumi_dummy_index, kumi_dummy_index.end()));
	watches.stopWatch(7);

	watches.startWatch(8);
	std::set_difference(autts_dummy_index.begin(), autts_dummy_index.end(),
		autts_dummy_index_used.begin(), autts_dummy_index_used.end(),  std::inserter<set<int> >(autts_dummy_index2, autts_dummy_index2.end()));

	autts_dummy_index.clear();
	autts_dummy_index_used.clear();
	std::copy(autts_dummy_index2.begin(),autts_dummy_index2.end(), std::inserter<set<int> >(autts_dummy_index, autts_dummy_index.end()));
	watches.stopWatch(8);

	watches.startWatch(9);
	std::set_difference(autid_dummy_index.begin(), autid_dummy_index.end(),
		autid_dummy_index_used.begin(), autid_dummy_index_used.end(),  std::inserter<set<int> >(autid_dummy_index2, autid_dummy_index2.end()));

	autid_dummy_index.clear();
	autid_dummy_index_used.clear();
	std::copy(autid_dummy_index2.begin(),autid_dummy_index2.end(),std::inserter<set<int> >(autid_dummy_index2, autid_dummy_index2.end()));
	watches.stopWatch(9);

	watches.stopWatch(2);
		

{
		WCHAR buff[512];
		char buf[128];
		char str[128];
		memset(buf,0,128);

		for (int i=0;i<10;i++) {
		memset(str,0,128);
		GamenGARAGE_partsParam::getSuutiChara((int)(watches.times[i]*100),str);
		strcat_s(buf,str);
		strcat_s(buf,",");
	//	GamenGARAGE_partsParam::getSuutiChara((int)(sizeof(AtariUnitAns)),str);
	//	strcat_s(buf,str);
		}
	
		//DebugTexts::instance()->setText(g,wcslen(buff),buff);
	//	if (watches.times[5] > 10) {
//			mylog::writelog(KTROBO::INFO, "setdayo::");
//			mylog::writelog(KTROBO::INFO, buf);
//		mylog::writelog(KTROBO::INFO, "\n");
	//	}
	}


}

void AtariHantei::maeCalcDummy(Graphics* g) {
	// maecalcdayoの後に呼ぶ
	

	if (state_changed_umeshunit.size() == 0) return;
	
	
	clearDummyInfo(true); // plus_count と　dummy_index_usedをクリアする
	ClockWatches watches;

	watches.startWatch(0);
	setStateChangedUMeshUnitToDummy();
	watches.stopWatch(0);
	
	
	watches.startWatch(1);
	if (isNeedMaeCalcWhenMaeCalcDummy()) {
		// plus_count の計算を行って max_count を超える場合はmaecalcさせる
		// dummyの計算は行わない
		watches.stopWatch(1);
		resetIsUnitUpdated();
		return;
	}
	watches.stopWatch(1);
	// isneedmaecalcwhenmaecalcdummy の計算によって求められたpluscount の情報と　statechangedumeshunitを使って
	// 
	watches.startWatch(2);
	setInfoOfStateChangedUMeshUnit();
	watches.stopWatch(2);

	



	clearDummyInfo(true);
	state_changed_umeshunit.clear(); // すべて適用したのでクリアする


















	

{
		WCHAR buff[512];
		char buf[128];
		char str[128];
		memset(buf,0,128);

		for (int i=0;i<3;i++) {
		memset(str,0,128);
		GamenGARAGE_partsParam::getSuutiChara((int)(watches.times[i]*100),str);
		strcat_s(buf,str);
		strcat_s(buf,",");
	//	GamenGARAGE_partsParam::getSuutiChara((int)(sizeof(AtariUnitAns)),str);
	//	strcat_s(buf,str);
		}
	
		//DebugTexts::instance()->setText(g,wcslen(buff),buff);
	//	if (watches.times[5] > 10) {
//			mylog::writelog(KTROBO::INFO, "testdayo::");
//			mylog::writelog(KTROBO::INFO, buf);
//		mylog::writelog(KTROBO::INFO, "\n");
	//	}
//	}



}




}