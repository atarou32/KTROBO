#include "KTRoboRobo.h"
#include "KTRoboGameError.h"
#include "KTRoboLockOnSystem.h"
#include "KTRoboArmPositioner.h"
#include "KTRoboWeapon.h"
#include "KTRoboGame.h"
#include "KTRoboTexture.h"
#include "KTRoboWeapon.h"

using namespace KTROBO;
Robo::Robo(void)
{
	head = 0;
	body = 0;
	leg = 0;
	arm = 0;
	raweapon = 0;
	laweapon = 0;
	rsweapon = 0;
	lsweapon = 0;
	iweapon = 0;
	fcs = 0;
	booster = 0;
	engine = 0;
	atarihan = 0;
//	MyMatrixIdentity(world);
	atari_leg = 0;
	atari_core = 0;
	atari_head = 0;
	atari_head2 = 0;
	atari_head3 = 0;
	atari_larm = 0;
	atari_rarm = 0;
	move_state = &movestop;
	setti_state = &kuutyuu;
	booster_state = &boostoff;
	settenjyou_state = &kuutyuu;
	setkabe_state = &kuutyuu;
	moveturn_state = &movestop;
	resetCount();
	ap = 0;
	aphelper = 0;
	ap_hidari = 0;
	aphelper_hidari = 0;
	apinfo = 0;
	target = MYVECTOR3(0,0,0);
	kabe_housen = MYVECTOR3(0,0,1);
	is_fireraweapon = false;
	jump_f_z = 0;
	jump_f_z_kabe = 0;
	updown_muki = 0;
	screen_height = 1;
	MyMatrixIdentity(world_without_rotx);
	pressed_space_count = 0;
	kuutyuu_count = 0;
	setti_count=0;
	setkabe_count = 0;
}


Robo::~Robo(void)
{
}

void Robo::upDownMuki(float mouse_y, float dmouse_y) {

	float dy;
	dy = (mouse_y - screen_height/2);
	if (abs(dmouse_y) < 700 && abs(dmouse_y) > 1) {
			dy = dmouse_y/10;
			updown_muki += dy/10.0f;
			if (updown_muki > 1.57) {
				updown_muki = 1.57;
			}
			if (updown_muki < -1.57) {
				updown_muki = -1.57;
			}
		} else {

	if ((abs(dy) >120) && (abs(dy) < 800) && (abs(mouse_y) > 1)) {
	updown_muki += dy/10000.0f;
	if (updown_muki > 1.57) {
		updown_muki = 1.57;
	}
	if (updown_muki < -1.57) {
		updown_muki = -1.57;
	}
	}else {
		



		if (abs(dy) < 50) {
		updown_muki = updown_muki/1.1f;
		}
		}
	}

	atarihan->setROTXYZ(updown_muki,atarihan->roty, atarihan->rotz);
	// legで打ち消す
	MYMATRIX mat;
	MyMatrixRotationX(mat,updown_muki);
	atari_leg->setMatrixKakeru(&mat);
	atari_leg->setInfo();
	atarihan->calcJyusinAndR();
	
}

void Robo::byouga(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {



	if (head) {
		if (head->head) {
			MyMatrixScaling(head->head->rootbone_matrix_local_kakeru,3,3,3);
			static int test=0;
			

			head->head->animate(test,true);
	//		head->head->drawWithObbs(g,world,view,proj);
		}
		if (head->head2) {
			MyMatrixScaling(head->head2->rootbone_matrix_local_kakeru,3,3,3);
			static int test=0;
			

			head->head2->animate(test,true);
	//		head->head2->drawWithObbs(g,world,view,proj);
		}
		if (head->head3) {
			MyMatrixScaling(head->head3->rootbone_matrix_local_kakeru,3,3,3);
			static int test=0;
			

			head->head3->animate(test,true);
	//		head->head3->drawWithObbs(g,world,view,proj);
		}

	}
	
	if (body) {
		if(body->body) {

			MyMatrixScaling(body->body->rootbone_matrix_local_kakeru,2,2,2);
			static int test=0;
			
		
			body->body->animate(test,true);
		//	body->body->drawWithObbs(g,&world,view,proj);
		}
	}

	if (leg) {
		if (leg->leg) {
			MyMatrixScaling(leg->leg->rootbone_matrix_local_kakeru,0.75,0.75,0.75);
			upDownMuki(0,0);
			static int test=0;
			test++;
				
		
			//leg->leg->animate(test,true);
		//	leg->leg->drawWithObbs(g,&world,view,proj);
		}
	}
	
	if (arm) {
		if (arm->larm) {
			MyMatrixScaling(arm->larm->rootbone_matrix_local_kakeru,0.91,0.91,0.91);
			static int test=0;
			test++;
				
		
		//	arm->larm->animate(test,true);

		//	arm->larm->drawWithObbs(g,&world,view,proj);
		}
		if (arm->rarm) {
			MyMatrixScaling(arm->rarm->rootbone_matrix_local_kakeru,0.91,0.91,0.91);
			static int test=0;
			test++;

			MYMATRIX wor;
			MYMATRIX wor2;
			MYMATRIX wor3;
		//	MyMatrixRotationZ(wor, 1.57f);//3.141592f);
		//	MyMatrixRotationX(wor2, 3.141592f);
			MyMatrixRotationY(wor3, 0);
			wor3._11 = -1;
			MyMatrixMultiply(arm->rarm->rootbone_matrix_local_kakeru, wor3, arm->rarm->rootbone_matrix_local_kakeru);
		
		//	MyMatrixMultiply(arm->rarm->rootbone_matrix_local_kakeru, wor2, arm->rarm->rootbone_matrix_local_kakeru);
		//	MyMatrixMultiply(arm->rarm->rootbone_matrix_local_kakeru, wor, arm->rarm->rootbone_matrix_local_kakeru);
				
		
		//	arm->rarm->animate(test,true);

		//	arm->rarm->drawWithObbs(g,&world,view,proj);
		}



	}
	


	atarihan->draw(g,view,proj,0,NULL,NULL,true,false,true,true);
	LockOnSystem sys;

	sys.byougaStudyPoint(g, &this->atarihan->world, view, 10,100,6,10,6,10,300,300,500);
	//sys.byougaBigStudyPoint(120,g, &this->atarihan->world, view, 10,100,10,20,10,20,300,300,500);
	aphelper->byougaAP8(g,view);
	if (raweapon) {
		raweapon->weapon->draw(g, &atarihan->world,view,proj);
	}

	if (laweapon) {
		laweapon->weapon->draw(g, &atarihan->world,view,proj);
	}

	if (booster) {
		booster->mesh->draw(g, &atarihan->world, view, proj);
	}

	if (rsweapon) {
		rsweapon->weapon->draw(g, &atarihan->world, view,proj);
	}
	if (lsweapon) {
		lsweapon->weapon->draw(g,&atarihan->world, view, proj);
	}

}

void Robo::remakeUMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (atari_core) {
		atari_core = 0;
	}
	if (atari_head) {
		atari_head = 0;
	}
	if (atari_head2) {
		atari_head2 = 0;
	}
	if (atari_head3) {
		atari_head3 = 0;
	}
	if (atari_larm) {
		atari_larm = 0;
	}
	if (atari_rarm) {
		atari_rarm = 0;
	}
	if (atari_leg) {
		atari_leg = 0;
	}

	if(atarihan) {
		delete atarihan;
		atarihan = 0;
	}

	atarihan = new UMeshUnit();
	// atarihanのumeshをつくる
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (body->body) {
	UMesh* um_core = new UMesh(g,body->body->filename, tex_loader,
		body->body,true, &wor,NULL,KTROBO_MESH_BONE_NULL,false);
	atarihan->setUMesh(um_core);
	atari_core = um_core;
	}
	if (leg->leg) {
	UMesh* um_leg = new UMesh(g, leg->leg->filename, tex_loader,
		leg->leg,false, &wor, body->body->Bones[body->body->BoneIndexes["legJointBone"]],body->body->BoneIndexes["legJointBone"],false);
	atarihan->setUMesh(um_leg);
	atari_leg = um_leg;
	}
	if (arm->rarm) {
	UMesh* um_rarm = new UMesh(g,arm->rarm->filename, tex_loader,
		arm->rarm, false, &wor,  body->body->Bones[body->body->BoneIndexes["rightArmJointBone"]],body->body->BoneIndexes["rightArmJointBone"],false);
	atarihan->setUMesh(um_rarm);

			MyMatrixScaling(arm->rarm->rootbone_matrix_local_kakeru,1.1,1.1,1.1);
			static int test=0;
			test++;

			MYMATRIX wor;
			MYMATRIX wor2;
			MYMATRIX wor3;
		//	MyMatrixRotationZ(wor, 3.141592f);
		//	MyMatrixRotationX(wor2, 3.141592f);
			MyMatrixRotationY(wor3, 0);//-3.141592f);
			wor3._11 = -1;
			MyMatrixMultiply(arm->rarm->rootbone_matrix_local_kakeru, wor3, arm->rarm->rootbone_matrix_local_kakeru);
	//		arm->rarm->animate(40,true);
			atari_rarm = um_rarm;

	}
	if (arm->larm) {
	UMesh* um_larm = new UMesh(g, arm->larm->filename, tex_loader,
		arm->larm, false, &wor,  body->body->Bones[body->body->BoneIndexes["leftArmJointBone"]],body->body->BoneIndexes["leftArmJointBone"],false);
	atarihan->setUMesh(um_larm);
		MyMatrixScaling(arm->larm->rootbone_matrix_local_kakeru,1.1,1.1,1.1);
	//	arm->larm->animate(40,true);
		atari_larm = um_larm;
	}

	if (head->head) {
	UMesh* um_head = new UMesh(g, head->head->filename, tex_loader,
		head->head, false, &wor, body->body->Bones[body->body->BoneIndexes["headJointBone"]],body->body->BoneIndexes["headJointBone"],false);
	atarihan->setUMesh(um_head);
	atari_head = um_head;
	}

	if (head->head2) {
	UMesh* um_head2 = new UMesh(g, head->head2->filename, tex_loader,
		head->head2, false, &wor, body->body->Bones[body->body->BoneIndexes["headJointBone"]],body->body->BoneIndexes["headJointBone"],false);
	atarihan->setUMesh(um_head2);
	atari_head2  = um_head2;
	}

	if (head->head3) {
	UMesh* um_head3 = new UMesh(g, head->head3->filename, tex_loader,
		head->head3, false, &wor, body->body->Bones[body->body->BoneIndexes["headJointBone"]],body->body->BoneIndexes["headJointBone"],false);
	atarihan->setUMesh(um_head3);
	atari_head3 = um_head3;
	}

	float frames[16];
	bool matrs[16];
	for (int i=0;i<16;i++) {
		frames[i] = 0;
		matrs[i] = true;
	}

	atarihan->calcAnimeFrame(atarihan->meshs.size(),frames,matrs);

	atarihan->setDT(0);
	atarihan->setROTXYZ(0,0,0);
	atarihan->setSCALEXYZ(1,1,1);
	atarihan->setV(&MYVECTOR3(0,0,0));





	//atarihan->setXYZ(5,3,5.5);
	atarihan->setXYZ(0,0,0);
	atarihan->calcJyusinAndR();

	if (arm->larm) {
		arm->larm->animate(40,true);
	}
	if (arm->rarm) {
		arm->rarm->animate(40,true);
	}

}

