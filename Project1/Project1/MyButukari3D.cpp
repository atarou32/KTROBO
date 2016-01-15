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

	// ��_���v�Z����
//	MYVECTOR3 ans_pos = ray->org + w * ray->dir;
//  w�����߂�
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
	float dot2 = MyVec3Dot(point2, test);
	test = -b->u[1];
	float dot3 = MyVec3Dot(point3, test);
	float dot4 = MyVec3Dot(point4, test);
	test = -b->u[2];
	float dot5 = MyVec3Dot(point5, test);
	float dot6 = MyVec3Dot(point6, test);

	if (dot1 >0 && dot2 >0 && dot3>0 && dot4 >0 && dot5 > 0 && dot6 > 0) {
		return true;
	}

	return false;
}


// �ڂ���ꍇ������邤���ɓ���
bool majiwaruTriangleOBB(MYVECTOR3* pqpr_suityoku, MYVECTOR3* p, MYVECTOR3* q, MYVECTOR3* r, OBB* b, MYVECTOR3* out_majiwaripos) {

	// OBB �Ɛڂ���O�ڋ�X
	float x_r_2 = b->e[0]*b->e[0] + b->e[1] * b->e[1] + b->e[2] * b->e[2];
//	MYVECTOR3 pqpr_suityoku;
	MYVECTOR3 pq = *q - *p;
	MYVECTOR3 pr = *r - *p;
	
	// �O�σx�N�g�������߂�
//	MyVec3Cross(&pqpr_suityoku, &pq, &pr);
	// �O�σx�N�g����pb�̒��S�̓��ς��Ƃ�
//	MyVec3Normalize(&pqpr_suityoku, &pqpr_suityoku);
	// p��b�̒��S�̒�����X�̒������ׂ�
	MYVECTOR3 pb = b->c - *p;
	MYVECTOR3 pq_pr = *pqpr_suityoku;
	float nai = MyVec3Dot(pq_pr, pb);
	
	float nai_2 = nai * nai;
	if (nai_2 > x_r_2) {
		// �O�ڋ��ƕ���PQR�������Ȃ��̂Ō����Ȃ�
		return false;
	}

	
	// ����PQR�ƊO�ڋ��̌��~�@�̒��S�����߂�
	MYVECTOR3 c = b->c - (*pqpr_suityoku) * nai;
	*out_majiwaripos = c;
	// OBB �̂Ȃ��ɓ_������ꍇ�͕K�������
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
		// �����Ȃ��̂ŊO�ɂ���H
		return false;
	}
	/*
	if (!majiwaruPointOBB(&c, b)) {
		return false;
	}
	*/

	return true;
}

// �ڂ���ꍇ������邤���ɓ���
// s_r_2 �͔��a���Q�悵������
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

    return 1;
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
        
    //��L=A0, L=A1, L=A2����
    float ra, rb;

    for(int i = 0; i < 3; i++)
    {
        ra = a->e[i];
        rb = b->e[0] * AbsR[i][0] + b->e[1] * AbsR[i][1] + b->e[2] * AbsR[i][2];
        if(fabsf(t[i]) > ra + rb)return 0;
    }
    //��L=B0, L=B1, L=B2����
    for(int i = 0; i < 3; i++)
    {
        ra = a->e[0] * AbsR[0][i] + a->e[1] * AbsR[1][i] + a->e[2] * AbsR[2][i];
        rb = b->e[i];
        if(fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb)return 0;
    }

    //��L=A0 X B0����
    ra = a->e[1] * AbsR[2][0] + a->e[2] * AbsR[1][0];
    rb = b->e[1] * AbsR[0][2] + b->e[2] * AbsR[0][1];
    if(fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)return 0;

    //��L=A0 X B1����
    ra = a->e[1] * AbsR[2][1] + a->e[2] * AbsR[1][1];
    rb = b->e[0] * AbsR[0][2] + b->e[2] * AbsR[0][0];
    if(fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)return 0;

    //��L=A0 X B2����
    ra = a->e[1] * AbsR[2][2] + a->e[2] * AbsR[1][2];
    rb = b->e[0] * AbsR[0][1] + b->e[1] * AbsR[0][0];
    if(fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)return 0;

    //��L=A1 X B0����
    ra = a->e[0] * AbsR[2][0] + a->e[2] * AbsR[0][0];
    rb = b->e[1] * AbsR[1][2] + b->e[2] * AbsR[1][1];
    if(fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)return 0;

    //��L=A1 X B1����
    ra = a->e[0] * AbsR[2][1] + a->e[2] * AbsR[0][1];
    rb = b->e[0] * AbsR[1][2] + b->e[2] * AbsR[1][0];
    if(fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)return 0;

    //��L=A1 X B2����
    ra = a->e[0] * AbsR[2][2] + a->e[2] * AbsR[0][2];
    rb = b->e[0] * AbsR[1][1] + b->e[1] * AbsR[1][0];
    if(fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)return 0;

    //��L=A2 X B0����
    ra = a->e[0] * AbsR[1][0] + a->e[1] * AbsR[0][0];
    rb = b->e[1] * AbsR[2][2] + b->e[2] * AbsR[2][1];
    if(fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)return 0;

    //��L=A2 X B1����
    ra = a->e[0] * AbsR[1][1] + a->e[1] * AbsR[0][1];
    rb = b->e[0] * AbsR[2][2] + b->e[2] * AbsR[2][0];
    if(fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)return 0;

    //��L=A2 X B2����
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


