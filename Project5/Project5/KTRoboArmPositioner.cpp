#include "KTRoboArmPositioner.h"
//#include "MyBillBoard.h"
//#include "NEWGUI_OBSERVER.h"
#include "KTRoboRobo.h"
//#include "MyCameraAndBaseLine.h"
#include "MyGyouretuKeisan.h"
#include <iostream>
#include <cmath>
#include "KTRoboLockOnSystem.h"
#include "KTRoboLog.h"
#include <string>
#include "MyTokenAnalyzer.h"
#include "KTRoboDebugText.h"

using namespace KTROBO;

ArmPositioner::~ArmPositioner(void)
{
}


bool ArmPositioner::positionArm(Graphics* g , MYMATRIX* view,  float framecnt, Robo* robo, MYVECTOR3* mokuhyou, bool is_migi) {
	MeshBone* arm1;
	MeshBone* arm2;
	MeshBone* handjoint;

	if (is_migi) {
		arm1 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["handBone"]];
	} else {
		arm1 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["handBone"]];
	}

	MYMATRIX maat;
	if (is_migi) {
	MyMatrixRotationX(maat,-1.57);
	MYMATRIX tmp;
	MyMatrixRotationZ(tmp, 1.57*2);
	MyMatrixMultiply(maat, maat, tmp);
	} else {
		MyMatrixRotationX(maat,-1.57);
		MYMATRIX tmp;
		MyMatrixRotationZ(tmp, -1.57*2);
		MyMatrixMultiply(maat, maat, tmp);
	}
	arm1->offset_matrix = maat;
	if(is_migi) {
	MyMatrixRotationZ(maat,1.57);
	} else {
		MyMatrixRotationZ(maat,1.57);
	}
	
	arm2->offset_matrix = maat;
	if (!is_migi) {
	robo->arm->larm->animate(framecnt,false);
	} else {

	robo->arm->rarm->animate(framecnt,false);
	}

	// 各ボーンの位置を求めて長さを求める
	MYVECTOR3 arm1_pos(0,0,0);
	MYVECTOR3 arm2_pos(0,0,0);
	MYVECTOR3 joint_pos(0,0,0);

	MYMATRIX mma;
	MyMatrixMultiply(mma,   arm1->matrix_local,arm1->parent_bone->combined_matrix);
	MYMATRIX mmt;
	//MyMatrixTranspose(mmt, mma);
	mmt = mma;
	MyVec3TransformCoord(arm1_pos, arm1_pos,mmt);
		
	MyMatrixMultiply(mma ,arm2->matrix_local, arm2->parent_bone->combined_matrix);
	//MyMatrixTranspose(mmt, mma);
	mmt = mma;
	MyVec3TransformCoord(arm2_pos, arm2_pos,mmt);
	
	MyMatrixMultiply(mma  , handjoint->matrix_local, handjoint->parent_bone->combined_matrix);
	//MyMatrixTranspose(mmt, mma);
	mmt = mma;
	MyVec3TransformCoord(joint_pos, joint_pos,mmt);

	OBB rec;
	rec.c = arm1_pos;
	rec.e = MYVECTOR3(1.05f,1.05f,1.05f);
	MYMATRIX ww;
	MyMatrixIdentity(ww);
	g->drawOBB(g, 0xFFFF0000, &ww,view,g->getProj(),&rec);
	rec.c = arm2_pos;
	g->drawOBB(g, 0xFF00FF00, &ww,view,g->getProj(),&rec);
	rec.c = joint_pos;
	g->drawOBB(g, 0xFF0000FF, &ww, view, g->getProj(), &rec);




	/*
	MyBillBoard b7 = MyBillBoard();
	b7.setTex(MyTextureLoader::loadClass(NEWGUI_PNG));
	MYRECT r;
	r.left = 0;
	r.right = 64;
	r.top = 0;
	r.bottom = 64;



	b7.setWidthHeightANDRECT(0.3,0.3,&r);

	MYMATRIX proj;
	float w = g->getWidth();
	float h3 = g->getHeight();
	MyMatrixPerspectiveFovRH(&proj,1,w/h3,1,1000);
	MYMATRIX world;
	MyMatrixTranslation(&world, arm1_pos.x,arm1_pos.y,arm1_pos.z);
	b7.render(g->getDevice(),&world, MyCameraAndBaseLine::Instance()->getCameraMat(), &proj,MyCameraAndBaseLine::Instance()->getFrom(),
		MyCameraAndBaseLine::Instance()->getAt());

	MyMatrixTranslation(&world, arm2_pos.x,arm2_pos.y,arm2_pos.z);
	b7.render(g->getDevice(),&world, MyCameraAndBaseLine::Instance()->getCameraMat(), &proj,MyCameraAndBaseLine::Instance()->getFrom(),
		MyCameraAndBaseLine::Instance()->getAt());

	MyMatrixTranslation(&world, joint_pos.x,joint_pos.y,joint_pos.z);
	b7.render(g->getDevice(),&world, MyCameraAndBaseLine::Instance()->getCameraMat(), &proj,MyCameraAndBaseLine::Instance()->getFrom(),
		MyCameraAndBaseLine::Instance()->getAt());
*/
	MYVECTOR3 tt = arm1_pos - arm2_pos;
	float a = MyVec3Length(tt);
	tt = arm2_pos - joint_pos;
	float b = MyVec3Length(tt);
	arm2_pos = arm2_pos - arm1_pos;
	joint_pos = joint_pos - arm1_pos;
	*mokuhyou =  *mokuhyou- arm1_pos;
	MYVECTOR3 mokuhyou_to_arm1 =  - *mokuhyou ;
	float L = MyVec3Length(mokuhyou_to_arm1);

	MYVECTOR3 suityoku_to_z_and_mta;
	tt = MYVECTOR3(0,0,1);
	MyVec3Cross(suityoku_to_z_and_mta, mokuhyou_to_arm1, tt);
	MyVec3Normalize(suityoku_to_z_and_mta, suityoku_to_z_and_mta);

	if (is_migi) {
	} else {
		suityoku_to_z_and_mta = suityoku_to_z_and_mta *  -1;
	}
	float leng = nobabairitu * (a + b);

	float kyori = L - leng;
	MYVECTOR3 zdayo(0,0,1);
	MYVECTOR3 arm_saki = *mokuhyou * leng / (float)L + suityoku_to_z_and_mta * kyori * tan(theta)* cos(theta2) -
		zdayo * kyori * tan(theta) * sin(theta2);

	float l = MyVec3Length(arm_saki);
	if (l > a+b) {
		return false;
	}
	float s = (a+ b+ l)/2;
	float S = sqrt(s* (s-a) * (s-b)* (s-l));
	float h = 2 * S / l;
	if (abs(h/a) > 1 || abs(h/b) > 1) {
		return false;
	}
	float alpha = asin(h/a);
	float beta = acos(h/a);
	float alpha2 = asin(h/b);
	float beta2 = acos(h/b);

	

	MYVECTOR3 motomeru_b;
	MYVECTOR3 b_from_mokuhyou = (arm_saki - *mokuhyou);
	MyVec3Normalize(b_from_mokuhyou,b_from_mokuhyou);
	motomeru_b = b_from_mokuhyou* b + arm_saki;


	if (abs(motomeru_b.float3.y/a) > 1) {
		return false;
	}
	float pusai = asin(-motomeru_b.float3.y /a);
	if (cos(pusai) ==0) {
		return false;
	}

	if (abs(motomeru_b.float3.x/(a*cos(pusai))) > 1) {
		return false;
	}

	float epsiron;
	epsiron = asin(motomeru_b.float3.x/(a*cos(pusai)));

	// pusai epsiron が求まったので適用する
	
	MYMATRIX mbbt;
	MYVECTOR3 yd(0,1,0);
	MYVECTOR3 xd(1,0,0);
	MYVECTOR3 zd(0,0,1);
	MYMATRIX cc;
	
	if (is_migi) {
	//	MyMatrixTranspose(cc, (arm1->combined_matrix));
		
		cc = arm1->combined_matrix;
		MyVec3TransformNormal(yd, yd, cc);
		//D3DXVec3TransformNormal(&xd, &xd, &cc);
	//	D3DXVec3TransformNormal(&zd, &zd, &cc);
	} else {
	//	MyMatrixTranspose(cc, (arm1->combined_matrix));
		cc = arm1->combined_matrix;
		MyVec3TransformNormal(yd, yd, cc);
		//D3DXVec3TransformNormal(&xd, &xd, &cc);
	//	D3DXVec3TransformNormal(&zd, &zd, &cc);
	}
	
	
	MyMatrixRotationAxis(maat, yd, epsiron);
	MyMatrixRotationAxis(mbbt, zd, pusai);

	MyMatrixMultiply(maat, maat, mbbt);

	//MYMATRIX aar;
	//MyMatrixRotationY(&aar, 3.14);
	//MyMatrixMultiply(&arm1->offset_matrix, &aar, &maat);
	
	
	MYMATRIX maat2;
	if (is_migi) {
	MyMatrixRotationX(maat2,-1.57);
		MYMATRIX tmp;
	MyMatrixRotationZ(tmp, 1.57*2);
	MyMatrixMultiply(maat2, maat2, tmp);
	} else {
		MyMatrixRotationX(maat2,-1.57);
			MYMATRIX tmp;
	MyMatrixRotationZ(tmp, -1.57*2);
	MyMatrixMultiply(maat2, maat2, tmp);
	}
	MyMatrixMultiply(maat, maat, maat2);
	


	arm1->offset_matrix = maat;

	yd = MYVECTOR3(0,1,0);
	xd = MYVECTOR3(1,0,0);
	zd = MYVECTOR3(0,0,1);
	/*
	if (is_migi) {
		D3DXMatrixTranspose(&cc, &(arm2->parent->combined_matrix * arm2->matrix_local));
		D3DXVec3TransformNormal(&yd, &(yd), &cc);
		D3DXVec3TransformNormal(&xd, &(xd), &cc);
		D3DXVec3TransformNormal(&zd, &zd, &cc);
	} else {
		D3DXMatrixTranspose(&cc, &(arm2->parent->combined_matrix * arm2->matrix_local));
		D3DXVec3TransformNormal(&yd, &(yd), &cc);
		D3DXVec3TransformNormal(&xd, &(xd), &cc);
		D3DXVec3TransformNormal(&zd, &zd, &cc);
	}
	*/
	MyMatrixRotationAxis(mbbt, xd,  alpha + alpha2- 3.1415926535/2);

	if(is_migi) {
	MyMatrixRotationZ(maat,1.57);
	} else {
		MyMatrixRotationZ(maat,1.57);
	}
	MyMatrixMultiply(maat,mbbt,maat);
	arm2->offset_matrix = maat;

	return true;

}

MYVECTOR3* getSUITYOKUVEC(MYVECTOR3* pos, MYVECTOR3* vec, MYVECTOR3* moku) {
	float a;
	//　これを求める
	//(moku - (pos + vec * a)).(vec) = 0; をとく
//(moku.x - pos.x - a * vec.x)*vec.x + (moku.y - pos.y - a * vec.y)
//	* vec.y + (moku.z - pos.z - a * vec.z)*vec.z = 0;
	//a = ((moku.x - pos.x)*vec.x + (moku.y - pos.y)*vec.y +(moku.z - pos.z)*vec.z)/(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);

	// a を求めたら pos+vec*a - moku を返す

	a = ((moku->float3.x - pos->float3.x)*vec->float3.x + (moku->float3.y - pos->float3.y) * vec->float3.y + (moku->float3.z - pos->float3.z) * vec->float3.z)/(vec->float3.x * vec->float3.x + vec->float3.y * vec->float3.y + vec->float3.z * vec->float3.z );
	static MYVECTOR3 s;

	s = *pos + *vec * a - *moku; // -vec だったのを+にした
	return &s;
}


bool ArmPositioner::positionArm2(float epsiron, float e, Robo* robo, MYVECTOR3* mokuhyou, bool is_migi) {

	MeshBone* arm1;
	MeshBone* arm2;
	MeshBone* handjoint;

	if (is_migi) {
		robo->arm->rarm->animate(40,true);//
		arm1 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["handBone"]];
	} else {
		robo->arm->larm->animate(40,true);
		arm1 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["handBone"]];
	}



	/*

	{

	if (!is_migi) {
	robo->arm->larm->animate(0,true);
	} else {

	robo->arm->rarm->animate(0,true);
	}
	*/

	/*
	MYMATRIX maat;
	if (is_migi) {
	MyMatrixRotationX(maat,-1.57);
	MYMATRIX tmp;
	MyMatrixRotationZ(tmp, 1.57*2);
	MyMatrixMultiply(maat, maat, tmp);
	} else {
		MyMatrixRotationX(maat,-1.57);
		MYMATRIX tmp;
		MyMatrixRotationZ(tmp, -1.57*2);
		MyMatrixMultiply(maat, maat, tmp);
	}
	arm1->offset_matrix = maat;
	if(is_migi) {
	MyMatrixRotationZ(maat,1.57);
	} else {
		MyMatrixRotationZ(maat,1.57);
	}
	
	arm2->offset_matrix = maat;
	if (!is_migi) {
	robo->arm->larm->animate(0,false);
	} else {

	robo->arm->rarm->animate(0,false);
	}
	}
	*/






	// 各ボーンの位置を求めて長さを求める
	MYVECTOR3 arm1_pos(0,0,0);
	MYVECTOR3 arm2_pos(0,0,0);
	MYVECTOR3 joint_pos(0,0,0);

	MYMATRIX mma;
	MyMatrixMultiply(mma,  arm1->matrix_local, arm1->parent_bone->combined_matrix);
	MYMATRIX mmt;
