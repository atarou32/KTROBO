#include "MyButukari3D.h"
#include "MyTextureLoader.h"



using namespace MYDEFINE;

int testPLANEDAYOFROMMOUSEPOINT(PLANEDAYO* plane, RAY* ray, MYVECTOR3* out_butukari_pos) {
	float c1  =  MyVec3Dot(ray->dir,plane->housen);
	MYVECTOR3 temp;
	MyVec3Subtract(temp,ray->org,plane->c);
	float c2 = MyVec3Dot(temp, plane->housen);
	if ( c1==0) {
		if (c2 ==0) {
			return PLANERAYBUTUKARI_FUKUMU;
		}
		return PLANERAYBUTUKARI_NONE;
	}

	// 交点を計算する
//	MYVECTOR3 ans_pos = ray->org + w * ray->dir;
//  wを求める
	float w = (MyVec3Dot(plane->housen, plane->c) - MyVec3Dot(plane->housen, ray->org))/ c1;
	MYVECTOR3 ans_pos = ray->org + /*w * */ray->dir;
	*out_butukari_pos = ans_pos;

	return PLANERAYBUTUKARI_BUTUKARI;
}


void ClosestPtPointOBB(MYVECTOR3 *p, OBB *b, MYVECTOR3 *q)
{
    MYVECTOR3 d = *p - b->c;
    *q = b->c;
    float dist;
    for(int i = 0; i < 3; i++)
    {
        dist = MyVec3Dot(d, b->u[i]);

        if(dist > b->e[i])
        {
            dist = b->e[i];
        }
        if(dist < -b->e[i])
        {
            dist = -b->e[i];
        }
        *q = *q + b->u[i] * dist;
    }
}


bool heimenUeni(MYVECTOR3* p, MYVECTOR3* u, MYVECTOR3* x) {

	MYVECTOR3 px = *x - *p;

	float test = MyVec3Dot((*u),px);
	if (test > 0) {
		return true;
	}

	return false;

}


bool majiwaruPointOBB(MYVECTOR3* point, OBB* b) {

	MYVECTOR3 point1 = b->c +b->u[0] *  b->e[0];
	MYVECTOR3 point2 = b->c +(-b->u[0]) *   b->e[0]; 
	MYVECTOR3 point3 = b->c +b->u[1] *	   b->e[1]; 
	MYVECTOR3 point4 = b->c +(-b->u[1]) *   b->e[1]; 
	MYVECTOR3 point5 = b->c +b->u[2] *	   b->e[2]; 
	MYVECTOR3 point6 = b->c +(-b->u[2]) *   b->e[2]; 

	point1 = *point - point1;
	point2 = *point - point2;
	point3 = *point - point3;
	point4 = *point - point4;
	point5 = *point - point5;
	point6 = *point - point6;
	MYXMFLOAT3 test = (-b->u[0]);
	float dot1 = MyVec3Dot(point1, test);
	test = b->u[0];
	float dot2 = MyVec3Dot(point2, test);
	test = -b->u[1];
	float dot3 = MyVec3Dot(point3, test);
	test = b->u[1];
	float dot4 = MyVec3Dot(point4, test);
	test = -b->u[2];
	float dot5 = MyVec3Dot(point5, test);
	test = b->u[2];
	float dot6 = MyVec3Dot(point6, test);

	if ((dot1 >=0) && (dot2 >=0) && (dot3>=0) && (dot4 >=0) && (dot5 >= 0) && (dot6 >= 0)) {
		return true;
	}

	return false;
}


