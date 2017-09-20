#include "KTRoboBullet.h"


using namespace KTROBO;
Bullet::~Bullet() {
	if (atarihan) {
		delete atarihan;
		atarihan = 0;
	}
}

void Bullet::Init(Graphics* g, AtariHantei* h, MeshInstanced* mi) {

	//if (!atarihan) {
		atarihan = new UMeshUnit();
		UMesh* n = new UMesh();
		n->is_bone_obbs_use[0] =true;
		n->bone_obbs[0].c = MYVECTOR3(0,0,0);//atarihan->x,atarihan->y,atarihan->z);
		n->bone_obbs[0].e = MYVECTOR3(1,1,1);
		n->bone_obbs[0].u[0] = MYVECTOR3(1,0,0);
		n->bone_obbs[0].u[1] = MYVECTOR3(0,1,0);
		n->bone_obbs[0].u[2] = MYVECTOR3(0,0,1);
		atarihan->setUMesh(n);
	
		//atarihan->calcJyusinAndR(true);




		h->setUMeshUnit(atarihan,AtariUnit::AtariType::ATARI_WAZA);
		atarihan->setIsEnabled(h, true);

		/*CS::instance()->enter(CS_RENDERDATA_CS, "un");
		atarihan->setIsEnabled(h,false);
		CS::instance()->leave(CS_RENDERDATA_CS, "un");
		*/


	//}

	this->mesh_i = mi;
}

void Bullet::setParam(AtariBase* robo, RoboParts* parts, MYVECTOR3* hassyapos, MYVECTOR3* vdayo, MYMATRIX* shoki_world) {
	this->robo = robo;
	this->robo_parts =parts;
	h_pos = *hassyapos;
	h_v = *vdayo; // 発射方向もかねる
	this->shoki_world = *shoki_world;
	


}


bool Bullet::fire(AtariHantei* hantei) {

	// エフェクトが実装されたらエフェクトも再生するようにする
	
	mesh_i->setIsRender(true);
	this->setIsUse(true);
	this->is_fired = true;
	atarihan->setV(&h_v);
	atarihan->setXYZ(h_pos.float3.x, h_pos.float3.y, h_pos.float3.z);
	dpos = MYVECTOR3(0,0,0);
	CS::instance()->enter(CS_RENDERDATA_CS, "un");
	//atarihan->setIsEnabled(hantei,true);
	// もともとtrueになっているので問題ない
	CS::instance()->leave(CS_RENDERDATA_CS, "un");
	return true;

}


void Bullet::atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp) {



}


void Bullet::byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp) {



}


void Bullet::update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp) {
	if(mesh_i && is_fired && is_use){
		
		MYMATRIX world = shoki_world;
		MYMATRIX dpos_wor;

		dpos = dpos + atarihan->v* dsecond;
		MyMatrixTranslation(dpos_wor, dpos.float3.x,dpos.float3.y,dpos.float3.z);
		MyMatrixMultiply(world,world,dpos_wor);
		// rotx roty rotz を計算する必要がある // setworldを作ることで対応 bool calcworldをつけるようにした
		//atarihan->setXYZ(rand() %1000/10.0,rand() % 1000/10.0,rand() %10000/10.0);//test,test2,0);
		atarihan->setWorld(&world);
		atarihan->setDT(dsecond);
		atarihan->calcJyusinAndR(false);
		mesh_i->setWorld(&world);
		mesh_i->setIsRender(true);

	} else if(mesh_i) {
	
	}

}




void BulletController::atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp) {
	AtariUnitAnsKWSK kuwasiku[2048];
	
	int temp = hantei->getAnsWaza(kuwasiku,2048);
	for (int i=0;i<temp;i++) {
		if (kuwasiku[i].aite_umesh && kuwasiku[i].my_umesh) {
			bullets[umesh_id_to_bullet_indexs[kuwasiku[i].my_umesh->getUMESHID()]].mesh_i->setIsRender(false);
		}



		//if (this->umesh_id_to_bullet_indexs.find(kuwasiku[i].my_umesh->getUMESHID()) != umesh_id_to_bullet_indexs.end()) {
		//	bullets[umesh_id_to_bullet_indexs[kuwasiku[i].my_umesh->getUMESHID()]].mesh_i->setIsRender(false);
		//}
		//if (this->umesh_id_to_bullet_indexs.find(kuwasiku[i].aite_umesh->getUMESHID()) != umesh_id_to_bullet_indexs.end()) {
		//	bullets[umesh_id_to_bullet_indexs[kuwasiku[i].my_umesh->getUMESHID()]].mesh_i->setIsRender(false);
		//}
	}

}

