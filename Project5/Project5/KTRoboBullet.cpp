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
		atarihan->setUMesh(n);
		atarihan->setIsEnabled(h, true);
		h->setUMeshUnit(atarihan,AtariUnit::AtariType::ATARI_WAZA);
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
	atarihan->setIsEnabled(hantei,true);
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
		//mesh_i->setIsRender(true);

	}


}




void BulletController::atariShori(AtariHantei* hantei, MYMATRIX* view, float dsecond, int stamp) {



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
			bullets[i].Init(g,hantei, mi);
			bullets[i].atarihan->setXYZ(i,i,i);
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