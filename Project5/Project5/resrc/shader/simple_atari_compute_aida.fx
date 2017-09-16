sampler sdayo : register(s0);

sampler decalSmp {
  	Filter = MIN_MAG_MAP_POINT;//MIN_MAG_MIP_LINEAR;
  	AddressU = WRAP;
  	AddressV = WRAP;
  	AddressW = WRAP;
  	ComparisonFunc = NEVER;
  	MinLOD = 0;
  	MaxLOD = FLOAT32_MAX;
};


struct AtariUnitInfo {
  int atari_idx;
  int vertexs_place;
  int indexs_place;
  int vertex_count;
  int index_count;
  column_major float4x4 world;
  float3 jyusin;
  float3 v;
  float r;
  float dt;
};

struct AtariUnitKumi {
  int atari_idx;
  int atari_idx2;
};

struct AtariUnitAns {
  int atari_idx;
  int obb_idx;
  int atari_idx2;
  int obb_idx2;
  float3 kouten_jyusin;
  float3 kouten_housen;
  int is_use;
};

struct AtariUnitOBB {
  float3 c;
  float3 u[3];
  float3 e;
};

struct AtariUnitTikeiToSoreigai {
  int atari_idx;
  int obb_idx;
  int atari_idx2;
  int offset;
};

struct AtariUnitTikeiIgaiDousi {
  int atari_idx;
  int obb_idx;
  int atari_idx2;
  int obb_idx2;
};

cbuffer c0 {
  int vertexs_count;
  int indexs_count;
  int auinfo_count;
  int obbs_count;
  int kumi_count;
  int ans_count;
  int igaidousi_count;
  int soreigai_count;
};

struct MESH_VERTEXKARI {
  float4 pos;
  float4 normal;
};

struct MESH_INDEX {
  uint index;
};


StructuredBuffer<MESH_VERTEXKARI> vertexBuf : register(t0);
StructuredBuffer<MESH_INDEX> indexBuf : register(t1);
StructuredBuffer<AtariUnitOBB> obbBuf : register(t2);
StructuredBuffer<AtariUnitInfo> auinfoBuf: register(t3);
StructuredBuffer<AtariUnitKumi> kumiBuf : register(t4);
StructuredBuffer<AtariUnitTikeiIgaiDousi> autidBuf : register(t6);
StructuredBuffer<AtariUnitTikeiToSoreigai> auttsBuf : register(t5);


RWStructuredBuffer<AtariUnitAns> ansBuf :register(u0);
RWStructuredBuffer<AtariUnitAns> ansBuf2 : register(u1);



uint getIndex(uint indexdayo) {
  return indexBuf[indexdayo].index;
};
/*
  return asuint(indexBuf.Load(index*4));
}
*/
bool majiwariKumi(AtariUnitInfo k1, AtariUnitInfo k2) {
  // 重心とr は計算済み

  if(k1.atari_idx == k2.atari_idx) return false;

  float3 jminus = k1.jyusin - k2.jyusin;
  float len = length(jminus);
  if (len > k1.r + k2.r) {
    return false;
  }

  return true;
}