void Robo::atarishori(Graphics* g, MYMATRIX* view,  AtariHantei* hantei, float dt, int stamp) {

	AtariUnitAnsKWSK ans[512];
	static float vdayo =0;
	static int iunko = 9;
	static bool bunko = false;
	bool setdayo = false;
	bool setkabedayo = false;
	move_state->exec(g, this, dt, stamp);
	moveturn_state->exec(g,this,dt,stamp);
	setti_state->exec(g,this,dt,stamp);
	booster_state->exec(g,this,dt,stamp);
	//setkabe_state = &kuutyuu;
	settenjyou_state = &kuutyuu;

	int atari_num = hantei->getAns(ans,atarihan,atari_leg,512);
	if ((atari_num > 0)) {
		for (int i=0;i<atari_num;i++) {
			if (ans[i].aite_type == AtariUnit::AtariType::ATARI_TIKEI) {
				if ((ans[i].ans->atari_idx == atari_leg->getUMESHID()) || (ans[i].ans->atari_idx2 == atari_leg->getUMESHID())) {
				if (atari_leg && atari_leg->mesh && ((ans[i].ans->obbidx == atari_leg->bone_obbs_idx[atari_leg->mesh->BoneIndexes["rightLegDownBone"]]) ||
					(ans[i].ans->obbidx2 == atari_leg->bone_obbs_idx[atari_leg->mesh->BoneIndexes["rightLegDownBone"]]))) {
					OBB* ob = &atari_leg->bone_obbs[atari_leg->mesh->BoneIndexes["rightLegDownBone"]];
					MYVECTOR3 a;
					MyVec3Subtract(a,ob->c, ans[i].ans->kouten_jyusin);
					float f = min(MyVec3Length(atarihan->v)*dt,abs(MyVec3Length(a) - abs(MyVec3Dot(ob->u[0],ans[i].ans->kouten_housen) * ob->e[0])
						- abs(MyVec3Dot(ob->u[1], ans[i].ans->kouten_housen)) * ob->e[1]
					- abs(MyVec3Dot(ob->u[2], ans[i].ans->kouten_housen)) * ob->e[2]));
					f = min ( f, 0.01f);
					if (ans[i].ans->kouten_housen.float3.z > 0) {
					atarihan->setXYZD(atarihan->x, atarihan->y, atarihan->z + abs(ans[i].ans->kouten_housen.float3.z) * 1.02f*f*f, 0.2);
					setti_state = &setti;
					//atarihan->setV(&MYVECTOR3(0,0,atarihan->v.float3.z));
					/*if ((move_state != &movestop) && move_state->isJump()) {
					move_state->leave(this,&movestop, move_state);
					movestop.enter(this, &movestop, move_state);
					move_state = &movestop;
					}*/
					vdayo = 0;
					setdayo = true;
					}
		
					
					
				}
				if (strpos(leg->data->getData("name")->string_data, "KTLTK") == -1) { // タンクじゃなければ
				if (atari_leg && atari_leg->mesh && ((ans[i].ans->obbidx == atari_leg->bone_obbs_idx[atari_leg->mesh->BoneIndexes["leftLegDownBone"]]) ||
					(ans[i].ans->obbidx2 == atari_leg->bone_obbs_idx[atari_leg->mesh->BoneIndexes["leftLegDownBone"]]))) {
			
					
					OBB* ob = &atari_leg->bone_obbs[atari_leg->mesh->BoneIndexes["leftLegDownBone"]];
					MYVECTOR3 a;
					MyVec3Subtract(a,ob->c, ans[i].ans->kouten_jyusin);
					float f = min(MyVec3Length(atarihan->v)*dt,abs(MyVec3Length(a) - abs(MyVec3Dot(ob->u[0],ans[i].ans->kouten_housen) * ob->e[0])
						- abs(MyVec3Dot(ob->u[1], ans[i].ans->kouten_housen)) * ob->e[1]
					- abs(MyVec3Dot(ob->u[2], ans[i].ans->kouten_housen)) * ob->e[2]));
					f = min ( f, 0.01f);
					if (ans[i].ans->kouten_housen.float3.z > 0) {
					atarihan->setXYZD(atarihan->x, atarihan->y, atarihan->z + abs(ans[i].ans->kouten_housen.float3.z) * 1.02f*f*f,0.2);
					/*if ((move_state != &movestop) && move_state->isJump()) {
					move_state->leave(this,&movestop, move_state);
					movestop.enter(this, &movestop, move_state);
					move_state = &movestop;
					}*/
					setti_state = &setti;
					vdayo = 0;
					//atarihan->setV(&MYVECTOR3(0,0,atarihan->v.float3.z));
					setdayo = true;
					}

				
					
					
				}
				}
				}
				
			}
		}
	}

	atari_num = hantei->getAns(ans,atarihan,NULL,512);
	if ((atari_num > 0)) {
		for (int i=0;i<atari_num;i++) {

			OBB rec;
			rec.c = ans[i].ans->kouten_jyusin;
			MYMATRIX iden;
			MyMatrixIdentity(iden);
			g->drawOBB(g,0xFFFF0000,&iden,view,g->getProj(),&rec);
			if (ans[i].aite_type == AtariUnit::AtariType::ATARI_TIKEI) {
				MYVECTOR3 sitadayo(0,0,-1);
				MYVECTOR3 uedayo(0,0,1);
				
				float sitadot = MyVec3Dot(sitadayo,ans[i].ans->kouten_housen);
				float uedot = MyVec3Dot(uedayo, ans[i].ans->kouten_housen);
				UMesh* temp_atari = ans[i].my_umesh;
				for (int k=0;k< KTROBO_MESH_BONE_MAX; k++) {
					if (temp_atari->is_bone_obbs_use[k]) {
						bool is_break = false;
						OBB* ob = &atari_leg->bone_obbs[atari_leg->mesh->BoneIndexes["rightLegDownBone"]];
						MYVECTOR3 a;
						MyVec3Subtract(a,ob->c, ans[i].ans->kouten_jyusin);

	
						OBB* ob2 = &temp_atari->bone_obbs[k];
						MYVECTOR3 a2;
						MyVec3Subtract(a2,ob2->c, ans[i].ans->kouten_jyusin);
						OBB obkaku = *ob2;
						obkaku.e = obkaku.e * 1.5;

						// 交点の重心が　ob2 の中にない場合はスキップする
						if (!majiwaruPointOBB(&ans[i].ans->kouten_jyusin, &obkaku)) {
							continue;
						}



//						float f = min(0.01f,abs(MyVec3Length(a)));
						//- abs(MyVec3Dot(ob->u[0],ans[i].ans->kouten_housen) * ob->e[0])
						//	- abs(MyVec3Dot(ob->u[1], ans[i].ans->kouten_housen)) * ob->e[1]
						//	- abs(MyVec3Dot(ob->u[2], ans[i].ans->kouten_housen)) * ob->e[2]));
						float te;
						
						MYVECTOR3 gen ( atarihan->x, atarihan->y , atarihan->z);
						MYVECTOR3 gen2;
						MyVec3Subtract(gen2, gen, ans[i].ans->kouten_jyusin);
						te = MyVec3Dot(gen2, ans[i].ans->kouten_housen);
						float f = max(0,abs(atarihan->r/2-abs(te)));
						f = max ( f, 0.4);

						float f2 = min(MyVec3Length(atarihan->v)*dt,abs(MyVec3Length(a2) - abs(MyVec3Dot(ob2->u[0],ans[i].ans->kouten_housen) * ob2->e[0])
							- abs(MyVec3Dot(ob2->u[1], ans[i].ans->kouten_housen)) * ob2->e[1]
							- abs(MyVec3Dot(ob2->u[2], ans[i].ans->kouten_housen)) * ob2->e[2]));
							f2 = max ( f2, 0.5f);
							f2 = min (f2,1.0f);
							if (!move_state->isJumpKABE()) {
								if (MyVec3Length(atarihan->v) < 1) {
									float speed = 0.1;
									f2 = min (f2, 0.10f +speed);
								} else {
									f2 = min (f2, 0.8f);
								}
							} else {
								f2 = max(f2, 0.4f);
							}

							if (move_state->isJump()) {
								f2 = max(f2, 0.4f);
							}
							float myspeed = 0.2f;
							if (getPressedSpaceCount() > 30) {
								myspeed = 0.050f;
							}

						if ((sitadot < 0.8f) || (uedot <0.8f)) {// &&(MyVec3Dot(a2, ans[i].ans->kouten_housen) > -0.85f)) {
							// setkabe
							if (setdayo) {
								if (uedot > 0.7f) continue;
								if (sitadot > 0.7f) continue;
								
							}

							OBB obda = *ob2;
							obda.e = MYVECTOR3(0.5f,0.5f,0.5f);
							g->drawOBB(g,0xFF00FF00,&iden,view,g->getProj(),&obda);
							atarihan->setXYZD(atarihan->x + ans[i].ans->kouten_housen.float3.x * 1.02f*f2*f2,
								atarihan->y + ans[i].ans->kouten_housen.float3.y * 1.02f*f2*f2,
								atarihan->z + ans[i].ans->kouten_housen.float3.z * 1.02f*f2*f2,myspeed);
						
							setkabe_state = &setkabe;
							setkabedayo = true;

							if ((uedot < 0.7) && (sitadot < 0.7)) {
							kabe_housen = ans[i].ans->kouten_housen;
							}
							is_break = true;
							//vdayo = 0;
						}

						if ((sitadot > 0.7f) && MyVec3Dot(a2, ans[i].ans->kouten_housen) > 0.15f) {
							atarihan->setXYZD(atarihan->x, atarihan->y, atarihan->z - abs(ans[i].ans->kouten_housen.float3.z) * 1.02f*f2*f2,myspeed);
							settenjyou_state = &settenjyou;
							vdayo = 0;
						}

						if (  MyVec3Dot(a, ans[i].ans->kouten_housen) < -0.3f) {
							if (!setdayo) {
							atarihan->setXYZD(atarihan->x, atarihan->y, atarihan->z + abs(ans[i].ans->kouten_housen.float3.z) * 1.02f*f*f,myspeed);
						//	setti_state = &setti;
							vdayo = 0;
							}
						}

						if (is_break) {
							break;
						}

			
					}

				}
				
				
			}
		}
	}


 atari_num = hantei->getAns(ans,atarihan,NULL,512);
	if ((atari_num > 0)) {
		for (int i=0;i<atari_num;i++) {
			if ((ans[i].aite_type == AtariUnit::AtariType::ATARI_CHARA) || (ans[i].aite_type == AtariUnit::AtariType::ATARI_OBJECT)) {
				UMesh* temp_atari = ans[i].my_umesh;
				for (int k=0;k< KTROBO_MESH_BONE_MAX; k++) {
					if (temp_atari->is_bone_obbs_use[k]) {

						OBB* ob = &temp_atari->bone_obbs[k];
						// 交点の重心が　ob2 の中にない場合はスキップする
						if (!majiwaruPointOBB(&ans[i].ans->kouten_jyusin, ob)) {
							continue;
						}

						// 自分の分だけ移動させる
						MYVECTOR3 aite_pos = MYVECTOR3(ans[i].aite->x, ans[i].aite->y, ans[i].aite->z);
						float aite_r = ans[i].aite->r;
						MYVECTOR3 jibun_pos = MYVECTOR3(atarihan->x, atarihan->y, atarihan->z);
						MYVECTOR3 a;
						MyVec3Subtract(a,jibun_pos, aite_pos);
						MyVec3Normalize(a,a);
						float f = min(MyVec3Length(atarihan->v)*dt,abs(MyVec3Length(a) - abs(MyVec3Dot(ob->u[0],ans[i].ans->kouten_housen) * ob->e[0])
							- abs(MyVec3Dot(ob->u[1], ans[i].ans->kouten_housen)) * ob->e[1]
							- abs(MyVec3Dot(ob->u[2], ans[i].ans->kouten_housen)) * ob->e[2]));
						f = max ( f, 0.1f);
						f = min ( f, 0.5f);
						

						if (setti_state == &setti) {
							// 設置しているときはｚ成分を無くす
							a.float3.z = 0;
						}

						atarihan->setXYZ(atarihan->x + a.float3.x * 1.0f*f*f,
							atarihan->y + a.float3.y * 1.0f*f*f,
							atarihan->z + a.float3.z * 1.0f*f*f);
					}

				}
			}
		}
	}


	
	if (setkabedayo == false) {

		// 一定のカウント数後　空中に戻す
		if (setkabe_count > this->KTROBO_ROBO_JUMPKABE_COUNT) {
			setkabe_state = &kuutyuu;
			resetSetKabe();
		} else {

			if (move_state->isJumpKABE()) {
				
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				incSetKabe();
				//incSetKabe();
				//incSetKabe();
				//incSetKabe();
			} else {

				if (setkabe_state != &kuutyuu) {

					incSetKabe();
				}
				
			}
		}
	} else {
		// ジャンプ中のときは０にしない
		if (move_state->isJump() || move_state->isJumpKABE()) {
			if (setkabe_state != &kuutyuu) {
				incSetKabe();
			}
		}else {
			atarihan->setV(&MYVECTOR3(0, 0,vdayo));
			if (move_state->isJumpKABE() || move_state->isJump()) {
				incSetKabe();
			}
		}
	}


	if (move_state->isJumpKABE()){
				
				if (dt < 50) {
				MYVECTOR3 xyz = atarihan->v * dt;
			//	atarihan->v = atarihan->v + MYVECTOR3(0,0,atarihan->v.float3.z * dt);

				atarihan->setXYZ(atarihan->x+xyz.float3.x , atarihan->y + xyz.float3.y , atarihan->z + xyz.float3.z);
				}
		 
	}

	if (move_state->isBoosterHi()) {
		if (setdayo == false) {
			// 空中にいる
		} else {
			// 接地している
		}

		if (setkabedayo == true) {
			// 壁に接している
		}


			if (dt < 50) {
				MYVECTOR3 xyz = atarihan->v * dt;
				atarihan->setXYZ(atarihan->x+xyz.float3.x , atarihan->y + xyz.float3.y , atarihan->z + xyz.float3.z);
			}


	} else if (setdayo == false) {

		// 空中にいる
		if (dt <50) {
		vdayo = atarihan->v.float3.z;
		if (booster_state != &boostoff) {
			// ブースタがなってるので
			atarihan->setV(&MYVECTOR3(atarihan->v.float3.x, atarihan->v.float3.y, -0.0002f));
			vdayo = atarihan->v.float3.z;
		} else {
			vdayo = vdayo - 0.000098*dt;
		
		}



		float xx = vdayo * dt;
		atarihan->setDT(dt);
		if (booster_state == &boostontaiki) {
			atarihan->setXYZ(atarihan->x + atarihan->v.float3.x * dt, atarihan->y + atarihan->v.float3.y * dt, atarihan->z + xx);
		} else {
			atarihan->setXYZ(atarihan->x, atarihan->y, atarihan->z + xx);
			atarihan->setV(&MYVECTOR3(atarihan->v.float3.x,atarihan->v.float3.y,vdayo));
		}
	
		setti_state = &kuutyuu;
		incKuutyuu();
		}


	} else {
		incSetti();


		if ((move_state == &movestop)) {
			
			if (dt < 50) {
				MYVECTOR3 xyz = atarihan->v * dt;
				atarihan->setXYZ(atarihan->x+xyz.float3.x , atarihan->y + xyz.float3.y , atarihan->z + xyz.float3.z);
			}
			atarihan->setDT(dt);
			if (booster_state == &boostontaiki) {

				atarihan->setV(&MYVECTOR3(atarihan->v.float3.x, atarihan->v.float3.y,vdayo));
			} else {
				atarihan->setV(&MYVECTOR3(0,0,vdayo));
			}
			
		} else if (move_state->isJump()){
			atarihan->setDT(dt);
			if (kuutyuu_count == 0) {
				if (dt < 50) {
				MYVECTOR3 xyz = atarihan->v * dt;
				atarihan->setXYZ(atarihan->x+xyz.float3.x , atarihan->y + xyz.float3.y , atarihan->z + xyz.float3.z);
				}

			} else {
				atarihan->setV(&MYVECTOR3(0,0,vdayo));
			}
		} else {
			
			if (dt < 50) {
				MYVECTOR3 xyz = atarihan->v * dt;
				atarihan->setXYZ(atarihan->x+xyz.float3.x , atarihan->y + xyz.float3.y , atarihan->z + xyz.float3.z);
			}
			if (booster_state != &boostontaiki) {
				atarihan->setV(&MYVECTOR3(0, 0,vdayo));
			}else {
				atarihan->setV(&MYVECTOR3(atarihan->v.float3.x, atarihan->v.float3.y,vdayo));
			}
			atarihan->setDT(dt);
		}
	}
	anime_loop_leg.animateTime(dt);
	anime_loop_leg.animate(atari_leg, true);
	

	if (ap) {
		MYVECTOR3 moku(0,0,0);
		MYMATRIX trans_world;
		MyMatrixInverse(trans_world, NULL, atarihan->world);
		MyVec3TransformCoord(moku,moku,trans_world);
//	
//		ap->positionArm2(0,0.5, meshrobo, &D3DXVECTOR3(40*cos(test),-80+40*sin(test),80+40*sin(test)), true);
//		ap->positionArm2(3.14/600,0.5,meshrobo, &D3DXVECTOR3(40*cos(test),-80+40*sin(test),80+40*sin(test)), false);
		static float test = 0;
		test += 0.1f;

		if (this->arm ) {
			
			if (this->arm->larm) {
//			this->arm->larm->animate(0,true);
//			
//		if (	ap->positionArm2(0.001,0.5, this, &moku/*
//											   MYVECTOR3(40*cos(test),-80+40*sin(test),80+40*sin(test)
//												   */, false)) {
//			arm->larm->animate(0,false);
//		}
//		
		
					MYMATRIX temp;
			MyMatrixMultiply(temp, this->atarihan->world, *view);
		//	if (ap->positionArm(g,&temp, 0,this, &moku, false)) {
		//		arm->larm->animate(0,false);
		//	}

			MYVECTOR3 te = MYVECTOR3(3,3,3);//1*cos(test),-2+4*sin(test)
												//	 ,3+4*sin(test));

			{
			MYMATRIX temp;
			MyMatrixInverse(temp,NULL,atarihan->world);
			MyVec3TransformCoord(te,te,temp);
			
		//	arm->larm->animate(40,true);
		//	if (ap->positionArm3(g,&temp,this, &te/*MYVECTOR3(40*cos(test),-80+40*sin(test)
						//							 ,80+40*sin(test))*/, false)) {
		//	arm->larm->animate(40,false);//false);
			{
			RAY ra;
			
			MeshBone* handbo = arm->larm->Bones[arm->larm->BoneIndexes["handBone"]];
			MYVECTOR3 tempd(0,0,0);
			MYMATRIX tempma;
			MyMatrixMultiply( tempma, handbo->matrix_local, handbo->parent_bone->combined_matrix);
			MyVec3TransformCoord(tempd,tempd,tempma);
			MYVECTOR3 tempdn(1,0,0);
			MyVec3TransformNormal(tempdn,tempdn, tempma);
			ra.org = tempd;
			ra.dir = tempdn * 30;
			MYMATRIX iden;
			MyMatrixIdentity(iden);

			g->drawRAY(g,0xFFFF0000, &atarihan->world,view,g->getProj(), 30,&ra);

			}
			}
			//}

			}
			if (this->arm->rarm) {
		//	this->arm->rarm->animate(0,true);
	//		this->arm->rarm->animate(0,false);
			MYVECTOR3 te = MYVECTOR3(3,3,3);//1*cos(test),-2+4*sin(test)
												//	 ,3+4*sin(test));
			{

			MYMATRIX temp;
			MyMatrixInverse(temp,NULL,atarihan->world);
			MyVec3TransformCoord(te,te,temp);
			MyMatrixMultiply(temp, this->atarihan->world, *view);
			
		//	if (ap->positionArm(g,&temp, 0,this,/* &moku*/&te, true)) {
			//	arm->rarm->animate(0,false);
		//	}
			MYMATRIX world;
			MyMatrixIdentity(world);
			g->drawTriangle(g,0xFFFF0000,&world,view,g->getProj(), 
				&MYVECTOR3(3,3,3),//1*cos(test),-2+4*sin(test),3+4*sin(test)+1),
				&MYVECTOR3(3,4,3),//1*cos(test),-2+4*sin(test)+1,3+4*sin(test)),
				&MYVECTOR3(3,4,4));//*cos(test)+1,-2+4*sin(test),3+4*sin(test)));


			static float unko=0;
			unko += 0.03f;
			if (unko > 1) {
				unko = -1+0.01f;;
			}
		
			int i=0;
			for (i=0;i<1;i++) {
				LockOnSystem los;
				static ArmPoint8Positioner posit;
				if (iunko > 8) {
				
					static int r =0;
					bool t=false;
					
					r = iunko % 800+200;
				
					
					te = apinfo->getIndexPos();//los.getPosOfStudyPoint(r, 10,100,6,10,6,10,1,1,3);
					MyVec3TransformCoord(te,te,atarihan->world);
					OBB ob;
					ob.c = te;
					te = apinfo->getIndexPos();//los.getPosOfStudyPoint(r, 10,100,6,10,6,10,1,1,3);
					g->drawOBBFill(g,0xFFFF0000,&world,view,g->getProj(),&ob);
					if (!bunko) {
						if (apinfo->isCalcFinished()) {
							aim(g, view);
							bunko = false;
						} else {
							aphelper->setMoku(&te);
							
							bunko = true;
						}
				
					}
					//ArmPoint app = posit.getPoint(&te);//posit.points[r];
					//app.is_ok = true;
					//getPoint(&te);
					//if (app.is_ok) {
					//	if (!setdayo || ap->getReseted()) {
						//ap->setTheta(app.dthetaxa,app.dthetaxb,app.dthetaya, app.dthetayb, app.dthetaza, app.dthetazb);
					//	setdayo = true;
					//	}
				//		//iunko++;
				//	}
					
				
				
				
				
				
				} else {
				te = los.getPosOfStudyPoint(iunko, 10,100,6,10,6,10,300,300,500);
				MyVec3TransformCoord(te,te,atarihan->world);
				OBB ob;
				ob.c = te;
				te = los.getPosOfStudyPoint(iunko, 10,100,6,10,6,10,300,300,500);
				g->drawOBBFill(g,0xFFFF0000,&world,view,g->getProj(),&ob);





				}
			
		for (int t=0;t<50;t++) {
			if (!aphelper->getIsCalced() && !apinfo->isCalcFinished()) {
			aphelper->calc(g,&temp);
			}
			if (!aphelper_hidari->getIsCalced() && !apinfo->isCalcFinished()) {
			aphelper_hidari->calc(g,&temp);
			}

		}

		if (aphelper->getIsCalced()) {
			iunko++;
			if (!apinfo->isCalcFinished()) {
				ArmPoint app = aphelper->getArmPoint();
				app.is_ok = true;
				app.pos = apinfo->getIndexPos();
			apinfo->saveDtheta(&app, apinfo->getNowIndex());
			apinfo->saveFileWithA();
			apinfo->setNextIndex();
				bunko = false;
			} else {
				if (bunko) {
				
				bunko = false;
				} else {
				bunko = true;
				}
			}

		
		}

					
//		if (ap->positionArm3(g,&temp,this,&te/*MYVECTOR3(40*cos(test),-80+40*sin(test)
//											,80+40*sin(test))*/, true) != KTROBO_ARMPOSITION_DAME) {
												
												 
							
//												 ap->setArm3(this,true, arm->rarm->Bones[arm->rarm->BoneIndexes["uparmBone"]],
//													 arm->rarm->Bones[arm->rarm->BoneIndexes["downarmBone"]]);
			
												 
												 
												 
//												 arm->rarm->animate(40,false);
//												 ArmPoint app;
//												 app.pos = te;
//												 ap->getTheta(&app);
											
												// posit.setPoint(iunko,&app);
//												 iunko++;
//												 setdayo = false;
												 
	//											 break;
	//	} else {
		//	ap->resetTheta();
		//			 ap->setArm3(this,true, arm->rarm->Bones[arm->rarm->BoneIndexes["uparmBone"]],
			//										 arm->rarm->Bones[arm->rarm->BoneIndexes["downarmBone"]]);
			
												 
												 
											 
	//										 arm->rarm->animate(40,false);
	//	}
			}
			
		//	if ( i == 15) {
			//	// arm のアニメはしない
				// ap->setArm3(this,true, arm->rarm->Bones[arm->rarm->BoneIndexes["uparmBone"]],
					//								 arm->rarm->Bones[arm->rarm->BoneIndexes["downarmBone"]]);
			
												 
												 
												 
						//						 arm->rarm->animate(40,false);
			//}


				{
			RAY ra;
			
			MeshBone* handbo = arm->rarm->Bones[arm->larm->BoneIndexes["handBone"]];
			MYVECTOR3 tempd(0,0,0);
			MYMATRIX tempma;
			MyMatrixMultiply( tempma, handbo->matrix_local, handbo->parent_bone->combined_matrix);
			MyVec3TransformCoord(tempd,tempd,tempma);
			MYVECTOR3 tempdn(1,0,0);
			MyVec3TransformNormal(tempdn,tempdn, tempma);
			ra.org = tempd;
			ra.dir = tempdn * 30;
			MYMATRIX iden;
			MyMatrixIdentity(iden);

			g->drawRAY(g,0xFFFF0000, &atarihan->world,view,g->getProj(), 30,&ra);

			}


			






			}
			
			}
		}
	
	}


	if (MyVec3Length(atarihan->v) > 5) {// seigen
		atarihan->setV(&MYVECTOR3(atarihan->v.float3.x/1.5f,atarihan->v.float3.y/1.5f, atarihan->v.float3.z/1.5f));
	}

	atarihan->calcJyusinAndR();
	/*
	OBB* ob = &atari_leg->bone_obbs[atari_leg->mesh->BoneIndexes["leftLegDownBone"]];
	OBB* ob2 = &atari_leg->bone_obbs[atari_leg->mesh->BoneIndexes["rightLegDownBone"]];
	// 貫通しないように大きくする
	//ob->e = ob->e * 1.3;
	//ob2->e = ob2->e * 1.3;
	*/


	raweapon->wf_rifle.update(dt,stamp);
	this->calcWorldWithoutRotX();


}