// 接する場合も交わるうちに入る
bool majiwaruTriangleOBB(MYVECTOR3* pqpr_suityoku, MYVECTOR3* p, MYVECTOR3* q, MYVECTOR3* r, OBB* b, MYVECTOR3* out_majiwaripos) {

	// OBB と接する外接球X
	float x_r_2 = b->e[0]*b->e[0] + b->e[1] * b->e[1] + b->e[2] * b->e[2];
//	MYVECTOR3 pqpr_suityoku;
	MYVECTOR3 pq = *q - *p;
	MYVECTOR3 pr = *r - *p;
	
	// 外積ベクトルを求める
//	MyVec3Cross(&pqpr_suityoku, &pq, &pr);
	// 外積ベクトルとpbの中心の内積をとる
//	MyVec3Normalize(&pqpr_suityoku, &pqpr_suityoku);
	// pとbの中心の長さとXの長さを比べる
	MYVECTOR3 pb = b->c - *p;
	MYVECTOR3 pq_pr = *pqpr_suityoku;
	float nai = MyVec3Dot(pq_pr, pb);
	
	float nai_2 = nai * nai;
	if (nai_2 > x_r_2) {
		// 外接球と平面PQRが交わらないので交わらない
		return false;
	}

	
	// 平面PQRと外接球の交円　の中心を求める
	MYVECTOR3 c = b->c - (*pqpr_suityoku) * nai;
	*out_majiwaripos = c;
	// OBB のなかに点がある場合は必ず交わる
	if (majiwaruPointOBB(p,b)) {
		return true;
	}
	if (majiwaruPointOBB(q,b)) {
		return true;
	}
	if (majiwaruPointOBB(r,b)) {
		return true;
	}

	

	MYVECTOR3 point1 = b->c + b->u[0]*     b->e[0]; 
	MYVECTOR3 point2 = b->c + (-b->u[0])*  b->e[0]; 
	MYVECTOR3 point3 = b->c +  b->u[1]*	   b->e[1];
	MYVECTOR3 point4 = b->c +  (-b->u[1])* b->e[1];
	MYVECTOR3 point5 = b->c +  b->u[2]*	   b->e[2];
	MYVECTOR3 point6 = b->c +  (-b->u[2])* b->e[2];


	if (heimenUeni(&point1, &b->u[0], p) && heimenUeni(&point1, &b->u[0], q)
		&& heimenUeni(&point1, &b->u[0], r)) {
			return false;
	}

	if (heimenUeni(&point2, &(-b->u[0]), p) && heimenUeni(&point2, &(-b->u[0]), q)
		&& heimenUeni(&point2, &(-b->u[0]), r)) {
			return false;
	}

	if (heimenUeni(&point3, &b->u[1], p) && heimenUeni(&point3, &b->u[1], q)
		&& heimenUeni(&point3, &b->u[1], r)) {
			return false;
	}

	if (heimenUeni(&point4, &(-b->u[1]), p) && heimenUeni(&point4, &(-b->u[1]), q)
		&& heimenUeni(&point4, &(-b->u[1]), r)) {
			return false;
	}

	if (heimenUeni(&point5, &(b->u[2]), p) && heimenUeni(&point5, &(b->u[2]), q)
		&& heimenUeni(&point5, &(b->u[2]), r)) {
			return false;
	}

	if (heimenUeni(&point6, &(-b->u[2]), p) && heimenUeni(&point6, &(-b->u[2]), q)
		&& heimenUeni(&point6, &(-b->u[2]), r)) {
			return false;
	}


	
	*out_majiwaripos = c;
	if (majiwaruTriangleCircle(x_r_2 - nai_2, &c, p, q, r,false)) {
		*out_majiwaripos = c;
		
	} else {
		// 交わらないので外にある？
		return false;
	}
	/*
	if (!majiwaruPointOBB(&c, b)) {
		return false;
	}
	*/

	return true;
}

// 接する場合も交わるうちに入る
// s_r_2 は半径を２乗したもの
bool majiwaruTriangleCircle(float s_r_2, MYVECTOR3* s_pos, MYVECTOR3* p,MYVECTOR3* q, MYVECTOR3* r,bool is_in_only) {

//	MYVECTOR3 pq2 = (*q + *p)/2;
//	MYVECTOR3 pr2 = (*r + *p)/2;
//	MYVECTOR3 qr2 = (*r + *q)/2;
	MYVECTOR3 ps = *s_pos - *p;
	MYVECTOR3 qs = *s_pos - *q;
	MYVECTOR3 rs = *s_pos - *r;
	MYVECTOR3 pq = *q - *p;
	MYVECTOR3 pr = *r - *p;
	MYVECTOR3 qr = *r - *q;

	MYVECTOR3 cross_p;
	MYVECTOR3 cross_q;
	MYVECTOR3 cross_r;
	MyVec3Cross(cross_p, pq, qs);
	MyVec3Cross(cross_q,qr, rs);
	MYVECTOR3 test = -pr;
	MyVec3Cross(cross_r, test, ps);//MyVec3Dot(&(-pr), &rs);

	float t = MyVec3Dot(cross_p, cross_q);
	float tr = MyVec3Dot(cross_q, cross_r);


	if (t > 0 && tr > 0) {
		return true;
	}

	if (is_in_only) {
		return false;
	}

	
	float w = -1;
	float s = -1;
	t = -1;

	float pr_pr = MyVec3Dot(pr, pr);
	test = -pq;
	MYVECTOR3 test2 = -pq;
	float qp_qp = MyVec3Dot(test, test2);
	test = -qr;
	float rq_rq = MyVec3Dot(test, test);
	float ps_pr = MyVec3Dot(ps, pr);
	test = -pq;
	float qs_qp = MyVec3Dot(qs, test);
	test = -qr;
	float rs_rq = MyVec3Dot(rs, test);

	if (pr_pr > 0) {
		w = ps_pr / pr_pr;
	}
	if (qp_qp > 0) {
		s = qs_qp / qp_qp;
	}
	if (rq_rq > 0) {
		t = rs_rq / rq_rq;
	}

	test = *p + pr * w - *s_pos;
	test2 = *p + pr * w - *s_pos;
	if (w >= 0 && MyVec3Dot(test,test2) <= s_r_2) {
		return true;
	}

	test = *q + (-pq) * s - *s_pos;

	if (s >= 0 && MyVec3Dot(test,test) <= s_r_2) {
		return true;
	}

	test = *r + (-qr) * t - *s_pos;
	if (t >= 0 && MyVec3Dot(test, test) <= s_r_2) {
		return true;
	}



	
	return false;
}


