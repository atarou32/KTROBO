#ifndef MYBUTUKARI3D_H
#define MYBUTUKARI3D_H


#pragma once
#include "MyDefine.h"

class KTRoboTikeiFace;
using namespace MYDEFINE;

struct KTRoboTikeiFaceForSCENETEST {
	KTRoboTikeiFace* f;
	int sector_x;
	int sector_y;
};

struct PLANEDAYO {
	MYVECTOR3 housen;
	MYVECTOR3 c; // 中心の座標
};

class OBB {
public:
	MYVECTOR3 c; // 中心の座標
	MYVECTOR3 u[3];
	MYVECTOR3 e; // 中心点から面までの長さ

	OBB() {
		c.float3.x = 0;
		c.float3.y = 0;
		c.float3.z = 0;
		u[0] = MYVECTOR3(1,0,0);
		u[1] = MYVECTOR3(0,1,0);
		u[2] = MYVECTOR3(0,0,1);
		e = MYVECTOR3(1,1,1);
	}

	OBB& operator=(OBB& value) {
		this->c = value.c;
		this->e = value.e;
		this->u[0] = value.u[0];
		this->u[1] = value.u[1];
		this->u[2] = value.u[2];
        return *this;
    }

	OBB& operator+(OBB& right_obb) {
		OBB left_obb = *this;
			float left_maxz = left_obb.c[2] + left_obb.e[2];
			float left_minz = left_obb.c[2] - left_obb.e[2];
			float left_maxy = left_obb.c[1] + left_obb.e[1];
			float left_miny = left_obb.c[1] - left_obb.e[1];
			float left_maxx = left_obb.c[0] + left_obb.e[0];
			float left_minx = left_obb.c[0] - left_obb.e[0];

			float right_maxz = right_obb.c[2] + right_obb.e[2];
			float right_minz = right_obb.c[2] - right_obb.e[2];
			float right_maxy = right_obb.c[1] + right_obb.e[1];
			float right_miny = right_obb.c[1] - right_obb.e[1];
			float right_maxx = right_obb.c[0] + right_obb.e[0];
			float right_minx = right_obb.c[0] - right_obb.e[0];

			float maxz = -100000;
			maxz = max(maxz, left_maxz);
			maxz = max(maxz, right_maxz);
			float minz = 100000;
			minz = min(minz, left_minz);
			minz = min(minz, right_minz);
			float maxx = -100000;
			float minx = 100000;
			float maxy = -100000;
			float miny = 100000;
			maxx = max(maxx, left_maxx);
			maxx = max(maxx, right_maxx);
			minx = min(minx, left_minx);
			minx = min(minx, right_minx);

			maxy = max(maxy, left_maxy);
			maxy = max(maxy, right_maxy);
			miny = min(miny, left_miny);
			miny = min(miny, right_miny);


			this->c.float3.z = (maxz + minz) / 2;
			this->c.float3.y = (maxy + miny )/2;
			this->c.float3.x = (maxx + minx) /2;
			this->u[0] = MYVECTOR3(1,0,0);
			this->u[1] = MYVECTOR3(0,1,0);
			this->u[2] = MYVECTOR3(0,0,1);
			this->e.float3.z = (maxz - minz) / 2;
			this->e.float3.y = (maxy - miny )/2;
			this->e.float3.x = (maxx - minx) /2;
			return *this;
	}
};

struct SPHERE
{
    MYVECTOR3 c;
    float r;
};

struct RAY
{
    MYVECTOR3 org;
    MYVECTOR3 dir;
};

typedef MYVECTOR3 intersection[2];
enum {
	INTERSECTION_NORMAL,
	INTERSECTION_TUV
};


#define PLANERAYBUTUKARI_FUKUMU 1
#define PLANERAYBUTUKARI_BUTUKARI 2
#define PLANERAYBUTUKARI_NONE 0

int testPLANEDAYOFROMMOUSEPOINT(PLANEDAYO* plane, RAY* ray, MYVECTOR3* out_butukari_pos);


void ClosestPtPointOBB(MYVECTOR3 *p, OBB *b, MYVECTOR3 *q);
int TestSphereOBB(SPHERE *s, OBB *b, MYVECTOR3 *p);

int TestSegmentOBB(RAY *ray, OBB *obb);

int TestOBBOBB(OBB *a, OBB *b);
void getTranslatedOBB(OBB* new_obb, OBB* old_obb, MYVECTOR3* pos, MYVECTOR3* rot, MYVECTOR3* scale);





#define YATTEYARU_MENPOINT_ON 0x0001
#define YATTEYARU_MENPOINT_OUT 0x0002
#define YATTEYARU_MENPOINT_IN 0x0004

int yatteyaruMenPoint(MYVECTOR3* point_pos, MYVECTOR3* men_pos, MYVECTOR3* menvec1, MYVECTOR3* menvec2);

bool majiwaruTriangleRay(intersection* out_inter, MYVECTOR3* pqpr_suityoku, MYVECTOR3* p, MYVECTOR3* binormal_u, MYVECTOR3* binormal_v, RAY* ray);
bool majiwaruTriangleSenbun(MYVECTOR3* pqpr_suityoku,  MYVECTOR3* p,MYVECTOR3* q, MYVECTOR3* r, RAY* ray);

bool majiwaruTriangleCircle(float s_r_2, MYVECTOR3* s_pos, MYVECTOR3* p,MYVECTOR3* q, MYVECTOR3* r,bool is_in_only=false);
bool majiwaruTriangleOBB(MYVECTOR3* pqpr_suityoku, MYVECTOR3* p, MYVECTOR3* q, MYVECTOR3* r, OBB* b, MYVECTOR3* out_majiwaripos);
bool majiwaruPointOBB(MYVECTOR3* point, OBB* b);

void hanneiMatrixOfPosRotScaledayo(MYMATRIX* out_mat, MYMATRIX* out_mat_rot, MYMATRIX* out_mat_scale, MYVECTOR3* now_put_part_pos,MYVECTOR3* now_put_part_rot, MYVECTOR3* now_put_part_scale);



// 三角形と三角形の交差判定を行う

struct TRIANGLEDAYO {
	MYVECTOR3 x;
	MYVECTOR3 y;
	MYVECTOR3 z;
};


#define YATTEYARU_TRIANGLE_KOUSA 1
#define YATTEYARU_TRIANGLE_OUT 2
#define YATTEYARU_TRIANGLE_UNKNOWN 0


int hanteiTRIANGLEDAYO( TRIANGLEDAYO* tri1, TRIANGLEDAYO* tri2, MYVECTOR3* k_ans );
int hanteiTRIANGLETOHEN( RAY* ray1, TRIANGLEDAYO* tri2, MYVECTOR3* jyusin, MYVECTOR3* housen,MYVECTOR3* ans_ten);
int hanteiTRIANGLETOOBB(TRIANGLEDAYO* tri1, OBB* obb2);




















#endif