void drawOBBFill(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX *view, MYMATRIX* proj, OBB* rec) {
	MYVECTOR3 point1;
	MYVECTOR3 point2;
	MYVECTOR3 point3;
	MYVECTOR3 point4;
	MYVECTOR3 point5;
	MYVECTOR3 point6;
	MYVECTOR3 point7;
	MYVECTOR3 point8;



	point1 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point2 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point3 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point4 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point5 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point6 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point7 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point8 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];




	DWORD FontColor = color;


//	  SIMPLEVERTEX v[]=
 //  {
  //    { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor},
   //   { point2.x, point2.y, point2.z, /*1.0f,*/ FontColor}, 
	//  { point3.x, point3.y, point3.z, FontColor},

 //     { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor}, 
//	  { point3.x, point3.y, point3.z, FontColor},
 //     { point4.x, point4.y, point4.z, /*1.0f,*/ FontColor},
	  
//	  { point1.x, point1.y, point1.z, FontColor},
//	  { point4.x, point4.y, point4.z, FontColor},
//	  { point5.x, point5.y, point5.z, FontColor},

//	  { point4.x, point4.y, point4.z, FontColor},
//	  { point8.x, point8.y, point8.z, /*1.0f,*/ FontColor}, 
 //     { point5.x, point5.y, point5.z, /*1.0f,*/ FontColor}, 

//	  { point2.x, point2.y, point2.z, /*1.0f,*/ FontColor},
 //     { point6.x, point6.y, point6.z, FontColor},
//	  { point3.x, point3.y, point3.z, FontColor},
      
//	  { point3.x, point3.y, point3.z, FontColor},
//	  { point6.x, point6.y, point6.z, FontColor},
//	  { point7.x, point7.y, point7.z, FontColor},
//	  
//	  { point4.x, point4.y, point4.z, FontColor},
//	  { point3.x, point3.y, point3.z, FontColor},
//	  { point7.x, point7.y, point7.z, FontColor},

//	  { point4.x, point4.y, point4.z, FontColor},
//	  { point7.x, point7.y, point7.z, FontColor},
//	  { point8.x, point8.y, point8.z, FontColor},

//	  { point5.x, point5.y, point5.z, FontColor},
//	  { point6.x, point6.y, point6.z, FontColor},
//	  { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor},

//	  { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor},
 //     { point6.x, point6.y, point6.z, FontColor},
//	  { point2.x, point2.y, point2.z, /*1.0f,*/ FontColor}, 
	   
//	  { point6.x, point6.y, point6.z, FontColor},
///	  { point5.x, point5.y, point5.z, /*1.0f,*/ FontColor}, 
//	  { point8.x, point8.y, point8.z, /*1.0f,*/ FontColor},
	  
	  
	  
	  
//	  { point7.x, point7.y, point7.z, FontColor},
//	  { point6.x, point6.y, point6.z, FontColor},
//	  { point8.x, point8.y, point8.z, FontColor},

	
//   };

	return;
}