//	MyMatrixTranspose(mmt, mma);
	MyVec3TransformCoord(arm1_pos, arm1_pos,mma);
		
	MyMatrixMultiply(mma  ,arm2->matrix_local,arm2->parent_bone->combined_matrix);
//	MyMatrixTranspose(mmt, mma);
	MyVec3TransformCoord(arm2_pos, arm2_pos,mma);
	
	MyMatrixMultiply(mma , handjoint->matrix_local, handjoint->parent_bone->combined_matrix);
//	MyMatrixTranspose(mmt, mma);
	MyVec3TransformCoord(joint_pos, joint_pos, mma);
	
	MYVECTOR3 tt;
	tt = arm1_pos - arm2_pos;
	float a = MyVec3Length(tt);
	tt = arm2_pos - joint_pos;
	float b = MyVec3Length(tt);
	arm2_pos = arm2_pos - arm1_pos;
	joint_pos = joint_pos - arm1_pos;
	*mokuhyou =  *mokuhyou- arm1_pos;
	MYVECTOR3 mokuhyou_to_arm1 =  - *mokuhyou ;
	float L = MyVec3Length(mokuhyou_to_arm1);

	float thetadash = asin(e * a / L);
	float mytheta = asin(e);

	float BX = L * cos(thetadash) - a * cos(mytheta);

	float d = 1 - (BX*BX + a * cos(mytheta)*BX)/L/L;
	float BM = BX* sin(thetadash);
	float b3 = d * mokuhyou->float3.z - BM * cos(epsiron);
	
	if (cos(epsiron) == 0) {
		return false;
	}

	if (abs(b3/a/cos(epsiron)) > 1) {
		return false;
	}

	float pusai = acos(-b3/a/cos(epsiron));

	MYMATRIX cc;
	MYVECTOR3 yd(0,-1,0);
	MYVECTOR3 xd(1,0,0);
	MYVECTOR3 zd(0,0,1);
	MYMATRIX maat;
	MYMATRIX mbbt;
	
	MyMatrixIdentity(cc);
	MeshBone* bb = arm1->parent_bone;

	while (bb->parent_bone) {
		bb = bb->parent_bone;
	}

	if (is_migi) {
		MyMatrixMultiply(cc,arm1->matrix_local, arm1->parent_bone->combined_matrix);
//		D3DXMatrixTranspose(&cc,&(arm1->parent->combined_matrix * arm1->matrix_local));
		//cc = arm1->combined_matrix;
		MyVec3TransformNormal(yd, (yd), cc);
		MyVec3TransformNormal(xd, (xd), cc);
		MyVec3TransformNormal(zd, zd, cc);
	} else {
		MyMatrixMultiply(cc,arm1->matrix_local, arm1->parent_bone->combined_matrix);
//		D3DXMatrixTranspose(&cc,&(arm1->parent->combined_matrix * arm1->matrix_local));
		//cc = arm1->combined_matrix;
		MYVECTOR3 tt;
		tt = -yd;
		MyVec3TransformNormal(yd, tt, cc);
		MyVec3TransformNormal(xd, (xd), cc);
		tt = -zd;
		MyVec3TransformNormal(zd, tt, cc);
	}

	static float unko2=0;
	unko2 += 0.3f;

	MyMatrixIdentity(maat);
	MyMatrixIdentity(mbbt);
	MyMatrixRotationAxis(maat, yd, -epsiron);
	MyMatrixRotationAxis(mbbt, xd, pusai);
	MYMATRIX mcct;


	float aba = acos(-mokuhyou->float3.y/L)/3;




	MyMatrixRotationAxis(mcct, zd, aba/*-3.14/2*//* 3.14/2*/);
	MyMatrixMultiply(maat, mbbt, maat);
	//MyMatrixMultiply(maat, mcct, maat);

	arm1->offset_matrix = maat;
	MyMatrixRotationZ(maat,1.57);
	MyMatrixMultiply(arm1->offset_matrix,maat, arm1->offset_matrix);


	
	yd = MYVECTOR3(0,1,0);
	xd = MYVECTOR3(1,0,0);
	zd = MYVECTOR3(0,0,1);
	MyMatrixIdentity(cc);
	/*
	if (is_migi) {
		D3DXMatrixTranspose(&cc, &(arm2->parent->combined_matrix *  arm2->matrix_local));
		D3DXVec3TransformNormal(&yd, &(yd), &cc);
		D3DXVec3TransformNormal(&xd, &(xd), &cc);
		D3DXVec3TransformNormal(&zd, &zd, &cc);
	} else {
		D3DXMatrixTranspose(&cc,&(arm2->parent->combined_matrix *  arm2->matrix_local));
		D3DXVec3TransformNormal(&yd, &(-yd), &cc);
		D3DXVec3TransformNormal(&xd, &(xd), &cc);
		D3DXVec3TransformNormal(&zd, &zd, &cc);
	}
	*/
	static float unko=0;
	unko+=0.2f;
	MyMatrixRotationAxis(mbbt, (xd),  mytheta-3.141592/2);
	aba = asin(mokuhyou->float3.x/L);
	MyMatrixRotationAxis(mcct, yd, -aba);//-3.14/2);////* 3.14/2*/);
//	mylog::writelog("%f,\n", aba);
	MyMatrixMultiply(mbbt, mbbt, mcct);

	arm2->offset_matrix = mbbt;
	//MyMatrixMultiply(arm2->offset_matrix, mbbt,arm2->offset_matrix);
	//MyMatrixRotationY(maat,1.57);
	//MyMatrixMultiply(arm2->offset_matrix,maat, arm2->offset_matrix);



	return true;
}



void ArmPositioner::resetTheta() {
		dthetaxa=(rand()%256)/256.0*3;
		dthetaya=(rand()%256)/256.0*0.3;
		dthetaza=(rand()%256)/256.0*0.3;

		dthetaxb=(rand()%256)/256.0/1;
		dthetayb=(rand()%256)/256.0*0.1;
		dthetazb=(rand()%256)/256.0*0.3;
		reseted = true;
}

void ArmPositioner::setArm3(Robo* robo, bool is_migi, MeshBone* arm1, MeshBone* arm2) {
	static float unko=0;
		unko += 0.3f;
	{
		MYMATRIX rotxmat;
		MYMATRIX rotymat;
		MYMATRIX rotzmat;
		

		if (dthetaxa <  -0.37) {
			dthetaxa = -0.37;
		}
		if (dthetaxa > 3.5) {
			dthetaxa = 3.5;
		}

		if (dthetaza <-0.2) {
			dthetaza =-0.2;
		}
		if (dthetaza > 0.2) {
			dthetaza = 0.2;
		}
		if (dthetaya <0) {
			dthetaya = 0;
		}
		if (dthetaya > 0.37) {
			dthetaya = 0.37;
		}


		MyMatrixRotationX(rotxmat, dthetaxa);
		MyMatrixRotationY(rotymat, dthetaya);
		MyMatrixRotationZ(rotzmat, dthetaza-3.14);





		MeshBone* bn = arm2;
		float frame = 40;//this->frame;
		unsigned short ans_minmax;
		unsigned short ans_maxmin;
		float weight;
		if (is_migi) {
		robo->arm->rarm->getOffsetMatrixToGetMinMaxAndWeightIndex(bn, frame, &ans_minmax, &ans_maxmin, &weight);
		} else {
				robo->arm->larm->getOffsetMatrixToGetMinMaxAndWeightIndex(bn, frame, &ans_minmax, &ans_maxmin, &weight);
		}
		MYMATRIX mat1 = bn->animes[ans_minmax]->matrix_basis;
		MYMATRIX mat2 = bn->animes[ans_maxmin]->matrix_basis;
		MYMATRIX mat3;
		for (int i=0;i<16;i++) {
			mat2.m[i/4][i%4] *= weight;
			mat1.m[i/4][i%4] *= (1-weight);
			mat3.m[i/4][i%4] = mat2.m[i/4][i%4] + mat1.m[i/4][i%4];
		}
		MYVECTOR3 v(1,1,1);
		WAsetScaleToMatrix(&mat3, &v);











		MYMATRIX offmat= mat3;
//			robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]]->offset_matrix;
		MyMatrixMultiply(offmat, offmat,rotzmat);
		MyMatrixMultiply(offmat, offmat,rotxmat);
//		MyMatrixMultiply(offmat, offmat, rotymat);
		MYMATRIX ans;
		MyMatrixMultiply(ans, rotzmat, rotxmat);
		MyMatrixMultiply(ans, ans, rotymat);
//		MyMatrixMultiply(ans, ans, trans);
		MyMatrixMultiply(ans, ans, mat3);
	//	MyMatrixIdentity(ans);
		if (is_migi) {
		robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]]->offset_matrix = ans;
		} else {
			robo->arm->larm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]]->offset_matrix = ans;
	
		}
	//	robo->arm->rarm->animate(40,false);
		}
		
		{

			MYMATRIX rotxmat;
		MYMATRIX rotymat;
		MYMATRIX rotzmat;


		if (dthetaxb > 1.57) {
			dthetaxb = 1.57;
		}
		if (dthetaxb < -1.57) {
			dthetaxb = -1.57;
		}

		dthetayb = 0;

		if (dthetazb > 0.9) {
			dthetazb = 0.9;
		}
		if (dthetazb < -0.9) {
			dthetazb = -0.9;
		}


		MyMatrixRotationX(rotxmat, dthetaxb);
		MyMatrixRotationY(rotymat, dthetayb);
		MyMatrixRotationZ(rotzmat, dthetazb);




	//	robo->arm->rarm->animate(40,true);//
		MeshBone* bn = arm1;
		float frame = 40;//this->frame;
		unsigned short ans_minmax;
		unsigned short ans_maxmin;
		float weight;
		if (is_migi) {
		robo->arm->rarm->getOffsetMatrixToGetMinMaxAndWeightIndex(bn, frame, &ans_minmax, &ans_maxmin, &weight);
		} else {
			robo->arm->larm->getOffsetMatrixToGetMinMaxAndWeightIndex(bn, frame, &ans_minmax, &ans_maxmin, &weight);

		}
		MYMATRIX mat1 = bn->animes[ans_minmax]->matrix_basis;
		MYMATRIX mat2 = bn->animes[ans_maxmin]->matrix_basis;
		MYMATRIX mat3;
		for (int i=0;i<16;i++) {
			mat2.m[i/4][i%4] *= weight;
			mat1.m[i/4][i%4] *= (1-weight);
			mat3.m[i/4][i%4] = mat2.m[i/4][i%4] + mat1.m[i/4][i%4];
		}
		MYVECTOR3 v(1,1,1);
		WAsetScaleToMatrix(&mat3, &v);











		MYMATRIX offmat= mat3;
//			robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]]->offset_matrix;
		MyMatrixMultiply(offmat, offmat,rotzmat);
		MyMatrixMultiply(offmat, offmat,rotxmat);
//		MyMatrixMultiply(offmat, offmat, rotymat);
		MYMATRIX ans;
		MyMatrixMultiply(ans, rotzmat, rotxmat);
		MyMatrixMultiply(ans, ans, rotymat);
//		MyMatrixMultiply(ans, ans, trans);
		MyMatrixMultiply(ans, ans, mat3);
		if (is_migi) {
		robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]]->offset_matrix = ans;
		} else {
			robo->arm->larm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]]->offset_matrix = ans;
	
		}



		}





}

int ArmPositioner::positionArm3(Graphics* g , MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi) {





	MeshBone* arm1;
	MeshBone* arm2;
	MeshBone* handjoint;

	if (is_migi) {
		arm1 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["handBone"]];
	} else {
		arm1 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["handBone"]];
	}

	// 各ボーンの位置を求めて長さを求める
	MYVECTOR3 arm1_pos(0,0,0);
	MYVECTOR3 arm2_pos(0,0,0);
	MYVECTOR3 joint_pos(0,0,0);

	MYMATRIX mma;
	MyMatrixMultiply(mma,  arm1->matrix_local, arm1->combined_matrix);
	MYMATRIX mmt;
//	MyMatrixTranspose(mmt, mma);
	MyVec3TransformCoord(arm1_pos, arm1_pos,mma);
		
	MyMatrixMultiply(mma  ,arm2->matrix_local,arm2->combined_matrix);
//	MyMatrixTranspose(mmt, mma);
	MyVec3TransformCoord(arm2_pos, arm2_pos,mma);
	
	MyMatrixMultiply(mma , handjoint->matrix_local, handjoint->combined_matrix);