void Robo::aim(Graphics* g, MYMATRIX* view) {

	// ロボ空間のtarget座標を取得する
	MYMATRIX mat;
	MyMatrixInverse(mat,NULL,atarihan->world);
	MYVECTOR3 tempmo;
	MyVec3TransformCoord(tempmo,target, mat);
//	tempmo = target;
	ArmPoint* podayo8[8];
	LockOnSystem los;
	if (!los.isIn(&tempmo,apinfo->dmin,apinfo->dmax, apinfo->mintate, apinfo->maxtate, apinfo->minyoko, apinfo->maxyoko,apinfo->dtate, apinfo->dyoko, apinfo->dd)) return;

    for (int i=0;i<8;i++) {
    		podayo8[i] = apinfo->getArmPointFromPointindex(i, &tempmo);
	}

	
	if (podayo8[0] && podayo8[1] && podayo8[2] && podayo8[3] &&
		podayo8[4] && podayo8[5] && podayo8[6] && podayo8[7]) {
		for (int i=0;i<8;i++) {
   			aphelper->setArmPoint8(i,podayo8[i]);
		//	aphelper_hidari->setArmPoint8(i,podayo8[i]);
		}
		aphelper->setMoku(&tempmo);
		aphelper->setNoCalcYet(true);
		aphelper->calc(g, view);

		// weapon に反映
		if (raweapon) {
			raweapon->weapon->animate(0,false);
		}

	}

	tempmo.float3.x *= -1;
	 for (int i=0;i<8;i++) {
    		podayo8[i] = apinfo->getArmPointFromPointindex(i, &tempmo);
	}

	
	if (podayo8[0] && podayo8[1] && podayo8[2] && podayo8[3] &&
		podayo8[4] && podayo8[5] && podayo8[6] && podayo8[7]) {
		for (int i=0;i<8;i++) {
   		//	aphelper->setArmPoint8(i,podayo8[i]);
			aphelper_hidari->setArmPoint8(i,podayo8[i]);
		}


             		aphelper_hidari->setMoku(&tempmo);
		aphelper_hidari->setNoCalcYet(true);
		aphelper_hidari->calc(g,view);
		// weapon に反映
		if (laweapon) {
			laweapon->weapon->animate(0,false);
		}
	}




}

void RoboParts::loadMesh(Graphics* g, MyTextureLoader* loader){
	CS::instance()->enter(CS_RENDERDATA_CS, "test");	
	mesh_loaded=true;
	CS::instance()->leave(CS_RENDERDATA_CS, "test");


}

void Robo::settyakuRArmWeaponWithArm() {
	if (raweapon && arm) {
	// handbone と接着
	raweapon->weapon->RootBone->parent_bone = arm->rarm->Bones[arm->rarm->BoneIndexes["handBone"]];
	raweapon->weapon->RootBone->parent_bone_index = arm->rarm->BoneIndexes["handBone"];
	MYMATRIX wo;
	MyMatrixIdentity(wo);
	wo._11 = 0;
	wo._22 = 0;
	wo._12 = -1;
	wo._21 = 1;
	wo._33 = -1;
	raweapon->weapon->rootbone_matrix_local_kakeru = wo;
	raweapon->weapon->animate(0,true);
	}
}

void Robo::settyakuLArmWeaponWithArm() {
	if (laweapon && arm) {
		laweapon->weapon->RootBone->parent_bone = arm->larm->Bones[arm->larm->BoneIndexes["handBone"]];
	laweapon->weapon->RootBone->parent_bone_index = arm->larm->BoneIndexes["handBone"];
	MYMATRIX wo;
	MyMatrixIdentity(wo);
	wo._11 = 0;
	wo._22 = 0;
	wo._12 = -1;
	wo._21 = 1;
	wo._33 = -1;
	laweapon->weapon->rootbone_matrix_local_kakeru = wo;
	laweapon->weapon->animate(0,true);
	}
}
void Robo::settyakuRShoulderWeaponWithBody() {
	if (rsweapon && body) {

		rsweapon->weapon->RootBone->parent_bone = body->body->Bones[body->body->BoneIndexes["rightKataJointBone"]];
		rsweapon->weapon->RootBone->parent_bone_index = body->body->BoneIndexes["rightKataJointBone"];
		MYMATRIX wo;
		MyMatrixIdentity(wo);
		//wo._11 = 0;
		//wo._22 = 0;
		//wo._12 = -1;
		//wo._21 = 1;
		wo._33 = -1;
		rsweapon->weapon->rootbone_matrix_local_kakeru = wo;
		rsweapon->weapon->animate(0,true);
	}

}

void Robo::settyakuLShoulderWeaponWithBody() {
	if (lsweapon && body) {

		lsweapon->weapon->RootBone->parent_bone = body->body->Bones[body->body->BoneIndexes["leftKataJointBone"]];
		lsweapon->weapon->RootBone->parent_bone_index = body->body->BoneIndexes["leftKataJointBone"];
		MYMATRIX wo;
		MyMatrixIdentity(wo);
		//wo._11 = 0;
		//wo._22 = 0;
		//wo._12 = -1;
		//wo._21 = 1;
		wo._33 = -1;
		lsweapon->weapon->rootbone_matrix_local_kakeru = wo;
		lsweapon->weapon->animate(0,true);
	}

}

void Robo::settyakuBoosterWithLeg() {
	if (booster && leg) {
	booster->mesh->RootBone->parent_bone = leg->leg->Bones[leg->leg->BoneIndexes["boosterBone"]];
	booster->mesh->RootBone->parent_bone_index = leg->leg->BoneIndexes["boosterBone"];
	MYMATRIX wo;
	MyMatrixIdentity(wo);
	wo._11 = 0;
	wo._22 = 0;
	wo._12 = -1;
	wo._21 = 1;
	wo._33 = -1;
	booster->mesh->rootbone_matrix_local_kakeru = wo;
	booster->mesh->animate(0,true);
	}

}