bool majiwariAtariUnitOBB(AtariUnitOBB a, AtariUnitOBB b) {

  const float EPSILON = 1.175494e-37;

    float R[3][3], AbsR[3][3];
    [unroll(3)]
    for(int i = 0; i < 3; i++)
    {
     	[unroll(3)]
        for(int j = 0; j < 3; j++)
        {
            R[i][j] = dot(a.u[i], b.u[j]);
            AbsR[i][j] = abs(R[i][j]) + EPSILON;
        }
    }
        
    float3 t = b.c - a.c;
    t = float3(dot(t, a.u[0]), dot(t, a.u[1]), dot(t, a.u[2]));
        
    //軸L=A0, L=A1, L=A2判定
    float ra, rb;
    [unroll(3)]
    for(int p = 0; p < 3; p++)
    {
        ra = a.e[p];
        rb = b.e[0] * AbsR[p][0] + b.e[1] * AbsR[p][1] + b.e[2] * AbsR[p][2];
        if(abs(t[p]) > ra + rb)return false;
  /*  }
    //軸L=B0, L=B1, L=B2判定
    [unroll(3)]
    for(int t2 = 0; t2 < 3; t2++)
    {*/
        ra = a.e[0] * AbsR[0][p] + a.e[1] * AbsR[1][p] + a.e[2] * AbsR[2][p];
        rb = b.e[p];
        if(abs(t[0] * R[0][p] + t[1] * R[1][p] + t[2] * R[2][p]) > ra + rb)return false;
    }

    //軸L=A0 X B0判定
    ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
    rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];
    if(abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)return false;

    //軸L=A0 X B1判定
    ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
    rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];
    if(abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)return false;

    //軸L=A0 X B2判定
    ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
    rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];
    if(abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)return false;

    //軸L=A1 X B0判定
    ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
    rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];
    if(abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)return false;

    //軸L=A1 X B1判定
    ra = a.e[0] * AbsR[2][1] + a.e[2] * AbsR[0][1];
    rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0];
    if(abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)return false;

    //軸L=A1 X B2判定
    ra = a.e[0] * AbsR[2][2] + a.e[2] * AbsR[0][2];
    rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0];
    if(abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)return false;

    //軸L=A2 X B0判定
    ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
    rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
    if(abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)return false;

    //軸L=A2 X B1判定
    ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
    rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];
    if(abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)return false;

    //軸L=A2 X B2判定
    ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
    rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];
    if(abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)return false;

  return true;
}

struct ANS_MAJIWARITIKEITOSOREIGAI {
  float3 pos;
  float3 housen;
  bool is_majiwari;
};



struct TRIANGLEDAYO {
float3 x;
float3 y;
float3 z;
};
struct RAY {
float3 org;
float3 dir;
};



ANS_MAJIWARITIKEITOSOREIGAI hanteiTRIANGLETOHEN(RAY ray1, TRIANGLEDAYO tri2, float3 jyusin, float3 housen) {

  ANS_MAJIWARITIKEITOSOREIGAI ans_ten;
  ans_ten.is_majiwari = false;
  ans_ten.pos = float3(0,0,0);
  ans_ten.housen = float3(0,0,0);

	// 1. 三角面と交わるかどうかまず判定する
	float3 a = ray1.org;
	float3 b = ray1.org + ray1.dir;

	float3 pp = a - jyusin;
	float3 ppp = housen;
	float3 pppp = jyusin;
	float dot1 = dot(pp, ppp);
	pp = b - jyusin;
	ppp = housen;
	float dot2 = dot(pp,ppp);

	if (dot1 * dot2 <= 0) {
		// 交わる
	} else {
                ans_ten.is_majiwari = false;
		return ans_ten;
	}

	//　三角面との交点Dを求める
	float bunbo = dot(a,ppp) - dot(b,ppp);
	if (abs(bunbo) < 0.000001) {
		// 交わらないとして処理をする
                ans_ten.is_majiwari = false;
		return ans_ten;
	}

	float xdayo = (dot(pppp, ppp) - dot(b,ppp)) / bunbo;
	float ydayo = 1 - xdayo;
	float3 ddayo = ray1.dir * ydayo + ray1.org;

	float3 ab2 = tri2.y - tri2.x;
	float3 bc2 = tri2.z - tri2.y;
	float3 ca2 = tri2.x - tri2.z;

	float3 ad2 = ddayo- tri2.x;
	float3 bd2 = ddayo- tri2.y;
	float3 cd2 = ddayo- tri2.z;

	float3 ae2;
	float3 af2;
	float3 ag2;

	ae2 = cross(ab2,ad2);
	af2 = cross(bc2,bd2);
	ag2 = cross(ca2,cd2);

	float aeaf = dot(ae2,af2);
	float afag = dot(af2,ag2);
	float aeag = dot(ae2,ag2);

	if ((aeaf >= 0) && (afag >=0) &&( aeag >=0)) {
		ans_ten.pos = ddayo;
                ans_ten.is_majiwari = true;
		return ans_ten;
	} else if((aeaf <=0) && (afag <=0) && (aeag <=0)) {
		ans_ten.pos = ddayo;
                ans_ten.is_majiwari = true;
		return ans_ten;
	}

        ans_ten.is_majiwari = false;
	return ans_ten;
}

struct SPHERE {
float3 c;
float r;
};