//	MyMatrixTranspose(mmt, mma);
	MyVec3TransformCoord(joint_pos, joint_pos, mma);
	

	MYVECTOR3 arm2tomoku = *moku - arm2_pos;
	MYVECTOR3 arm2joint =arm2_pos - joint_pos;
	float arm2jointlen = MyVec3Length(arm2joint);
	MyVec3Normalize(arm2tomoku, arm2tomoku);
	arm2tomoku = arm2tomoku * arm2jointlen;
	static float unko = 0;
	MYVECTOR3 unkov((rand() % 256)/256*0.05f-0.025f,
		(rand() % 256)/256*0.05f-0.025f,
		(rand() % 256)/256*0.05f-0.025f);
	MYVECTOR3 mokuhf = arm2tomoku + arm2_pos;//+ unkov;



	


	MYMATRIX wo;
	MyMatrixIdentity(wo);
	OBB re;
	re.c = mokuhf;

	g->drawOBB(g,0xFFFFFF00,&wo,view,g->getProj(), &re);

	mokuhf = (mokuhf- joint_pos)*4/4.0 + joint_pos;
	re.c = mokuhf;
	g->drawOBB(g,0xFF00FF00,&wo,view,g->getProj(), &re);


	re.c = arm1_pos;
	g->drawOBB(g,0xFFDDFFDD,&wo,view,g->getProj(), &re);
	re.c = arm2_pos;
	g->drawOBB(g,0xFFDDDDFF,&wo,view,g->getProj(), &re);
	re.c = joint_pos;
	g->drawOBB(g,0xFFFFDDDD,&wo,view,g->getProj(), &re);

	MYVECTOR3 moku_len;
	moku_len = mokuhf - joint_pos;


	MYVECTOR3 unkovec = joint_pos - arm2_pos;
	MyVec3Normalize(unkovec,unkovec);
	MYVECTOR3 mokutoarm2 = *moku - arm2_pos;
	float dd = MyVec3Dot(unkovec,mokutoarm2);
	float ddd = MyVec3Length(mokutoarm2);
	float dddd = sqrt(ddd * ddd - dd* dd);// kyori

	MYVECTOR3 tempmoku;
	tempmoku = arm2_pos + unkovec * dd;
	dmoku = tempmoku - *moku;


	float len = dddd;
	float mlen = MyVec3Length(moku_len);
	static int mada_count=0;
	float warukazu = 1;
	static bool und= false;
	und = false;

	//mokuhf = *moku;
	if (len < 0.3f) {
               		mada_count = 0;
		return KTROBO_ARMPOSITION_FINISH;// すでに到達している
	} else {
		

		if (len < 0.6f) {
			mada_count = 0;
			warukazu = 600000;
			return KTROBO_ARMPOSITION_FINISH;
			//und = true;
		} else if (mlen < 0.12f) {
			mada_count = 0;
			warukazu = 380;
			und = true;
		} else if (mlen < 0.9f) {
			mada_count -=1;
			warukazu = 160;
			und = true;

		} else if (mlen <1.8f) {
			
			warukazu = 2;
		} else {
			warukazu = 1;
			mada_count += 3;
		//	resetTheta();
      		positionArm33(g, view, robo, moku, is_migi);

		}
		mada_count++;
		if (mada_count > 76000) { // 760
			resetTheta();
			this->setArm3(robo,is_migi,arm1,arm2);
			if (is_migi) {
			robo->arm->rarm->animate(40,false);
			} else {
				robo->arm->larm->animate(40,false);
			}
			mada_count = 0;
		//	und = false;//true;
		}
	}
	

	if (is_migi) {
		
	
		
			MyIKMODOKI modoki(robo->arm->rarm,&robo->atarihan->world,&mokuhf,"modoruBone", "handBone");
		modoki.setXFreeBone("downarmBone");
		modoki.setYFreeBone("downarmBone");
		modoki.setZFreeBone("downarmBone");
		
		modoki.setXFreeBone("uparmBone");
		modoki.setYFreeBone("uparmBone");
		modoki.setZFreeBone("uparmBone");
		
		for (int i = 0; i<13;i++) {
	
		
		for(int k = 0;k < 1;k++) {
		modoki.updateStep();
		}
		float dthetax = modoki.getdthetaXBone("uparmBone")/60.0/warukazu;
		float dthetay = modoki.getdthetaYBone("uparmBone")/60.0/warukazu;
		float dthetaz = modoki.getdthetaZBone("uparmBone")/60.0/warukazu;
		unsigned int unko_count=0;
		if (abs(dthetax) < 0.000001) {
			unko_count++;
		}
		if (abs(dthetay) < 0.000001) {
			unko_count++;
		}
		if (abs(dthetaz) < 0.000001) {
			unko_count++;
		}

		if (und) {
			dthetax /= 2.0f;
			dthetay /= 2.0f;
			dthetaz /= 2.0f;
		}

		dthetaxa += dthetax;
	

		dthetaya += dthetay;
		
		dthetaza += dthetaz;



		dthetax = modoki.getdthetaXBone("downarmBone")/60/warukazu;
		dthetay = modoki.getdthetaYBone("downarmBone")/60/warukazu;
		dthetaz = modoki.getdthetaZBone("downarmBone")/60/warukazu;
		if (abs(dthetax) < 0.000001) {
			unko_count++;
		}
		if (abs(dthetay) < 0.000001) {
			unko_count++;
		}
		if (abs(dthetaz) < 0.000001) {
			unko_count++;
		}

		if (und) {
			dthetax /= 2.0f;
			dthetay /= 2.0f;
			dthetaz /= 2.0f;
		}






		dthetaxb += dthetax;
		dthetayb += dthetay;
		dthetazb += dthetaz;
	
		if ((unko_count == 6) && !und) {
			resetTheta();
		}

		this->setArm3(robo,true,arm1,arm2);
		robo->arm->rarm->animate(40,false);

		}
	} else {

			
			MyIKMODOKI modoki(robo->arm->larm,&robo->atarihan->world,&mokuhf,"modoruBone", "handBone");
		modoki.setXFreeBone("downarmBone");
		modoki.setYFreeBone("downarmBone");
		modoki.setZFreeBone("downarmBone");
		
		modoki.setXFreeBone("uparmBone");
		modoki.setYFreeBone("uparmBone");
		modoki.setZFreeBone("uparmBone");
		
		for (int i = 0; i<13;i++) {
	
		
		for(int k = 0;k < 1;k++) {
		modoki.updateStep();
		}
		float dthetax = modoki.getdthetaXBone("uparmBone")/60.0/warukazu;
		float dthetay = modoki.getdthetaYBone("uparmBone")/60.0/warukazu;
		float dthetaz = modoki.getdthetaZBone("uparmBone")/60.0/warukazu;
		unsigned int unko_count=0;
		if (abs(dthetax) < 0.000001) {
			unko_count++;
		}
		if (abs(dthetay) < 0.000001) {
			unko_count++;
		}
		if (abs(dthetaz) < 0.000001) {
			unko_count++;
		}

		if (und) {
			dthetax /= 2.0f;
			dthetay /= 2.0f;
			dthetaz /= 2.0f;
		}

		dthetaxa += dthetax;
	

		dthetaya += dthetay;
		
		dthetaza += dthetaz;



		dthetax = modoki.getdthetaXBone("downarmBone")/60/warukazu;
		dthetay = modoki.getdthetaYBone("downarmBone")/60/warukazu;
		dthetaz = modoki.getdthetaZBone("downarmBone")/60/warukazu;
		if (abs(dthetax) < 0.000001) {
			unko_count++;
		}
		if (abs(dthetay) < 0.000001) {
			unko_count++;
		}
		if (abs(dthetaz) < 0.000001) {
			unko_count++;
		}

		if (und) {
			dthetax /= 2.0f;
			dthetay /= 2.0f;
			dthetaz /= 2.0f;
		}






		dthetaxb += dthetax;
		dthetayb += dthetay;
		dthetazb += dthetaz;
	
		if ((unko_count == 6) && !und) {
			resetTheta();
		}

		this->setArm3(robo,false,arm1,arm2);
		robo->arm->larm->animate(40,false);

		}

	}

//	if (und) {
//		return KTROBO_ARMPOSITION_OK;
//	}

	return KTROBO_ARMPOSITION_DAME;
}


int ArmPositioner::new_positionArm3(Graphics* g, MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi) {





	MeshBone* arm1;
	MeshBone* arm2;
	MeshBone* handjoint;

	if (is_migi) {
		arm1 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["handBone"]];
		//		robo->arm->rarm->animate(40, false);
	}
	else {
		arm1 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["handBone"]];
		//		robo->arm->larm->animate(40, false);
	}

	// 各ボーンの位置を求めて長さを求める
	MYVECTOR3 arm1_pos(0, 0, 0);
	MYVECTOR3 arm2_pos(0, 0, 0);
	MYVECTOR3 joint_pos(0, 0, 0);

	MYMATRIX mma;

	MyMatrixMultiply(mma, arm1->matrix_local, arm1->combined_matrix);
	MYMATRIX mmt;
	//	MyMatrixTranspose(mmt, mma);
	MyVec3TransformCoord(arm1_pos, arm1_pos, mma);

	MyMatrixMultiply(mma, arm2->matrix_local, arm2->combined_matrix);
	//	MyMatrixTranspose(mmt, mma);
	MyVec3TransformCoord(arm2_pos, arm2_pos, mma);

	MyMatrixMultiply(mma, handjoint->matrix_local, handjoint->combined_matrix);
	//	MyMatrixTranspose(mmt, mma);
	MyVec3TransformCoord(joint_pos, joint_pos, mma);

	MYVECTOR3 jyusin = (arm2_pos * 2 + arm1_pos + joint_pos * 3) / 6.0f;
	MYVECTOR3 jyusin2moku = *moku - jyusin;
	float jyusin2mokulen = MyVec3Length(jyusin2moku);

	MYVECTOR3 arm2tomoku = *moku - arm2_pos;
	MYVECTOR3 arm2joint = arm2_pos - joint_pos;
	MYVECTOR3 arm1arm2 = arm2_pos - arm1_pos;
	float arm2jointlen = MyVec3Length(arm2joint);
	float arm2arm1len = MyVec3Length(arm1arm2);
	MyVec3Normalize(arm2tomoku, arm2tomoku);
	arm2tomoku = arm2tomoku * (arm2jointlen);
	jyusin2moku = jyusin2moku / jyusin2mokulen * arm2jointlen;

	static float unko = 0;

	MYVECTOR3 mokuhf = arm2tomoku + arm2_pos;
	MYVECTOR3 tyouunko = mokuhf - joint_pos;
	float tyouunkolen = MyVec3Length(tyouunko);
	MYVECTOR3 mokudd = tyouunko * max(tyouunkolen, 0.3f) / tyouunkolen + joint_pos;
	static MYVECTOR3 maemoku(0, 0, 0);
	mokuhf = (maemoku*0.2 + mokudd * 0.8);// +joint_pos;

	maemoku = mokuhf;

	MYMATRIX wo;
	MyMatrixIdentity(wo);
	//wo = robo->atarihan->world;
	OBB re;
	re.c = MYVECTOR3(10, -10, 10);// mokuhf;
	mokudayo = mokuhf;


	MYVECTOR3 moku_len;
	moku_len = mokuhf - joint_pos;


	MYVECTOR3 unkovec = joint_pos - arm2_pos;
	MyVec3Normalize(unkovec, unkovec);
	MYVECTOR3 mokutoarm2 = *moku - arm2_pos;
	float dd = MyVec3Dot(unkovec, mokutoarm2);
	float ddd = MyVec3Length(mokutoarm2);
	float dddd = sqrt(ddd * ddd - dd * dd);// kyori

	MYVECTOR3 tempmoku;
	tempmoku = arm2_pos + unkovec * dd;
	dmoku = tempmoku - *moku;


	float len = dddd;
	float mlen = MyVec3Length(moku_len);
	static int mada_count = 0;
	float warukazu = 1;
	static bool und = false;
	und = false;
	char buf[512];
	WCHAR wbuf[512];
	memset(buf, 0, 512);
	if (len * 1000 < 9500) {
	_itoa_s(len * 1000, buf, 10);
	stringconverter sc;
	sc.charToWCHAR(buf, wbuf);
	DebugTexts::instance()->setText(g, wcslen(wbuf), wbuf);
	//mokuhf = *moku;
	}

	if (len < 9.2f) {
		mada_count = 0;
		return KTROBO_ARMPOSITION_FINISH;// すでに到達している
	}
	else {
		if (len < 9.42f) {//0.62 //1.62
			mada_count = 0;
			warukazu = 600000;
			return KTROBO_ARMPOSITION_FINISH;
			//und = true;
		}
		else if (mlen < 0.12f) {
			mada_count = 0;
			warukazu = 400;
			und = true;
		}
		else if (mlen < 0.32f) {
			mada_count = 0;
			warukazu = 380;//380
			und = true;
		}
		else if (mlen < 0.9f) {
			mada_count -= 1;
			warukazu = 160;//160
			und = true;

		}
		else if (mlen <1.8f) {

			warukazu = 2;
		}
		else {
			warukazu = 1;
			mada_count += 3;
		//	resetTheta();
			positionArm33(g, view, robo, moku, is_migi);

		}

		mada_count++;
		if (mada_count > 460) { // 760
			resetTheta();
			warukazu = 600000;
			this->setArm3(robo, is_migi, arm1, arm2);
			if (is_migi) {
				robo->arm->rarm->animate(40, false);
			}
			else {
				robo->arm->larm->animate(40, false);
			}
			mada_count = 0;
			//	und = false;//true;
		}
	}


	if (is_migi) {


	
		MyIKMODOKI modoki(robo->arm->rarm, &robo->atarihan->world, &mokuhf, "modoruBone", "handBone");
		modoki.setXFreeBone("downarmBone");
		modoki.setYFreeBone("downarmBone");
		modoki.setZFreeBone("downarmBone");

		modoki.setXFreeBone("uparmBone");
		modoki.setYFreeBone("uparmBone");
		modoki.setZFreeBone("uparmBone");
		float dthetazenbu = 0;
		for (int i = 0; i<11; i++) {


			for (int k = 0; k < 4; k++) {
				modoki.updateStep();
			}
			float dthetax = modoki.getdthetaXBone("uparmBone") / 60.0 / warukazu;
			float dthetay = modoki.getdthetaYBone("uparmBone") / 60.0 / warukazu;
			float dthetaz = modoki.getdthetaZBone("uparmBone") / 60.0 / warukazu;

			if (isnan(dthetax) || isnan(dthetay) || isnan(dthetaz)) {
				robo->arm->rarm->animate(40, true);
				resetTheta();
				break;
			}

			unsigned int unko_count = 0;
			if (abs(dthetax) < 0.000001) {
				unko_count++;
			}
			if (abs(dthetay) < 0.000001) {
				unko_count++;
			}
			if (abs(dthetaz) < 0.000001) {
				unko_count++;
			}

			if (und) {
				dthetax /= 2.0f;
				dthetay /= 2.0f;
				dthetaz /= 2.0f;
			}

			dthetaxa += dthetax;


			dthetaya += dthetay;

			dthetaza += dthetaz;
			dthetazenbu += dthetax + dthetay + dthetaz;


			dthetax = modoki.getdthetaXBone("downarmBone") / 60 / warukazu;
			dthetay = modoki.getdthetaYBone("downarmBone") / 60 / warukazu;
			dthetaz = modoki.getdthetaZBone("downarmBone") / 60 / warukazu;

			if (isnan(dthetax) || isnan(dthetay) || isnan(dthetaz)) {
				robo->arm->rarm->animate(40, true);
				resetTheta();
				break;

			}

			if (abs(dthetax) < 0.000001) {
				unko_count++;
			}
			if (abs(dthetay) < 0.000001) {
				unko_count++;
			}
			if (abs(dthetaz) < 0.000001) {
				unko_count++;
			}

			if (und) {
				dthetax /= 2.0f;
				dthetay /= 2.0f;
				dthetaz /= 2.0f;
			}






			dthetaxb += dthetax;
			dthetayb += dthetay;
			dthetazb += dthetaz;
			dthetazenbu += dthetax + dthetay + dthetaz;
			if ((unko_count == 6) && !und) {
				resetTheta();
			}

			this->setArm3(robo, true, arm1, arm2);
			robo->arm->rarm->animate(40, false);
			if (dthetazenbu >= 0.01f) {
				//break;
			}
		}
	}
	else {


		MyIKMODOKI modoki(robo->arm->larm, &robo->atarihan->world, &mokuhf, "modoruBone", "handBone");
		modoki.setXFreeBone("downarmBone");
		modoki.setYFreeBone("downarmBone");
		modoki.setZFreeBone("downarmBone");

		modoki.setXFreeBone("uparmBone");
		modoki.setYFreeBone("uparmBone");
		modoki.setZFreeBone("uparmBone");

		for (int i = 0; i<13; i++) {


			for (int k = 0; k < 1; k++) {
				modoki.updateStep();
			}
			float dthetax = modoki.getdthetaXBone("uparmBone") / 60.0 / warukazu;
			float dthetay = modoki.getdthetaYBone("uparmBone") / 60.0 / warukazu;
			float dthetaz = modoki.getdthetaZBone("uparmBone") / 60.0 / warukazu;

			if (isnan(dthetax) || isnan(dthetay) || isnan(dthetaz)) {
				robo->arm->larm->animate(40, true);
				resetTheta();
				break;

			}

			unsigned int unko_count = 0;
			if (abs(dthetax) < 0.000001) {
				unko_count++;
			}
			if (abs(dthetay) < 0.000001) {
				unko_count++;
			}
			if (abs(dthetaz) < 0.000001) {
				unko_count++;
			}

			if (und) {
				dthetax /= 2.0f;
				dthetay /= 2.0f;
				dthetaz /= 2.0f;
			}

			dthetaxa += dthetax;


			dthetaya += dthetay;

			dthetaza += dthetaz;



			dthetax = modoki.getdthetaXBone("downarmBone") / 60 / warukazu;
			dthetay = modoki.getdthetaYBone("downarmBone") / 60 / warukazu;
			dthetaz = modoki.getdthetaZBone("downarmBone") / 60 / warukazu;

			if (isnan(dthetax) || isnan(dthetay) || isnan(dthetaz)) {
				robo->arm->larm->animate(40, true);
				resetTheta();
				break;

			}


			if (abs(dthetax) < 0.000001) {
				unko_count++;
			}
			if (abs(dthetay) < 0.000001) {
				unko_count++;
			}
			if (abs(dthetaz) < 0.000001) {
				unko_count++;
			}

			if (und) {
				dthetax /= 2.0f;
				dthetay /= 2.0f;
				dthetaz /= 2.0f;
			}






			dthetaxb += dthetax;
			dthetayb += dthetay;
			dthetazb += dthetaz;

			if ((unko_count == 6) && !und) {
				resetTheta();
			}

			this->setArm3(robo, false, arm1, arm2);
			robo->arm->larm->animate(40, false);

		}

	}

	//	if (und) {
	//		return KTROBO_ARMPOSITION_OK;
	//	}

	return KTROBO_ARMPOSITION_DAME;
}


