#include "KTRoboRobo.h"
#include "KTRoboGameError.h"
#include "KTRoboLockOnSystem.h"

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
	move_state = &movestop;
	setti_state = &kuutyuu;
	booster_state = &boostoff;
	settenjyou_state = &kuutyuu;
	setkabe_state = &kuutyuu;
	resetCount();
	ap = 0;
}


Robo::~Robo(void)
{
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



}

void Robo::atarishori(Graphics* g, MYMATRIX* view,  AtariHantei* hantei, float dt, int stamp) {

	AtariUnitAnsKWSK ans[128];
	static float vdayo =0;
	static int iunko = 8;
	bool setdayo = false;
	move_state->exec(g, this, dt, stamp);



	int atari_num = hantei->getAns(ans,atarihan,atari_leg,128);
	if (atari_num > 0 ) {
		for (int i=0;i<atari_num;i++) {
			if (ans[i].aite_type == AtariUnit::AtariType::ATARI_TIKEI) {
				if ((ans[i].ans->atari_idx == atari_leg->getUMESHID()) || (ans[i].ans->atari_idx2 == atari_leg->getUMESHID())) {
				if (atari_leg && atari_leg->mesh && ((ans[i].ans->obbidx == atari_leg->bone_obbs_idx[atari_leg->mesh->BoneIndexes["rightLegDownBone"]]) ||
					(ans[i].ans->obbidx2 == atari_leg->bone_obbs_idx[atari_leg->mesh->BoneIndexes["rightLegDownBone"]]))) {
					OBB* ob = &atari_leg->bone_obbs[atari_leg->mesh->BoneIndexes["rightLegDownBone"]];
					MYVECTOR3 a;
					MyVec3Subtract(a,ob->c, ans[i].ans->kouten_jyusin);
					float f = min((ob->e[0]/2 + ob->e[1]/2 + ob->e[2]/2)/3,abs(MyVec3Length(a) - abs(MyVec3Dot(ob->u[0],ans[i].ans->kouten_housen) * ob->e[0])
						- abs(MyVec3Dot(ob->u[1], ans[i].ans->kouten_housen)) * ob->e[1]
					- abs(MyVec3Dot(ob->u[2], ans[i].ans->kouten_housen)) * ob->e[2]));
					
					if (ans[i].ans->kouten_housen.float3.z > 0) {
					atarihan->setXYZ(atarihan->x, atarihan->y, atarihan->z + ans[i].ans->kouten_housen.float3.z * 1.1f*f*f);
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
				
				if (atari_leg && atari_leg->mesh && ((ans[i].ans->obbidx == atari_leg->bone_obbs_idx[atari_leg->mesh->BoneIndexes["leftLegDownBone"]]) ||
					(ans[i].ans->obbidx2 == atari_leg->bone_obbs_idx[atari_leg->mesh->BoneIndexes["leftLegDownBone"]]))) {
			
					
					OBB* ob = &atari_leg->bone_obbs[atari_leg->mesh->BoneIndexes["leftLegDownBone"]];
					MYVECTOR3 a;
					MyVec3Subtract(a,ob->c, ans[i].ans->kouten_jyusin);
					float f = min((ob->e[0]/2 + ob->e[1]/2 + ob->e[2]/2)/3,abs(MyVec3Length(a) - abs(MyVec3Dot(ob->u[0],ans[i].ans->kouten_housen) * ob->e[0])
						- abs(MyVec3Dot(ob->u[1], ans[i].ans->kouten_housen)) * ob->e[1]
					- abs(MyVec3Dot(ob->u[2], ans[i].ans->kouten_housen)) * ob->e[2]));
					if (ans[i].ans->kouten_housen.float3.z > 0) {
					atarihan->setXYZ(atarihan->x, atarihan->y, atarihan->z + ans[i].ans->kouten_housen.float3.z * 1.1f*f*f);
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

	if (setdayo == false) {

		// 空中にいる
		if (dt <200) {
		vdayo = atarihan->v.float3.z;
		vdayo = vdayo - 0.00000098*dt;
		float xx = vdayo * dt;
		atarihan->setDT(dt);
		atarihan->setXYZ(atarihan->x, atarihan->y, atarihan->z + xx);
		atarihan->setV(&MYVECTOR3(atarihan->v.float3.x,atarihan->v.float3.y,vdayo));
		setti_state = &kuutyuu;
		incKuutyuu();
		}


	} else {
		incSetti();
		if (move_state == &movestop) {
			atarihan->setV(&MYVECTOR3(0,0,0));
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
			
			arm->larm->animate(40,true);
			if (ap->positionArm3(g,&temp,this, &te/*MYVECTOR3(40*cos(test),-80+40*sin(test)
													 ,80+40*sin(test))*/, false)) {
			arm->larm->animate(40,false);//false);
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
			}

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
					
					r = iunko % 8;
				
					
					te = los.getPosOfStudyPoint(r, 10,100,6,10,6,10,300,300,100);
					MyVec3TransformCoord(te,te,atarihan->world);
					OBB ob;
					ob.c = te;
					te = los.getPosOfStudyPoint(r, 10,100,6,10,6,10,300,300,100);
					g->drawOBBFill(g,0xFFFF0000,&world,view,g->getProj(),&ob);

					
					ArmPoint app = posit.points[r];
					app.is_ok = true;
					//getPoint(&te);
					if (app.is_ok) {
						ap->setTheta(app.dthetaxa,app.dthetaxb,app.dthetaya, app.dthetayb, app.dthetaza, app.dthetazb);
					}
					
				
				
				
				
				
				} else {
				te = los.getPosOfStudyPoint(iunko, 10,100,6,10,6,10,300,300,500);
				MyVec3TransformCoord(te,te,atarihan->world);
				OBB ob;
				ob.c = te;
				te = los.getPosOfStudyPoint(iunko, 10,100,6,10,6,10,300,300,500);
				g->drawOBBFill(g,0xFFFF0000,&world,view,g->getProj(),&ob);





				}
			

				

					
		if (ap->positionArm3(g,&temp,this,&te/*MYVECTOR3(40*cos(test),-80+40*sin(test)
											,80+40*sin(test))*/, true) != KTROBO_ARMPOSITION_DAME) {
												
												 
							
												 ap->setArm3(this,true, arm->rarm->Bones[arm->rarm->BoneIndexes["uparmBone"]],
													 arm->rarm->Bones[arm->rarm->BoneIndexes["downarmBone"]]);
			
												 
												 
												 
												 arm->rarm->animate(40,false);
												 ArmPoint app;
												 app.pos = te;
												 ap->getTheta(&app);
											
												 posit.setPoint(iunko,&app);
												 iunko++;
												 
												 
												 break;
		} else {
		//	ap->resetTheta();
		//			 ap->setArm3(this,true, arm->rarm->Bones[arm->rarm->BoneIndexes["uparmBone"]],
			//										 arm->rarm->Bones[arm->rarm->BoneIndexes["downarmBone"]]);
			
												 
												 
												 
											 arm->rarm->animate(40,false);
		}
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

	atarihan->calcJyusinAndR();
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


	// atarihanのumeshをつくる
	MYMATRIX wor;
	MyMatrixIdentity(wor);
	if (body->body) {
	UMesh* um_core = new UMesh(g,body->body->filename, tex_loader,
		body->body,true, &wor,NULL,KTROBO_MESH_BONE_NULL,false);
	atarihan->setUMesh(um_core);
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
			arm->rarm->animate(40,true);


	}
	if (arm->larm) {
	UMesh* um_larm = new UMesh(g, arm->larm->filename, tex_loader,
		arm->larm, false, &wor,  body->body->Bones[body->body->BoneIndexes["leftArmJointBone"]],body->body->BoneIndexes["leftArmJointBone"],false);
	atarihan->setUMesh(um_larm);
		MyMatrixScaling(arm->larm->rootbone_matrix_local_kakeru,1.1,1.1,1.1);
		arm->larm->animate(40,true);
	}

	if (head->head) {
	UMesh* um_head = new UMesh(g, head->head->filename, tex_loader,
		head->head, false, &wor, body->body->Bones[body->body->BoneIndexes["headJointBone"]],body->body->BoneIndexes["headJointBone"],false);
	atarihan->setUMesh(um_head);
	}

	if (head->head2) {
	UMesh* um_head2 = new UMesh(g, head->head2->filename, tex_loader,
		head->head2, false, &wor, body->body->Bones[body->body->BoneIndexes["headJointBone"]],body->body->BoneIndexes["headJointBone"],false);
	atarihan->setUMesh(um_head2);
	}

	if (head->head3) {
	UMesh* um_head3 = new UMesh(g, head->head3->filename, tex_loader,
		head->head3, false, &wor, body->body->Bones[body->body->BoneIndexes["headJointBone"]],body->body->BoneIndexes["headJointBone"],false);
	atarihan->setUMesh(um_head3);
	}

	float frames[16];
	bool matrs[16];
	for (int i=0;i<16;i++) {
		frames[i] = 0;
		matrs[i] = true;
	}

	atarihan->calcAnimeFrame(atarihan->meshs.size(),frames,matrs);
	hantei->setUMeshUnit(atarihan,AtariUnit::AtariType::ATARI_CHARA);
	atarihan->setDT(0);
	atarihan->setROTXYZ(0,0,0);
	atarihan->setSCALEXYZ(1,1,1);
	atarihan->setV(&MYVECTOR3(0,0,0));





	atarihan->setXYZ(5,3,5.5);
	atarihan->calcJyusinAndR();
	anime_loop_leg.setAnime(10,30,true);
	anime_loop_leg.setTimeAndSpeed(0.01,0);
//	world = atarihan->world;
	ap = new ArmPositioner(3.14/60000,3.14/3,0.62);
	ap->setTheta(0.96429, -0.92417,0.1193,0,0.20,0.19);
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
			if (input->getKEYSTATE()['W'] & KTROBO_INPUT_BUTTON_PRESSED) {

				if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {

					if ((move_state != &movejumpforward) && (setti_state != &kuutyuu)) {
						move_state->leave(this, &movejumpforward, move_state);
						movejumpforward.enter(this, &movejumpforward, move_state);
						move_state = &movejumpforward;
					}

				} else {

					if ((move_state != &moveforward) && ((move_state != &movejumpforward) 
						|| ((move_state == &movejumpforward) && kuutyuu_count>3)
						)) {
						move_state->leave(this, &moveforward, move_state);
						moveforward.enter(this, &moveforward, move_state);
						move_state = &moveforward;
					}
				}
			} else if(input->getKEYSTATE()['A'] & KTROBO_INPUT_BUTTON_PRESSED) {
				if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {

					if ((move_state != &movejumpleft) && (setti_state != &kuutyuu)) {
						move_state->leave(this, &movejumpleft, move_state);
						movejumpleft.enter(this, &movejumpleft, move_state);
						move_state = &movejumpleft;
					}

				} else {

				if ((move_state != &moveleft) && ((move_state != &movejumpleft)
					
					
					|| ((move_state == &movejumpleft) && kuutyuu_count>3)
						)) {
					
					move_state->leave(this, &moveleft, move_state);
					moveleft.enter(this, &moveleft, move_state);
					move_state = &moveleft;
				}
				}
			} else if(input->getKEYSTATE()['D'] & KTROBO_INPUT_BUTTON_PRESSED) {
				if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {

					if ((move_state != &movejumpright) && (setti_state != &kuutyuu)) {
						move_state->leave(this, &movejumpright, move_state);
						movejumpright.enter(this, &movejumpright, move_state);
						move_state = &movejumpright;
					}

				} else {
				if ((move_state != &moveright) && ((move_state != &movejumpright)
					
					|| ((move_state == &movejumpright) && kuutyuu_count>3)
						)) {
					
					
					
					
					move_state->leave(this, &moveright, move_state);
					moveright.enter(this, &moveright, move_state);
					move_state = &moveright;
				}
				}
			} else if(input->getKEYSTATE()['S'] & KTROBO_INPUT_BUTTON_PRESSED) {
				if (input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {

					if ((move_state != &movejumpback)  && (setti_state != &kuutyuu)) {
						move_state->leave(this, &movejumpback, move_state);
						movejumpback.enter(this, &movejumpback, move_state);
						move_state = &movejumpback;
					}

				} else {
				if ((move_state != &moveback) && ((move_state != &movejumpback)
					
					|| ((move_state == &movejumpback) && kuutyuu_count>3)
						)) {
					
					move_state->leave(this, &moveback, move_state);
					moveback.enter(this, &moveback, move_state);
					move_state = &moveback;
				}
				}
			} else if(input->getKEYSTATE()['E'] & KTROBO_INPUT_BUTTON_PRESSED) {
				if ((move_state != &moveleftturn)) {
					move_state->leave(this, &moveleftturn, move_state);
					moveleftturn.enter(this, &moveleftturn, move_state);
					move_state = &moveleftturn;
				}
			} else if(input->getKEYSTATE()['Q'] & KTROBO_INPUT_BUTTON_PRESSED) {
				if ((move_state != &moverightturn)) {
					move_state->leave(this, &moverightturn, move_state);
					moveleftturn.enter(this, &moverightturn, move_state);
					move_state = &moverightturn;
				}
			} else if(input->getKEYSTATE()[VK_SPACE] & KTROBO_INPUT_BUTTON_PRESSED) {
				if (((move_state != &movejump) && (move_state != &movejumpforward) &&
					(move_state != &movejumpback) && (move_state != &movejumpleft) &&
					(move_state != &movejumpright))  && (setti_state != &kuutyuu)) {
					move_state->leave(this, &movejump, move_state);
					movejump.enter(this, &movejump, move_state);
					move_state = &movejump;
				}
			} else {
				if ((move_state != &movestop)) {
					move_state->leave(this, &movestop, move_state);
					movestop.enter(this, &movestop, move_state);
					move_state = &movestop;
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

	if (robo->setti_state == &robo->setti) {
	float speed = 0.005f;
	MYVECTOR3 mae(0,-1,0);
	MyVec3TransformNormal(mae,mae, robo->atarihan->world);
	MyVec3Normalize(mae,mae);
	mae =  mae * speed * dsecond;
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
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
		if (robo->setti_state == &robo->setti) {
	MYVECTOR3 mae(0,1,0);
	MyVec3TransformNormal(mae,mae, robo->atarihan->world);
	MyVec3Normalize(mae,mae);
	mae =  mae * speed * dsecond;
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
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
		if (robo->setti_state == &robo->setti) {
	MYVECTOR3 mae(1,0,0);
	MyVec3TransformNormal(mae,mae, robo->atarihan->world);
	MyVec3Normalize(mae,mae);
	mae =  mae * speed * dsecond;
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
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
		if (robo->setti_state == &robo->setti) {
	MYVECTOR3 mae(-1,0,0);
	MyVec3TransformNormal(mae,mae, robo->atarihan->world);
	MyVec3Normalize(mae,mae);
	mae =  mae * speed * dsecond;
	robo->atarihan->setXYZ(robo->atarihan->x+ mae.float3.x, robo->atarihan->y+mae.float3.y , robo->atarihan->z+ mae.float3.z);
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
	float speed = -0.0015f;
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
	float speed = 0.0015f;
	float dtheta =  speed * dsecond;
	robo->atarihan->setROTXYZ(robo->atarihan->rotx, robo->atarihan->roty, robo->atarihan->rotz + dtheta);

}

void RoboMovingState_JUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	robo->atarihan->v = MYVECTOR3(0,0,0.0019f);
}
void RoboMovingState_JUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
int RoboMovingState_JUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_JUMP;
}
void RoboMovingState_JUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	t += dsecond;
}


void RoboMovingState_FORWARDJUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(0,-1,0);
	MyVec3TransformNormal(ve,ve, robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->v = MYVECTOR3(0,0,0.0019f)+ve * 0.005f;
}
void RoboMovingState_FORWARDJUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
int RoboMovingState_FORWARDJUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_FORWARDJUMP;
}
void RoboMovingState_FORWARDJUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {

	MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);


}


void RoboMovingState_BACKJUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(0,1,0);
	MyVec3TransformNormal(ve,ve, robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->v = MYVECTOR3(0,0,0.0019f)+ve * 0.005f;
}
void RoboMovingState_BACKJUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
int RoboMovingState_BACKJUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_BACKJUMP;
}
void RoboMovingState_BACKJUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
		MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);

}


void RoboMovingState_LEFTJUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(1,0,0);
	MyVec3TransformNormal(ve,ve, robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->v = MYVECTOR3(0,0,0.0019f)+ve * 0.005f;
}
void RoboMovingState_LEFTJUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
int RoboMovingState_LEFTJUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_LEFTJUMP;
}
void RoboMovingState_LEFTJUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);
	
}


void RoboMovingState_RIGHTJUMP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
	RoboState::enter(robo,now_state,before_state);
	robo->anime_loop_leg.setAnime(105,115,false);
	MYVECTOR3 ve(-1,0,0);
	MyVec3TransformNormal(ve,ve, robo->atarihan->world);
	MyVec3Normalize(ve,ve);
	robo->atarihan->v = MYVECTOR3(0,0,0.0019f)+ve * 0.005f;
}
void RoboMovingState_RIGHTJUMP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
	t = 0;
}
int RoboMovingState_RIGHTJUMP::getStateID() {
	return KTROBO_ID_ROBOMOVINGSTATE_RIGHTJUMP;
}
void RoboMovingState_RIGHTJUMP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
	MYVECTOR3 pos(robo->atarihan->x, robo->atarihan->y, robo->atarihan->z);
	MYVECTOR3 dpos = robo->atarihan->v * dsecond;
	dpos.float3.z = 0; // z関係はあたり処理で
	pos = pos + dpos;
	robo->atarihan->setXYZ(pos.float3.x,pos.float3.y, pos.float3.z);
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
}
void RoboBoosterState_BOOSTFORWARD::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTFORWARD::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_FORWARD;
}
void RoboBoosterState_BOOSTFORWARD::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
}


void RoboBoosterState_BOOSTBACK::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
void RoboBoosterState_BOOSTBACK::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTBACK::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_BACK;
}
void RoboBoosterState_BOOSTBACK::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
}


void RoboBoosterState_BOOSTLEFT::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
void RoboBoosterState_BOOSTLEFT::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTLEFT::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_LEFT;
}
void RoboBoosterState_BOOSTLEFT::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
}


void RoboBoosterState_BOOSTRIGHT::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
void RoboBoosterState_BOOSTRIGHT::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTRIGHT::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_RIGHT;
}
void RoboBoosterState_BOOSTRIGHT::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
}


void RoboBoosterState_BOOSTUP::enter(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
void RoboBoosterState_BOOSTUP::leave(Robo* robo, RoboState* now_state, RoboState* before_state) {
}
int RoboBoosterState_BOOSTUP::getStateID() {
	return KTROBO_ID_ROBOBOOSTERSTATE_UP;
}
void RoboBoosterState_BOOSTUP::exec(Graphics* g, Robo* robo, float dsecond, int stamp) {
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


void RoboState::enter(Robo* robo, RoboState* now_state, RoboState* before_state)
	
	{
		robo->resetCount();
	}