ANS_MAJIWARITIKEITOSOREIGAI hanteiTRIANGLEDAYO( TRIANGLEDAYO tri1, TRIANGLEDAYO tri2) {

 ANS_MAJIWARITIKEITOSOREIGAI ans_ten;
 ans_ten.is_majiwari = false;
  ans_ten.pos = float3(0,0,0);
  ans_ten.housen = float3(0,0,0);

	//　1.外接球同士で交わるかどうか判定する
	// 外接球を求める

	SPHERE sph1;
	SPHERE sph2;

	sph1.c = (tri1.x + tri1.y + tri1.z)/3;
	sph2.c = (tri2.x + tri2.y + tri2.z)/3;

	float3 sa = sph1.c - sph2.c;
	float3 sa1 = tri1.x - sph1.c;
	float3 sa2 = tri2.x - sph2.c;

	// ２乗のままで計算する　（計算簡易のため）
	// そのため交わっていなくても交わってると判定される場合がある

	float x2;
	float r12;
	float r22;

	x2 = sa.x * sa.x + sa.y * sa.y + sa.z * sa.z;
	r12 = sa1.x * sa1.x + sa1.y * sa1.y + sa1.z * sa1.z;
	r22 = sa2.x * sa2.x * sa2.y * sa2.y + sa2.z * sa2.z;

	if (x2 > 2 *r12 + 2 *r22) {
		// 遠くにあるので交わらない
                ans_ten.is_majiwari = false;
                return ans_ten;
		//return YATTEYARU_TRIANGLE_OUT;
	}

	// 2. 三角形１の辺に関して 三角形2に対する交点を求める
	RAY ray1;
	RAY ray2;
	RAY ray3;

	ray1.dir = tri1.y - tri1.x;
	ray1.org = tri1.x;

	ray2.dir = tri1.z - tri1.y;
	ray2.org = tri1.y;

	ray3.dir = tri1.x - tri1.z;
	ray3.org = tri1.z;



	float3 jyusin = (tri2.x + tri2.y + tri2.z) /3;
	float3 housen;
	float3 xy = (tri2.y-tri2.x);
	float3 xz = (tri2.z - tri2.x);
	housen = cross(xy, xz);

	int temp_ans = 0;
	ANS_MAJIWARITIKEITOSOREIGAI a = hanteiTRIANGLETOHEN(ray1,tri2,jyusin, housen);
	if (a.is_majiwari) {
		temp_ans++;
	}
	ANS_MAJIWARITIKEITOSOREIGAI a2 = hanteiTRIANGLETOHEN(ray2,tri2,jyusin, housen);
	if (a2.is_majiwari) {
		temp_ans++;
	}
	ANS_MAJIWARITIKEITOSOREIGAI a3 = hanteiTRIANGLETOHEN(ray3, tri2, jyusin, housen);
	if (a3.is_majiwari) {
		temp_ans++;
	}

	if (temp_ans == 0) {
		// 交わらない
                ans_ten.is_majiwari = false;
		return ans_ten;
		//return YATTEYARU_TRIANGLE_OUT;
	}

	if (a.is_majiwari) {
          ans_ten.pos = ans_ten.pos + a.pos;
        }
        if (a2.is_majiwari) {
          ans_ten.pos = ans_ten.pos + a2.pos;
        }
        if (a3.is_majiwari) {
          ans_ten.pos = ans_ten.pos + a3.pos;
        } 
	ans_ten.pos = ans_ten.pos / temp_ans;	
        ans_ten.housen = housen;
        ans_ten.is_majiwari = true;

	return ans_ten;

}

bool majiwariPointOBB(float3 p, AtariUnitOBB b) {

	float3 point1 = b.c +b.u[0] *  b.e[0];
	float3 point2 = b.c +(-b.u[0]) *   b.e[0]; 
	float3 point3 = b.c +b.u[1] *	   b.e[1]; 
	float3 point4 = b.c +(-b.u[1]) *   b.e[1]; 
	float3 point5 = b.c +b.u[2] *	   b.e[2]; 
	float3 point6 = b.c +(-b.u[2]) *   b.e[2]; 

	point1 = p - point1;
	point2 = p - point2;
	point3 = p - point3;
	point4 = p - point4;
	point5 = p - point5;
	point6 = p - point6;
	float3 test = (-b.u[0]);
	float dot1 = dot(point1, test);
	test = b.u[0];
	float dot2 = dot(point2, test);
	test = -b.u[1];
	float dot3 = dot(point3, test);
	test = b.u[1];
	float dot4 = dot(point4, test);
	test = -b.u[2];
	float dot5 = dot(point5, test);
	test = b.u[2];
	float dot6 = dot(point6, test);

	if ((dot1 >=0) && (dot2 >=0) && (dot3>=0) && (dot4 >=0) && (dot5 >= 0) && (dot6 >= 0)) {
		return true;
	}
	if ((dot1 <=0) && (dot2 <=0) && (dot3 <= 0) && (dot4 <=0) && (dot5 <= 0) && (dot6 <= 0)) {
		return true;
	}

	return false;
}