bool ArmPositioner::positionArm33(Graphics* g, MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi) {



	MeshBone* arm1;
	MeshBone* arm2;
	MeshBone* handjoint;

	if (is_migi) {
		arm1 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["handBone"]];
	} else {
		arm1 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["handBone"]];
	}


	MYMATRIX maat;
/*	if (is_migi) {
	MyMatrixRotationX(maat,-1.57);
	MYMATRIX tmp;
	MyMatrixRotationZ(tmp, 1.57*2);
	MyMatrixMultiply(maat, maat, tmp);
	} else {
		MyMatrixRotationX(maat,-1.57);
		MYMATRIX tmp;
		MyMatrixRotationZ(tmp, -1.57*2);
		MyMatrixMultiply(maat, maat, tmp);
	}
	arm1->offset_matrix = maat;
	if(is_migi) {
	MyMatrixRotationZ(maat,1.57);
	} else {
		MyMatrixRotationZ(maat,1.57);
	}
	
	arm2->offset_matrix = maat;
	*/
	
	/*
	if (!is_migi) {
	robo->arm->larm->animate(40,true);
	} else {

	robo->arm->rarm->animate(40,true);
	}
	*/

	// 各ボーンの位置を求めて長さを求める
	MYVECTOR3 arm1_pos(0,0,0);
	MYVECTOR3 arm2_pos(0,0,0);
	MYVECTOR3 joint_pos(0,0,0);

	MYMATRIX mma;
	MyMatrixMultiply(mma,   arm1->matrix_local,arm1->parent_bone->combined_matrix);
	MYMATRIX mmt;
	//MyMatrixTranspose(mmt, mma);
	mmt = mma;
	MyVec3TransformCoord(arm1_pos, arm1_pos,mmt);
		
	MyMatrixMultiply(mma ,arm2->matrix_local, arm2->parent_bone->combined_matrix);
	//MyMatrixTranspose(mmt, mma);
	mmt = mma;
	MyVec3TransformCoord(arm2_pos, arm2_pos,mmt);
	
	MyMatrixMultiply(mma  , handjoint->matrix_local, handjoint->parent_bone->combined_matrix);
	//MyMatrixTranspose(mmt, mma);
	mmt = mma;
	MyVec3TransformCoord(joint_pos, joint_pos,mmt);

	MYVECTOR3 arm2tomoku = *moku - arm2_pos;
	MYVECTOR3 arm2tojoint = joint_pos - arm2_pos;
	MYVECTOR3 jointtomoku = arm2tomoku - arm2tojoint;
	MYVECTOR3 crossdayo;
	float dotdayo = MyVec3Dot(arm2tomoku, arm2tojoint);
	if (dotdayo < 0) {
		// あさっての方向を向いているので変更する
		if (dthetaxa > 1.2) {
		dthetaxa  -= 0.02f;
		} else if(dthetaxa < -0.2) {
			dthetaxa = 1.57f;
		}







	}

	
	MyVec3Cross(crossdayo, arm2tomoku, arm2tojoint);
	
	float super_unko = 1;
	float super_unko2=1;
	{
	

	}
	{
	MYVECTOR3 crossdesu;
	MYVECTOR3 tempz(0,0,1);
	MyVec3Cross(crossdesu, tempz, arm2tojoint);
	MYVECTOR3 kotai;
	MyVec3Cross(kotai, crossdesu, arm2tojoint);

	float tes = MyVec3Dot(kotai, arm2tomoku);
	float tes2 = MyVec3Dot(crossdesu, arm2tomoku);
	if (tes > 0) {
		super_unko2 = -1;
	}
	if (tes2 > 0) {
		// 右にある
		super_unko = 1;
	} else {
		// 左にある
		super_unko = -1;
	}



	}
	float asindayo;
	if (MyVec3Length(arm2tomoku) != 0) {
		asindayo = asin(MyVec3Length(crossdayo) / MyVec3Length(arm2tomoku) / MyVec3Length(arm2tojoint));
	} else {
		asindayo = 0;
	}
	MYMATRIX tempmat;
	MyVec3Normalize(crossdayo,crossdayo);
	MyMatrixRotationAxis(tempmat,crossdayo,asindayo);
	MYVECTOR3 xx(1,0,0);
	MYVECTOR3 zz(0,0,1);
	MYVECTOR3 xxx;
	MYVECTOR3 zzz;
	MyVec3TransformNormal(xxx,xx,tempmat);
	MyVec3TransformNormal(zzz,zz,tempmat);
	MYVECTOR3 crossx;
	MYVECTOR3 crossz;
	MyVec3Cross(crossx, xx,xxx);
	MyVec3Cross(crossz, zz,zzz);
	float asinx = asin(MyVec3Length(crossx));
	float asinz = asin(MyVec3Length(crossz));
	static float unko=0;
	unko += 0.3f;
	
	if (_finite(asinx)) {
        dthetaxa += asinx/1000.0f* super_unko2;//asinx
	}
	if (_finite(asinz)) {
		dthetaxb += asinz/1000.0f* super_unko;// + unko;
	}
	
//	return true;



	MYVECTOR3 tt = arm1_pos - arm2_pos;
	float a = MyVec3Length(tt);
	tt = arm2_pos - joint_pos;
	float b = MyVec3Length(tt);
	arm2_pos = arm2_pos - arm1_pos;
	joint_pos = joint_pos - arm1_pos;
	MYVECTOR3 mokuhyou = *moku;
	mokuhyou =  mokuhyou- arm1_pos;
	MYVECTOR3 mokuhyou_to_arm1 =  - mokuhyou ;
	float L = MyVec3Length(mokuhyou_to_arm1);

	MYVECTOR3 suityoku_to_z_and_mta;
	tt = MYVECTOR3(0,0,1);
	MyVec3Cross(suityoku_to_z_and_mta, mokuhyou_to_arm1, tt);
	MyVec3Normalize(suityoku_to_z_and_mta, suityoku_to_z_and_mta);

	if (is_migi) {
	} else {
		suityoku_to_z_and_mta = suityoku_to_z_and_mta *  -1;
	}
	float leng = nobabairitu * (a + b);

	float kyori = L - leng;
	MYVECTOR3 zdayo(0,0,1);
	MYVECTOR3 arm_saki = mokuhyou * leng / (float)L + suityoku_to_z_and_mta * kyori * tan(theta)* cos(theta2) -
		zdayo * kyori * tan(theta) * sin(theta2);

	float l = MyVec3Length(arm_saki);
	if (l > a+b) {
		return false;
	}
	float s = (a+ b+ l)/2;
	float S = sqrt(s* (s-a) * (s-b)* (s-l));
	float h = 2 * S / l;
	if (abs(h/a) > 1 || abs(h/b) > 1) {
		return false;
	}
	float alpha = asin(h/a);
	float beta = acos(h/a);
	float alpha2 = asin(h/b);
	float beta2 = acos(h/b);

	

	MYVECTOR3 motomeru_b;
	MYVECTOR3 b_from_mokuhyou = (arm_saki - mokuhyou);
	MyVec3Normalize(b_from_mokuhyou,b_from_mokuhyou);
	motomeru_b = b_from_mokuhyou* b + arm_saki;


	if (abs(motomeru_b.float3.y/a) > 1) {
		return false;
	}
	float pusai = asin(-motomeru_b.float3.y /a);
	if (cos(pusai) ==0) {
		return false;
	}

	if (abs(motomeru_b.float3.x/(a*cos(pusai))) > 1) {
		return false;
	}

	float epsiron;
	epsiron = asin(motomeru_b.float3.x/(a*cos(pusai)));

	// pusai epsiron が求まったので適用する
	
	MYMATRIX mbbt;
	MYVECTOR3 yd(0,1,0);
	MYVECTOR3 xd(1,0,0);
	MYVECTOR3 zd(0,0,1);
	MYMATRIX cc;
	
	if (is_migi) {
	//	MyMatrixTranspose(cc, (arm1->combined_matrix));
		
		cc = arm1->combined_matrix;
		MyVec3TransformNormal(yd, yd, cc);
		//D3DXVec3TransformNormal(&xd, &xd, &cc);
	//	D3DXVec3TransformNormal(&zd, &zd, &cc);
	} else {
	//	MyMatrixTranspose(cc, (arm1->combined_matrix));
		cc = arm1->combined_matrix;
		MyVec3TransformNormal(yd, yd, cc);
		//D3DXVec3TransformNormal(&xd, &xd, &cc);
	//	D3DXVec3TransformNormal(&zd, &zd, &cc);
	}
	
	
	MyMatrixRotationAxis(maat, yd, epsiron);
	MyMatrixRotationAxis(mbbt, zd, pusai);

	MyMatrixMultiply(maat, maat, mbbt);

	
	yd = MYVECTOR3(0,1,0);
	xd = MYVECTOR3(1,0,0);
	zd = MYVECTOR3(0,0,1);
	
	MyMatrixRotationAxis(mbbt, xd,  alpha + alpha2- 3.1415926535/2);

	// episiron psi alpha の三角をdthetaに入れる
	