void Robo::init(Graphics* g, MyTextureLoader* tex_loader, AtariHantei* hantei) {
/*	Mesh* head;
	Mesh* body;
	Mesh* leg;
	Mesh* rarm;
	Mesh* larm;
	RArmWeapon* raweapon;
	LArmWeapon* laweapon;
	RShoulderWeapon* rsweapon;
	LShoulderWeapon* lsweapon;
*/
	screen_height = g->getScreenHeight();
	head = 0;
	body = 0;
	leg = 0;
	arm = 0;
	raweapon = 0;
	laweapon = 0;
	rsweapon = 0;
	lsweapon = 0;
	iweapon = 0;
	fcs = 0;
	booster = 0;
	engine = 0;
	// headparts のメタデータの読み込み
	atarihan = new UMeshUnit();
	//MyMatrixIdentity(world);
	MyTokenAnalyzer ma;
	{
	ma.load("resrc/ktrobo/info/metadata/ktroboheadpartsmetadata.txt");
	RoboDataMetaData* head_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		head_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	
	ma.load("resrc/ktrobo/info/ktroboheadparts.txt");

	head = new RoboHead();
	try {
	head->init(&ma,head_md,g,tex_loader);
		} catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete head_md;
		ma.deletedayo();
		throw err;
	}
	delete head_md;

	ma.deletedayo();
	}
	
	// 次はbody
	{
	ma.load("resrc/ktrobo/info/metadata/ktrobocorepartsmetadata.txt");
	RoboDataMetaData* body_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		body_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	ma.load("resrc/ktrobo/info/ktrobocoreparts.txt");


	body = new RoboBody();
	try {
	body->init(&ma,body_md,g,tex_loader);
	} catch (GameError* err) {
		ma.deletedayo();
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete body_md;
		throw err;
	}

	delete body_md;

	ma.deletedayo();
	}

	// 次はarm
	{
	ma.load("resrc/ktrobo/info/metadata/ktroboarmpartsmetadata.txt");
	RoboDataMetaData* arm_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		arm_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	ma.load("resrc/ktrobo/info/ktroboarmparts.txt");


	arm = new RoboArm();
	try {
	arm->init(&ma,arm_md,g,tex_loader);
		} catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete arm_md;
		ma.deletedayo();
		throw err;
	}
	delete arm_md;

	ma.deletedayo();
	}



	// 次はleg
	{
	ma.load("resrc/ktrobo/info/metadata/ktrobolegpartsmetadata.txt");
	RoboDataMetaData* leg_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		leg_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();

	ma.load("resrc/ktrobo/info/leg/ktrobolegt2.txt");

	leg = new RoboLeg();
	try { 
	leg->init(&ma,leg_md,g,tex_loader);
		} catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete leg_md;
		ma.deletedayo();
		throw err;
	}
	delete leg_md;

	ma.deletedayo();
	}



	{
	// RWEAPON
	ma.load("resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt");
	RoboDataMetaData* rweapon_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		rweapon_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	ma.load("resrc/ktrobo/info/rweapon/ktroborarmweaponrifle.txt");
	
	raweapon = new RArmWeapon();
	try {
	raweapon->init(&ma,rweapon_md,g,tex_loader);
		} catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete rweapon_md;
		ma.deletedayo();
		throw err;
	}
	delete rweapon_md;
	ma.deletedayo();

	
	settyakuRArmWeaponWithArm();
	}

	{
	// LWEAPON
	ma.load("resrc/ktrobo/info/metadata/weapon/ktrobopartsweaponmetadata.txt");
	RoboDataMetaData* lweapon_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		lweapon_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	ma.load("resrc/ktrobo/info/lweapon/ktrobolarmweaponrifle.txt");
	
	laweapon = new LArmWeapon();
	try {
	laweapon->init(&ma,lweapon_md,g,tex_loader);
		} catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete lweapon_md;
		ma.deletedayo();
		throw err;
	}
	delete lweapon_md;
	ma.deletedayo();

	settyakuLArmWeaponWithArm();

	}



	{
	// BOOSTER
	ma.load("resrc/ktrobo/info/metadata/ktroboboosterpartsmetadata.txt");
	RoboDataMetaData* booster_md = new RoboDataMetaData();
	RoboMetaDataPart rmdp;
	rmdp.clear();
	int dnum = ma.GetIntToken();
	for (int i=0;i<dnum;i++) {
		rmdp.clear();
		rmdp.readline(&ma);
		booster_md->setData(rmdp.data_name,rmdp.data_name2,rmdp.data_type,rmdp.data_sentence,rmdp.data_compare);
	}

	ma.deletedayo();
	ma.load("resrc/ktrobo/info/ktroboboosterparts.txt");
	
	booster = new RoboBooster();
	try {
	booster->init(&ma,booster_md,g,tex_loader);
		} catch (GameError* err) {
		
	//	MessageBoxA(g->getHWND(), err->getMessage(), err->getErrorCodeString(err->getErrorCode()), MB_OK);
		delete booster_md;
		ma.deletedayo();
		throw err;
	}
	delete booster_md;
	ma.deletedayo();

	settyakuBoosterWithLeg();

	// booster の計算　
	roboparam.boostercalc.Init(this, booster);

	}



	this->remakeUMesh(g,tex_loader);
	
	if (hantei) {
		hantei->setUMeshUnit(atarihan,AtariUnit::AtariType::ATARI_CHARA);
	}
	

	anime_loop_leg.setAnime(10,30,true);
	anime_loop_leg.setTimeAndSpeed(0.01,0);
//	world = atarihan->world;
	ap = new ArmPositioner(3.14/60000,3.14/3,0.62);
	ap->setTheta(0.96429, -0.92417,0.1193,0,0.20,0.19);
	aphelper = new ArmPositionerHelper(this,ap,true);

	ap_hidari = new ArmPositioner(3.14/60000,3.14/3,0.62);
	ap_hidari->setTheta(0.96429, -0.92417,0.1193,0,0.20,0.19);
	aphelper_hidari = new ArmPositionerHelper(this,ap_hidari,false);

	apinfo = new ArmPointIndexInfo("ktrobofcs2.txt", 10,100,6,10,6,10,2,2,10);
	if (apinfo->hasFile()) {
		apinfo->loadFile();
	} else {
		apinfo->makeNewFile();
		apinfo->loadFile();
	}
	

	roboparam.calcParam();
	
}

void Robo::release() {
	if (head) {
		head->Release();
		delete head;
		head = 0;
	}
	if (body) {
		body->Release();
		delete body;
		body = 0;
	}
	if (leg) {
		leg->Release();
		delete leg;
		leg = 0;
	}
	if (arm) {
		arm->Release();
		delete arm;
		arm = 0;
	}
	if (raweapon) {
		raweapon->Release();
		delete raweapon;
		raweapon = 0;
	}

	if (laweapon) {
		laweapon->Release();
		delete laweapon;
		laweapon = 0;
	}

	if (rsweapon) {
		rsweapon->Release();
		delete rsweapon;
		rsweapon = 0;
	}

	if (lsweapon) {
		lsweapon->Release();
		delete lsweapon;
		lsweapon = 0;
	}

	if (iweapon) {
		iweapon->Release();
		delete iweapon;
		iweapon = 0;
	}

	if (fcs) {
		fcs->Release();
		delete fcs;
		fcs = 0;
	}

	if (booster) {
		booster->Release();
		delete booster;
		booster = 0;
	}

	if (engine) {
		engine->Release();
		delete engine;
		engine = 0;
	}

	if (atarihan) {
		delete atarihan;
		atarihan = 0;
	}

	if (ap) {
		delete ap;
		ap = 0;
	}

	if (aphelper) {
		delete aphelper;
		aphelper = 0;
	}

	
	if (ap_hidari) {
		delete ap_hidari;
		ap_hidari = 0;
	}

	if (aphelper_hidari) {
		delete aphelper_hidari;
		aphelper_hidari = 0;
	}


	if (apinfo) {
		delete apinfo;
		apinfo = 0;
	}
}

void RoboHead::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (head) {
		head->draw(g,&wor,view,proj);
	}
	if (head2) {
		head2->draw(g,&wor,view,proj);
	}
	if (head3) {
		head3->draw(g,&wor,view,proj);
	}
}


void RoboBooster::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (mesh) {
		mesh->draw(g,&wor,view,proj);
	}
	
}


void RoboFCS::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (mesh) {
		mesh->draw(g,&wor,view,proj);
	}
	
}


void RoboBody::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (body) {
		body->draw(g,&wor,view,proj);
	}
	
}


void RoboEngine::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (mesh) {
		mesh->draw(g,&wor,view,proj);
	}
	
}


void RoboArm::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (rarm) {
		rarm->draw(g,&wor,view,proj);
	}
	
}


void RoboLeg::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (leg) {
		leg->draw(g,&wor,view,proj);
	}
	
}


void RArmWeapon::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (weapon) {
		weapon->draw(g,&wor,view,proj);
	}
	
}


void LArmWeapon::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (weapon) {
		weapon->draw(g,&wor,view,proj);
	}
	
}


void RShoulderWeapon::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (weapon) {
		weapon->draw(g,&wor,view,proj);
	}
	
}


void LShoulderWeapon::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (weapon) {
		weapon->draw(g,&wor,view,proj);
	}
	
}


void InsideWeapon::drawMesh(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (weapon) {
		weapon->draw(g,&wor,view,proj);
	}
	
}







void RoboHead::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	RoboHead* new_parts = new RoboHead();
	new_parts->init(this);

	if (robo->head) {
		robo->head->Release();
		delete robo->head;
		robo->head = 0;
	}
	robo->head = new_parts;
	RoboHead*  head = new_parts;

	robo->remakeUMesh(g,tex_loader); 
}

void RoboLeg::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	RoboLeg* new_parts = new RoboLeg();
	new_parts->init(this);

	if (robo->leg) {
		robo->leg->Release();
		delete robo->leg;
		robo->leg = 0;
	}
	robo->leg = new_parts;
	RoboLeg*  leg = new_parts;

	robo->remakeUMesh(g,tex_loader);
	robo->settyakuBoosterWithLeg();
}

void RoboBody::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	RoboBody* new_parts = new RoboBody();
	new_parts->init(this);
	if (robo->body) {
		robo->body->Release();
		delete robo->body;
		robo->body = 0;
	}
	robo->body = new_parts;
	robo->remakeUMesh(g,tex_loader);
	robo->settyakuBoosterWithLeg();
	robo->settyakuRArmWeaponWithArm();
	robo->settyakuLArmWeaponWithArm();
}


void RoboArm::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	RoboArm* new_parts = new RoboArm();
	new_parts->init(this);
	if (robo->arm) {
		robo->arm->Release();
		delete robo->arm;
		robo->arm = 0;
	}
	robo->arm = new_parts;
	robo->remakeUMesh(g,tex_loader);
	//robo->settyakuBoosterWithLeg();
	robo->settyakuRArmWeaponWithArm();
	robo->settyakuLArmWeaponWithArm();
}


void RArmWeapon::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	RArmWeapon* new_parts = new RArmWeapon();
	new_parts->init(this);
	if (robo->raweapon) {
		robo->raweapon->Release();
		delete robo->raweapon;
		robo->raweapon = 0;
	}
	robo->raweapon = new_parts;
	//robo->remakeUMesh(g,tex_loader);
	//robo->settyakuBoosterWithLeg();
	robo->settyakuRArmWeaponWithArm();
	//robo->settyakuLArmWeaponWithArm();
}

void LArmWeapon::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	LArmWeapon* new_parts = new LArmWeapon();
	new_parts->init(this);
	if (robo->laweapon) {
		robo->laweapon->Release();
		delete robo->laweapon;
		robo->laweapon = 0;
	}
	robo->laweapon = new_parts;
	//robo->remakeUMesh(g,tex_loader);
	//robo->settyakuBoosterWithLeg();
	//robo->settyakuRArmWeaponWithArm();
	robo->settyakuLArmWeaponWithArm();
}

void RoboBooster::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	RoboBooster* new_parts = new RoboBooster();
	new_parts->init(this);
	if (robo->booster) {
		robo->booster->Release();
		delete robo->booster;
		robo->booster = 0;
	}
	robo->booster = new_parts;
	robo->settyakuBoosterWithLeg();
	robo->roboparam.boostercalc.Init(robo, robo->booster);
}

void RoboFCS::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	RoboFCS* new_parts = new RoboFCS();
	new_parts->init(this);
	if (robo->fcs) {
		robo->fcs->Release();
		delete robo->fcs;
		robo->fcs  = 0;
	}
	robo->fcs = new_parts;
	
}


void RoboEngine::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	RoboEngine* new_parts = new RoboEngine();
	new_parts->init(this);
	if (robo->engine) {
		robo->engine->Release();
		delete robo->engine;
		robo->engine  = 0;
	}
	robo->engine = new_parts;
	
}


void RShoulderWeapon::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	RShoulderWeapon* new_parts = new RShoulderWeapon();
	new_parts->init(this);
	if (robo->rsweapon) {
		robo->rsweapon->Release();
		delete robo->rsweapon;
		robo->rsweapon  = 0;
	}
	robo->rsweapon = new_parts;
	robo->settyakuRShoulderWeaponWithBody();
}


void LShoulderWeapon::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	LShoulderWeapon* new_parts = new LShoulderWeapon();
	new_parts->init(this);
	if (robo->lsweapon) {
		robo->lsweapon->Release();
		delete robo->rsweapon;
		robo->lsweapon  = 0;
	}
	robo->lsweapon = new_parts;
	robo->settyakuLShoulderWeaponWithBody();
}

void InsideWeapon::equipRobo(Robo* robo, Graphics* g, MyTextureLoader* tex_loader) {
	InsideWeapon* new_parts = new InsideWeapon();
	new_parts->init(this);
	if (robo->iweapon) {
		robo->iweapon->Release();
		delete robo->iweapon;
		robo->iweapon = 0;
	}
	robo->iweapon = new_parts;
}

void RoboHead::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || head) return;
	if (hasMeshLoaded()) return;

	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->head = new Mesh();
			head->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			head->readAnime(rdp->string_data);
			head->animate(0,true);
		}
	}

	rdp = data->getData("mesh2");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->head2 = new Mesh();
			head2->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime2");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			head2->readAnime(rdp->string_data);
			head2->animate(0,true);
		}
	}

	rdp = data->getData("mesh3");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->head3 = new Mesh();
			head3->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime3");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			head3->readAnime(rdp->string_data);
			head3->animate(0,true);
		}
	}

	RoboParts::loadMesh(g,tex_loader);

}

RoboParts::~RoboParts() {
	//Release();
}

void RoboHead::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {

	data = new RoboData();
	
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}


	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->head = new Mesh();
			head->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			head->readAnime(rdp->string_data);
			head->animate(0,true);
		}
	}

	rdp = data->getData("mesh2");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->head2 = new Mesh();
			head2->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime2");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			head2->readAnime(rdp->string_data);
			head2->animate(0,true);
		}
	}

	rdp = data->getData("mesh3");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->head3 = new Mesh();
			head3->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime3");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			head3->readAnime(rdp->string_data);
			head3->animate(0,true);
		}
	}


}

void RoboBody::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || body) return;
	if (hasMeshLoaded()) return;
	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->body = new Mesh();
			body->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			body->readAnime(rdp->string_data);
			body->animate(0,true);
		}
	}
	RoboParts::loadMesh(g,tex_loader);
}

void RoboBody::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {
//	throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");

	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->body = new Mesh();
			body->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			body->readAnime(rdp->string_data);
			body->animate(0,true);
		}
	}

}

void RoboArm::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || larm) return;
	if (hasMeshLoaded()) return;
	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->larm = new Mesh();
			this->rarm = new Mesh();
			larm->readMesh(g,rdp->string_data,tex_loader);
			rarm->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			larm->readAnime(rdp->string_data);
			rarm->readAnime(rdp->string_data);
			larm->animate(0,true);
			rarm->animate(0,true);
		}
	}


	RoboParts::loadMesh(g,tex_loader);


}
void RoboArm::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {

	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}


	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->larm = new Mesh();
			this->rarm = new Mesh();
			larm->readMesh(g,rdp->string_data,tex_loader);
			rarm->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			larm->readAnime(rdp->string_data);
			rarm->readAnime(rdp->string_data);
			larm->animate(0,true);
			rarm->animate(0,true);
		}
	}


}

void RoboLeg::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || leg) return;
	if (hasMeshLoaded()) return;
	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->leg = new Mesh();
			leg->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			leg->readAnime(rdp->string_data);
			leg->animate(0,true);
		}
	}
	RoboParts::loadMesh(g,tex_loader);
}


void RoboLeg::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	data = new RoboData();

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->leg = new Mesh();
			leg->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			leg->readAnime(rdp->string_data);
			leg->animate(0,true);
		}
	}

}