void BulletController::byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj, float dsecond, int stamp) {

	if (mis) {
		mis->render(g);
	}


}


void BulletController::update(Graphics* g, AtariHantei* hantei, float dsecond, int stamp) {
	if (bullets) {
	for (int i=0;i<KTROBO_BULLET_CONTROLLER_BULLET_NUM;i++) {
		bullets[i].update(g,hantei, dsecond, stamp);
	}
	}
}


BulletController::BulletController() {
	this->hantei = 0;
	this->bullets = 0;
	mis = 0;
	dummy_mesh = 0;
}

BulletController::~BulletController() {
}
void BulletController::Init(Graphics* g, AtariHantei* hantei, MyTextureLoader* loader) {

	this->hantei = hantei;

	if (!dummy_mesh) {
		dummy_mesh = new Mesh();
		dummy_mesh->readMesh(g, KTROBO_BULLET_MESH_DUMMY_FILENAME,loader);
		dummy_mesh->readAnime(KTROBO_BULLET_MESH_ANIME_DUMMY_FILENAME);
		dummy_mesh->animate(0,true);
	}

	if (!mis) {
		mis = new MeshInstanceds(g, loader);
	}

	if (hantei && mis) {
		MYMATRIX idenmat;
		MyMatrixIdentity(idenmat);
		bullets = new Bullet[KTROBO_BULLET_CONTROLLER_BULLET_NUM];
		for (int i=0;i<KTROBO_BULLET_CONTROLLER_BULLET_NUM; i++) {
			// mesh_instanceds を入れる
			
			MeshInstanced* mi = mis->makeInstanced(dummy_mesh,dummy_mesh,NULL,KTROBO_MESH_BONE_NULL,false,&idenmat);
			//この時点でhanteiに追加される
			bullets[i].Init(g,hantei, mi);
			//bullets[i].atarihan->setXYZ(i,i,i);

			bullets[i].atarihan->setXYZ(0, + 10, rand()%1000000/100-50);
			bullets[i].atarihan->calcJyusinAndR(true);
			// calcjyusinrでは今のところ計算されないので
			bullets[i].atarihan->meshs[0]->bone_obbs[0].c = MYVECTOR3(bullets[i].atarihan->x, bullets[i].atarihan->y, bullets[i].atarihan->z);
			bullets[i].atarihan->jyusin = bullets[i].atarihan->meshs[0]->bone_obbs[0].c;
			bullets[i].atarihan->r = 1;
			MYMATRIX world;
			float x = bullets[i].atarihan->x;
			float y = bullets[i].atarihan->y;
			float z = bullets[i].atarihan->z;
			MyMatrixTranslation(world,x,y,z);
			mi->setWorld(&world);
			mi->setIsRender(true);
			umesh_id_to_bullet_indexs.insert(std::pair<int,int>(bullets[i].atarihan->meshs[0]->getUMESHID(),i));
		}
	}

	




}

Bullet* BulletController::getEmptyBullet() {
	if (!bullets) return NULL;

	for (int i=0;i<KTROBO_BULLET_CONTROLLER_BULLET_NUM;i++) {
		if (!bullets[i].getIsUse()) {
			return &bullets[i];
		}
	}
	return NULL;

}


void BulletController::Release() {
	if (hantei) {
		hantei->clearUMeshUnits();
	}
	if (bullets) {
		delete[] bullets;
		bullets = 0;
	}

	if (mis) {
		mis->Release();
		delete mis;
		mis = 0;
	}

	if (dummy_mesh) {
		dummy_mesh->Release();
		delete dummy_mesh;
		dummy_mesh = 0;
	}
}


void BulletController::calcUpdate(Graphics* g) {
	// mis用
	if (mis) {

		mis->loadAnimeMatrixBasisToTexture(g);
		mis->loadMatrixLocalToTexture(g);
		mis->calcCombinedMatrixToTexture(g);
		mis->loadColorToTexture(g);

	}

}