//	dthetaxb = (dthetaxb+(alpha + alpha2 - 3.14))/2;
//	dthetaxa = (dthetaxa + epsiron+1.57f)/2;
	dthetaya = (dthetaya + pusai)/2;
	dthetayb += (rand() %256)/256.0*0.04 - 0.02;
	dthetaza += (rand() %256)/256.0*0.04 - 0.02;
	dthetazb += (rand() %256)/256.0*0.04 - 0.02;



	return true;

}


int ArmPositioner::positionArm34(Graphics* g , MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi) {

	// uparmbone の位置を決定する


	MeshBone* arm1;
	MeshBone* arm2;
	MeshBone* handjoint;

	if (is_migi) {
		arm1 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["handBone"]];
	} else {
		arm1 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["handBone"]];
	}

	
	
	if (is_migi) {
	robo->arm->rarm->animate(40,true);
	} else {
		robo->arm->larm->animate(40,true);
	}
	
	return 0;

	// 各ボーンの位置を求めて長さを求める
	MYVECTOR3 arm1_pos(0,0,0);
	MYVECTOR3 arm2_pos(0,0,0);
	MYVECTOR3 joint_pos(0,0,0);

	MYMATRIX mma;
	MyMatrixMultiply(mma,   arm1->matrix_local,arm1->parent_bone->combined_matrix);
	MYMATRIX mmt;
	//MyMatrixTranspose(mmt, mma);
	mmt = mma;
	MyVec3TransformCoord(arm1_pos, arm1_pos,mmt);
		
	MyMatrixMultiply(mma ,arm2->matrix_local, arm2->parent_bone->combined_matrix);
	//MyMatrixTranspose(mmt, mma);
	mmt = mma;
	MyVec3TransformCoord(arm2_pos, arm2_pos,mmt);
	
	MyMatrixMultiply(mma  , handjoint->matrix_local, handjoint->parent_bone->combined_matrix);
	//MyMatrixTranspose(mmt, mma);
	mmt = mma;
	MyVec3TransformCoord(joint_pos, joint_pos,mmt);

	MYVECTOR3 mokuhf = arm2_pos;
	
	this->setArm3(robo,is_migi, arm1,arm2);
	if (is_migi) {
	robo->arm->rarm->animate(40,false);
	} else {
		robo->arm->larm->animate(40,false);
	}

	if (is_migi) {
		
	
		
		MyIKMODOKI modoki(robo->arm->rarm,&robo->atarihan->world,&mokuhf,"modoruBone", "downarmBone");
		
		modoki.setXFreeBone("uparmBone");
		modoki.setYFreeBone("uparmBone");
		modoki.setZFreeBone("uparmBone");
		
		for (int i = 0; i<13;i++) {
	
		
		for(int k = 0;k < 1;k++) {
		modoki.updateStep();
		}
		float dthetax = modoki.getdthetaXBone("uparmBone")/60.0;
		float dthetay = modoki.getdthetaYBone("uparmBone")/60.0;
		float dthetaz = modoki.getdthetaZBone("uparmBone")/60.0;

		dthetaxa += dthetax;
		dthetaya += dthetay;
		dthetaza += dthetaz;




		this->setArm3(robo,true,arm1,arm2);
		robo->arm->rarm->animate(40,false);

		}

	} else {
		float dthetaxa=0;
		float dthetaya=0;
		float dthetaza=0;
		MyIKMODOKI modoki(robo->arm->larm,&robo->atarihan->world,&mokuhf,"uparmBone", "handBone");
			modoki.setXFreeBone("downarmBone");
		modoki.setYFreeBone("downarmBone");
		modoki.setZFreeBone("downarmBone");
		for (int i= 0;i<5;i++) {
	
	
		for(int i = 0;i < 1;i++) {
			modoki.updateStep();
		}
		float dthetax = modoki.getdthetaXBone("downarmBone")/3.14/60;
		float dthetay = modoki.getdthetaYBone("downarmBone")/3.14/60;
		float dthetaz = modoki.getdthetaZBone("downarmBone")/3.14/60;
		dthetaxa += dthetax;
		dthetaya += dthetay;
		dthetaza += dthetaz;
		
		MYMATRIX rotxmat;
		MYMATRIX rotymat;
		MYMATRIX rotzmat;
		MyMatrixRotationY(rotxmat, dthetaxa);
		MyMatrixRotationY(rotymat, dthetaya);
		MyMatrixRotationY(rotzmat, dthetaza);
		MYMATRIX offmat;
		MyMatrixMultiply(offmat, rotzmat, rotxmat);
		MyMatrixMultiply(offmat, offmat, rotymat);
		robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]]->offset_matrix = offmat;
		}
	}



	return this->positionArm3(g,view,robo,moku,is_migi);
}





void ShudouArmPositioner::Init(HWND hw, Texture* tex, lua_State* l, int screen_width, int screen_height) {


	gui = new GUI();
	//gui->Init(hw, tex,l,screen_width,screen_height);

	screen_window_id = gui->makeWindow(0,0,screen_width,screen_height);
	gui->setEffect(screen_window_id,true);
	gui->setEnable(screen_window_id,true);
	gui->setRender(screen_window_id, true);


	MYRECT zentai;

	zentai.left = 600;
	zentai.right = 800;
	zentai.top = 40;
	zentai.bottom = 70;

	slider_dxb = gui->makeSliderH(&zentai,1.57,-1.57,0,"test.lua");
	gui->setEffect(slider_dxb,true);
	gui->setEnable(slider_dxb,true);
	gui->setRender(slider_dxb,true);
	gui->setCallLuaToSlider(slider_dxb , false);
	gui->setPartToWindow(screen_window_id, slider_dxb);



	zentai.left = 600;
	zentai.right = 800;
	zentai.top = 0;
	zentai.bottom = 30;

	slider_dxa = gui->makeSliderH(&zentai,3.5,-0.37,0,"test.lua");
	gui->setEffect(slider_dxa,true);
	gui->setEnable(slider_dxa,true);
	gui->setRender(slider_dxa,true);
	gui->setCallLuaToSlider(slider_dxa , false);
	gui->setPartToWindow(screen_window_id, slider_dxa);
	



	zentai.left = 600;
	zentai.right = 800;
	zentai.top = 80;
	zentai.bottom = 110;

	slider_dya = gui->makeSliderH(&zentai,0.2,-0.2,0,"test.lua");
	gui->setEffect(slider_dya,true);
	gui->setEnable(slider_dya,true);
	gui->setRender(slider_dya,true);
	gui->setCallLuaToSlider(slider_dya , false);
	gui->setPartToWindow(screen_window_id, slider_dya);


	zentai.left = 600;
	zentai.right = 800;
	zentai.top = 120;
	zentai.bottom = 150;

	slider_dyb = gui->makeSliderH(&zentai,0,0,0,"test.lua");
	gui->setEffect(slider_dyb,true);
	gui->setEnable(slider_dyb,true);
	gui->setRender(slider_dyb,true);
	gui->setCallLuaToSlider(slider_dyb , false);
	gui->setPartToWindow(screen_window_id, slider_dyb);


	zentai.left = 600;
	zentai.right = 800;
	zentai.top = 160;
	zentai.bottom = 190;

	slider_dza = gui->makeSliderH(&zentai,0.37,0,0,"test.lua");
	gui->setEffect(slider_dza,true);
	gui->setEnable(slider_dza,true);
	gui->setRender(slider_dza,true);
	gui->setCallLuaToSlider(slider_dza , false);
	gui->setPartToWindow(screen_window_id, slider_dza);


	zentai.left = 600;
	zentai.right = 800;
	zentai.top = 200;
	zentai.bottom = 230;

	slider_dzb = gui->makeSliderH(&zentai,0.9,-0.9,0,"test.lua");
	gui->setEffect(slider_dzb,true);
	gui->setEnable(slider_dzb,true);
	gui->setRender(slider_dzb,true);
	gui->setCallLuaToSlider(slider_dzb , false);
	gui->setPartToWindow(screen_window_id, slider_dzb);
	gui->setRootWindowToInputMessageDispatcher(screen_window_id);
	
	toggle_button = gui->makeButton(100,300,50,50,"test2.lua", 9, "test");
	gui->setEffect(toggle_button,true);
	gui->setEnable(toggle_button,true);
	gui->setRender(toggle_button,true);
	gui->setCallLuaToButton(toggle_button,false);
	gui->setPartToWindow(screen_window_id,toggle_button);
}

void ShudouArmPositioner::Del() {
	if (gui) {
		gui->deleteAll();
		//gui->Release();
		delete gui;
		gui = 0;
	}
}

bool ShudouArmPositioner::update() {
	bool t=false;
	bool t2=false;
	if (gui->getTriedToSendFromSlider(slider_dxa)) {
		t = true;
	}

	if (gui->getTriedToSendFromSlider(slider_dxb)) {
		t = true;
	}

	if (gui->getTriedToSendFromSlider(slider_dya)) {
		t = true;
	}

	if (gui->getTriedToSendFromSlider(slider_dyb)) {
		t = true;
	}

	if (gui->getTriedToSendFromSlider(slider_dza)) {
		t = true;
	}

	if (gui->getTriedToSendFromSlider(slider_dzb)) {
		t = true;
	}

	if (gui->getTriedToSendFromButton(toggle_button)) {
		t2 = true;
//		ap->resetTheta();
	}

	if (t) {
		dthetaxa = gui->getNowFromSlider(slider_dxa);
		dthetaxb = gui->getNowFromSlider(slider_dxb);
		dthetaya = gui->getNowFromSlider(slider_dya);
		dthetayb = gui->getNowFromSlider(slider_dyb);
		dthetaza = gui->getNowFromSlider(slider_dza);
		dthetazb = gui->getNowFromSlider(slider_dzb);
		ap->resetTheta();
		ap->setIsSet(true);
		ap->setTheta(dthetaxa, dthetaxb, dthetaya, dthetayb, dthetaza, dthetazb);
		
	}
	
	if (t2) {
		if (ap->getIsSet()) {
			ap->setIsSet(false);
		} else {
			ap->setIsSet(true);
		}
	}
	
	return t;
}

void ArmPositioner::hanneiSetTheta(Robo* robo, bool is_migi) {


	MeshBone* arm1;
	MeshBone* arm2;
	MeshBone* handjoint;

	if (is_migi) {
		arm1 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["handBone"]];
	} else {
		arm1 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["uparmBone"]];
		arm2 = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]];
		handjoint = robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["handBone"]];
	}


	setArm3(robo,is_migi,arm1,arm2);
	if (is_migi) {
		robo->arm->rarm->animate(40,false);
	} else {
		robo->arm->larm->animate(40,false);
	}

	//is_set = false;


}

void ArmPoint8Positioner::setPoint(int index, ArmPoint* ap) {
if (index >= 8) return;
points[index].dthetaxa = ap->dthetaxa;
points[index].dthetaxb = ap->dthetaxb;
points[index].dthetaya = ap->dthetaya;
points[index].dthetayb = ap->dthetayb;
points[index].dthetaza = ap->dthetaza;
points[index].dthetazb = ap->dthetazb;
points[index].pos = ap->pos;
}