void drawRAY(KTROBO::Graphics* g, DWORD FontColor, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, RAY* ray, int length) {
		 
//	SIMPLEVERTEX v[]=
 //  {
//	   { ray->org.x, ray->org.y, ray->org.z, /*1.0f,*/ FontColor},
//	   { ray->org.x + length * ray->dir.x, ray->org.y + length * ray->dir.y, ray->org.z + length*ray->dir.z, /*1.0f,*/ FontColor}, 
 //    
  // };


	return;
}

void drawTriangle(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, MYVECTOR3* p, MYVECTOR3* q, MYVECTOR3* r) {
	DWORD FontColor = color;
//	  SIMPLEVERTEX v[]=
 //  {
  //    { p->x, p->y, p->z, /*1.0f,*/ FontColor},
   //   { q->x, q->y, q->z, /*1.0f,*/ FontColor}, 
    //  { r->x, r->y, r->z, /*1.0f,*/ FontColor}, 
	 // };

	return;
}


void drawOBB(KTROBO::Graphics* g, DWORD color, MYMATRIX* world, MYMATRIX* view, MYMATRIX* proj, OBB* rec) {
	MYVECTOR3 point1;
	MYVECTOR3 point2;
	MYVECTOR3 point3;
	MYVECTOR3 point4;
	MYVECTOR3 point5;
	MYVECTOR3 point6;
	MYVECTOR3 point7;
	MYVECTOR3 point8;



	point1 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point2 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point3 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point4 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] + rec->u[2] * rec->e[2];
	point5 = rec->c + rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point6 = rec->c + rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point7 = rec->c - rec->u[0] * rec->e[0] - rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];
	point8 = rec->c - rec->u[0] * rec->e[0] + rec->u[1] * rec->e[1] - rec->u[2] * rec->e[2];




	DWORD FontColor = color;

	
//	  SIMPLEVERTEX v[]=
 //  {
  //    { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor},
   //   { point2.x, point2.y, point2.z, /*1.0f,*/ FontColor}, 
  //    { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor}, 
  //    { point4.x, point4.y, point4.z, /*1.0f,*/ FontColor},
//	  { point3.x, point3.y, point3.z, FontColor},
//	  { point2.x, point2.y, point2.z, FontColor},
//	  { point3.x, point3.y, point3.z, FontColor},
//	  { point4.x, point4.y, point4.z, FontColor},
//	  
//	  { point5.x, point5.y, point5.z, /*1.0f,*/ FontColor},
 //     { point6.x, point6.y, point6.z, /*1.0f,*/ FontColor}, 
  //    { point5.x, point5.y, point5.z, /*1.0f,*/ FontColor}, 
   //   { point8.x, point8.y, point8.z, /*1.0f,*/ FontColor},
	//  { point7.x, point7.y, point7.z, FontColor},
	 // { point6.x, point6.y, point6.z, FontColor},
//	  { point7.x, point7.y, point7.z, FontColor},
//	  { point8.x, point8.y, point8.z, FontColor},
//
//	  { point1.x, point1.y, point1.z, /*1.0f,*/ FontColor},
 //     { point5.x, point5.y, point5.z, /*1.0f,*/ FontColor}, 
  //    { point2.x, point2.y, point2.z, /*1.0f,*/ FontColor}, 
   //   { point6.x, point6.y, point6.z, /*1.0f,*/ FontColor},
	//  { point3.x, point3.y, point3.z, FontColor},
	 // { point7.x, point7.y, point7.z, FontColor},
//	  { point8.x, point8.y, point8.z, FontColor},
//	  { point4.x, point4.y, point4.z, FontColor}

	//   };



	return;
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
	// ���������ʂƏՓ˂��N�����Ă��邩�ǂ������ׂ�
	if (MyVec3Dot(s3, test) * MyVec3Dot(s4, test) <0) {
		// ������Ă���
	} else {
		return false;
	}

	

	float dist1 = abs(MyVec3Dot(s3, test));
	float dist2 = abs(MyVec3Dot(s4, test));

	if (dist1 +dist2 < EPSI) {
		return false;
	}

	MYVECTOR3 kouten = s1 + (s2 - s1) *(dist1/(dist1+dist2));

	// ���Ƃ͒��ɂ��邩�ǂ������ׂ�

	if (majiwaruTriangleCircle(1, &kouten, p, q, r,true)) {
		return true;
	}




	return false;
}