int TestSphereOBB(SPHERE *s, OBB *b, MYVECTOR3 *p)
{
        ClosestPtPointOBB(&s->c, b, p);
        MYVECTOR3 v = *p - s->c;
        return MyVec3Dot(v, v) <= s->r * s->r;
}

int TestSegmentOBB(RAY *ray, OBB *obb)
{

	/*
    const float EPSILON = 1.175494e-37;

    MYVECTOR3 m = (ray->org + ray->dir) * 0.5f;
    MYVECTOR3 d = ray->dir - m;
    m = m - obb->c;
    m = MYVECTOR3(MyVec3Dot(obb->u[0], m), MyVec3Dot(obb->u[1], m), MyVec3Dot(obb->u[2], m));
    d = MYVECTOR3(MyVec3Dot(obb->u[0], d), MyVec3Dot(obb->u[1], d), MyVec3Dot(obb->u[2], d));

    float adx = fabsf(d[0]);
    if(fabsf(m[0]) > obb->e[0] + adx) return 0;
    float ady = fabsf(d[1]);
    if(fabsf(m[1]) > obb->e[1] + ady) return 0;
    float adz = fabsf(d[2]);
    if(fabsf(m[2]) > obb->e[2] + adz) return 0;
    adx += EPSILON; 
    ady += EPSILON; 
    adz += EPSILON;
        
    if(fabsf(m[1] * d[2] - m[2] * d[1]) > obb->e[1] * adz + obb->e[2] * ady ) return 0;
    if(fabsf(m[2] * d[0] - m[0] * d[2]) > obb->e[0] * adz + obb->e[2] * adx ) return 0;
    if(fabsf(m[0] * d[1] - m[1] * d[0]) > obb->e[0] * ady + obb->e[1] * adx ) return 0;
	*/




	for (int i=0;i<3;i++) {
		for (int k=0;k<2;k++) {
			float test = 1;
			if (k==0) {
			} else {
				test = -1;
			}


			TRIANGLEDAYO tri2;
			TRIANGLEDAYO tri22;
			MYVECTOR3 jyusin;
			MYVECTOR3 housen;
			MYVECTOR3 ans_ten;

			MYVECTOR3 tyu = obb->c + obb->u[i] * test * obb->e[i];
			if (i==0) {
				tri2.x = tyu + obb->u[1] * obb->e[1] + obb->u[2] * obb->e[2];
				tri2.y = tyu - obb->u[1] * obb->e[1] + obb->u[2] * obb->e[2];
				tri2.z = tyu + obb->u[1] * obb->e[1] - obb->u[2] * obb->e[2];
				tri22.x = tyu + obb->u[1] * obb->e[1] - obb->u[2] * obb->e[2];
				tri22.y = tyu - obb->u[1] * obb->e[1] + obb->u[2] * obb->e[2];
				tri22.z = tyu - obb->u[1] * obb->e[1] - obb->u[2] * obb->e[2];
			} else if(i == 1) {
				tri2.x = tyu + obb->u[2] * obb->e[2] + obb->u[0] * obb->e[0];
				tri2.y = tyu - obb->u[2] * obb->e[2] + obb->u[0] * obb->e[0];
				tri2.z = tyu + obb->u[2] * obb->e[2] - obb->u[0] * obb->e[0];
				tri22.x = tyu + obb->u[2] * obb->e[2] - obb->u[0] * obb->e[0];
				tri22.y = tyu - obb->u[2] * obb->e[2] + obb->u[0] * obb->e[0];
				tri22.z = tyu - obb->u[2] * obb->e[2] - obb->u[0] * obb->e[0];
			} else if(i==2) {
				tri2.x = tyu + obb->u[0] * obb->e[0] + obb->u[1] * obb->e[1];
				tri2.y = tyu - obb->u[0] * obb->e[0] + obb->u[1] * obb->e[1];
				tri2.z = tyu + obb->u[0] * obb->e[0] - obb->u[1] * obb->e[1];
				tri22.x = tyu + obb->u[0] * obb->e[0] - obb->u[1] * obb->e[1];
				tri22.y = tyu - obb->u[0] * obb->e[0] + obb->u[1] * obb->e[1];
				tri22.z = tyu - obb->u[0] * obb->e[0] - obb->u[1] * obb->e[1];
			}

			jyusin = tri2.x/3 + tri2.y/3 + tri2.z /3;
			MYVECTOR3 p = tri2.x - jyusin;
			MYVECTOR3 pp = tri2.y - jyusin;
			MyVec3Cross(housen,p,pp);

			int t = hanteiTRIANGLETOHEN(ray, &tri2, &jyusin, &housen, &ans_ten);

			if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;
			jyusin = tri22.x/3 + tri22.y/3 + tri22.z /3;
			p = tri22.x - jyusin;
			pp = tri22.y - jyusin;
			MyVec3Cross(housen,p,pp);
			t = hanteiTRIANGLETOHEN(ray, &tri22, &jyusin, &housen, &ans_ten);
			if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

		}
	}

    return 0;
}