ArmPoint getPointPart(ArmPoint* kitenpoint, ArmPoint* point1, ArmPoint* point2, ArmPoint* point3, MYVECTOR3* moku) {
	ArmPoint ans;
	ans.pos = MYVECTOR3(0,0,0);
	ans.dthetaxa = 0;
	ans.dthetaxb = 0;
	ans.dthetaya = 0;
	ans.dthetayb = 0;
	ans.dthetaza = 0;
	ans.dthetazb = 0;
	ans.is_ok = false;
	// 表現できない場合は予測してdtheta を返す
	MYVECTOR3 vec1 = point1->pos - kitenpoint->pos;
	MYVECTOR3 vec2 = point2->pos - kitenpoint->pos;
	MYVECTOR3 vec3 = point3->pos - kitenpoint->pos;

	MYVECTOR3 n_vec1;
	MyVec3Normalize(n_vec1,vec1);
	MYVECTOR3 n_vec2;
	MyVec3Normalize(n_vec2, vec2);
	MYVECTOR3 n_vec3;
	MyVec3Normalize(n_vec3, vec3);

	MYVECTOR3 kitentomoku = *moku - kitenpoint->pos;
	
	double A11 = n_vec1.float3.x;
	double A12 = n_vec1.float3.y;
	double A13 = n_vec1.float3.z;

	double A21 = n_vec2.float3.x;
	double A22 = n_vec2.float3.y;
	double A23 = n_vec2.float3.z;
	
	double A31 = n_vec3.float3.x;
	double A32 = n_vec3.float3.y;
	double A33 = n_vec3.float3.z;
	double detA;
	detA = A11*A22*A33 + A12*A23*A31 + A13*A21*A32 - A13*A22*A31 - A11*A23*A32- A12*A21*A33;

	if (abs(detA) <0.000001f) {
              		detA = 1;
	}

	double x1a = ((A22*A33- A32*A23)* kitentomoku.float3.x - (A21* A33 - A31 * A23)* kitentomoku.float3.y + (A21 * A32 - A31*A22)*kitentomoku.float3.z) /detA;
	double x2a = (-(A12 * A33 - A32*A13)*kitentomoku.float3.x + (A11*A33 - A31* A13)*kitentomoku.float3.y - (A11 * A32 - A31*A12)*kitentomoku.float3.z)/detA;
	double x3a = ((A12 * A23 - A22*A13)*kitentomoku.float3.x - (A11*A23 - A21* A13)*kitentomoku.float3.y + (A11*A22 - A21*A12)*kitentomoku.float3.z)/detA;

	// これで　moku = kiten + x1a * n_vec1 + x2a * n_vec2 + x3a * n_vec3 と表せる
	double hi1 = x1a / MyVec3Length(vec1);
	double hi2 = x2a / MyVec3Length(vec2);
	double hi3 = x3a / MyVec3Length(vec3);

/*	if ((hi1 <0) || (hi2 < 0) ||(hi3 <0)) {
		return ans;
	}

	if ((hi1 > 1) || (hi2 > 1) || (hi3 > 1)) {
		return ans;
	}
*/
	bool sumhizero = false;
	double sumhi = abs(hi1) + abs(hi2)+abs(hi3);
	if (sumhi < 0.000001) {
		sumhi = 1;
		sumhizero = true;
	}
	// dtheta の計算を行う
	double dthetaxa1 = kitenpoint->dthetaxa * ( 1-hi1) + point1->dthetaxa * hi1;
	double dthetaxb1 = kitenpoint->dthetaxb * ( 1-hi1) + point1->dthetaxb * hi1;
	double dthetaya1 = kitenpoint->dthetaya * ( 1-hi1) + point1->dthetaya * hi1;
	double dthetayb1 = kitenpoint->dthetayb * ( 1-hi1) + point1->dthetayb * hi1;
	double dthetaza1 = kitenpoint->dthetaza * ( 1-hi1) + point1->dthetaza * hi1;
	double dthetazb1 = kitenpoint->dthetazb * (1 - hi1) + point1->dthetazb * hi1;

	double dthetaxa2 = kitenpoint->dthetaxa * ( 1-hi2) + point2->dthetaxa * hi2;
	double dthetaxb2 = kitenpoint->dthetaxb * ( 1-hi2) + point2->dthetaxb * hi2;
	double dthetaya2 = kitenpoint->dthetaya * ( 1-hi2) + point2->dthetaya * hi2;
	double dthetayb2 = kitenpoint->dthetayb * ( 1-hi2) + point2->dthetayb * hi2;
	double dthetaza2 = kitenpoint->dthetaza * ( 1-hi2) + point2->dthetaza * hi2;
	double dthetazb2 = kitenpoint->dthetazb * ( 1-hi2) + point2->dthetazb * hi2;
	
	double dthetaxa3 = kitenpoint->dthetaxa * ( 1-hi3) + point3->dthetaxa * hi3;
	double dthetaxb3 = kitenpoint->dthetaxb * ( 1-hi3) + point3->dthetaxb * hi3;
	double dthetaya3 = kitenpoint->dthetaya * ( 1-hi3) + point3->dthetaya * hi3;
	double dthetayb3 = kitenpoint->dthetayb * ( 1-hi3) + point3->dthetayb * hi3;
	double dthetaza3 = kitenpoint->dthetaza * ( 1-hi3) + point3->dthetaza * hi3;
	double dthetazb3 = kitenpoint->dthetazb * ( 1-hi3) + point3->dthetazb * hi3;

	ans.dthetaxa = (float)(dthetaxa1 * abs(hi1/sumhi) + dthetaxa2 * abs(hi2/sumhi) + dthetaxa3 * abs(hi3/sumhi));//- 2*kitenpoint->dthetaxa;
	ans.dthetaxb =(float)(dthetaxb1 *abs(hi1/sumhi)+ dthetaxb2 *abs(hi2/sumhi) + dthetaxb3 *abs( hi3/sumhi));//- 2*kitenpoint->dthetaxb;
	ans.dthetaya =(float)(dthetaya1 *abs(hi1/sumhi)+ dthetaya2 *abs(hi2/sumhi) + dthetaya3 *abs( hi3/sumhi));//- 2*kitenpoint->dthetaya;
	ans.dthetayb =(float)(dthetayb1 *abs(hi1/sumhi)+ dthetayb2 *abs(hi2/sumhi) + dthetayb3 *abs( hi3/sumhi));//- 2*kitenpoint->dthetayb;
	ans.dthetaza =(float)(dthetaza1 *abs(hi1/sumhi)+ dthetaza2 *abs(hi2/sumhi) + dthetaza3 *abs( hi3/sumhi));//- 2*kitenpoint->dthetaza;
	ans.dthetazb =(float)(dthetazb1 *abs(hi1/sumhi)+ dthetazb2 *abs(hi2/sumhi) + dthetazb3 *abs( hi3/sumhi));//- 2*kitenpoint->dthetazb;
	if (sumhizero) {
		ans.dthetaxa = dthetaxa1;
		ans.dthetaxb = dthetaxb1;
		ans.dthetaya = dthetaya1;
		ans.dthetayb = dthetayb1;
		ans.dthetaza = dthetaza1;
		ans.dthetazb = dthetazb1;
	}
	ans.pos = kitenpoint->pos + n_vec1 * x1a+ n_vec2*x2a+ n_vec3*x3a;

	ans.is_ok = true;
	return ans;
}
ArmPoint ArmPoint8Positioner::getPoint(MYVECTOR3* moku) {
	static ArmPoint ans;
	ans.pos = MYVECTOR3(0,0,0);
	ans.dthetaxa = 0;
	ans.dthetaxb = 0;
	ans.dthetaya = 0;
	ans.dthetayb = 0;
	ans.dthetaza = 0;
	ans.dthetazb = 0;
	ans.is_ok = false;
	// どうやってやればいいのか・・
	// 12ベクトルのうち空間を構成する3ベクトルの組を４つ作って
	// 3ベクトルでmoku の位置を表現させて
	// 表現できない場合でも　計算に加える
	// そして平均（重心）を取る

	// 8近傍から一番近い点を取得する
	// その点でgetPointPartする
	MYVECTOR3 kyoriv[8];
	float kyori[8];
	for (int i=0;i<8;i++) {
		kyoriv[i] = *moku - points[i].pos;
		kyori[i] = MyVec3Length(kyoriv[i]);
	}

	float minkyori = kyori[0];
	int index = 0;
	for (int i=0;i<8;i++) {
		if (minkyori > kyori[i]) {
			index = i;
			minkyori = kyori[i];
		}
	}

	ArmPoint anp;
	if (index == KTROBO_ARMPOINT8_MHS) {
		anp = getPointPart(&points[KTROBO_ARMPOINT8_MHS], &points[KTROBO_ARMPOINT8_MHU],
			&points[KTROBO_ARMPOINT8_MMS], &points[KTROBO_ARMPOINT8_UHS], moku);
	} else if(index == KTROBO_ARMPOINT8_MHU) {
		anp = getPointPart(&points[KTROBO_ARMPOINT8_MHU],&points[KTROBO_ARMPOINT8_UHU],
		&points[KTROBO_ARMPOINT8_MMU], &points[KTROBO_ARMPOINT8_MHS],moku);
	} else if(index == KTROBO_ARMPOINT8_MMS) {
		anp = getPointPart(&points[KTROBO_ARMPOINT8_MMS], &points[KTROBO_ARMPOINT8_MMU],
			&points[KTROBO_ARMPOINT8_MHS], &points[KTROBO_ARMPOINT8_UMS], moku);
	} else if(index == KTROBO_ARMPOINT8_MMU) {
		anp = getPointPart(&points[KTROBO_ARMPOINT8_MMU], &points[KTROBO_ARMPOINT8_MHU],
		&points[KTROBO_ARMPOINT8_MMS], &points[KTROBO_ARMPOINT8_UMU], moku);
	} else if(index == KTROBO_ARMPOINT8_UHS) {
		anp = getPointPart(&points[KTROBO_ARMPOINT8_UHS], &points[KTROBO_ARMPOINT8_UHU],
		&points[KTROBO_ARMPOINT8_MHS],&points[KTROBO_ARMPOINT8_UMS], moku);
	} else if(index == KTROBO_ARMPOINT8_UHU) {
		anp = getPointPart(&points[KTROBO_ARMPOINT8_UHU], &points[KTROBO_ARMPOINT8_MHU],
			&points[KTROBO_ARMPOINT8_UMU], &points[KTROBO_ARMPOINT8_UHS],moku);
	} else if(index == KTROBO_ARMPOINT8_UMS) {
		anp = getPointPart(&points[KTROBO_ARMPOINT8_UMS],&points[KTROBO_ARMPOINT8_UHS],
		&points[KTROBO_ARMPOINT8_UMU], &points[KTROBO_ARMPOINT8_MMS], moku);
	} else if(index == KTROBO_ARMPOINT8_UMU) {
		anp = getPointPart(&points[KTROBO_ARMPOINT8_UMU], &points[KTROBO_ARMPOINT8_MMU],
			&points[KTROBO_ARMPOINT8_UHU], &points[KTROBO_ARMPOINT8_UMS],moku);
	} else {
		anp.is_ok = false;
	}

	if (anp.is_ok) {
		ans.dthetaxa += anp.dthetaxa;
		ans.dthetaxb += anp.dthetaxb;
		ans.dthetaya += anp.dthetaya;
		ans.dthetayb += anp.dthetayb;
		ans.dthetaza += anp.dthetaza;
		ans.dthetazb += anp.dthetazb;
		ans.is_ok = true;
		ans.pos = anp.pos;
		return ans;
	}

	ArmPoint anp1 = getPointPart(&points[KTROBO_ARMPOINT8_MHU],&points[KTROBO_ARMPOINT8_UHU],
		&points[KTROBO_ARMPOINT8_MMU], &points[KTROBO_ARMPOINT8_MHS],moku);
	
	int c=0;

	if (anp1.is_ok) {
		c++;
		ans.dthetaxa += anp1.dthetaxa;
		ans.dthetaxb += anp1.dthetaxb;
		ans.dthetaya += anp1.dthetaya;
		ans.dthetayb += anp1.dthetayb;
		ans.dthetaza += anp1.dthetaza;
		ans.dthetazb += anp1.dthetazb;
		ans.pos = ans.pos +  anp1.pos;
		ans.is_ok = true;
		return ans;
	}
	
	
	
	ArmPoint anp2 = getPointPart(&points[KTROBO_ARMPOINT8_UMS],&points[KTROBO_ARMPOINT8_UHS],
		&points[KTROBO_ARMPOINT8_UMU], &points[KTROBO_ARMPOINT8_MMS], moku);
	if (anp2.is_ok) {
		c++;
		ans.dthetaxa += anp2.dthetaxa;
		ans.dthetaxb += anp2.dthetaxb;
		ans.dthetaya += anp2.dthetaya;
		ans.dthetayb += anp2.dthetayb;
		ans.dthetaza += anp2.dthetaza;
		ans.dthetazb += anp2.dthetazb;
		ans.pos = ans.pos +  anp2.pos;
		ans.is_ok = true;
		return ans;
	}
	ArmPoint anp3 = getPointPart(&points[KTROBO_ARMPOINT8_UHS], &points[KTROBO_ARMPOINT8_UHU],
		&points[KTROBO_ARMPOINT8_MHS],&points[KTROBO_ARMPOINT8_UMS], moku);
	
	if (anp3.is_ok) {
		c++;
		ans.dthetaxa += anp3.dthetaxa;
		ans.dthetaxb += anp3.dthetaxb;
		ans.dthetaya += anp3.dthetaya;
		ans.dthetayb += anp3.dthetayb;
		ans.dthetaza += anp3.dthetaza;
		ans.dthetazb += anp3.dthetazb;
		ans.pos = ans.pos +  anp3.pos;
		ans.is_ok = true;
		return ans;
	}


	ArmPoint anp4 = getPointPart(&points[KTROBO_ARMPOINT8_MMU], &points[KTROBO_ARMPOINT8_MHU],
		&points[KTROBO_ARMPOINT8_MMS], &points[KTROBO_ARMPOINT8_UMU], moku);

	if (anp4.is_ok) {
		c++;
		ans.dthetaxa += anp4.dthetaxa;
		ans.dthetaxb += anp4.dthetaxb;
		ans.dthetaya += anp4.dthetaya;
		ans.dthetayb += anp4.dthetayb;
		ans.dthetaza += anp4.dthetaza;
		ans.dthetazb += anp4.dthetazb;
		ans.pos = ans.pos +  anp4.pos;
		ans.is_ok = true;
		return ans;
	}
	/*
	if (c ==0) {
		return ans;
	} else {
		ans.dthetaxa /= c;
		ans.dthetaxb /= c;
		ans.dthetaya /= c;
		ans.dthetayb /= c;
		ans.dthetaza /= c;
		ans.dthetazb /= c;
		ans.pos = ans.pos / c;
	}
	*/
	//ans.is_ok = true;
	return ans;
}

bool isInPointPart(MYVECTOR3* moku, MYVECTOR3* pos1, MYVECTOR3* pos2, MYVECTOR3* pos3, MYVECTOR3* jyusin8) {
	MYVECTOR3 pointt = (*pos1 + *pos2 + *pos3)/3;
	// 重心とすみの点から面の法線を求める
	MYVECTOR3 vec1;
	MYVECTOR3 vec2;
	vec1 = *pos1 - pointt;
	vec2 = *pos2 - pointt;
	MYVECTOR3 hou1;
	MyVec3Cross(hou1, vec1,vec2);
	MYVECTOR3 vec3;
	MYVECTOR3 vec4;
	vec3 = *jyusin8 - pointt;
	vec4 = *moku - pointt;

	// 求めた法線と　重心から８すみの重心、　重心からmokuの方向ベクトルのdotをとる
	// dotの積をそれぞれ求める
	float dot1 = MyVec3Dot(hou1, vec3);
	float dot2 = MyVec3Dot(hou1, vec4);

	if (dot1* dot2 >= 0) {
		return true;
	}
	return false;
}



