#include "KTRoboWeapon.h"
#include "KTRoboBullet.h"
#include "KTRoboGame.h"
#include "KTRoboEffect.h"

using namespace KTROBO;

WeaponFire::WeaponFire(void)
{
	this->is_canfire = true;
	this->reloadtime = 3300;
	this->dtime = -1;
	
}


WeaponFire::~WeaponFire(void)
{

}


WeaponFireRifle::WeaponFireRifle(void)
{

}


WeaponFireRifle::~WeaponFireRifle(void)
{

}

void WeaponFire::update(float dsecond, int stamp) {
	if (is_canfire) {
	} else {
		dtime += dsecond;
		if (dtime > reloadtime) {
			dtime = -1;
			is_canfire = true;
		}
	}
}

void WeaponFire::fire(Robo* robo, RoboParts* parts, Graphics* g, Game* game, Scene* scene, BulletController* controller, AtariHantei* hantei, MySound* sound, MYMATRIX* robo_world, MYVECTOR3* vec, MYVECTOR3* pos, MeshBone* fire_bone) {

	if (is_canfire) {
		is_canfire = false;
		dtime = 0;
	}

}

void WeaponFireRifle::fire(Robo* robo, RoboParts* parts, Graphics* g, Game* game, Scene* scene, BulletController* controller, AtariHantei* hantei, MySound* sound, MYMATRIX* robo_world, MYVECTOR3* vec, MYVECTOR3* pos, MeshBone* fire_bone) {

	
	if (is_canfire) {

	
		Bullet* c = controller->getEmptyBullet();

		if (c) {
		} else {
			return;
		}
		
		c->setParam(robo,parts,pos,vec,robo_world);
		c->atarihan->meshs[0]->changeMesh(controller->dummy_mesh); // changemesh‚·‚éÛ‚Í@Œ³‚Ìobb_idx‚Æ‚©obb_use‚ª•Ï‚í‚ç‚È‚¢‚æ‚¤‚É‚µ‚È‚¯‚ê‚Î‚È‚ç‚È‚¢
//		c->atarihan->calcJyusinAndR(false);
		sound->playCue("se_maoudamashii_battle_gun01");
		game->weapon_effect_manager->makeWeaponEffect("bakuhatu", 800,true, robo_world, robo,fire_bone);
		WeaponFire::fire(robo, parts, g, game,scene, controller, hantei, sound, robo_world, vec,pos,fire_bone);
		c->fire(game, hantei);
		

		
	}
}

WeaponEffectManager::~WeaponEffectManager() {
	vector<WeaponEffectStruct*>::iterator it = effect_structs.begin();
	while(effect_structs.end() != it) {
		WeaponEffectStruct* ss = *it;
		delete ss;
		ss = 0;
		it++;
	}
	effect_structs.clear();
}


void WeaponEffectManager::update(float dt) {
	if (abs(dt) >200) return;

	vector<WeaponEffectStruct*>::iterator it = effect_structs.begin();
	CS::instance()->enter(CS_RENDERDATA_CS, "update effect impl");
	while(effect_structs.end() != it) {
		WeaponEffectStruct* ss = *it;

		if (ss->is_use) {
			if (ss->is_world_update && ss->world_update_base && ss->fire_bone) {
				MYMATRIX bullet_world;
				MeshBone* bone = ss->fire_bone;//raweapon->weapon->Bones[raweapon->weapon->BoneIndexes["fireBone"]];
				MyMatrixMultiply(bullet_world, bone->matrix_local, bone->combined_matrix);        //umesh_unit->meshs[0]->mesh->Bones[umesh_unit->meshs[0]->mesh->BoneIndexes["hidariteBone"]]->matrix_local,umesh_unit->meshs[0]->mesh->Bones[umesh_unit->meshs[0]->mesh->BoneIndexes["hidariteBone"]]->combined_matrix);
				MyMatrixMultiply(bullet_world,bullet_world,ss->world_update_base->atarihan->world);


				manager->setEffectImplWorld(ss->effect_impl_id,&bullet_world);
			}
			if (dt > 0) {
				ss->now_time += dt;
				if (ss->now_time > ss->alive_time) {
					manager->setEffectImplIsRender(ss->effect_impl_id,false);
					manager->lightdeleteEffectImpl(ss->effect_impl_id);
					ss->is_once_used = true;
					ss->is_use = false;
				}
			}
					/*

						void setEffectImplWorld(int effect_impl_id, MYMATRIX* world);
						void setEffectImplTime(int effect_impl_id, float time);
						void setEffectImplIsRender(int effect_impl_id, bool t);

					*/

		}
		it++;
	}

	CS::instance()->leave(CS_RENDERDATA_CS, "update effect impl");
}

void WeaponEffectManager::makeWeaponEffect(char* effect_name, float alive_time, bool is_world_update, MYMATRIX* world, AtariBase* world_update_base, MeshBone* fire_bone) {

	bool has_unused = false;
	WeaponEffectStruct* sss=0;
	vector<WeaponEffectStruct*>::iterator it = effect_structs.begin();
	while(effect_structs.end() != it) {
		WeaponEffectStruct* ss = *it;
		if (!ss->is_use && ss->is_once_used) {
			sss = ss;
			has_unused = true;
			break;
		}
		it++;
	}
	if (!has_unused) {
		sss = new WeaponEffectStruct();
	}
	sss->effect_id = manager->getEffectFromName(effect_name);
	sss->effect_impl_id = manager->getEffectImpl(sss->effect_id);
	manager->setEffectImplTime(sss->effect_impl_id,0);
	MYMATRIX worldd;
	MYVECTOR3 pos(0,0,0);
	MyVec3TransformCoord(pos,pos,*world);
	MyMatrixTranslation(worldd,pos.float3.x,pos.float3.y,pos.float3.z);


	manager->setEffectImplWorld(sss->effect_impl_id, &worldd);
	
	sss->is_world_update = is_world_update;
	if (is_world_update) {
		sss->world_update_base = world_update_base;
	} else {
		sss->world_update_base = false;
	}

	if (fire_bone) {
		sss->fire_bone = fire_bone;
	} else {
		sss->fire_bone = NULL;
	}

	sss->alive_time = alive_time;
	sss->now_time = 0;
	sss->is_use = true;
	
	manager->setEffectImplIsRender(sss->effect_impl_id,false);
	manager->setEffectImplIsStart(sss->effect_impl_id,true);
	if (!has_unused) {
		effect_structs.push_back(sss);
	}


}