bool heimenUeni(float3 p, float3 u, float3 x) {

	float3 px = x - p;

	float test = dot((u),px);
	if (test > 0) {
		return true;
	}

	return false;
}

ANS_MAJIWARITIKEITOSOREIGAI majiwariAtariTriangleToOBB(MESH_VERTEXKARI v1, MESH_VERTEXKARI v2, MESH_VERTEXKARI v3, AtariUnitOBB b) {

  ANS_MAJIWARITIKEITOSOREIGAI ans;
  ans.is_majiwari = false;
  ans.pos = float3(0,0,0);
  ans.housen = float3(0,0,1);
  TRIANGLEDAYO tri1;// = (TRIANGLEDAYO)0;
  tri1.x = float3(v1.pos.x,v1.pos.y,v1.pos.z);
  tri1.y = float3(v2.pos.x,v2.pos.y,v2.pos.z);
  tri1.z = float3(v3.pos.x,v3.pos.y,v3.pos.z);
  float3 p = tri1.x;
  float3 q = tri1.y;
  float3 r = tri1.z;
 
  float3 housen_tri1 =float3(0,0,0);
  housen_tri1 = cross(tri1.z - tri1.x, tri1.y - tri1.x);
  housen_tri1 = normalize(housen_tri1);
  ans.housen = housen_tri1; ///////////////////////////////////////////////////////////////////////////////
  float nai = dot(housen_tri1,b.c - tri1.x);
  float nai_2 = nai*nai;
  float x_r_2 = dot(b.e,b.e);
  float3 jyusin_tri1 = float3(0,0,0);
  float leng=0;
  float3 majiwaripos = float3(0,0,0);
jyusin_tri1 = (tri1.x + tri1.y + tri1.z)/3;
 
  majiwaripos = b.c - (housen_tri1) * nai;
  if (majiwariPointOBB(tri1.x, b)) {
     ans.is_majiwari = true;
     ans.pos = jyusin_tri1;//majiwaripos;
     ans.housen = housen_tri1;
     return ans;
  }

  if (majiwariPointOBB(tri1.y, b)) {
     ans.is_majiwari = true;
     ans.pos = jyusin_tri1;//majiwaripos;
 ans.housen = housen_tri1;
     return ans;
  }

  if (majiwariPointOBB(tri1.z, b)) {
     ans.is_majiwari = true;
     ans.pos = jyusin_tri1;//majiwaripos;
 ans.housen = housen_tri1;
     return ans;
  }
  
  /*
  float r_tri1 = 0;
  r_tri1 = length(tri1.x - jyusin_tri1);
  if(nai_2 > x_r_2) {
    return ans;
  }
  
  leng = length(jyusin_tri1 - b.c);
  if (leng > r_tri1 + b.e[0]+b.e[1]+b.e[2]) {
    return ans;
  }
  */

  
  float3 point1 = b.c + b.u[0]*     b.e[0]; 
  float3 point2 = b.c + (-b.u[0])*  b.e[0]; 
  float3 point3 = b.c +  b.u[1]*	   b.e[1];
  float3 point4 = b.c +  (-b.u[1])* b.e[1];
  float3 point5 = b.c +  b.u[2]*	   b.e[2];
  float3 point6 = b.c +  (-b.u[2])* b.e[2];


	if (heimenUeni(point1, b.u[0], p) && heimenUeni(point1, b.u[0], q)
		&& heimenUeni(point1, b.u[0], r)) {
			return ans;
	}

	if (heimenUeni(point2, (-b.u[0]), p) && heimenUeni(point2, (-b.u[0]), q)
		&& heimenUeni(point2, (-b.u[0]), r)) {
			return ans;
	}

	if (heimenUeni(point3, b.u[1], p) && heimenUeni(point3, b.u[1], q)
		&& heimenUeni(point3, b.u[1], r)) {
			return ans;
	}

	if (heimenUeni(point4, (-b.u[1]), p) && heimenUeni(point4, (-b.u[1]), q)
		&& heimenUeni(point4, (-b.u[1]), r)) {
			return ans;
	}

	if (heimenUeni(point5, (b.u[2]), p) && heimenUeni(point5, (b.u[2]), q)
		&& heimenUeni(point5, (b.u[2]), r)) {
			return ans;
	}

	if (heimenUeni(point6, (-b.u[2]), p) && heimenUeni(point6, (-b.u[2]), q)
		&& heimenUeni(point6, (-b.u[2]), r)) {
			return ans;
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
tri2.x = float3(0,0,0);
tri2.y = float3(0,0,0);
tri2.z = float3(0,0,0);
tri22.x = float3(0,0,0);
tri22.y = float3(0,0,0);
tri22.z = float3(0,0,0);
	
float test = 1;
int i=0;
float3 tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri2.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri2.z = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
tri22.x = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
tri22.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri22.z = tyu - b.u[1] * b.e[1] - b.u[2] * b.e[2];
ANS_MAJIWARITIKEITOSOREIGAI t = hanteiTRIANGLEDAYO(tri1, tri2);
t.housen = housen_tri1;
if (t.is_majiwari) return t;
t = hanteiTRIANGLEDAYO(tri2, tri1);
t.housen = housen_tri1;
if (t.is_majiwari) return t;


ANS_MAJIWARITIKEITOSOREIGAI t2 = hanteiTRIANGLEDAYO(tri1, tri22);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;
t2 = hanteiTRIANGLEDAYO(tri22, tri1);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;

test = -1;
i=0;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri2.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri2.z = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
tri22.x = tyu + b.u[1] * b.e[1] - b.u[2] * b.e[2];
tri22.y = tyu - b.u[1] * b.e[1] + b.u[2] * b.e[2];
tri22.z = tyu - b.u[1] * b.e[1] - b.u[2] * b.e[2];
t = hanteiTRIANGLEDAYO(tri1, tri2);
t.housen = housen_tri1;
if (t.is_majiwari) return t;
t = hanteiTRIANGLEDAYO(tri2, tri1);
t.housen = housen_tri1;
if (t.is_majiwari) return t;


t2 = hanteiTRIANGLEDAYO(tri1, tri22);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;
t2 = hanteiTRIANGLEDAYO(tri22, tri1);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;



test = 1;
i=1;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri2.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri2.z = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
tri22.x = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
tri22.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri22.z = tyu - b.u[2] * b.e[2] - b.u[0] * b.e[0];
t = hanteiTRIANGLEDAYO(tri1, tri2);
t.housen = housen_tri1;
if (t.is_majiwari) return t;
t = hanteiTRIANGLEDAYO(tri2, tri1);
t.housen = housen_tri1;
if (t.is_majiwari) return t;


t2 = hanteiTRIANGLEDAYO(tri1, tri22);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;
t2 = hanteiTRIANGLEDAYO(tri22, tri1);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;


test = -1;
i=1;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri2.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri2.z = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
tri22.x = tyu + b.u[2] * b.e[2] - b.u[0] * b.e[0];
tri22.y = tyu - b.u[2] * b.e[2] + b.u[0] * b.e[0];
tri22.z = tyu - b.u[2] * b.e[2] - b.u[0] * b.e[0];
t = hanteiTRIANGLEDAYO(tri1, tri2);
t.housen = housen_tri1;
if (t.is_majiwari) return t;
t = hanteiTRIANGLEDAYO(tri2, tri1);
t.housen = housen_tri1;
if (t.is_majiwari) return t;


t2 = hanteiTRIANGLEDAYO(tri1, tri22);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;
t2 = hanteiTRIANGLEDAYO(tri22, tri1);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;


test = 1;
i=2;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri2.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri2.z = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
tri22.x = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
tri22.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri22.z = tyu - b.u[0] * b.e[0] - b.u[1] * b.e[1];
t = hanteiTRIANGLEDAYO(tri1, tri2);
t.housen = housen_tri1;
if (t.is_majiwari) return t;
t = hanteiTRIANGLEDAYO(tri2, tri1);
t.housen = housen_tri1;
if (t.is_majiwari) return t;


t2 = hanteiTRIANGLEDAYO(tri1, tri22);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;
t2 = hanteiTRIANGLEDAYO(tri22, tri1);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;



test = -1;
i=2;
tyu = b.c + b.u[i] * test * b.e[i];
tri2.x = tyu + b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri2.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri2.z = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
tri22.x = tyu + b.u[0] * b.e[0] - b.u[1] * b.e[1];
tri22.y = tyu - b.u[0] * b.e[0] + b.u[1] * b.e[1];
tri22.z = tyu - b.u[0] * b.e[0] - b.u[1] * b.e[1];
t = hanteiTRIANGLEDAYO(tri1, tri2);
t.housen = housen_tri1;
if (t.is_majiwari) return t;
t = hanteiTRIANGLEDAYO(tri2, tri1);
t.housen = housen_tri1;
if (t.is_majiwari) return t;


t2 = hanteiTRIANGLEDAYO(tri1, tri22);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;
t2 = hanteiTRIANGLEDAYO(tri22, tri1);
t2.housen = housen_tri1;
if (t2.is_majiwari) return t2;


  ans.is_majiwari = false;


  return ans;
}


#define KTROBO_MAX_TIKEI_HITOTU_INDEX 2048

ANS_MAJIWARITIKEITOSOREIGAI majiwariAtariUnitTikeiToSoreigai(MESH_VERTEXKARI vertex1,
 MESH_VERTEXKARI vertex2, MESH_VERTEXKARI vertex3, AtariUnitOBB obb1, AtariUnitInfo au2) {

  ANS_MAJIWARITIKEITOSOREIGAI ans;
  
  MESH_VERTEXKARI henkan_v1;
  MESH_VERTEXKARI henkan_v2;
  MESH_VERTEXKARI henkan_v3;
  
  henkan_v1.pos = mul(au2.world, vertex1.pos);
  henkan_v1.normal = mul(au2.world, vertex1.normal);

  henkan_v2.pos = mul(au2.world, vertex2.pos);
  henkan_v2.normal = mul(au2.world, vertex2.normal);  

  henkan_v3.pos = mul(au2.world, vertex3.pos);
  henkan_v3.normal = mul(au2.world, vertex3.normal);
 // obb1.c = float3(0,0,0);
  //obb1.u[0] = float3(1,0,0);
  //obb1.u[1] = float3(0,1,0);
  //obb1.u[2] = float3(0,0,1);
  //obb1.e = float3(10,10,10);
  
  ans = majiwariAtariTriangleToOBB(henkan_v1,henkan_v2,henkan_v3, obb1);
  if(ans.is_majiwari) {
    ans.is_majiwari = true;
    float4 norm = (henkan_v1.normal + henkan_v2.normal + henkan_v3.normal)/3;
    float3 norm3 = float3(norm.x,norm.y,norm.z);
    if (dot(norm3, ans.housen) < 0) {
      ans.housen = - ans.housen;
    }
  }

  return ans;
}



// group にはmaxインスタンスの数を指定する
// numthreads にはmaxbonenum の数を指定する
[numthreads(16,8,1)]
void CalcCS(uint3 gtid: SV_GroupID, uint gid: SV_GroupIndex )
{

uint struct_idx = (gtid.x *32*32+ gtid.y * 32 + gtid.z)* 128+ gid;
uint k_count = kumi_count;
uint igai_dousi_k_count = igaidousi_count;
int soreigai_idx = gtid.x*32*32+gtid.y*32+gtid.z;//struct_idx / 128;//KTROBO_MAX_TIKEI_HITOTU_INDEX;
int soreigai_index_idx = gid;//(struct_idx - soreigai_idx*128);//KTROBO_MAX_TIKEI_HITOTU_INDEX;



if (igai_dousi_k_count > struct_idx) {
	AtariUnitTikeiIgaiDousi autid = autidBuf[struct_idx];
  bool tt = majiwariKumi(auinfoBuf[autid.atari_idx], auinfoBuf[autid.atari_idx2]);
  if (tt) {
  
  
  	bool t = majiwariAtariUnitOBB(obbBuf[autid.obb_idx], obbBuf[autid.obb_idx2]);
  	if (t) {
  	  uint idxd;
  	  idxd = ansBuf2.IncrementCounter();
  	  if (idxd >= ans_count) return;
  	  ansBuf2[idxd].is_use = 1;
  	  ansBuf2[idxd].atari_idx = autid.atari_idx;
  	  ansBuf2[idxd].atari_idx2 = autid.atari_idx2;
  	  ansBuf2[idxd].obb_idx = autid.obb_idx;
  	  ansBuf2[idxd].obb_idx2 = autid.obb_idx2;
  	  ansBuf2[idxd].kouten_jyusin = (obbBuf[autid.obb_idx].c + obbBuf[autid.obb_idx2].c)/2;
  	  ansBuf2[idxd].kouten_housen = normalize(obbBuf[autid.obb_idx].c - obbBuf[autid.obb_idx2].c);
  	} 
  }
}

if (soreigai_count > soreigai_idx) {

  AtariUnitTikeiToSoreigai autts = auttsBuf[soreigai_idx];
  if (autts.atari_idx == autts.atari_idx2) return;
  
  
  
  
  AtariUnitInfo au1 = auinfoBuf[autts.atari_idx];
  AtariUnitInfo au2 = auinfoBuf[autts.atari_idx2];
  bool t = majiwariKumi(auinfoBuf[autts.atari_idx],auinfoBuf[autts.atari_idx2]);
 
  if (t) {



  AtariUnitOBB obb1 = obbBuf[autts.obb_idx];
  int vertex_place = au2.vertexs_place;
  int au2_indexs_place = au2.indexs_place + 3 * soreigai_index_idx;
  int au2_indexs_count = au2.index_count;
    int i = soreigai_index_idx;
    if (3*i+2 > au2_indexs_count) return;

    uint inde1 = getIndex(au2_indexs_place);
    uint inde2 = getIndex(au2_indexs_place +1);
    uint inde3 = getIndex(au2_indexs_place +2);
    if (inde1 >= vertexs_count) return;
    if (inde2 >= vertexs_count) return;
    if (inde3 >= vertexs_count) return;
    MESH_VERTEXKARI vertex1 = vertexBuf[vertex_place+inde1];
    MESH_VERTEXKARI vertex2 = vertexBuf[vertex_place+inde2];
    MESH_VERTEXKARI vertex3 = vertexBuf[vertex_place+inde3];

	float obb1u0 = abs(dot(au1.v * au1.dt,obb1.u[0]));
	float obb1u1 = abs(dot(au1.v * au1.dt,obb1.u[1]));
	float obb1u2 = abs(dot(au1.v * au1.dt,obb1.u[2]));
	AtariUnitOBB obbdayo;
	obbdayo.c = obb1.c - au1.v * au1.dt/2;
	obbdayo.u[0] = obb1.u[0];
	obbdayo.u[1] = obb1.u[1];
	obbdayo.u[2] = obb1.u[2];
	obbdayo.e[0] = obb1.e[0] + obb1u0;
	obbdayo.e[1] = obb1.e[1] + obb1u1;
	obbdayo.e[2] = obb1.e[2] + obb1u2;




    ANS_MAJIWARITIKEITOSOREIGAI at = majiwariAtariUnitTikeiToSoreigai(vertex1, vertex2, vertex3, obbdayo, au2);

    if (at.is_majiwari) {
      uint idxd;
      idxd = ansBuf2.IncrementCounter();
  	  if (idxd >= ans_count) return;
      ansBuf2[idxd].is_use = 1;
      ansBuf2[idxd].atari_idx = autts.atari_idx;
      ansBuf2[idxd].atari_idx2 = autts.atari_idx2;
      ansBuf2[idxd].obb_idx = autts.obb_idx;
      ansBuf2[idxd].obb_idx2 = 0;
      ansBuf2[idxd].kouten_jyusin = float3(at.pos.x,at.pos.y,at.pos.z);
      ansBuf2[idxd].kouten_housen = float3(at.housen.x, at.housen.y, at.housen.z);
     return;
    }  

	}






}

}