int TestOBBOBB(OBB *a, OBB *b)
{
    const float EPSILON = 1.175494e-37;

    float R[3][3], AbsR[3][3];
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            R[i][j] = MyVec3Dot(a->u[i], b->u[j]);
            AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
        }
    }
        
    MYVECTOR3 t = b->c - a->c;
    t = MYVECTOR3(MyVec3Dot(t, a->u[0]), MyVec3Dot(t, a->u[1]), MyVec3Dot(t, a->u[2]));
        
    //軸L=A0, L=A1, L=A2判定
    float ra, rb;

    for(int i = 0; i < 3; i++)
    {
        ra = a->e[i];
        rb = b->e[0] * AbsR[i][0] + b->e[1] * AbsR[i][1] + b->e[2] * AbsR[i][2];
        if(fabsf(t[i]) > ra + rb)return 0;
    }
    //軸L=B0, L=B1, L=B2判定
    for(int i = 0; i < 3; i++)
    {
        ra = a->e[0] * AbsR[0][i] + a->e[1] * AbsR[1][i] + a->e[2] * AbsR[2][i];
        rb = b->e[i];
        if(fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb)return 0;
    }

    //軸L=A0 X B0判定
    ra = a->e[1] * AbsR[2][0] + a->e[2] * AbsR[1][0];
    rb = b->e[1] * AbsR[0][2] + b->e[2] * AbsR[0][1];
    if(fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)return 0;

    //軸L=A0 X B1判定
    ra = a->e[1] * AbsR[2][1] + a->e[2] * AbsR[1][1];
    rb = b->e[0] * AbsR[0][2] + b->e[2] * AbsR[0][0];
    if(fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)return 0;

    //軸L=A0 X B2判定
    ra = a->e[1] * AbsR[2][2] + a->e[2] * AbsR[1][2];
    rb = b->e[0] * AbsR[0][1] + b->e[1] * AbsR[0][0];
    if(fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)return 0;

    //軸L=A1 X B0判定
    ra = a->e[0] * AbsR[2][0] + a->e[2] * AbsR[0][0];
    rb = b->e[1] * AbsR[1][2] + b->e[2] * AbsR[1][1];
    if(fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)return 0;

    //軸L=A1 X B1判定
    ra = a->e[0] * AbsR[2][1] + a->e[2] * AbsR[0][1];
    rb = b->e[0] * AbsR[1][2] + b->e[2] * AbsR[1][0];
    if(fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)return 0;

    //軸L=A1 X B2判定
    ra = a->e[0] * AbsR[2][2] + a->e[2] * AbsR[0][2];
    rb = b->e[0] * AbsR[1][1] + b->e[1] * AbsR[1][0];
    if(fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)return 0;

    //軸L=A2 X B0判定
    ra = a->e[0] * AbsR[1][0] + a->e[1] * AbsR[0][0];
    rb = b->e[1] * AbsR[2][2] + b->e[2] * AbsR[2][1];
    if(fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)return 0;

    //軸L=A2 X B1判定
    ra = a->e[0] * AbsR[1][1] + a->e[1] * AbsR[0][1];
    rb = b->e[0] * AbsR[2][2] + b->e[2] * AbsR[2][0];
    if(fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)return 0;

    //軸L=A2 X B2判定
    ra = a->e[0] * AbsR[1][2] + a->e[1] * AbsR[0][2];
    rb = b->e[0] * AbsR[2][1] + b->e[1] * AbsR[2][0];
    if(fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)return 0;

    return 1;
}






int yatteyaruMenPoint(MYVECTOR3* point_pos, MYVECTOR3* men_pos, MYVECTOR3* menvec1, MYVECTOR3* menvec2) {
	MYVECTOR3 point_vec = *point_pos - *men_pos;
	MYVECTOR3 men_vec3;
	MYVECTOR3 m1 = *menvec1;
	MYVECTOR3 m2 = *menvec2;
	MyVec3Cross(men_vec3, m1, m2);

	float dot = MyVec3Dot(men_vec3, point_vec);

	if (dot ==0) {
		return YATTEYARU_MENPOINT_ON;
	}else if(dot < 0) {
		return YATTEYARU_MENPOINT_OUT;
	}else {
		return YATTEYARU_MENPOINT_IN;
	}
}




void hanneiMatrixOfPosRotScaledayo(MYMATRIX* mat, MYMATRIX* mat_rot, MYMATRIX* mat_scale, MYVECTOR3* now_put_part_pos,MYVECTOR3* now_put_part_rot, MYVECTOR3* now_put_part_scale) {
	MYMATRIX temp;
	MYMATRIX tempx;
	MYMATRIX tempy;
	MYMATRIX tempz;
	MYMATRIX temp3;
	MYMATRIX temp_rot;
	MyMatrixTranslation(temp,now_put_part_pos->float3.x, now_put_part_pos->float3.y, now_put_part_pos->float3.z);
	
	MyMatrixRotationX(tempx, now_put_part_rot->float3.x);
	MyMatrixRotationY(tempy, now_put_part_rot->float3.y);
	MyMatrixRotationZ(tempz, now_put_part_rot->float3.z);
	MyMatrixIdentity(temp3);
	MyMatrixMultiply(temp_rot, temp3, tempz);
	MyMatrixMultiply(temp_rot, temp_rot, tempx);
	MyMatrixMultiply(temp_rot, temp_rot, tempy);
	MyMatrixScaling(temp3, now_put_part_scale->float3.x,now_put_part_scale->float3.y, now_put_part_scale->float3.z);
	if (mat_scale) {
		*mat_scale = temp3;
	}
	if (mat_rot) {
		*mat_rot = temp_rot;
	}
	if (mat) {
	MyMatrixIdentity(*mat);
	MyMatrixMultiply(*mat,*mat, temp3);
	MyMatrixMultiply(*mat, *mat, temp_rot);
	MyMatrixMultiply(*mat, *mat, temp);
	}
}