void RArmWeapon::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {

	if (!data || weapon) return;
	if (hasMeshLoaded()) return;
	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}

	RoboParts::loadMesh(g,tex_loader);

}

void RArmWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {

	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}


}

void LArmWeapon::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || weapon) return;
	if (hasMeshLoaded()) return;
		// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}
	RoboParts::loadMesh(g,tex_loader);
}

void LArmWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}

}

void RoboPartsEmpty::emptyRArmWeapon(Robo* robo, bool is_delete) {
	if (robo->raweapon && is_delete) {
		robo->raweapon->Release();
		delete robo->raweapon;
		robo->raweapon = 0;
	}
	robo->raweapon = 0;


}

void RoboPartsEmpty::emptyLArmWeapon(Robo* robo, bool is_delete) {
	if (robo->laweapon && is_delete) {
		robo->laweapon->Release();
		delete robo->laweapon;
		robo->laweapon = 0;
	}
	robo->laweapon = 0;

}

void RoboPartsEmpty::emptyRShoulderWeapon(Robo* robo, bool is_delete) {

	if (robo->rsweapon && is_delete) {
		robo->rsweapon->Release();
		delete robo->rsweapon;
		robo->rsweapon = 0;
	}
	robo->rsweapon = 0;

}


void RoboPartsEmpty::emptyLShoulderWeapon(Robo* robo, bool is_delete) {
	if (robo->lsweapon && is_delete) {
		robo->lsweapon->Release();
		delete robo->lsweapon;
		robo->lsweapon = 0;
	}
	robo->lsweapon = 0;

}

void RoboPartsEmpty::emptyInsideWeapon(Robo* robo, bool is_delete) {
	if (robo->iweapon && is_delete) {
		robo->iweapon->Release();
		delete robo->iweapon;
		robo->iweapon = 0;
	}
	robo->iweapon = 0;

}

void RShoulderWeapon::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || weapon) return;
	if (hasMeshLoaded()) return;
	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}
	RoboParts::loadMesh(g,tex_loader);
}

void RShoulderWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}


}

void LShoulderWeapon::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || weapon) return;
	if (hasMeshLoaded()) return;
	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}
	RoboParts::loadMesh(g,tex_loader);




}

void LShoulderWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

		// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}

}

void InsideWeapon::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || weapon) return;
	if (hasMeshLoaded()) return;
		// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}


	RoboParts::loadMesh(g,tex_loader);






}

void InsideWeapon::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->weapon = new Mesh();
			weapon->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			weapon->readAnime(rdp->string_data);
			weapon->animate(0,true);
		}
	}


}



void RoboBooster::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || mesh) return;
	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->mesh = new Mesh();
			mesh->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			mesh->readAnime(rdp->string_data);
			mesh->animate(0,true);
		}
	}

	RoboParts::loadMesh(g,tex_loader);




}


void RoboBooster::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->mesh = new Mesh();
			mesh->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			mesh->readAnime(rdp->string_data);
			mesh->animate(0,true);
		}
	}


}


void RoboEngine::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {
	if (!data || mesh) return;
	if (hasMeshLoaded()) return;
	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->mesh = new Mesh();
			mesh->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			mesh->readAnime(rdp->string_data);
			mesh->animate(0,true);
		}
	}
	RoboParts::loadMesh(g,tex_loader);
}



void RoboEngine::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->mesh = new Mesh();
			mesh->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			mesh->readAnime(rdp->string_data);
			mesh->animate(0,true);
		}
	}


}


void RoboFCS::loadMesh(Graphics* g, MyTextureLoader* tex_loader) {

	if (!data || mesh) return;
	if (hasMeshLoaded()) return;
    // data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->mesh = new Mesh();
			mesh->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			mesh->readAnime(rdp->string_data);
			mesh->animate(0,true);
		}
	}
	RoboParts::loadMesh(g,tex_loader);
}




void RoboFCS::init(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data, Graphics* g, MyTextureLoader* tex_loader) {


	data = new RoboData();
	ma->GetToken();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}

	// data にデータが入っているので
	RoboDataPart* rdp = data->getData("mesh");
	if (rdp) {
		if (strcmp("@", rdp->string_data) != 0) {
			// 読み込む
			this->mesh = new Mesh();
			mesh->readMesh(g,rdp->string_data,tex_loader);
			// 次はアニメ
			rdp = data->getData("anime");
			if (!rdp) { 
				throw new GameError(KTROBO::FATAL_ERROR, "there is no anime in init robo");
			}
			mesh->readAnime(rdp->string_data);
			mesh->animate(0,true);
		}
	}


}


void RoboAnimeLoop::animate(UMesh* umesh, bool calculateoffsetmatrix) {

	umesh->setAnimeFrame(now);
	umesh->Animate(calculateoffsetmatrix);
};



bool Robo::handleMessage(int msg, void* data, DWORD time) {
/*	if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*) data;
		if (input->getKEYSTATE()[VK_DOWN] & KTROBO_INPUT_BUTTON_DOWN) {

			CS::instance()->enter(CS_SOUND_CS, "enter");
			sound->stopCue(yumes[sound_index]);
			sound_index =(sound_index+1) % 6;
			sound->playCue(yumes[sound_index]);
			CS::instance()->leave(CS_SOUND_CS, "leave");
		}


	}
*/


	//if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*) data;
		float x = input->getMOUSESTATE()->mouse_x;
		float y = input->getMOUSESTATE()->mouse_y;
		CS::instance()->enter(CS_MESSAGE_CS, "enter");
		if (move_state->isBoosterHi() && !move_state->isCanMoveWhenBoost(this)) {
			// ハイブースト中なので何もできない
				if (input->getKEYSTATE()['T'] & KTROBO_INPUT_BUTTON_DOWN) {
				is_fireraweapon = true;
			} else {
				is_fireraweapon = false;
			}
			if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
			if (input->getKEYSTATE()['F'] & KTROBO_INPUT_BUTTON_DOWN) {
				if (booster_state == &boostoff) {
					booster_state->leave(this, &boostoff, &boostontaiki);
					boostontaiki.enter(this,&boostoff, &boostontaiki);
					booster_state = &boostontaiki;
				} else if( booster_state == &boostontaiki) {
					booster_state->leave(this, &boostontaiki, &boostoff);
					boostoff.enter(this, &boostontaiki, &boostoff);
					booster_state = &boostoff;
					if (move_state->isBoosterHi()) {
						move_state->leave(this, &movestop, move_state);
						movestop.enter(this, &movestop, move_state);
						move_state = &movestop;
					}
				}
			}
			}
			//Sleep(1);
		} else {

			if (move_state->isCanBoost(this)) {
				if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {
					incPressedSpaceCount();
				}

				if (msg == KTROBO_INPUT_MESSAGE_ID_KEYUP) {
					if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_UP) {
						resetPressedSpaceCount();
					}
				}
				if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
					if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_DOWN) {
						resetPressedSpaceCount();
					}

					if (input->getKEYSTATE()['C'] & KTROBO_INPUT_BUTTON_DOWN) {
						if (input->getKEYSTATE()['W'] & KTROBO_INPUT_BUTTON_PRESSED) {
							if ((booster_state == &boostontaiki) && (move_state != &boostforward)) {
								move_state->leave(this, &boostforward, move_state);
								boostforward.enter(this,&boostforward, move_state);
								move_state = &boostforward;
									CS::instance()->leave(CS_MESSAGE_CS, "leave");
								return true;
							}
						} else if((input->getKEYSTATE()['A'] & KTROBO_INPUT_BUTTON_PRESSED)) {
							if ((booster_state == &boostontaiki) && (move_state != &boostleft)) {
								move_state->leave(this, &boostleft, move_state);
								boostleft.enter(this,&boostleft, move_state);
								move_state = &boostleft;
								CS::instance()->leave(CS_MESSAGE_CS, "leave");
								return true;
							}
						} else if (input->getKEYSTATE()['S'] & KTROBO_INPUT_BUTTON_PRESSED) {
							if ((booster_state == &boostontaiki) && (move_state != &boostback)) {
								move_state->leave(this, &boostback, move_state);
								boostback.enter(this,&boostback, move_state);
								move_state = &boostback;
								CS::instance()->leave(CS_MESSAGE_CS, "leave");
								return true;
							}
						} else if (input->getKEYSTATE()['D'] & KTROBO_INPUT_BUTTON_PRESSED) {
							if ((booster_state == &boostontaiki) && (move_state != &boostright)) {
								move_state->leave(this, &boostright, move_state);
								boostright.enter(this,&boostright, move_state);
								move_state = &boostright;
								CS::instance()->leave(CS_MESSAGE_CS, "leave");
								return true;
							}
						} else {
							if ((booster_state == &boostontaiki) && ( move_state != &boostup) ) {
								move_state->leave(this, &boostup, move_state);
								boostup.enter(this, &boostup, move_state);
								move_state = &boostup;
								CS::instance()->leave(CS_MESSAGE_CS, "leave");
								return true;
							}
						}
					}
				}
			}
			if (input->getKEYSTATE()['W'] & KTROBO_INPUT_BUTTON_PRESSED) {

				if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {

					if ((move_state != &movejumpforward) && !move_state->isJumpKABE() &&  (setti_state != &kuutyuu)) {
						move_state->leave(this, &movejumpforward, move_state);
						movejumpforward.enter(this, &movejumpforward, move_state);
						move_state = &movejumpforward;
					}
					
					if (!move_state->isJump() && !move_state->isJumpKABE() && (setkabe_state != &kuutyuu)) {
						move_state->leave(this, &movejumpforwardkabe, move_state);
 						movejumpforwardkabe.enter(this, &movejumpforwardkabe, move_state);
						move_state = &movejumpforwardkabe;
					}



			

				} else {


					if (input->getKEYSTATE()['A'] & KTROBO_INPUT_BUTTON_PRESSED) {
						if ((move_state != &moveleftforward) && ( move_state != &movejumpforward) && (move_state != &movejumpleft)) {
							move_state->leave(this,&moveleftforward, move_state);
							moveleftforward.enter(this, &moveleftforward, move_state);
							move_state = &moveleftforward;
						}
					} else if( input->getKEYSTATE()['D'] & KTROBO_INPUT_BUTTON_PRESSED) {
						if ((move_state != &moverightforward) && ( move_state != &movejumpforward) && (move_state != &movejumpright)) {
							move_state->leave(this,&moverightforward, move_state);
							moveleftforward.enter(this, &moverightforward, move_state);
							move_state = &moverightforward;
						}
					} else {

						if ((move_state != &moveforward) && ((move_state != &movejumpforward) 
							)
							) {
							move_state->leave(this, &moveforward, move_state);
							moveforward.enter(this, &moveforward, move_state);
							move_state = &moveforward;
						}
					}
				}
			} else if(input->getKEYSTATE()['A'] & KTROBO_INPUT_BUTTON_PRESSED) {
				if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {

					if ((move_state != &movejumpleft) && (setti_state != &kuutyuu)) {
						move_state->leave(this, &movejumpleft, move_state);
						movejumpleft.enter(this, &movejumpleft, move_state);
						move_state = &movejumpleft;
					}
					
					if (!move_state->isJump() && !move_state->isJumpKABE() &&(setkabe_state != &kuutyuu)) {
						move_state->leave(this, &movejumpleftkabe, move_state);
						movejumpleftkabe.enter(this, &movejumpleftkabe, move_state);
						move_state = &movejumpleftkabe;
					}

				} else {
					if (input->getKEYSTATE()['S'] & KTROBO_INPUT_BUTTON_PRESSED) {
						if ((move_state != &moveleftback) && ( move_state != &movejumpback) && (move_state != &movejumpleft)) {
							move_state->leave(this,&moveleftback, move_state);
							moveleftforward.enter(this, &moveleftback, move_state);
							move_state = &moveleftback;
						}
					} else {
						if ((move_state != &moveleft) && ((move_state != &movejumpleft)
							)) {
					
							move_state->leave(this, &moveleft, move_state);
							moveleft.enter(this, &moveleft, move_state);
							move_state = &moveleft;
						}
					}
				}
			} else if(input->getKEYSTATE()['D'] & KTROBO_INPUT_BUTTON_PRESSED) {
				if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {

					if ((move_state != &movejumpright) && (setti_state != &kuutyuu)) {
						move_state->leave(this, &movejumpright, move_state);
						movejumpright.enter(this, &movejumpright, move_state);
						move_state = &movejumpright;
					}
					if (!move_state->isJump() && !move_state->isJumpKABE() &&(setkabe_state != &kuutyuu)) {
						move_state->leave(this, &movejumprightkabe, move_state);
						movejumprightkabe.enter(this, &movejumprightkabe, move_state);
						move_state = &movejumprightkabe;
					}

				} else {
					if (input->getKEYSTATE()['S'] & KTROBO_INPUT_BUTTON_PRESSED) {
						if ((move_state != &moverightback) && ( move_state != &movejumpback) && (move_state != &movejumpright)) {
							move_state->leave(this,&moverightback, move_state);
							moveleftforward.enter(this, &moverightback, move_state);
							move_state = &moverightback;
						}
					} else {
						if ((move_state != &moveright) && (move_state != &movejumpright)
							) {
					
					
					
					
							move_state->leave(this, &moveright, move_state);
							moveright.enter(this, &moveright, move_state);
							move_state = &moveright;
						}
					}
				}
			} else if(input->getKEYSTATE()['S'] & KTROBO_INPUT_BUTTON_PRESSED) {
				if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {

					if ((move_state != &movejumpback)  && !move_state->isJumpKABE() && (setti_state != &kuutyuu)) {
						move_state->leave(this, &movejumpback, move_state);
						movejumpback.enter(this, &movejumpback, move_state);
						move_state = &movejumpback;
					}
					
					if (!move_state->isJump() && !move_state->isJumpKABE() &&(setkabe_state != &kuutyuu)) {
						move_state->leave(this, &movejumpbackkabe, move_state);
						movejumpbackkabe.enter(this, &movejumpbackkabe, move_state);
						move_state = &movejumpbackkabe;
					}

				} else {
				if ((move_state != &moveback) && (move_state != &movejumpback)
					)
						 {
					
					move_state->leave(this, &moveback, move_state);
					moveback.enter(this, &moveback, move_state);
					move_state = &moveback;
				}
				}
			} else if(input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {
				if (((move_state != &movejump) && (move_state != &movejumpforward) &&
					(move_state != &movejumpback) && (move_state != &movejumpleft) &&
					(move_state != &movejumpright))  && (setti_state != &kuutyuu)) {
					move_state->leave(this, &movejump, move_state);
					movejump.enter(this, &movejump, move_state);
					move_state = &movejump;
				} else if ((!move_state->isJumpKABE()) && (setkabe_state != &kuutyuu)) {
					move_state->leave(this, &movejumpkabe, move_state);
					movejumpkabe.enter(this, &movejumpkabe, move_state);
					move_state = &movejumpkabe;
				}
			} else {
				if ((move_state != &movestop)) {
					move_state->leave(this, &movestop, move_state);
					movestop.enter(this, &movestop, move_state);
					move_state = &movestop;
				}
			}
				
			this->upDownMuki(input->getMOUSESTATE()->mouse_y, input->getMOUSESTATE()->mouse_dy);
				

			if(input->getMOUSESTATE()->mouse_l_button_pressed) {
				if ((moveturn_state != &moveleftturn)) {
					moveturn_state->leave(this, &moveleftturn, moveturn_state);
					moveleftturn.enter(this, &moveleftturn, moveturn_state);
					moveturn_state = &moveleftturn;
				}
			} else if(input->getMOUSESTATE()->mouse_r_button_pressed) {
				if ((moveturn_state != &moverightturn)) {
					moveturn_state->leave(this, &moverightturn, moveturn_state);
					moveleftturn.enter(this, &moverightturn, moveturn_state);
					moveturn_state = &moverightturn;
				}
			} else {
				if ((moveturn_state != &movestop)) {
					moveturn_state->leave(this, &movestop, moveturn_state);
					movestop.enter(this, &movestop, moveturn_state);
					moveturn_state = &movestop;
				}
			}

			
			if (input->getKEYSTATE()['T'] & KTROBO_INPUT_BUTTON_DOWN) {
				is_fireraweapon = true;
			} else {
				is_fireraweapon = false;
			}
			if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
			if (input->getKEYSTATE()['F'] & KTROBO_INPUT_BUTTON_DOWN) {
				if (booster_state == &boostoff) {
					booster_state->leave(this, &boostoff, &boostontaiki);
					boostontaiki.enter(this,&boostoff, &boostontaiki);
					booster_state = &boostontaiki;
				} else if( booster_state == &boostontaiki) {
					booster_state->leave(this, &boostontaiki, &boostoff);
					boostoff.enter(this, &boostontaiki, &boostoff);
					booster_state = &boostoff;
					if (move_state->isBoosterHi()) {
						move_state->leave(this, &movestop, move_state);
						movestop.enter(this, &movestop, move_state);
						move_state = &movestop;
					}
				}
			}
			}

		}

			CS::instance()->leave(CS_MESSAGE_CS, "leave");
			return true;