bool ArmPoint8Positioner::isInPoint(MYVECTOR3* moku) {

	// ８すみの三角形重心を求める
	MYVECTOR3 jyusin8 = (points[0].pos + points[1].pos + points[2].pos + points[3].pos + points[4].pos + points[5].pos + points[6].pos + points[7].pos)/8;
	bool bpoint1 = isInPointPart(moku,&points[KTROBO_ARMPOINT8_MHS].pos , &points[KTROBO_ARMPOINT8_MHU].pos , &points[KTROBO_ARMPOINT8_MMU].pos, &jyusin8);
	bool bpoint2 = isInPointPart(moku,&points[KTROBO_ARMPOINT8_MHS].pos , &points[KTROBO_ARMPOINT8_MHU].pos , &points[KTROBO_ARMPOINT8_MMS].pos, &jyusin8);
	bool bpoint3 =isInPointPart(moku,&points[KTROBO_ARMPOINT8_MHS].pos , &points[KTROBO_ARMPOINT8_MHU].pos , &points[KTROBO_ARMPOINT8_UHU].pos, &jyusin8);
	bool bpoint4 =isInPointPart(moku,&points[KTROBO_ARMPOINT8_MHS].pos , &points[KTROBO_ARMPOINT8_MHU].pos , &points[KTROBO_ARMPOINT8_UHS].pos, &jyusin8);
	bool bpoint5 =isInPointPart(moku,&points[KTROBO_ARMPOINT8_UMU].pos , &points[KTROBO_ARMPOINT8_UMS].pos , &points[KTROBO_ARMPOINT8_MMU].pos, &jyusin8);
	bool bpoint6 =isInPointPart(moku,&points[KTROBO_ARMPOINT8_UMU].pos , &points[KTROBO_ARMPOINT8_UMS].pos , &points[KTROBO_ARMPOINT8_MMS].pos, &jyusin8);
	bool bpoint7 =isInPointPart(moku,&points[KTROBO_ARMPOINT8_UMU].pos , &points[KTROBO_ARMPOINT8_UMS].pos , &points[KTROBO_ARMPOINT8_UHU].pos, &jyusin8);
	bool bpoint8 =isInPointPart(moku,&points[KTROBO_ARMPOINT8_UMU].pos , &points[KTROBO_ARMPOINT8_UMS].pos , &points[KTROBO_ARMPOINT8_UHS].pos, &jyusin8);
	bool bpoint9 =isInPointPart(moku,&points[KTROBO_ARMPOINT8_MHU].pos , &points[KTROBO_ARMPOINT8_UHU].pos , &points[KTROBO_ARMPOINT8_UMU].pos, &jyusin8);
	bool bpoint10= isInPointPart(moku,&points[KTROBO_ARMPOINT8_MHU].pos , &points[KTROBO_ARMPOINT8_UHU].pos , &points[KTROBO_ARMPOINT8_MMU].pos, &jyusin8);
	bool bpoint11= isInPointPart(moku,&points[KTROBO_ARMPOINT8_MHS].pos , &points[KTROBO_ARMPOINT8_UHS].pos , &points[KTROBO_ARMPOINT8_MMS].pos, &jyusin8);
	bool bpoint12= isInPointPart(moku,&points[KTROBO_ARMPOINT8_MHS].pos , &points[KTROBO_ARMPOINT8_UHS].pos , &points[KTROBO_ARMPOINT8_UMS].pos, &jyusin8);

	

	


	
	// dot積がどれも正であれば８すみの中にある

	if (bpoint1 && bpoint2 && bpoint3 && bpoint4 && bpoint5 && bpoint6 && bpoint7 && bpoint8 && bpoint9 && bpoint10 && bpoint11 && bpoint12) {
		return true;
	}
	

	return false;



}

void ArmPositionerHelper::setArmPoint8(int index, ArmPoint* po) {
	ap8.setPoint(index,po);
}


void ArmPositionerHelper::byougaAP8(Graphics* g, MYMATRIX* view) {
	OBB ob;
	ob.e.float3.x = 0.5;
	ob.e.float3.y = 1;
	ob.e.float3.z = 0.5;
	for (int i=0;i<8;i++) {
	ob.c = ap8.points[i].pos;
	g->drawOBB(g,0xFFFFFFFF, &robo->atarihan->world,view,g->getProj(), &ob);
	}
	ob.c = tempmoku;
	//g->drawOBBFill(g, 0xFFFFFFFF, &robo->atarihan->world, view, g->getProj(), &ob);
	ob.c = ap->mokudayo;
	ob.e = MYVECTOR3(2, 2, 2);
	//g->drawOBBFill(g, 0xFFFFFFFF, &robo->atarihan->world, view, g->getProj(), &ob);

}
void ArmPositionerHelper::calc(Graphics* g, MYMATRIX* view) {
		
	
	

	if (nocalcyet) {
		ddmoku = MYVECTOR3(0,0,0);
		ddunko = MYVECTOR3(0,0,0);
		dbb = 100000;
		unko_count = 0;
		bairitu = 1;
		uunko = false;

		ArmPoint anp = ap8.getPoint(&moku);
		if (anp.is_ok) {
			ap->setTheta(anp.dthetaxa,anp.dthetaxb,anp.dthetaya,anp.dthetayb,anp.dthetaza,anp.dthetazb);
		}
	
//		if (is_calced) {
//			return;
//		}

	} else {
		
//		if (is_calced) {
//			return;
//		}
		

		// dmokuをしようする
	
		// tempmokuを計算する
		float unko = MyVec3Length(dmoku);
		//unko = unko/10;
		if (unko < 0.000001) {
			unko = 1;
		}

		bairitu = MyVec3Length(moku);
		tempmoku = moku - ddunko/2.0f; //*unko/bairitu;
		MYVECTOR3 dd = dmoku;
		MYVECTOR3 dd2 = ddmoku;
		float dotdd = MyVec3Dot(dd,dd2);
		float ddd = MyVec3Length(dd);
		if (dotdd < 0) {
		//	bairitu *= 0.399992f;
			float ddunkod = MyVec3Length(ddunko);
			if (abs(ddunkod) ==0) {
				ddunkod = 1;
				ddunko.float3.z = 1.0f;
			}
			if (ddunkod > 100000) {
				ddunko = ddunko /100;
			}
			MYVECTOR3 dmokudayo = dmoku/bairitu*1.5;
			if (abs(dmokudayo.float3.z) > 3.3f) {
			dmokudayo.float3.z /= 100.0f;
			dd.float3.z /=100;
			}
			ddunko = ddunko - dmokudayo/bairitu/*ddunkod**/*2.5;
			uunko = false;
		} else {
			// 近づいているので
		//	bairitu *= 1.2002f;
			uunko = false;
			float ddunkod = MyVec3Length(ddunko);
			if (abs(ddunkod) ==0) {
				ddunkod =1;
				ddunko.float3.z = 1.0f;
			}
			if (ddunkod > 100000) {
				ddunko = ddunko /100;
			}
			MYVECTOR3 dmokudayo = dmoku/bairitu*1.5;
			if (abs(dmokudayo.float3.z) > 3.3f) {
			dmokudayo.float3.z /= 1000.0f;
			dd.float3.z /=100;
			}
			if (abs(dmokudayo.float3.x) > 3.3f) {
			dmokudayo.float3.x/= 1000.0f;
			dd.float3.x /=100;
			}

			ddunko = ddunko + dmokudayo/ddunkod*MyVec3Length(dmoku)*2.5;/// ddunkod;
		}
		dbb = ddd;

		ddmoku = dmoku;
		MYVECTOR3 dddmoku = ddmoku - dmoku;
		if (unko_count ||((abs(dd.float3.z) < 1.95 && MyVec3Length(dddmoku) < 0.7f))) {
			// ないときはsetthetaしない
			//ap->resetTheta();
			if (unko_count ==0) {
				unko_count = 10;
			}
			unko_count--;
		} else {

		MYMATRIX wo;
		MyMatrixIdentity(wo);
		OBB ob;

	//	if (ap8.isInPoint(&tempmoku)) {

		ob.c = tempmoku;
		g->drawOBBFill(g,0xFFFFFF00,&wo,view,g->getProj(),&ob);
		ArmPoint anp = ap8.getPoint(&tempmoku);
		if (anp.is_ok) {
			
			ap->setTheta(anp.dthetaxa,anp.dthetaxb,anp.dthetaya,anp.dthetayb,anp.dthetaza,anp.dthetazb);
		}else {
			
		}
		}
//		}

	}

	MYVECTOR3 ttmp =moku;
	if (ap->positionArm3(g,view,robo,&ttmp, is_migi) != KTROBO_ARMPOSITION_DAME) {


											if (is_migi) {
		
												 ap->setArm3(robo,true, robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]],
													 robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]]);
											} else {
												 ap->setArm3(robo,false, robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["uparmBone"]],
													 robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]]);
								
											}
												 if (nocalcyet) {
													 dmoku = MYVECTOR3(0,0,0);
													nocalcyet = false;
													} else {
													dmoku = ap->getDMOKU();
													}
											if (is_migi) {	 
												 robo->arm->rarm->animate(40,false);
											} else {
												robo->arm->larm->animate(40,false);
											}

		is_calced = true;
	} else {

		if (nocalcyet) {
			nocalcyet = false;
			dmoku = MYVECTOR3(0,0,0);
		} else {
		dmoku = ap->getDMOKU();
		}
		if (is_migi) {
		robo->arm->rarm->animate(40,false);
		} else {
			robo->arm->larm->animate(40,false);
		}
	}

	if (ap->getReseted()) {
		nocalcyet = true;
	}
}


void ArmPositionerHelper::new_calc(Graphics* g, MYMATRIX* view) {
	MYVECTOR3 ttmp = moku;

	if (ap->getIsSet()) {

		ap->hanneiSetTheta(robo, is_migi);

		ddmoku = MYVECTOR3(0, 0, 0);
		ddunko = MYVECTOR3(0, 0, 0);
		dbb = 100000;
		unko_count = 0;
		bairitu = 1;
		uunko = false;
		dmoku = MYVECTOR3(0, 0, 0);
		nocalcyet = false;
		ap->setDMOKU(&dmoku);
		return;
	}


	if (ap->getReseted()) {
		
		ap->hanneiSetTheta(robo, is_migi);

		ddmoku = MYVECTOR3(0, 0, 0);
		ddunko = MYVECTOR3(0, 0, 0);
		dbb = 100000;
		unko_count = 0;
		bairitu = 1;
		uunko = false;
		dmoku = MYVECTOR3(0, 0, 0);
		nocalcyet = false;
	}

	if (nocalcyet) {
		ddmoku = MYVECTOR3(0, 0, 0);
		ddunko = MYVECTOR3(0, 0, 0);
		dbb = 100000;
		unko_count = 0;
		bairitu = 1;
		uunko = false;

		ArmPoint anp = ap8.getPoint(&moku);
		if (anp.is_ok) {
			ap->setTheta(anp.dthetaxa, anp.dthetaxb, anp.dthetaya, anp.dthetayb, anp.dthetaza, anp.dthetazb);
		}

		//		if (is_calced) {
		//			return;
		//		}

	}
	else {

		// tempmokuを計算する
		float unko = MyVec3Length(dmoku);
		//unko = unko/10;
		if (unko < 0.000001) {
			unko = 1;
		}

		bairitu = MyVec3Length(moku);
		tempmoku = moku - ddunko / 2.0f; //*unko/bairitu;
		MYVECTOR3 dd = dmoku;
		MYVECTOR3 dd2 = ddmoku;
		float dotdd = MyVec3Dot(dd, dd2);
		float ddd = MyVec3Length(dd);
		float ddd2 = MyVec3Length(dd2);
		if (dotdd < 0) {
			//	bairitu *= 0.399992f;
			float ddunkod = MyVec3Length(ddunko);
			if (abs(ddunkod) == 0) {
				ddunkod = 1;
				ddunko.float3.z = 1.0f;
			}
			if (ddunkod > 100000) {
				ddunko = ddunko / 100;
			}
			MYVECTOR3 dmokudayo = dmoku / bairitu * 1.5;
			if (abs(dmokudayo.float3.z) > 3.3f) {
				dmokudayo.float3.z /= 100.0f;
				dd.float3.z /= 100;
			}
			ddunko = ddunko - dmokudayo / bairitu/*ddunkod**/ * 2.5;
			uunko = false;
			DebugTexts::instance()->setText(g, 1, L"反");

		}
		else {

			if (ddd > ddd2) {
				DebugTexts::instance()->setText(g, 1, L"遠");
				MYVECTOR3 dmokudayo = dmoku / bairitu / 1.5;
				float ddunkod = MyVec3Length(ddunko);
				ddunko = ddunko + dmokudayo / ddunkod * MyVec3Length(dmoku)*5.5;/// ddunkod;
			}
			else {
				DebugTexts::instance()->setText(g, 1, L"近");



				// 近づいているので
				//	bairitu *= 1.2002f;
				uunko = false;
				float ddunkod = MyVec3Length(ddunko);
				if (abs(ddunkod) == 0) {
					ddunkod = 1;
					ddunko.float3.z = 1.0f;
				}
				if (ddunkod > 100000) {
					ddunko = ddunko / 100;
				}
				MYVECTOR3 dmokudayo = dmoku / bairitu * 1.5;
				if (abs(dmokudayo.float3.z) > 3.3f) {
					dmokudayo.float3.z /= 1000.0f;
					dd.float3.z /= 100;
				}
				if (abs(dmokudayo.float3.x) > 3.3f) {
					dmokudayo.float3.x /= 1000.0f;
					dd.float3.x /= 100;
				}
			
			ddunko = ddunko + dmokudayo / ddunkod * MyVec3Length(dmoku)*2.5;/// ddunkod;
			//DebugTexts::instance()->setText(g, 1, L"近");
			}


		}
		dbb = ddd;

		ddmoku = dmoku;
		MYVECTOR3 dddmoku = ddmoku - dmoku;
/*		if (unko_count || ((abs(dd.float3.z) < 1.95 && MyVec3Length(dddmoku) < 0.7f))) {
			// ないときはsetthetaしない
			//ap->resetTheta();
			if (unko_count == 0) {
				unko_count = 10;
			}
			unko_count--;
		}
		else {
		*/
//			MYMATRIX wo;
//			MyMatrixIdentity(wo);
//			wo = robo->atarihan->world;
//			OBB ob;

			//	if (ap8.isInPoint(&tempmoku)) {
	/*
			ob.c = tempmoku;
			g->drawOBBFill(g, 0xFFFFFF00, &wo, view, g->getProj(), &ob);

			ob.c = ap->mokudayo;
			g->drawOBBFill(g, 0xFFFFFFFF, &wo, view, g->getProj(), &ob);
	*/
			ArmPoint anp = ap8.getPoint(&tempmoku);
			if (anp.is_ok) {

				ap->setTheta(anp.dthetaxa, anp.dthetaxb, anp.dthetaya, anp.dthetayb, anp.dthetaza, anp.dthetazb);
			}
			else {

			}
	//	}
	}

	if (ap->new_positionArm3(g, view, robo, &ttmp, is_migi) != KTROBO_ARMPOSITION_DAME) {


		if (is_migi) {

			ap->setArm3(robo, true, robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]],
				robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]]);
		}
		else {
			ap->setArm3(robo, false, robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["uparmBone"]],
				robo->arm->larm->Bones[robo->arm->larm->BoneIndexes["downarmBone"]]);

		}
		if (nocalcyet) {
			nocalcyet = false;
			dmoku = MYVECTOR3(0, 0, 0);
		}
		else {
			dmoku = ap->getDMOKU();
		}
		
		if (is_migi) {
			robo->arm->rarm->animate(40, false);
		}
		else {
			robo->arm->larm->animate(40, false);
		}

		is_calced = true;
	}
	else {
		if (nocalcyet) {
			nocalcyet = false;
			dmoku = MYVECTOR3(0, 0, 0);
		}
		else {
			dmoku = ap->getDMOKU();
		}
	
		if (is_migi) {
			robo->arm->rarm->animate(40, false);
		}
		else {
			robo->arm->larm->animate(40, false);
		}
	}


}