void getTranslatedOBB(OBB* new_obb, OBB* old_obb, MYVECTOR3* pos, MYVECTOR3* rot, MYVECTOR3* scale) {
	MYMATRIX rot_w;
	MYMATRIX wo;
	MYMATRIX scale_w;
	OBB temp_ob = *old_obb;
	hanneiMatrixOfPosRotScaledayo(&wo,&rot_w, &scale_w, pos, rot, scale);
	MyVec3TransformCoord(temp_ob.c, temp_ob.c, wo);
	MyVec3TransformNormal(temp_ob.u[0], temp_ob.u[0], rot_w);
	MyVec3TransformNormal(temp_ob.u[1], temp_ob.u[1], rot_w);
	MyVec3TransformNormal(temp_ob.u[2], temp_ob.u[2], rot_w);
	MyVec3TransformNormal(temp_ob.e, temp_ob.e, scale_w);

	*new_obb = temp_ob;



}





bool majiwaruTriangleRay(intersection* out_inter,MYVECTOR3* pqpr_suityoku, MYVECTOR3* p, MYVECTOR3* binormal_u, MYVECTOR3* binormal_v, RAY* ray) {
	 static const float EPSILON = 1e-10;
    static const float dist = 1000000.0f;
    float t, u, v;
    
	MYVECTOR3 r_org(ray->org-*p);
	MYVECTOR3 test = *pqpr_suityoku;
    float nd =  MyVec3Dot(r_org, test);
	float nv = -MyVec3Dot(ray->dir, test);
        
    if( nv < EPSILON )return false;
        
    t = nd/nv;
    
    if( t < 0 || dist < t )return false;
    
    MYVECTOR3 pos(ray->dir*t + r_org);
	test = *binormal_u;
    u = MyVec3Dot(pos,test);
	test = *binormal_v;
    v = MyVec3Dot(pos,test);
    
    if( (EPSILON > u)||(EPSILON > v)||(u+v > 1-EPSILON) )return false;
    
    
	(*out_inter)[INTERSECTION_NORMAL] = *pqpr_suityoku;   
    (*out_inter)[INTERSECTION_TUV]    = MYVECTOR3(t,u,v);
    
    return true;
    
}









bool majiwaruTriangleSenbun(MYVECTOR3* pqpr_suityoku,  MYVECTOR3* p,MYVECTOR3* q, MYVECTOR3* r, RAY* ray) {

	static const float EPSI = 1e-10;

	MYVECTOR3 s1(ray->org);
	MYVECTOR3 s2(ray->org + ray->dir);
	MYVECTOR3 s3(s1 - *p);
	MYVECTOR3 s4(s2 - *p);
	MYVECTOR3 test = *pqpr_suityoku;
	// 線分が平面と衝突を起こしているかどうか調べる
	if (MyVec3Dot(s3, test) * MyVec3Dot(s4, test) <0) {
		// 交わっている
	} else {
		return false;
	}

	

	float dist1 = abs(MyVec3Dot(s3, test));
	float dist2 = abs(MyVec3Dot(s4, test));

	if (dist1 +dist2 < EPSI) {
		return false;
	}

	MYVECTOR3 kouten = s1 + (s2 - s1) *(dist1/(dist1+dist2));

	// あとは中にあるかどうか調べる

	if (majiwaruTriangleCircle(1, &kouten, p, q, r,true)) {
		return true;
	}




	return false;
}



