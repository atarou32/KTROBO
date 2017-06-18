#include "KTRoboArmPositioner.h"
//#include "MyBillBoard.h"
//#include "NEWGUI_OBSERVER.h"
#include "KTRoboRobo.h"
//#include "MyCameraAndBaseLine.h"
#include "MyGyouretuKeisan.h"

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

	
	MyMatrixIdentity(maat);
	MyMatrixIdentity(mbbt);
	MyMatrixRotationAxis(maat, yd, -epsiron);
	MyMatrixRotationAxis(mbbt, xd, pusai);
	MYMATRIX mcct;


	float aba = acos(-mokuhyou->float3.y/L)/3;




	MyMatrixRotationAxis(mcct, zd, aba/*-3.14/2*//* 3.14/2*/);
	MyMatrixMultiply(maat, mbbt, maat);
	//D3DXMatrixMultiply(&maat, &mcct, &maat);

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
	MyMatrixRotationAxis(mcct, yd, aba-3.14/2);////* 3.14/2*/);
//	mylog::writelog("%f,\n", aba);
	MyMatrixMultiply(mbbt, mbbt, mcct);

	arm2->offset_matrix = mbbt;
	//MyMatrixMultiply(arm2->offset_matrix, mbbt,arm2->offset_matrix);
	//MyMatrixRotationY(maat,1.57);
	//MyMatrixMultiply(arm2->offset_matrix,maat, arm2->offset_matrix);



	return true;
}


bool ArmPositioner::positionArm3(Graphics* g , MYMATRIX* view, Robo* robo, MYVECTOR3* moku, bool is_migi) {





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

	MYVECTOR3 mokuhf = arm2tomoku + arm2_pos;

	


	MYMATRIX wo;
	MyMatrixIdentity(wo);
	OBB re;
	re.c = mokuhf;

	g->drawOBB(g,0xFFFFFF00,&wo,view,g->getProj(), &re);

	if (is_migi) {
		float dthetaxa=0;
		float dthetaya=0;
		float dthetaza=0;

		float dthetaxb=0;
		float dthetayb=0;
		float dthetazb=0;

		for (int i = 0; i<5;i++) {
		MyIKMODOKI modoki(robo->arm->rarm,&robo->atarihan->world,&mokuhf,"doBone", "handBone");
		modoki.setXFreeBone("downarmBone");
		modoki.setYFreeBone("downarmBone");
		modoki.setZFreeBone("downarmBone");
		/*
		modoki.setXFreeBone("uparmBone");
		modoki.setYFreeBone("uparmBone");
		modoki.setZFreeBone("uparmBone");
		*/
		for(int i = 0;i < 1;i++) {
		modoki.updateStep();
		}
		float dthetax = modoki.getdthetaXBone("downarmBone")/60.0;
		float dthetay = modoki.getdthetaYBone("downarmBone")/60.0;
		float dthetaz = modoki.getdthetaZBone("downarmBone")/60.0;

		dthetaxa += dthetax;
		dthetaya += dthetay;
		dthetaza += dthetaz;
/*
		dthetax = modoki.getdthetaXBone("uparmBone")/3.14/60;
		dthetay = modoki.getdthetaYBone("uparmBone")/3.14/60;
		dthetaz = modoki.getdthetaZBone("uparmBone")/3.14/60;


		dthetaxb += dthetax;
		dthetayb += dthetay;
		dthetazb += dthetaz;
		*/
		{
		MYMATRIX rotxmat;
		MYMATRIX rotymat;
		MYMATRIX rotzmat;
		MyMatrixRotationX(rotxmat, dthetaxa);
		MyMatrixRotationY(rotymat, dthetaya);
		MyMatrixRotationZ(rotzmat, dthetaza);





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
//		MyMatrixMultiply(ans, ans, rotymat);
//		MyMatrixMultiply(ans, ans, trans);
		MyMatrixMultiply(ans, ans, mat3);
		robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["downarmBone"]]->offset_matrix = ans;
		robo->arm->rarm->animate(40,false);
		}
		/*
		{
		MYMATRIX rotxmat;
		MYMATRIX rotymat;
		MYMATRIX rotzmat;
		MyMatrixRotationX(rotxmat, dthetaxb);
		MyMatrixRotationY(rotymat, dthetayb);
		MyMatrixRotationZ(rotzmat, dthetazb);
		MYMATRIX offmat= 
			robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]]->offset_matrix;
		MyMatrixMultiply(offmat, offmat,rotzmat);
		MyMatrixMultiply(offmat, offmat,rotxmat);
		MyMatrixMultiply(offmat, offmat, rotymat);

		robo->arm->rarm->Bones[robo->arm->rarm->BoneIndexes["uparmBone"]]->offset_matrix = offmat;
		}
		*/



		}

	} else {
		float dthetaxa=0;
		float dthetaya=0;
		float dthetaza=0;
		for (int i= 0;i<5;i++) {
		MyIKMODOKI modoki(robo->arm->larm,&wo,&mokuhf,"uparmBone", "handBone");
		modoki.setXFreeBone("downarmBone");
		modoki.setYFreeBone("downarmBone");
		modoki.setZFreeBone("downarmBone");
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



	return true;
}