/*	} else {
				if (move_state != &movestop) {
					move_state->leave(this, &movestop, move_state);
					movestop.enter(this, &movestop, move_state);
					move_state = &movestop;
				}
	}
	*/

	return false;

};





#define KTROBO_ID_ROBOMOVINGSTATE_STOP 1
#define KTROBO_ID_ROBOMOVINGSTATE_FORWARD 2
#define KTROBO_ID_ROBOMOVINGSTATE_BACK 3
#define KTROBO_ID_ROBOMOVINGSTATE_LEFT 4
#define KTROBO_ID_ROBOMOVINGSTATE_RIGHT 5
#define KTROBO_ID_ROBOMOVINGSTATE_LEFTTURN 6
#define KTROBO_ID_ROBOMOVINGSTATE_RIGHTTURN 7
#define KTROBO_ID_ROBOMOVINGSTATE_JUMP 8
#define KTROBO_ID_ROBOMOVINGSTATE_FORWARDJUMP 9
#define KTROBO_ID_ROBOMOVINGSTATE_BACKJUMP 10
#define KTROBO_ID_ROBOMOVINGSTATE_LEFTJUMP 11
#define KTROBO_ID_ROBOMOVINGSTATE_RIGHTJUMP 12

#define KTROBO_ID_ROBOSETTISTATE_KUUTYUU 13
#define KTROBO_ID_ROBOSETTISTATE_SETTENJYOU 14
#define KTROBO_ID_ROBOSETTISTATE_SETTI 15
#define KTROBO_ID_ROBOSETTISTATE_SETKABE 16

#define KTROBO_ID_ROBOBOOSTERSTATE_OFF 17
#define KTROBO_ID_ROBOBOOSTERSTATE_ONTAIKI 18
#define KTROBO_ID_ROBOBOOSTERSTATE_FORWARD 19
#define KTROBO_ID_ROBOBOOSTERSTATE_BACK 20
#define KTROBO_ID_ROBOBOOSTERSTATE_LEFT 21
#define KTROBO_ID_ROBOBOOSTERSTATE_RIGHT 22
#define KTROBO_ID_ROBOBOOSTERSTATE_UP 23

#define KTROBO_ID_ROBOMOVINGSTATE_LEFTFORWARD 24
#define KTROBO_ID_ROBOMOVINGSTATE_LEFTBACK 25
#define KTROBO_ID_ROBOMOVINGSTATE_RIGHTFORWARD 26
#define KTROBO_ID_ROBOMOVINGSTATE_RIGHTBACK 27

#define KTROBO_ID_ROBOMOVINGSTATE_JUMPKABE 28
#define KTROBO_ID_ROBOMOVINGSTATE_FORWARDJUMPKABE 29
#define KTROBO_ID_ROBOMOVINGSTATE_BACKJUMPKABE 30
#define KTROBO_ID_ROBOMOVINGSTATE_LEFTJUMPKABE 31
#define KTROBO_ID_ROBOMOVINGSTATE_RIGHTJUMPKABE 32

void RoboMovingState_STOP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,10,true);
}
void RoboMovingState_STOP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_STOP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_STOP;
}
void RoboMovingState_STOP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
}

void RoboMovingState_FORWARD::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_FORWARD::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_FORWARD::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_FORWARD;
}
void RoboMovingState_FORWARD::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	if (dsecond > 200) return;
	if (robo->move_state->isBoosterHi()) return;
	if ((robo->setti_state == &robo->setti) || (robo->booster_state == &robo->boostontaiki)) {
	float speed = 0.005f;
	if (robo->booster_state == &robo->boostontaiki) {
		speed *= 3.14f;
	}

	MYVECTOR3 mae(0,-1,0);
	MyVec3TransformNormal(mae,mae, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae,mae);
	mae =  mae * speed * dsecond;
	if (robo->booster_state == &robo->boostoff) {
		robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
	}
		if (robo->booster_state == &robo->boostontaiki) {
			MYVECTOR3 vec = robo->atarihan->v;
			MYVECTOR3 vec2 = vec * 0.8 + mae * speed*0.2;
			float vec2len = MyVec3Length(vec2);
			if (vec2len > speed) {
				MyVec3Normalize(vec2,vec2);
				vec2 = vec2 * speed;
			}
			robo->atarihan->setV(&vec2);

		}
	}
}





void RoboMovingState_LEFTFORWARD::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_LEFTFORWARD::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_LEFTFORWARD::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_LEFTFORWARD;
}
void RoboMovingState_LEFTFORWARD::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	if (dsecond > 200) return;
	if (robo->move_state->isBoosterHi()) return;
	if ((robo->setti_state == &robo->setti) || (robo->booster_state == &robo->boostontaiki)) {
	float speed = 0.005f;
	if (robo->booster_state == &robo->boostontaiki) {
		speed *= 3.14f;
	}
	MYVECTOR3 mae(0,-1,0);
	MyVec3TransformNormal(mae,mae, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae,mae);

	MYVECTOR3 mae2(1,0,0);
	MyVec3TransformNormal(mae2,mae2, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae2,mae2);
	mae = mae + mae2;
	MyVec3Normalize(mae,mae);

	mae =  mae * speed * dsecond;
	if (robo->booster_state == &robo->boostoff) {
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
	}
		if (robo->booster_state == &robo->boostontaiki) {
			MYVECTOR3 vec = robo->atarihan->v;
			MYVECTOR3 vec2 = vec*0.8 + mae * speed*0.2;
			float vec2len = MyVec3Length(vec2);
			if (vec2len > speed) {
				MyVec3Normalize(vec2,vec2);
				vec2 = vec2 * speed;
			}
			robo->atarihan->setV(&vec2);

		}
	
	
	}
}


void RoboMovingState_RIGHTFORWARD::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_RIGHTFORWARD::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_RIGHTFORWARD::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_RIGHTFORWARD;
}
void RoboMovingState_RIGHTFORWARD::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	if (dsecond > 200) return;
	if (robo->move_state->isBoosterHi()) return;
	if ((robo->setti_state == &robo->setti) || (robo->booster_state == &robo->boostontaiki)) {
	float speed = 0.005f;
	if (robo->booster_state == &robo->boostontaiki) {
		speed *= 3.14f;
	}
	MYVECTOR3 mae(0,-1,0);
	MyVec3TransformNormal(mae,mae, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae,mae);

	MYVECTOR3 mae2(-1,0,0);
	MyVec3TransformNormal(mae2,mae2, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae2,mae2);
	mae = mae + mae2;
	MyVec3Normalize(mae,mae);

	mae =  mae * speed * dsecond;
	if (robo->booster_state == &robo->boostoff) {
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
	}	if (robo->booster_state == &robo->boostontaiki) {
			MYVECTOR3 vec = robo->atarihan->v;
			MYVECTOR3 vec2 = vec*0.8 + mae * speed*0.2;
			float vec2len = MyVec3Length(vec2);
			if (vec2len > speed) {
				MyVec3Normalize(vec2,vec2);
				vec2 = vec2 * speed;
			}
			robo->atarihan->setV(&vec2);

		}
	}
}


void RoboMovingState_LEFTBACK::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_LEFTBACK::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_LEFTBACK::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_LEFTBACK;
}
void RoboMovingState_LEFTBACK::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	if (dsecond > 200) return;
	if (robo->move_state->isBoosterHi()) return;
	if ((robo->setti_state == &robo->setti) || (robo->booster_state == &robo->boostontaiki)) {
	float speed = 0.005f;
	if (robo->booster_state == &robo->boostontaiki) {
		speed *= 3.14f;
	}
	MYVECTOR3 mae(0,1,0);
	MyVec3TransformNormal(mae,mae, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae,mae);

	MYVECTOR3 mae2(1,0,0);
	MyVec3TransformNormal(mae2,mae2, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae2,mae2);
	mae = mae + mae2;
	MyVec3Normalize(mae,mae);

	mae =  mae * speed * dsecond;
	if (robo->booster_state == &robo->boostoff) {
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
	}	if (robo->booster_state == &robo->boostontaiki) {
			MYVECTOR3 vec = robo->atarihan->v;
			MYVECTOR3 vec2 = vec*0.8 + mae * speed*0.2;
			float vec2len = MyVec3Length(vec2);
			if (vec2len > speed) {
				MyVec3Normalize(vec2,vec2);
				vec2 = vec2 * speed;
			}
			robo->atarihan->setV(&vec2);

		}
	}
}


void RoboMovingState_RIGHTBACK::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_RIGHTBACK::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_RIGHTBACK::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_RIGHTBACK;
}
void RoboMovingState_RIGHTBACK::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	if (dsecond > 200) return;
	if (robo->move_state->isBoosterHi()) return;
	if ((robo->setti_state == &robo->setti) || (robo->booster_state == &robo->boostontaiki)) {
	float speed = 0.005f;
	if (robo->booster_state == &robo->boostontaiki) {
		speed *= 3.14f;
	}
	MYVECTOR3 mae(0,1,0);
	MyVec3TransformNormal(mae,mae, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae,mae);

	MYVECTOR3 mae2(-1,0,0);
	MyVec3TransformNormal(mae2,mae2, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae2,mae2);
	mae = mae + mae2;
	MyVec3Normalize(mae,mae);

	mae =  mae * speed * dsecond;
	if (robo->booster_state == &robo->boostoff) {
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
	}	if (robo->booster_state == &robo->boostontaiki) {
			MYVECTOR3 vec = robo->atarihan->v;
			MYVECTOR3 vec2 = vec*0.8 + mae * speed*0.2;
			float vec2len = MyVec3Length(vec2);
			if (vec2len > speed) {
				MyVec3Normalize(vec2,vec2);
				vec2 = vec2 * speed;
			}
			robo->atarihan->setV(&vec2);

		}
	}
}



void RoboMovingState_BACK::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_BACK::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_BACK::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_BACK;
}
void RoboMovingState_BACK::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	float speed = 0.005f;
	if (robo->booster_state == &robo->boostontaiki) {
		speed *= 3.14f;
	}
		if (dsecond > 200) return;
		if (robo->move_state->isBoosterHi()) return;
		if ((robo->setti_state == &robo->setti) || (robo->booster_state == &robo->boostontaiki)) {
	MYVECTOR3 mae(0,1,0);
	MyVec3TransformNormal(mae,mae, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae,mae);
	mae =  mae * speed * dsecond;
	if (robo->booster_state == &robo->boostoff) {
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
	}	
			if (robo->booster_state == &robo->boostontaiki) {
			MYVECTOR3 vec = robo->atarihan->v;
			MYVECTOR3 vec2 = vec*0.8 + mae * speed*0.2;
			float vec2len = MyVec3Length(vec2);
			if (vec2len > speed) {
				MyVec3Normalize(vec2,vec2);
				vec2 = vec2 * speed;
			}
			robo->atarihan->setV(&vec2);

		}
		
		}
}

void RoboMovingState_LEFT::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_LEFT::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_LEFT::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_LEFT;
}
void RoboMovingState_LEFT::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	float speed = 0.005f;
	if (robo->booster_state == &robo->boostontaiki) {
		speed *= 3.14f;
	}
		if (dsecond > 200) return;
		if (robo->move_state->isBoosterHi()) return;
		if ((robo->setti_state == &robo->setti) || (robo->booster_state == &robo->boostontaiki)) {
	MYVECTOR3 mae(1,0,0);
	MyVec3TransformNormal(mae,mae, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae,mae);
	mae =  mae * speed * dsecond;
	if (robo->booster_state == &robo->boostoff) {
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
	}		
			if (robo->booster_state == &robo->boostontaiki) {
			MYVECTOR3 vec = robo->atarihan->v;
			MYVECTOR3 vec2 = vec*0.8 + mae * speed*0.2;
			float vec2len = MyVec3Length(vec2);
			if (vec2len > speed) {
				MyVec3Normalize(vec2,vec2);
				vec2 = vec2 * speed;
			}
			robo->atarihan->setV(&vec2);

		}
		
		}
}

void RoboMovingState_RIGHT::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_RIGHT::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_RIGHT::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_RIGHT;
}
void RoboMovingState_RIGHT::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	float speed = 0.005f;
	if (robo->booster_state == &robo->boostontaiki) {
		speed *= 3.14f;
	}
		if (dsecond > 200) return;
		if (robo->move_state->isBoosterHi()) return;
		if ((robo->setti_state == &robo->setti) || (robo->booster_state == &robo->boostontaiki)) {
	MYVECTOR3 mae(-1,0,0);
	MyVec3TransformNormal(mae,mae, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(mae,mae);
	mae =  mae * speed * dsecond;
	if (robo->booster_state == &robo->boostoff) {
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
	}	
			if (robo->booster_state == &robo->boostontaiki) {
			MYVECTOR3 vec = robo->atarihan->v;
			MYVECTOR3 vec2 = vec*0.8 + mae * speed*0.2;
			float vec2len = MyVec3Length(vec2);
			if (vec2len > speed) {
				MyVec3Normalize(vec2,vec2);
				vec2 = vec2 * speed;
			}
			robo->atarihan->setV(&vec2);

		}
		
		}
}

void RoboMovingState_LEFTTURN::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_LEFTTURN::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_LEFTTURN::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_LEFTTURN;
}
void RoboMovingState_LEFTTURN::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	float speed = 0.0015f;
		if (dsecond > 200) return;
	float dtheta =  speed * dsecond;
	robo->atarihan->setROTXYZ(robo->atarihan->rotx, robo->atarihan->roty, robo->atarihan->rotz + dtheta);

}