int hanteiTRIANGLETOHEN( RAY* ray1, TRIANGLEDAYO* tri2, MYVECTOR3* jyusin, MYVECTOR3* housen, MYVECTOR3* ans_ten) {

	// 1. 三角面と交わるかどうかまず判定する
	MYVECTOR3 a = ray1->org;
	MYVECTOR3 b = ray1->org + ray1->dir;

	MYVECTOR3 pp = a - *jyusin;
	MYVECTOR3 ppp = *housen;
	MYVECTOR3 pppp = *jyusin;
	float dot1 = MyVec3Dot(pp, ppp);
	pp = b - *jyusin;
	ppp = *housen;
	float dot2 = MyVec3Dot(pp,ppp);

	if (dot1 * dot2 < 0) {
		// 交わる
	} else {
		return YATTEYARU_TRIANGLE_OUT;
	}

	//　三角面との交点Dを求める
	float bunbo = MyVec3Dot(a,ppp) - MyVec3Dot(b,ppp);
	if (abs(bunbo) < 0.000001) {
		// 交わらないとして処理をする
		return YATTEYARU_TRIANGLE_OUT;
	}

	float xdayo = (MyVec3Dot(pppp, ppp) - MyVec3Dot(b,ppp)) / bunbo;
	float ydayo = 1 - xdayo;
	MYVECTOR3 ddayo = ray1->dir * ydayo + ray1->org;

	MYVECTOR3 ab2 = tri2->y - tri2->x;
	MYVECTOR3 bc2 = tri2->z - tri2->y;
	MYVECTOR3 ca2 = tri2->x - tri2->z;

	MYVECTOR3 ad2 = ddayo- tri2->x;
	MYVECTOR3 bd2 = ddayo- tri2->y;
	MYVECTOR3 cd2 = ddayo- tri2->z;

	MYVECTOR3 ae2;
	MYVECTOR3 af2;
	MYVECTOR3 ag2;

	MyVec3Cross(ae2,ab2,ad2);
	MyVec3Cross(af2,bc2,bd2);
	MyVec3Cross(ag2,ca2,cd2);

	float aeaf = MyVec3Dot(ae2,af2);
	float afag = MyVec3Dot(af2,ag2);
	float aeag = MyVec3Dot(ae2,ag2);

	if ((aeaf >= 0) && (afag >=0) &&( aeag >=0)) {
		*ans_ten = ddayo;
		return YATTEYARU_TRIANGLE_KOUSA;
	} else if((aeaf <=0) && (afag <=0) && (aeag <=0)) {
		*ans_ten = ddayo;
		return YATTEYARU_TRIANGLE_KOUSA;
	}

	return YATTEYARU_TRIANGLE_OUT;
}



int hanteiTRIANGLEDAYO( TRIANGLEDAYO* tri1, TRIANGLEDAYO* tri2, MYVECTOR3* k_ans ) {

	//　1.外接球同士で交わるかどうか判定する
	// 外接球を求める

	SPHERE sph1;
	SPHERE sph2;

	sph1.c = (tri1->x + tri1->y + tri1->z)/3;
	sph2.c = (tri2->x + tri2->y + tri2->z)/3;

	MYVECTOR3 sa = sph1.c - sph2.c;
	MYVECTOR3 sa1 = tri1->x - sph1.c;
	MYVECTOR3 sa2 = tri2->x - sph2.c;

	// ２乗のままで計算する　（計算簡易のため）
	// そのため交わっていなくても交わってると判定される場合がある

	float x2;
	float r12;
	float r22;

	x2 = sa.float3.x * sa.float3.x + sa.float3.y * sa.float3.y + sa.float3.z * sa.float3.z;
	r12 = sa1.float3.x * sa1.float3.x + sa1.float3.y * sa1.float3.y + sa1.float3.z * sa1.float3.z;
	r22 = sa2.float3.x * sa2.float3.x * sa2.float3.y * sa2.float3.y + sa2.float3.z * sa2.float3.z;

	if (x2 > 2 *r12 + 2 *r22) {
		// 遠くにあるので交わらない
		return YATTEYARU_TRIANGLE_OUT;
	}

	// 2. 三角形１の辺に関して 三角形2に対する交点を求める
	RAY ray1;
	RAY ray2;
	RAY ray3;

	ray1.dir = tri1->y - tri1->x;
	ray1.org = tri1->x;

	ray2.dir = tri1->z - tri1->y;
	ray2.org = tri1->y;

	ray3.dir = tri1->x - tri1->z;
	ray3.org = tri1->z;



	MYVECTOR3 jyusin = (tri2->x + tri2->y + tri2->z) /3;
	MYVECTOR3 housen;
	MYVECTOR3 xy = (tri2->y-tri2->x);
	MYVECTOR3 xz = (tri2->z - tri2->x);
	MyVec3Cross(housen, xy, xz);



	MYVECTOR3 ans_ten[3];
	int temp_ans = 0;
	int a = hanteiTRIANGLETOHEN(&ray1,tri2,&jyusin, &housen, &ans_ten[temp_ans]);
	if (a == YATTEYARU_TRIANGLE_KOUSA) {
		temp_ans++;
	}
	a = hanteiTRIANGLETOHEN(&ray2,tri2,&jyusin, &housen,&ans_ten[temp_ans]);
	if (a == YATTEYARU_TRIANGLE_KOUSA) {
		temp_ans++;
	}
	a = hanteiTRIANGLETOHEN(&ray3, tri2, &jyusin, &housen,&ans_ten[temp_ans]);
	if (a == YATTEYARU_TRIANGLE_KOUSA) {
		temp_ans++;
	}

	if (temp_ans == 0) {
		// 交わらない
		return YATTEYARU_TRIANGLE_OUT;
	}

	k_ans->float3.x = 0;
	k_ans->float3.y = 0;
	k_ans->float3.z = 0;

	for (int i=0; i < temp_ans;i++) {
		k_ans->float3.x = k_ans->float3.x + ans_ten[i].float3.x;
		k_ans->float3.y = k_ans->float3.y + ans_ten[i].float3.y;
		k_ans->float3.z = k_ans->float3.z + ans_ten[i].float3.z;
	}
	
	k_ans->float3.x = k_ans->float3.x/temp_ans;
	k_ans->float3.y = k_ans->float3.y/temp_ans;
	k_ans->float3.z = k_ans->float3.z/temp_ans;



	return YATTEYARU_TRIANGLE_KOUSA;

}

