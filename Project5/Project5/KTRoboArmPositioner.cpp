#include "KTRoboArmPositioner.h"
//#include "MyBillBoard.h"
//#include "NEWGUI_OBSERVER.h"
#include "KTRoboRobo.h"
//#include "MyCameraAndBaseLine.h"
#include "MyGyouretuKeisan.h"
#include <iostream>
#include <cmath>
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
		robo->arm->rarm->getOffsetMatrixToGetMinMaxAndWeightIndex(bn, frame, &ans_minmax, &ans_maxmin, &weight);
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
		robo->arm->rarm->getOffsetMatrixToGetMinMaxAndWeightIndex(bn, frame, &ans_minmax, &ans_maxmin, &weight);
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
	MYVECTOR3 mokuhf = arm2tomoku + arm2_pos+ unkov;



	


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
		

		if (len < 0.5f) {
			mada_count = 0;
			warukazu = 600000;
			return KTROBO_ARMPOSITION_FINISH;
			//und = true;
		} else if (mlen < 0.12f) {
			mada_count = 0;
			warukazu = 3800;
			und = true;
		} else if (mlen < 0.9f) {
			mada_count -=1;
			warukazu = 160;
			//und = true;

		} else if (mlen <1.8f) {
			
			warukazu = 2;
		} else {
			warukazu = 1;
			mada_count += 3;
		//	resetTheta();
      		positionArm33(g, view, robo, moku, is_migi);

		}
		mada_count++;
		if (mada_count > 760) {
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
		float dthetaxa=0;
		float dthetaya=0;
		float dthetaza=0;
		MyIKMODOKI modoki(robo->arm->larm,&wo,&mokuhf,"uparmBone", "handBone");
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


	if (und) {
		return KTROBO_ARMPOSITION_OK;
	}

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
	gui->Init(hw, tex,l,screen_width,screen_height);

	screen_window_id = gui->makeWindow(0,0,screen_width,screen_height);
	gui->setEffect(screen_window_id,true);
	gui->setEnable(screen_window_id,true);
	gui->setRender(screen_window_id, true);


	MYRECT zentai;
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

}

void ShudouArmPositioner::Del() {
	if (gui) {
		gui->deleteAll();
		delete gui;
		gui = 0;
	}
}

void ShudouArmPositioner::update() {
	bool t=false;

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
	if (t) {
		dthetaxa = gui->getNowFromSlider(slider_dxa);
		dthetaxb = gui->getNowFromSlider(slider_dxb);
		dthetaya = gui->getNowFromSlider(slider_dya);
		dthetayb = gui->getNowFromSlider(slider_dyb);
		dthetaza = gui->getNowFromSlider(slider_dza);
		dthetazb = gui->getNowFromSlider(slider_dzb);
		ap->setTheta(dthetaxa, dthetaxb, dthetaya, dthetayb, dthetaza, dthetazb);
	}

}