void RoboMovingState_RIGHTTURN::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);

	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboMovingState_RIGHTTURN::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboMovingState_RIGHTTURN::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_RIGHTTURN;
}
void RoboMovingState_RIGHTTURN::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	float speed = -0.0015f;
		if (dsecond > 200) return;
	float dtheta =  speed * dsecond;
	robo->atarihan->setROTXYZ(robo->atarihan->rotx, robo->atarihan->roty, robo->atarihan->rotz + dtheta);

}

void RoboMovingState_JUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {

	if (before_state->isJump()) return;

	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	robo->atarihan->v = MYVECTOR3(0,0,0.019f);
	robo->jump_f_z = 0.000019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_JUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z = 0;
	robo->setkabe_count = 10000000;


}
int RoboMovingState_JUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_JUMP;
}
void RoboMovingState_JUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	t += dsecond;
		if (dsecond > 200) return;
		if (robo->setti_state == &robo->setti) {
		robo->atarihan->v = robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z*dsecond);
	}
}


void RoboMovingState_FORWARDJUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	if (before_state->isJump()) return;
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(0,-1,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->v = MYVECTOR3(0,0,0.019f)+ve * 0.02f;
	robo->jump_f_z = 0.000019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_FORWARDJUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z = 0;
	robo->setkabe_count = 10000000;
}
int RoboMovingState_FORWARDJUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_FORWARDJUMP;
}
void RoboMovingState_FORWARDJUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
		if (dsecond > 200) return;
	MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);
	if (robo->setti_state == &robo->setti) {
		robo->atarihan->v = robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z*dsecond);
	}

}


void RoboMovingState_BACKJUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	if (before_state->isJump()) return;
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(0,1,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->v = MYVECTOR3(0,0,0.019f)+ve * 0.02f;
	robo->jump_f_z = 0.000019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_BACKJUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z = 0;
	robo->setkabe_count = 10000000;
}
int RoboMovingState_BACKJUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_BACKJUMP;
}
void RoboMovingState_BACKJUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
		if (dsecond > 200) return;
		MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);
	if (robo->setti_state == &robo->setti) {
		robo->atarihan->v = robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z*dsecond);
	}
}


void RoboMovingState_LEFTJUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	if (before_state->isJump()) return;
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(1,0,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->v = MYVECTOR3(0,0,0.019f)+ve * 0.02f;
	robo->jump_f_z = 0.000019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_LEFTJUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z = 0;
		robo->setkabe_count = 10000000;
}
int RoboMovingState_LEFTJUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_LEFTJUMP;
}
void RoboMovingState_LEFTJUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
		if (dsecond > 200) return;
	MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);
	if (robo->setti_state == &robo->setti) {
		robo->atarihan->v = robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z*dsecond);
	}
}


void RoboMovingState_RIGHTJUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	if (before_state->isJump()) return;
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(-1,0,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->v = MYVECTOR3(0,0,0.019f)+ve * 0.02f;
	robo->jump_f_z = 0.000019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_RIGHTJUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z = 0;
	robo->setkabe_count = 10000000;
}
int RoboMovingState_RIGHTJUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_RIGHTJUMP;
}
void RoboMovingState_RIGHTJUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	if (dsecond > 200) return;
	MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);
	if (robo->setti_state == &robo->setti) {
		robo->atarihan->v = robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z*dsecond);
	}
}



void RoboMovingState_JUMPKABE::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {

	if (before_state->isJumpKABE()) return;

	RoboState::enter(robo,now_state,before_state);
	robo->resetSetKabe();
	robo->anime_loop_leg.setAnime(105,115,false);
	robo->atarihan->v = MYVECTOR3(0,0,0);//0.019f);
	robo->jump_f_z_kabe = 0.00019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_JUMPKABE::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z_kabe = 0;
	robo->resetSetKabe();
}
int RoboMovingState_JUMPKABE::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_JUMPKABE;
}
void RoboMovingState_JUMPKABE::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	t += dsecond;
		if (dsecond > 200) return;
		if ((robo->setkabe_count>0)&& (robo->setkabe_count < robo->KTROBO_ROBO_JUMPKABE_COUNT) &&(robo->getPressedSpaceCount() < robo->KTROBO_ROBO_JUMPKABE_COUNT/8)) {
			robo->atarihan->setV(&(robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z_kabe*dsecond)));
		}
	
}


void RoboMovingState_FORWARDJUMPKABE::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	if (before_state->isJumpKABE()) return;
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	robo->resetSetKabe();
	MYVECTOR3 ve(0,-1,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->setV(&MYVECTOR3(0,0,0.019f));//+ve * 0.02f;
	robo->jump_f_z_kabe = 0.00019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_FORWARDJUMPKABE::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z_kabe = 0;
	robo->resetSetKabe();
}
int RoboMovingState_FORWARDJUMPKABE::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_FORWARDJUMPKABE;
}
void RoboMovingState_FORWARDJUMPKABE::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
		if (dsecond > 200) return;
	MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	MYVECTOR3 ve(0,-1,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//)robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	ve = (ve*0.8 + robo->kabe_housen*0.2);
	MyVec3Normalize(ve,ve);
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	
	if ((robo->setkabe_count>0)&& (robo->setkabe_count < robo->KTROBO_ROBO_JUMPKABE_COUNT)&&(robo->getPressedSpaceCount() < robo->KTROBO_ROBO_JUMPKABE_COUNT/8)) {
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);
		robo->atarihan->setV(&(ve*0.002 + robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z_kabe*dsecond)));
	}

}


void RoboMovingState_BACKJUMPKABE::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	if (before_state->isJumpKABE()) return;
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	robo->resetSetKabe();
	MYVECTOR3 ve(0,1,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);

	robo->atarihan->setV(&MYVECTOR3(0,0,0.019f));//+ve * 0.02f;
	robo->jump_f_z_kabe = 0.00019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_BACKJUMPKABE::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z_kabe = 0;
	robo->resetSetKabe();
}
int RoboMovingState_BACKJUMPKABE::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_BACKJUMPKABE;
}
void RoboMovingState_BACKJUMPKABE::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
		if (dsecond > 200) return;
		MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
		MYVECTOR3 ve(0,1,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	ve = ve*0.8 + robo->kabe_housen*0.2;
	MyVec3Normalize(ve,ve);

	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	if ((robo->setkabe_count>0)&& (robo->setkabe_count < robo->KTROBO_ROBO_JUMPKABE_COUNT)&&(robo->getPressedSpaceCount() < robo->KTROBO_ROBO_JUMPKABE_COUNT/8)) {
		robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);
		robo->atarihan->setV(&(ve*0.002 + robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z_kabe*dsecond)));//robo->atarihan->v = robo->atarihan->v+MYVECTOR3(0,0,robo->jump_f_z_kabe*dsecond);
	}
}


void RoboMovingState_LEFTJUMPKABE::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	if (before_state->isJumpKABE()) return;
	RoboState::enter(robo,now_state,before_state);
	robo->resetSetKabe();
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(1,0,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->setV(&MYVECTOR3(0,0,0.019f));//+ve * 0.02f;
	robo->jump_f_z_kabe = 0.00019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_LEFTJUMPKABE::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z_kabe = 0;
	robo->resetSetKabe();
}
int RoboMovingState_LEFTJUMPKABE::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_LEFTJUMPKABE;
}
void RoboMovingState_LEFTJUMPKABE::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
		if (dsecond > 200) return;
	MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	if ((robo->setkabe_count>0)&& (robo->setkabe_count < robo->KTROBO_ROBO_JUMPKABE_COUNT)&&(robo->getPressedSpaceCount() < robo->KTROBO_ROBO_JUMPKABE_COUNT/8)) {
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);
	
		MYVECTOR3 ve(1,0,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	ve = ve*0.8 + robo->kabe_housen*0.2;
	MyVec3Normalize(ve,ve);
		robo->atarihan->setV(&(ve*0.002 + robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z_kabe*dsecond)));;//robo->atarihan->v = robo->atarihan->v+ MYVECTOR3(0,0,robo->jump_f_z_kabe*dsecond);
	}
}


void RoboMovingState_RIGHTJUMPKABE::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	if (before_state->isJumpKABE()) return;
	RoboState::enter(robo,now_state,before_state);
	robo->resetSetKabe();
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(-1,0,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->setV(&MYVECTOR3(0,0,0.019f));//+ve * 0.02f;
	robo->jump_f_z_kabe = 0.00019f;
	robo->atarihan->setXYZ(robo->atarihan->x, robo->atarihan->y , robo->atarihan->z + 0.01f);
}
void RoboMovingState_RIGHTJUMPKABE::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
	robo->jump_f_z_kabe = 0;
	robo->resetSetKabe();
}
int RoboMovingState_RIGHTJUMPKABE::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_RIGHTJUMPKABE;
}
void RoboMovingState_RIGHTJUMPKABE::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
		if (dsecond > 200) return;
	MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
		if ((robo->setkabe_count>0) && (robo->setkabe_count < robo->KTROBO_ROBO_JUMPKABE_COUNT)&&(robo->getPressedSpaceCount() < robo->KTROBO_ROBO_JUMPKABE_COUNT/2)) {
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);

			MYVECTOR3 ve(-1,0,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	ve = ve*0.8 + robo->kabe_housen*0.2;
	MyVec3Normalize(ve,ve);
		robo->atarihan->setV(&(ve*0.002 + robo->atarihan->v + MYVECTOR3(0,0,robo->jump_f_z_kabe*dsecond)));//robo->atarihan->v = robo->atarihan->v+ MYVECTOR3(0,0,robo->jump_f_z_kabe*dsecond);
	}
}




void RoboSETTIState_KUUTYUU::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboSETTIState_KUUTYUU::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboSETTIState_KUUTYUU::getStateID() {
	return KTROBO_ID_ROBOSETTISTATE_KUUTYUU;
}
void RoboSETTIState_KUUTYUU::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	// 空気抵抗があるので速度が減るようにしないといけない
	MYVECTOR3 newv = MYVECTOR3(robo->atarihan->v.float3.x / 1.07f, robo->atarihan->v.float3.y/1.07f, robo->atarihan->v.float3.z);
	float ff = robo->roboparam.boostercalc.getFudanSpeed();
	if (MyVec3Length(newv) < ff) {
		if (robo->booster_state == &robo->boostontaiki) {
			newv = MYVECTOR3(robo->atarihan->v.float3.x / 1.03f, robo->atarihan->v.float3.y/1.03f, robo->atarihan->v.float3.z);
		}
	}
	robo->atarihan->setV(&newv);
}


void RoboSETTIState_SETKABE::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboSETTIState_SETKABE::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboSETTIState_SETKABE::getStateID() {
	return KTROBO_ID_ROBOSETTISTATE_SETKABE;
}
void RoboSETTIState_SETKABE::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
}


void RoboSETTIState_SETTI::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboSETTIState_SETTI::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboSETTIState_SETTI::getStateID() {
	return KTROBO_ID_ROBOSETTISTATE_SETTI;
}
void RoboSETTIState_SETTI::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	// 空気抵抗があるので速度が減るようにしないといけない
	MYVECTOR3 newv = MYVECTOR3(robo->atarihan->v.float3.x / 1.07f, robo->atarihan->v.float3.y/1.07f, robo->atarihan->v.float3.z);
	robo->atarihan->setV(&newv);
}


void RoboSETTIState_SETTENJYOU::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	robo->anime_loop_leg.setAnime(10,30,true);
}
void RoboSETTIState_SETTENJYOU::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboSETTIState_SETTENJYOU::getStateID() {
	return KTROBO_ID_ROBOSETTISTATE_SETTENJYOU;
}
void RoboSETTIState_SETTENJYOU::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
}


void RoboBoosterState_OFF::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
void RoboBoosterState_OFF::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_OFF::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_OFF;
}
void RoboBoosterState_OFF::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
}

void RoboBoosterState_ONTAIKI::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
void RoboBoosterState_ONTAIKI::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_ONTAIKI::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_ONTAIKI;
}
void RoboBoosterState_ONTAIKI::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
}

void RoboBoosterState_BOOSTFORWARD::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
void RoboBoosterState_BOOSTFORWARD::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTFORWARD::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_FORWARD;
}
void RoboBoosterState_BOOSTFORWARD::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	t += dsecond;
	float te = robo->roboparam.boostercalc.getSpeed(t);
	//robo->atarihan->setV(&MYVECTOR3(robo->atarihan->v.float3.x, robo->atarihan->v.float3.y, te));
	MYVECTOR3 ve(0,-1,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	MYVECTOR3 bdayo = ve * te;

	robo->atarihan->setV(&bdayo);

	if (robo->roboparam.boostercalc.isCanMove(t)) {
		this->leave(robo, &robo->movestop, this);
		robo->movestop.enter(robo, &robo->movestop, this);
		robo->move_state = &robo->movestop;
	}
}


void RoboBoosterState_BOOSTBACK::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
void RoboBoosterState_BOOSTBACK::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTBACK::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_BACK;
}
void RoboBoosterState_BOOSTBACK::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	t += dsecond;
	float te = robo->roboparam.boostercalc.getSpeed(t);
	//robo->atarihan->setV(&MYVECTOR3(robo->atarihan->v.float3.x, robo->atarihan->v.float3.y, te));
	MYVECTOR3 ve(0,1,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	MYVECTOR3 bdayo = ve * te;

	robo->atarihan->setV(&bdayo);
	if (robo->roboparam.boostercalc.isCanMove(t)) {
		this->leave(robo, &robo->movestop, this);
		robo->movestop.enter(robo, &robo->movestop, this);
		robo->move_state = &robo->movestop;
	}
}


void RoboBoosterState_BOOSTLEFT::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
void RoboBoosterState_BOOSTLEFT::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTLEFT::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_LEFT;
}
void RoboBoosterState_BOOSTLEFT::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	t += dsecond;
	float te = robo->roboparam.boostercalc.getSpeed(t);
	//robo->atarihan->setV(&MYVECTOR3(robo->atarihan->v.float3.x, robo->atarihan->v.float3.y, te));
	MYVECTOR3 ve(1,0,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX());//robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	MYVECTOR3 bdayo = ve * te;

	robo->atarihan->setV(&bdayo);
	if (robo->roboparam.boostercalc.isCanMove(t)) {
		this->leave(robo, &robo->movestop, this);
		robo->movestop.enter(robo, &robo->movestop, this);
		robo->move_state = &robo->movestop;
	}
}


void RoboBoosterState_BOOSTRIGHT::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
void RoboBoosterState_BOOSTRIGHT::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTRIGHT::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_RIGHT;
}
void RoboBoosterState_BOOSTRIGHT::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	t += dsecond;
	float te = robo->roboparam.boostercalc.getSpeed(t);
	MYVECTOR3 ve(-1,0,0);
	MyVec3TransformNormal(ve,ve, *robo->getWorldWithoutRotX()/*atarihan->world*/);
	MyVec3Normalize(ve,ve);
	MYVECTOR3 bdayo = ve * te;

	robo->atarihan->setV(&bdayo);

	if (robo->roboparam.boostercalc.isCanMove(t)) {
		this->leave(robo, &robo->movestop, this);
		robo->movestop.enter(robo, &robo->movestop, this);
		robo->move_state = &robo->movestop;
	}
}