int hanteiTRIANGLETOOBB(TRIANGLEDAYO* tri11, OBB* obb) {
	OBB b = *obb;
	TRIANGLEDAYO tri1;// = (TRIANGLEDAYO)0;
	tri1.x = MYVECTOR3(tri11->x);
  tri1.y = MYVECTOR3(tri11->y);
  tri1.z = MYVECTOR3(tri11->z);
  MYVECTOR3 p = tri1.x;
  MYVECTOR3 q = tri1.y;
  MYVECTOR3 r = tri1.z;
 
  MYVECTOR3 housen_tri1 = MYVECTOR3(0,0,0);
  MYVECTOR3 xz = tri1.z - tri1.x;
  MYVECTOR3 xy = tri1.y - tri1.x;
  MyVec3Cross(housen_tri1,xz, xy);
  MyVec3Normalize(housen_tri1,housen_tri1);
  MYVECTOR3 ctoti = b.c - tri1.x;
  float nai = MyVec3Dot(housen_tri1,ctoti);
  float nai_2 = nai*nai;
  float x_r_2 = MyVec3Dot(b.e,b.e);
  MYVECTOR3 jyusin_tri1 = MYVECTOR3(0,0,0);
  float leng=0;
  MYVECTOR3 majiwaripos = MYVECTOR3(0,0,0);

 

  
  jyusin_tri1 = (tri1.x + tri1.y + tri1.z)/3;
  float r_tri1 = 0;
  MYVECTOR3 tritoj = tri1.x - jyusin_tri1;
  r_tri1 = MyVec3Length(tritoj);


  majiwaripos = b.c - (housen_tri1) * nai;
  if (majiwaruPointOBB(&tri1.x, obb)) {
     return 1;
  }

  if (majiwaruPointOBB(&tri1.y, obb)) {
     return 1;
  }

  if (majiwaruPointOBB(&tri1.z, obb)) {
     return 1;
  }




/*
  if(nai_2 > x_r_2) {
    return 0;
  }
  */
  tritoj = jyusin_tri1 - b.c;
  /*
  leng = MyVec3Length(tritoj);
  
  if (leng > r_tri1 + b.e[0]+b.e[1]+b.e[2]) {
   // return 0;
  }
  */
 
  
  MYVECTOR3 point1 = b.c + b.u[0]*     b.e[0]; 
  MYVECTOR3 point2 = b.c + (-b.u[0])*  b.e[0]; 
  MYVECTOR3 point3 = b.c +  b.u[1]*	   b.e[1];
  MYVECTOR3 point4 = b.c +  (-b.u[1])* b.e[1];
  MYVECTOR3 point5 = b.c +  b.u[2]*	   b.e[2];
  MYVECTOR3 point6 = b.c +  (-b.u[2])* b.e[2];


	if (heimenUeni(&point1, &b.u[0], &p) && heimenUeni(&point1, &b.u[0], &q)
		&& heimenUeni(&point1, &b.u[0], &r)) {
			return 0;
	}

	if (heimenUeni(&point2, &(-b.u[0]), &p) && heimenUeni(&point2, &(-b.u[0]), &q)
		&& heimenUeni(&point2, &(-b.u[0]), &r)) {
			return 0;
	}

	if (heimenUeni(&point3, &b.u[1], &p) && heimenUeni(&point3, &b.u[1], &q)
		&& heimenUeni(&point3, &b.u[1], &r)) {
			return 0;
	}

	if (heimenUeni(&point4, &(-b.u[1]), &p) && heimenUeni(&point4, &(-b.u[1]), &q)
		&& heimenUeni(&point4, &(-b.u[1]), &r)) {
			return 0;
	}

	if (heimenUeni(&point5, &(b.u[2]), &p) && heimenUeni(&point5, &(b.u[2]), &q)
		&& heimenUeni(&point5, &(b.u[2]), &r)) {
			return 0;
	}

	if (heimenUeni(&point6, &(-b.u[2]), &p) && heimenUeni(&point6, &(-b.u[2]), &q)
		&& heimenUeni(&point6, &(-b.u[2]), &r)) {
			return 0;
	}

/*
int i = 0;
int k=0;
[unroll(3)]
	for (i=0;i<3;i++) {
[unroll(2)]
		for (k=0;k<2;k++) {
			float test = 1;
			if (k==0) {
			} else {
				test = -1;
			}


			TRIANGLEDAYO tri2;
			TRIANGLEDAYO tri22;
tri2.x = float3(0,0,0);
tri2.y = float3(0,0,0);
tri2.z = float3(0,0,0);
tri22.x = float3(0,0,0);
tri22.y = float3(0,0,0);
tri22.z = float3(0,0,0);
	//		float3 jyusin;
//			float3 housen;
//			float3 ans_ten;

			float3 tyu = b.c + b.u[i] * test * b.e[i];
			if (i==0) {
				tri2.x = tyu + b.u[1] * b.e[1] + b.u[2] * b.e[2];
				tri2.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
				tri2.z = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
				tri22.x = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
				tri22.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
				tri22.z = tyu - b.u[1] * b.e[1] - b.u[2] * b.e[2];
			} else if(i == 1) {
				tri2.x = tyu + b.u[2] * b.e[2] + b.u[0] * b.e[0];
				tri2.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
				tri2.z = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
				tri22.x = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
				tri22.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
				tri22.z = tyu - b.u[2] * b.e[2] - b.u[0] * b.e[0];
			} else if(i==2) {
				tri2.x = tyu + b.u[0] * b.e[0] + b.u[1] * b.e[1];
				tri2.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
				tri2.z = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
				tri22.x = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
				tri22.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
				tri22.z = tyu - b.u[0] * b.e[0] - b.u[1] * b.e[1];
			}

			ANS_MAJIWARITIKEITOSOREIGAI t = hanteiTRIANGLEDAYO(tri1, tri2);
                        t.housen = housen_tri1;
			if (t.is_majiwari) return t;

			ANS_MAJIWARITIKEITOSOREIGAI t2 = hanteiTRIANGLEDAYO(tri1, tri22);
			t2.housen = housen_tri1;
			if (t2.is_majiwari) return t2;

		}
	}
  */



TRIANGLEDAYO tri2;
TRIANGLEDAYO tri22;
tri2.x = MYVECTOR3(0,0,0);
tri2.y = MYVECTOR3(0,0,0);
tri2.z = MYVECTOR3(0,0,0);
tri22.x = MYVECTOR3(0,0,0);
tri22.y = MYVECTOR3(0,0,0);
tri22.z = MYVECTOR3(0,0,0);
	
float test = 1;
int i=0;
MYVECTOR3 tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri2.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri2.z = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
tri22.x = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
tri22.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri22.z = tyu - b.u[1] * b.e[1] - b.u[2] * b.e[2];
int t =  hanteiTRIANGLEDAYO(&tri1, &tri2,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

 t =  hanteiTRIANGLEDAYO(&tri2, &tri1,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

int t2 = hanteiTRIANGLEDAYO(&tri1, &tri22,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;
t2 = hanteiTRIANGLEDAYO(&tri22, &tri1,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;




test = -1;
i=0;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri2.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri2.z = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
tri22.x = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
tri22.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri22.z = tyu - b.u[1] * b.e[1] - b.u[2] * b.e[2];
t =  hanteiTRIANGLEDAYO(&tri1, &tri2,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

 t =  hanteiTRIANGLEDAYO(&tri2, &tri1,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

t2 = hanteiTRIANGLEDAYO(&tri1, &tri22,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;
t2 = hanteiTRIANGLEDAYO(&tri22, &tri1,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;



test = 1;
i=1;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri2.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri2.z = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
tri22.x = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
tri22.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri22.z = tyu - b.u[2] * b.e[2] - b.u[0] * b.e[0];
t =  hanteiTRIANGLEDAYO(&tri1, &tri2,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

 t =  hanteiTRIANGLEDAYO(&tri2, &tri1,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

t2 = hanteiTRIANGLEDAYO(&tri1, &tri22,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;
t2 = hanteiTRIANGLEDAYO(&tri22, &tri1,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;

test = -1;
i=1;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri2.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri2.z = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
tri22.x = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
tri22.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri22.z = tyu - b.u[2] * b.e[2] - b.u[0] * b.e[0];
t =  hanteiTRIANGLEDAYO(&tri1, &tri2,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

 t =  hanteiTRIANGLEDAYO(&tri2, &tri1,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

t2 = hanteiTRIANGLEDAYO(&tri1, &tri22,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;
t2 = hanteiTRIANGLEDAYO(&tri22, &tri1,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;


test = 1;
i=2;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri2.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri2.z = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
tri22.x = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
tri22.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri22.z = tyu - b.u[0] * b.e[0] - b.u[1] * b.e[1];
t =  hanteiTRIANGLEDAYO(&tri1, &tri2,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

 t =  hanteiTRIANGLEDAYO(&tri2, &tri1,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

t2 = hanteiTRIANGLEDAYO(&tri1, &tri22,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;
t2 = hanteiTRIANGLEDAYO(&tri22, &tri1,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;



test = -1;
i=2;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri2.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri2.z = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
tri22.x = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
tri22.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri22.z = tyu - b.u[0] * b.e[0] - b.u[1] * b.e[1];
t =  hanteiTRIANGLEDAYO(&tri1, &tri2,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

 t =  hanteiTRIANGLEDAYO(&tri2, &tri1,&tyu);
if (t == YATTEYARU_TRIANGLE_KOUSA) return 1;

t2 = hanteiTRIANGLEDAYO(&tri1, &tri22,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;
t2 = hanteiTRIANGLEDAYO(&tri22, &tri1,&tyu);
if (t2 == YATTEYARU_TRIANGLE_KOUSA) return 1;

  return 0;
}