ArmPointIndexInfo::ArmPointIndexInfo(string filename, float dmin, float dmax,
									 float mintate, float maxtate, float minyoko, float maxyoko,
									 float dtate, float dyoko, float dd) {
	this->filename = filename;
	this->dmin = dmin;
	this->dmax = dmax;
	this->mintate = mintate;
	this->maxtate = maxtate;
	this->minyoko = minyoko;
	this->maxyoko = maxyoko;
	this->dtate = dtate;
	this->dyoko = dyoko;
	this->dd = dd;
	apw = 0;
}
ArmPointIndexInfo::~ArmPointIndexInfo() {
	vector<ArmPointWithIndex*>::iterator it;
	it = points.begin();
	while(it != points.end()) {

		ArmPointWithIndex* i = *it;
		delete i;
		i = 0;
		it = it + 1;
	}
	points.clear();
}


ArmPoint* ArmPointIndexInfo::getArmPointFromPointindex(int pointindex, MYVECTOR3* pos) {
	LockOnSystem los;
	int index = los.getIndexOfPoint8(pointindex, pos, dmin,dmax,mintate,maxtate,minyoko,maxyoko,dtate,dyoko,dd);
	if (index == -1) {
		return NULL;
	}

	return getArmPoint(index);
}

ArmPoint* ArmPointIndexInfo::getArmPoint(int index) {
	if (index >=0 && index < points.size()) {
		if (points[index]->index == index) {
			return &points[index]->point;
		} else {
			vector<ArmPointWithIndex*>::iterator it;
			it = points.begin();
			while(it != points.end()) {
				ArmPointWithIndex* ii = *it;
				if (ii->index == index) {
					return &ii->point;
				}
				it = it + 1;
			}
		}
	}

	return NULL;
}
void ArmPointIndexInfo::makeNewFile() {
	FILE* file;
	CS::instance()->enter(CS_LOG_CS, "armpoint make");
	if(0 != fopen_s(&file,filename.c_str(),"w")) {
		KTROBO::mylog::writelog(KTROBO::WARNING, "%s の書き込みに失敗", filename.c_str());
		CS::instance()->leave(CS_LOG_CS, "armpoint makel");
		return;
	}
	fclose(file);

	LockOnSystem los;
	int max_pointnum = los.getStudyPointNum(dmin,dmax,mintate,maxtate,minyoko,maxyoko,dtate,dyoko,dd);

	// lockonsysteminfo 
	KTROBO::mylog::writelog(filename.c_str(), "LOCKONSYSTEMINFO {\n");
	mylog::writelog(filename.c_str(), "filename=\"%s\";\n", filename.c_str());
	mylog::writelog(filename.c_str(), "pointnum=%d;\n",max_pointnum);
	mylog::writelog(filename.c_str(), "dmin=%f;\n", dmin);
	mylog::writelog(filename.c_str(), "dmax=%f;\n", dmax);
	mylog::writelog(filename.c_str(), "mintate=%f;\n", mintate);
	mylog::writelog(filename.c_str(), "maxtate=%f;\n", maxtate);
	mylog::writelog(filename.c_str(), "minyoko=%f;\n", minyoko);
	mylog::writelog(filename.c_str(), "maxyoko=%f;\n", maxyoko);
	mylog::writelog(filename.c_str(), "dtate=%f;\n", dtate);
	mylog::writelog(filename.c_str(), "dyoko=%f;\n", dyoko);
	mylog::writelog(filename.c_str(), "dd=%f;\n", dd);
	mylog::writelog(filename.c_str(), "}\n");
	CS::instance()->leave(CS_LOG_CS, "armpoint makel");
}

void ArmPointIndexInfo::loadFile() {
	MyTokenAnalyzer mt;
	if (!mt.load(filename.c_str())) {
		KTROBO::mylog::writelog(KTROBO::WARNING, "%s の書き込みに失敗", filename.c_str());
		return;
	}

	mt.GetToken(); // LOCKONSYSTEMINFO
	mt.GetToken(); // {
	mt.GetToken(); // filename
	mt.GetToken();
	char buf[512];
	memset(buf,0,512);
	mystrcpy(buf,512,0,mt.Toke());
	filename = string(buf);
	mt.GetToken(); // pointnum
	int max_pointnum = mt.GetIntToken();
	mt.GetToken(); // dmin
	float tempdmin = mt.GetFloatToken();
	dmin = tempdmin;
	mt.GetToken(); // dmax
	dmax = mt.GetFloatToken();
	mt.GetToken(); // mintate
	mintate = mt.GetFloatToken();
	mt.GetToken(); // maxtate
	maxtate = mt.GetFloatToken();
	mt.GetToken(); // minyoko
	minyoko = mt.GetFloatToken();
	mt.GetToken(); // maxyoko
	maxyoko = mt.GetFloatToken();
	mt.GetToken(); // dtate
	dtate = mt.GetFloatToken();
	mt.GetToken(); // dyoko
	dyoko = mt.GetFloatToken();
	mt.GetToken(); // dd
	dd = mt.GetFloatToken();

	mt.GetToken(); // }

	// 以下はarmpointwithindex
	mt.GetToken();
	while(!mt.enddayo()) {
		if (strcmp("APWITHINDEX",mt.Toke())==0) {
			mt.GetToken(); // {
			int tempindex = mt.GetIntToken();
			float posx = mt.GetFloatToken();
			float posy = mt.GetFloatToken();
			float posz = mt.GetFloatToken();
			float dthetaxa = mt.GetFloatToken();
			float dthetaxb = mt.GetFloatToken();
			float dthetaya = mt.GetFloatToken();
			float dthetayb = mt.GetFloatToken();
			float dthetaza = mt.GetFloatToken();
			float dthetazb = mt.GetFloatToken();
			mt.GetToken(); // }
			ArmPointWithIndex* ap = new ArmPointWithIndex();
			ap->index = tempindex;
			ap->indexinfo = this;
			ap->is_calced = true;
			ap->point.pos = MYVECTOR3(posx,posy,posz);
			ap->point.dthetaxa = dthetaxa;
			ap->point.dthetaxb = dthetaxb;
			ap->point.dthetaya = dthetaya;
			ap->point.dthetayb = dthetayb;
			ap->point.dthetaza = dthetaza;
			ap->point.dthetazb = dthetazb;
			ponums.insert(ap->index);
			points.push_back(ap);
		}
		mt.GetToken(); // next
	}
	mt.deletedayo();
	setNextIndex();
}
bool ArmPointIndexInfo::hasFile() {

	FILE* file;
	CS::instance()->enter(CS_LOG_CS, "armpoint hasfile");
	if(0 != fopen_s(&file,filename.c_str(),"r")) {
		CS::instance()->leave(CS_LOG_CS, "armpoint hasfile");
		return false;
	}
	fclose(file);
	CS::instance()->leave(CS_LOG_CS, "armpoint hasfile");
	return true;

}

// 現状のデータを全てセーブする
void ArmPointIndexInfo::saveFile() {
	makeNewFile();
	vector<ArmPointWithIndex*>::iterator it;
	it = points.begin();
	while(points.end() != it) {

		ArmPointWithIndex* apwi = *it;
		if (apwi->is_calced) {

			mylog::writelog(filename.c_str(), "APWITHINDEX {\n");
			mylog::writelog(filename.c_str(), "%d,",apwi->index);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.pos.float3.x);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.pos.float3.y);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.pos.float3.z);

			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetaxa);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetaxb);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetaya);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetayb);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetaza);
			mylog::writelog(filename.c_str(), "%f;\n",apwi->point.dthetazb);


			mylog::writelog(filename.c_str(), "}\n");
		}
		it = it + 1;
	}

}
void ArmPointIndexInfo::saveFileWithA() {
	ArmPointWithIndex* apwi = apw;
	if (apwi) {
	if (apwi->is_calced) {

			mylog::writelog(filename.c_str(), "APWITHINDEX {\n");
			mylog::writelog(filename.c_str(), "%d,",apwi->index);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.pos.float3.x);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.pos.float3.y);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.pos.float3.z);

			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetaxa);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetaxb);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetaya);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetayb);
			mylog::writelog(filename.c_str(), "%f,",apwi->point.dthetaza);
			mylog::writelog(filename.c_str(), "%f;\n",apwi->point.dthetazb);


			mylog::writelog(filename.c_str(), "}\n");
		}
	}

}

#define KTROBO_ARMPOINT_NONNUM -1
int ArmPointIndexInfo::getNonCalcedIndex() {
	LockOnSystem los;
	int pointnum = los.getStudyPointNum(dmin,dmax,mintate,maxtate,minyoko,maxyoko,dtate,dyoko,dd);
	for (int i=0;i<pointnum;i++) {
		if (ponums.find(i) != ponums.end()) {
			continue;
		}
		return i;
	}

	return KTROBO_ARMPOINT_NONNUM;

}

void ArmPointIndexInfo::setNextIndex() {
	if (!apw || apw->is_calced) {
			// next index をセットする
			int nexindex = getNonCalcedIndex();
			if (apw && (apw->index == -1)) return;
			apw = new ArmPointWithIndex();
			apw->indexinfo = this;
			apw->index = nexindex;
			now_index = apw->index;
			apw->point.pos = getIndexPos();
			apw->point.dthetaxa = 0;
			apw->point.dthetaxb = 0;
			apw->point.dthetaya = 0;
			apw->point.dthetayb = 0;
			apw->point.dthetaza = 0;
			apw->point.dthetazb = 0;
			apw->is_calced = false;
			apw->point.is_ok = false;
			points.push_back(apw);
			ponums.insert(apw->index);
	}
		
}
MYVECTOR3 ArmPointIndexInfo::getIndexPos() {
	LockOnSystem los;
	if (apw) {
		MYVECTOR3 test = los.getPosOfStudyPoint(apw->index, dmin, dmax, mintate, maxtate, minyoko, maxyoko, dtate, dyoko, dd);
		return test;
	}

	return MYVECTOR3(0,0,0);
}
bool ArmPointIndexInfo::isCalced() {
		if (apw) {
			return apw->is_calced;
		}
		return false;
}
bool ArmPointIndexInfo::isCalcFinished() {
	LockOnSystem los;
	int pointnum = los.getStudyPointNum(dmin,dmax,mintate,maxtate,minyoko,maxyoko,dtate,dyoko,dd);

	if (pointnum >= points.size()) {
		return false;
	}
	return true;
}

int ArmPointIndexInfo::getNowIndex() {

	if (apw) {
		return apw->index;
	}
	return KTROBO_ARMPOINT_NONNUM;

}


void ArmPointIndexInfo::saveDtheta(ArmPoint* save_data, int index) {

	if (apw) {
		if (apw->index != index) {
			mylog::writelog(KTROBO::WARNING, "armpoint index savedthetaerror");
			return;
		} else {
			apw->point = *save_data;
			apw->point.is_ok = true;
			apw->is_calced = true;
			return;
		}
	}


}