void RoboBoosterState_BOOSTUP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
void RoboBoosterState_BOOSTUP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTUP::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_UP;
}
void RoboBoosterState_BOOSTUP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	t += dsecond;
	float te = robo->roboparam.boostercalc.getSpeed(t);
	robo->atarihan->setV(&MYVECTOR3(robo->atarihan->v.float3.x, robo->atarihan->v.float3.y,te));

	if (robo->roboparam.boostercalc.isCanMove(t)) {
		this->leave(robo, &robo->movestop, this);
		robo->movestop.enter(robo, &robo->movestop, this);
		robo->move_state = &robo->movestop;
	}

}


bool RoboState::isJump() {
	int id = getStateID();
	if (id == KTROBO_ID_ROBOMOVINGSTATE_BACKJUMP) {
		return true;
	}
	if (id == KTROBO_ID_ROBOMOVINGSTATE_FORWARDJUMP) {
		return true;
	}
	if (id == KTROBO_ID_ROBOMOVINGSTATE_LEFTJUMP) {
		return true;
	}
	if (id == KTROBO_ID_ROBOMOVINGSTATE_RIGHTJUMP) {
		return true;
	}

	if (id == KTROBO_ID_ROBOMOVINGSTATE_JUMP) {
		return true;
	}
	return false;
}

bool RoboState::isBoosterHi() {
	int id = getStateID();

	if (id == KTROBO_ID_ROBOBOOSTERSTATE_BACK) {
		return true;
	}
	if (id == KTROBO_ID_ROBOBOOSTERSTATE_FORWARD) {
		return true;
	}
	if (id == KTROBO_ID_ROBOBOOSTERSTATE_LEFT) {
		return true;
	}
	if (id == KTROBO_ID_ROBOBOOSTERSTATE_RIGHT) {
		return true;
	}

	if (id == KTROBO_ID_ROBOBOOSTERSTATE_UP) {
		return true;
	}
	return false;
}


bool RoboState::isCanBoost(Robo* robo) {
	int id = getStateID();
	return true;
}


bool RoboState::isCanMoveWhenBoost(Robo* robo) {
	int id = getStateID();
	return false;
}


bool RoboState::isJumpKABE() {
	int id = getStateID();
	if (id == KTROBO_ID_ROBOMOVINGSTATE_BACKJUMPKABE) {
		return true;
	}
	if (id == KTROBO_ID_ROBOMOVINGSTATE_FORWARDJUMPKABE) {
		return true;
	}
	if (id == KTROBO_ID_ROBOMOVINGSTATE_LEFTJUMPKABE) {
		return true;
	}
	if (id == KTROBO_ID_ROBOMOVINGSTATE_RIGHTJUMPKABE) {
		return true;
	}

	if (id == KTROBO_ID_ROBOMOVINGSTATE_JUMPKABE) {
		return true;
	}
	return false;
}


void RoboState::enter(Robo* robo, RoboState* now_state, RoboState* before_state)
	
	{
		robo->resetCount();
	}



void Robo::fireUpdate(Graphics* g, MyTextureLoader* tex_loader, MYMATRIX* view, AtariHantei* hantei, float dt, int stamp, Game* game, KTROBO::Texture* tex) {


	if (is_fireraweapon) {
		raweapon->wf_rifle.fire(g,NULL,hantei, game->getSound(),tex,NULL,view, NULL,NULL);
	}

}


RoboBoosterCalc::RoboBoosterCalc() {
	this->robo = NULL;
	this->booster = NULL;
	this->energy_drain = 9000;
	this->fudanspeed = 0.018f;
	this->maxatospeed = 0.024f;
	this->maxspeed = 0.045f;
	this->time_backto_fudan = 1000;
	this->time_maxato_made = 700;
	this->time_maxspeed_jizoku_made = 300;
	this->time_maxspeed_made = 200;
	this->time_to_reload = 1000;
	this->time_to_canmove = 1200;
}

RoboBoosterCalc::~RoboBoosterCalc() {
}

void RoboBoosterCalc::Init(Robo* robo, RoboBooster* booster) {

	this->robo = robo;
	this->booster = booster;
	this->energy_drain = 9000;
	this->fudanspeed = 0.018f;
	this->maxatospeed = 0.024f;
	this->maxspeed = 0.025f;
	this->time_backto_fudan = 1000;
	this->time_maxato_made = 700;
	this->time_maxspeed_jizoku_made = 300;
	this->time_maxspeed_made = 200;
	this->time_to_reload = 1400;
	this->time_to_canmove = 500;
}

float RoboBoosterCalc::getSpeed(float dsecond) {
	if (time_backto_fudan < dsecond) return fudanspeed;
	if ((time_maxspeed_made < dsecond) && (time_maxspeed_jizoku_made > dsecond)) return maxspeed;
	if ((time_maxato_made < dsecond) && (time_backto_fudan > dsecond)) {

		float te = (time_backto_fudan - time_maxato_made);
		if (te < 0.00001) {
			te = 1;
		}

		return fudanspeed  + (maxatospeed - fudanspeed) * (time_backto_fudan - dsecond) / te;
	}

	if ((time_maxspeed_jizoku_made < dsecond) && (time_maxato_made > dsecond)) {
		float te = (time_maxato_made - time_maxspeed_jizoku_made);
		if (te < 0.00001) {
			te = 1;
		}

		return maxatospeed  + (maxspeed - maxatospeed) * (time_maxato_made - dsecond) / te;
	}





	return fudanspeed;

}
bool RoboBoosterCalc::isCanReload(float dsecond) {
	if (time_to_reload < dsecond) {
		return true;
	}
	return false;
}

RoboParam::RoboParam() {
	this->def = 1000;
	this->edef = 1000;
	this->maxap = 10000;
	this->allweight = 4000;
	this->canweight = 4500;
	this->amari_energy = 3400;
	this->energyshuturyoku = 7000;
	this->energy_pool = 50000;
	this->nowap = 10000;
	this->now_energy = 50000;
	robo = 0;
}

RoboParam::~RoboParam() {


}
bool RoboBoosterState_BOOSTUP::isCanBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanReload(t)) {
			return true;
		}
		return false;
}
bool RoboBoosterState_BOOSTUP::isCanMoveWhenBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanMove(t)) {
			return true;
		}
		return false;
}

bool RoboBoosterState_BOOSTFORWARD::isCanBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanReload(t)) {
			return true;
		}
		return false;
}
bool RoboBoosterState_BOOSTFORWARD::isCanMoveWhenBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanMove(t)) {
			return true;
		}
		return false;
}

bool RoboBoosterState_BOOSTRIGHT::isCanBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanReload(t)) {
			return true;
		}
		return false;
}
bool RoboBoosterState_BOOSTRIGHT::isCanMoveWhenBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanMove(t)) {
			return true;
		}
		return false;
}

bool RoboBoosterState_BOOSTLEFT::isCanBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanReload(t)) {
			return true;
		}
		return false;
}
bool RoboBoosterState_BOOSTLEFT::isCanMoveWhenBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanMove(t)) {
			return true;
		}
		return false;
}

bool RoboBoosterState_BOOSTBACK::isCanBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanReload(t)) {
			return true;
		}
		return false;
}
bool RoboBoosterState_BOOSTBACK::isCanMoveWhenBoost(Robo* robo) {
		if (robo->roboparam.boostercalc.isCanMove(t)) {
			return true;
		}
		return false;
}


void RoboParts::loadData(MyTokenAnalyzer* ma, RoboDataMetaData* meta_data) {


	data = new RoboData();
	if (strcmp(ma->Toke(),"{") != 0) {
		throw new GameError(KTROBO::FATAL_ERROR,"there is no { in init robo");
	}

	while(!ma->enddayo()) {
		ma->GetToken();
		if (strcmp(ma->Toke(), "}")==0) {
			break;
		}

		char* p = ma->Toke();
		RoboMetaDataPart* pp = meta_data->getData(p);
		if (pp) {
			if (strcmp(pp->data_type, "INT")==0) {
				this->data->setData(ma->GetIntToken(), pp->data_name,pp->data_name2,"",0);
			} else if(strcmp(pp->data_type, "STRING")==0) {
				ma->GetToken();
				this->data->setData(0,pp->data_name,pp->data_name2,ma->Toke(),0);
			} else if(strcmp(pp->data_type, "FLOAT")==0) {
				this->data->setData(0,pp->data_name,pp->data_name2,"",ma->GetFloatToken());
			} else {
				// UNKNOWN
				throw new GameError(KTROBO::FATAL_ERROR,"unknown token in init robo");
			}



		}
	}
}



void RoboParam::calcParam() {
	def = 0;
	edef = 0;
	maxap = 0;
	energyshuturyoku = 0;
	energy_pool = 0;
	int edrain = 0;
	this->allweight = 0;
	this->canweight = 0;
	this->amari_energy = 0;

	if (robo) {
		if (robo->head) {
			def += robo->head->data->getData("DEF")->int_data;
			edef += robo->head->data->getData("EDEF")->int_data;
			maxap += robo->head->data->getData("AP")->int_data;
			edrain += robo->head->data->getData("EDRAIN")->int_data;
			allweight += robo->head->data->getData("WEIGHT")->int_data;

		}

		if (robo->arm) {
			def += robo->arm->data->getData("DEF")->int_data;
			edef += robo->arm->data->getData("EDEF")->int_data;
			maxap += robo->arm->data->getData("AP")->int_data;
			edrain += robo->arm->data->getData("EDRAIN")->int_data;
			allweight += robo->arm->data->getData("WEIGHT")->int_data;
		}

		if (robo->leg) {
			def += robo->leg->data->getData("DEF")->int_data;
			edef += robo->leg->data->getData("EDEF")->int_data;
			maxap += robo->leg->data->getData("AP")->int_data;
			edrain += robo->leg->data->getData("EDRAIN")->int_data;
			allweight += robo->leg->data->getData("WEIGHT")->int_data;
			canweight = robo->leg->data->getData("LOADMAX")->int_data;
		}

		if (robo->body) {
			def += robo->body->data->getData("DEF")->int_data;
			edef += robo->body->data->getData("EDEF")->int_data;
			maxap += robo->body->data->getData("AP")->int_data;
			edrain += robo->body->data->getData("EDRAIN")->int_data;
			allweight += robo->body->data->getData("WEIGHT")->int_data;
		}

		if (robo->booster) {
			
			edrain += robo->booster->data->getData("EDRAIN")->int_data;
			allweight += robo->booster->data->getData("WEIGHT")->int_data;
		}

		if (robo->fcs) {
			edrain += robo->fcs->data->getData("EDRAIN")->int_data;
			allweight += robo->fcs->data->getData("WEIGHT")->int_data;
		}

		if (robo->engine) {
			this->energyshuturyoku = robo->engine->data->getData("EPOWER")->int_data;
			this->energy_pool = robo->engine->data->getData("EPOOL")->int_data;
			allweight += robo->engine->data->getData("WEIGHT")->int_data;
		}

		if (robo->raweapon) {
			edrain += robo->raweapon->data->getData("EDRAIN")->int_data;
			allweight += robo->raweapon->data->getData("WEIGHT")->int_data;
		}

		if (robo->laweapon) {
			edrain += robo->laweapon->data->getData("EDRAIN")->int_data;
			allweight += robo->laweapon->data->getData("WEIGHT")->int_data;
		}

		if (robo->rsweapon) {
			edrain += robo->rsweapon->data->getData("EDRAIN")->int_data;
			allweight += robo->rsweapon->data->getData("WEIGHT")->int_data;
		}

		if (robo->lsweapon) {
			edrain += robo->lsweapon->data->getData("EDRAIN")->int_data;
			allweight += robo->lsweapon->data->getData("WEIGHT")->int_data;
		}




	}
	amari_energy = this->energyshuturyoku - edrain;

	nowap = maxap;
	now_energy = energy_pool;
}

int RoboParam::getMaxAP() {

	return this->maxap;

}


int RoboParam::getAllWeight() {
	return this->allweight;

}

int RoboParam::getCanWeight() {

	return this->canweight;

}


int RoboParam::getDef() {
	return this->def;

}

int RoboParam::getEDef() {
	return this->edef;

}

int RoboParam::getAmariEnergy() {

	return this->amari_energy;

}


int RoboParam::getEnergyShuturyoku() {
	return this->energyshuturyoku;

}

int RoboParam::getEnergyPool() {

	return this->energy_pool;


}


char* RoboParam::getNameOfHead() {
	if (!robo) return "なし";
	if (this->robo->head) {
		return this->robo->head->data->getData("name")->string_data;
	}
	return "なし";


}


char* RoboParam::getNameOfBody() {
	if (!robo) return "なし";
	if (this->robo->body) {
		return this->robo->body->data->getData("name")->string_data;
	}
	return "なし";
}


char* RoboParam::getNameOfArm() {
	if (!robo) return "なし";
	if (this->robo->arm) {
		return this->robo->arm->data->getData("name")->string_data;
	}
	return "なし";


}


char* RoboParam::getNameOfLeg() {
	if (!robo) return "なし";
	if (this->robo->leg) {
		return this->robo->leg->data->getData("name")->string_data;
	}
	return "なし";


}


char* RoboParam::getNameOfInside() {
	if (!robo) return "なし";
	if (this->robo->iweapon) {
		return this->robo->iweapon->data->getData("name")->string_data;
	}
	return "なし";


}


char* RoboParam::getNameOfRArmWeapon() {
	if (!robo) return "なし";
	if (this->robo->raweapon) {
		return this->robo->raweapon->data->getData("name")->string_data;
	}
	return "なし";

}

char* RoboParam::getNameOfLArmWeapon() {
	if (!robo) return "なし";
	if (this->robo->laweapon) {
		return this->robo->laweapon->data->getData("name")->string_data;
	}
	return "なし";

}


char* RoboParam::getNameOfRShoulderWeapon() {
	if (!robo) return "なし";
	if (this->robo->rsweapon) {
		return this->robo->rsweapon->data->getData("name")->string_data;
	}
	return "なし";
	
}

char* RoboParam::getNameOfLShoulderWeapon() {
	if (!robo) return "なし";
	if (this->robo->lsweapon) {
		return this->robo->lsweapon->data->getData("name")->string_data;
	}
	return "なし";
}



char* RoboParam::getNameOfBooster() {
	if (!robo) return "なし";
	if (this->robo->booster) {
		return this->robo->booster->data->getData("name")->string_data;
	}
	return "なし";

}

char* RoboParam::getNameOfFCS() {
	if (!robo) return "なし";
	if (this->robo->fcs) {
		return this->robo->fcs->data->getData("name")->string_data;
	}
	return "なし";

}

char* RoboParam::getNameOfEngine() {
	if (!robo) return "なし";
	if (this->robo->engine) {
		return this->robo->engine->data->getData("name")->string_data;
	}
	return "なし";

}

RoboDataPart RoboData::emptydata;