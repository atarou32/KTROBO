#include "MyGyouretuKeisan.h"
#include "KTRoboGameError.h"
#include "memory.h"

using namespace KTROBO;

MyGyouretuKeisan::MyGyouretuKeisan(void)
{
}


MyGyouretuKeisan::~MyGyouretuKeisan(void)
{
}


void MyIKMODOKI::calcYMat() {
	memset(ymat,0,sizeof(float)*GYOURETU_MAX*GYOURETU_MAX);
	int n = 0;
	n += x_freebone.size();
	n += y_freebone.size();
	n += z_freebone.size();

	int gyou = 3;

	if (n*gyou >= GYOURETU_MAX*GYOURETU_MAX) {
		throw new KTROBO::GameError(KTROBO::WARNING, "gyouretumax up error");
	}
	
	// ヤコビアン行列を求める
	bone_rotxdayo.clear();
	bone_rotydayo.clear();
	bone_rotzdayo.clear();

	MeshBone* moku_bone = mesh->Bones[mesh->BoneIndexes[mokuhyou_bonename.c_str()]];
	MeshBone* temp_bone = moku_bone;

	MYVECTOR3 now_moku_iti;
	MeshBone* btemp_bone= moku_bone;



	MYMATRIX mymat;
	MyMatrixMultiply(mymat, temp_bone->matrix_local,temp_bone->combined_matrix);
	MyMatrixMultiply(mymat,mymat,world);
	MYVECTOR3 gen(0,0,0);
	MYVECTOR3 gen_h(0,0,0);
	MyVec3TransformCoord(gen_h,gen,mymat);
		
	now_moku_iti = gen_h;
	MYVECTOR3 mymoku = mokuhyou - now_moku_iti;
	//mymoku = mymoku * 0.8f;
	mymoku = mymoku + now_moku_iti;






	int temp_retu = 0;
	while ((strcmp(btemp_bone->bone_name, moto_bonename.c_str()) !=0) && (temp_bone != NULL) && (temp_retu < n)) {
		MYVECTOR3 x(1,0,0);
		MYVECTOR3 y(0,1,0);
		MYVECTOR3 z(0,0,1);
		MYVECTOR3 gen(0,0,0);
		// ボーン座標系からワールド座標系に変換する
		MYVECTOR3 x_h(0,0,0);
		MYVECTOR3 y_h(0,0,0);
		MYVECTOR3 z_h(0,0,0);
		MYVECTOR3 gen_h(0,0,0);
		MYMATRIX mymat;
		MyMatrixMultiply(mymat, temp_bone->matrix_local,temp_bone->combined_matrix);
		MyMatrixMultiply(mymat, mymat, world);
		MyVec3TransformNormal(x_h,x,mymat);
		MyVec3TransformNormal(y_h,y,mymat);
		MyVec3TransformNormal(z_h,z,mymat);
		MyVec3TransformCoord(gen_h,gen,mymat);
		
		if (temp_bone == moku_bone) {
			now_moku_iti = gen_h;
		}


		// ワールド座標系におけるボーン座標の原点から目標地点までの方向ベクトルを求める
		MYVECTOR3 gen_to_moku;
		MyVec3Subtract(gen_to_moku, mymoku, gen_h);

		MYVECTOR3 sx_h;
		MYVECTOR3 sy_h;
		MYVECTOR3 sz_h;
		MyVec3Cross(sx_h, x_h, gen_to_moku);
		MyVec3Cross(sy_h, y_h, gen_to_moku);
		MyVec3Cross(sz_h, z_h, gen_to_moku);

		if (x_freebone.find(temp_bone->bone_name) != x_freebone.end()) {
			// あったので入れる
			ymat[temp_retu*3] = sx_h.float3.x;
			ymat[temp_retu*3+1] = sx_h.float3.y;
			ymat[temp_retu*3+2] = sx_h.float3.z;
			bone_rotxdayo.insert(pair<string,int>(temp_bone->bone_name, temp_retu));
			temp_retu++;
		}

		if (y_freebone.find(temp_bone->bone_name) != y_freebone.end()) {
			// あったので入れる
			ymat[temp_retu*3] = sy_h.float3.x;
			ymat[temp_retu*3+1] = sy_h.float3.y;
			ymat[temp_retu*3+2] = sy_h.float3.z;
			bone_rotydayo.insert(pair<string,int>(temp_bone->bone_name, temp_retu));
			temp_retu++;
		}


		if (z_freebone.find(temp_bone->bone_name) != z_freebone.end()) {
			// あったので入れる
			ymat[temp_retu*3] = sz_h.float3.x;
			ymat[temp_retu*3+1] = sz_h.float3.y;
			ymat[temp_retu*3+2] = sz_h.float3.z;
			bone_rotzdayo.insert(pair<string,int>(temp_bone->bone_name, temp_retu));
			temp_retu++;
		}
		btemp_bone = temp_bone;
		temp_bone = temp_bone->parent_bone;

	}

	// 転置したのがヤコビ行列
	float tempmat[GYOURETU_MAX*GYOURETU_MAX];
	memset(tempmat,0,sizeof(tempmat));
	MyGyouretuKeisan mk;
	mk.getMatrixTranspose(ymat, temp_retu, 3, tempmat);
	memset(ymat,0,sizeof(float)*GYOURETU_MAX*GYOURETU_MAX);
	for (int i=0;i<GYOURETU_MAX*GYOURETU_MAX;i++) {
		ymat[i] = tempmat[i];
	}

	// ヤコビ行列が求まった

	// 逆ヤコビ行列を求める
	memset(tempmat,0,sizeof(tempmat));
	memset(inv_ymat,0,sizeof(float)*GYOURETU_MAX*GYOURETU_MAX);

	mk.getInverseYFromY(3,temp_retu,ymat,3,n,inv_ymat);
	// dx/dt dy/dt dz/dt を定める必要がある
	MYVECTOR3 vdayo = mymoku - now_moku_iti;

	float length = MyVec3Length(vdayo);
	if (length < 0.00001) {
		vdayo = MYVECTOR3(0,0,0);
	}

	MyVec3Normalize(vdayo,vdayo);

	vdayo.float3.x *= 1;
	vdayo.float3.y *= 1;
	vdayo.float3.z *= 1;


	float un=1;

	float absdayo = 0;
	int inde = 0;
	for (int i=0;i<3;i++) {
		for (int k=0;k<temp_retu;k++) {
			if (absdayo < abs(inv_ymat[3*k+i])) {
				inde = 3*k+i;
				absdayo = abs(inv_ymat[3*k+i]);
			}
		}
	}


	if (absdayo > 1) {
		float test = absdayo;
		while(test > un) {
			un *= 10;
		}
	} else {
		float test = absdayo;
		while(test < un) {
			un /= 10;
		}
	}
	un=1;

	float vtemp[3];
	vtemp[0] = vdayo.float3.x/un;
	vtemp[1] = vdayo.float3.y/un;
	vtemp[2] = vdayo.float3.z/un;
	memset(dthetadayo,0,sizeof(float)*GYOURETU_MAX*GYOURETU_MAX);
	mk.getMatrixMultiply(temp_retu,3,inv_ymat,3,1,vtemp, dthetadayo);

	float maxdayo=0;
	
	for (int i=0;i<temp_retu;i++) {
		if (maxdayo < abs(dthetadayo[i])) {
			maxdayo = abs(dthetadayo[i]);
		}
	}
	if (maxdayo > 1) {
		float test = maxdayo;
		while(test > un) {
			un *= 10;
		}
	} else {
		float test = maxdayo;
		while(test < un) {
			un /= 10;
		}
	}
	
	for (int i=0;i<temp_retu;i++) {
		if (un > 0.0000001) {
			dthetadayo[i] /= un * 1;
		} else {
			// un
		//	dthetadayo[i] *= 10000000;
		}
	}




}


void MyIKMODOKI::updateStep() {

calcYMat();





}
void MyGyouretuKeisan::getMatrixTranspose(float* mat, int gyou, int retu, float* out_mat) {

	for (int i=0;i<gyou;i++) {
		for (int j=0;j<retu;j++) {
			out_mat[gyou*j+i] = mat[retu*i+j];
		}
	}
}


void MyGyouretuKeisan::getInverseYFromY(int gyou, int retu, float* mat, int XM, int NTHETA, float* out_mat) {

	float temp[GYOURETU_MAX*GYOURETU_MAX];
	memset(temp,0,sizeof(temp));
	float temp2[GYOURETU_MAX*GYOURETU_MAX];
	memset(temp2, 0, sizeof(temp2));
	float temp3[GYOURETU_MAX*GYOURETU_MAX];
	memset(temp3, 0, sizeof(temp3));

	if (XM > NTHETA) {
		// M > N のとき　[JT J ]-1 JT
		getMatrixTranspose(mat,gyou,retu,temp);
		getMatrixMultiply(retu,gyou,temp,gyou,retu,mat,temp2);
		getMatrixInverse(temp2,retu,temp3);
		getMatrixMultiply(retu,retu,temp3,retu,gyou,temp,out_mat);
		//getMatrixMultiply(retu,gyou,out_mat, gyou,retu, mat,temp2);
		//getMatrixMultiply(gyou,retu,mat, retu,gyou, out_mat,temp2);
	} else if(XM == NTHETA) {
		// M= N のとき　J-1

		getMatrixInverse(mat, retu, out_mat);

	} else {
		// M < N のとき　JT [J JT]-1 
		getMatrixTranspose(mat,gyou,retu,temp);
		getMatrixMultiply(gyou,retu,mat,retu,gyou,temp,temp2);
		getMatrixInverse(temp2,gyou,temp3);
		getMatrixMultiply(retu,gyou,temp,gyou,gyou,temp3,out_mat);
		//getMatrixMultiply(gyou,retu,mat, retu,gyou, out_mat,temp2);
		//getMatrixTranspose(out_mat, retu, gyou, temp3);
		//getMatrixMultiply(retu,gyou,temp,gyou,retu,temp3,temp2);
	}
}



void MyGyouretuKeisan::getMatrixMultiply(int gyou, int retu, float* mat, int gyou2, int retu2, float* mat2, float* out_mat) {
	float out[GYOURETU_MAX*GYOURETU_MAX];
	memset(out, 0, sizeof(out));
	if (retu != gyou2 || GYOURETU_MAX <= retu || GYOURETU_MAX <= gyou || GYOURETU_MAX <= retu2 || GYOURETU_MAX <= gyou2) {
		throw new KTROBO::GameError(KTROBO::WARNING, "error no retu gyou itti");
	}

	for (int k=0;k<gyou;k++) {
		for (int l=0;l < retu2; l++) {
			float t = 0;
			for (int i=0;i<retu;i++) {
				t += mat[retu*k+i] * mat2[retu2*i+l];
			}
			out[k*retu2+l] = t;
		}
	}

	for (int i=0;i<retu2*gyou;i++) {
		out_mat[i] = out[i];
	}
}


float MyGyouretuKeisan::det(const float* mat, int retu) {
	float temp[GYOURETU_MAX*GYOURETU_MAX];
	if (retu >= GYOURETU_MAX) {
		throw new KTROBO::GameError(KTROBO::WARNING, "error in deter");
	}
	memset(temp, 0 ,sizeof(temp));
	memcpy(temp,mat,sizeof(float)*retu*retu);

	if (retu == 2) {
		return mat[0]*mat[3]-mat[1]*mat[2];
	}

	if (retu == 3) {
		return mat[0*retu+0]*mat[1*retu+1]*mat[2*retu+2] + mat[retu*0+1]*mat[retu*1+2]*mat[retu*2+0]
		+mat[0*retu+2]*mat[retu*1+0]*mat[retu*2+1] - mat[retu*0+2]*mat[retu*2+0]*mat[retu*1+1] - mat[retu*1+0]*mat[retu*0+1]*mat[retu*2+2]
		-mat[retu*0+0]*mat[retu*2+1]*mat[retu*1+2];
	}

	if (retu == 4) {
		return mat[0*retu+0]*mat[1*retu+1]*mat[2*retu+2]*mat[3*retu+3] + mat[0+retu*0]*mat[retu*1+2]*mat[retu*2+3]*mat[retu*3+1]
		+mat[0*retu+0]*mat[retu*1+3]*mat[retu*2+1]*mat[retu*3+2]-mat[0*retu+0]*mat[1*retu+3]*mat[2*retu+2]*mat[retu*3+1]
		-mat[0*retu+0]*mat[1*retu+2]*mat[2*retu+1]*mat[3*retu+3]
		-mat[0*retu+0]*mat[1*retu+1]*mat[2*retu+3]*mat[3*retu+2]
		-mat[0*retu+1]*mat[1*retu+0]*mat[2*retu+2]*mat[3*retu+3]
		-mat[0*retu+2]*mat[1*retu+0]*mat[2*retu+3]*mat[3*retu+1]
		-mat[0*retu+3]*mat[1*retu+0]*mat[2*retu+1]*mat[3*retu+2]
		+mat[0*retu+3]*mat[1*retu+0]*mat[2*retu+2]*mat[3*retu+1]
		+mat[0*retu+2]*mat[1*retu+0]*mat[2*retu+1]*mat[3*retu+3]
		+mat[0*retu+1]*mat[1*retu+0]*mat[2*retu+3]*mat[3*retu+2]
		+mat[0*retu+1]*mat[1*retu+2]*mat[2*retu+0]*mat[3*retu+3]
		+mat[0*retu+2]*mat[1*retu+3]*mat[2*retu+0]*mat[3*retu+1]
		+mat[0*retu+3]*mat[1*retu+1]*mat[2*retu+0]*mat[3*retu+2]
		-mat[0*retu+3]*mat[1*retu+2]*mat[2*retu+0]*mat[3*retu+1]
		-mat[0*retu+2]*mat[1*retu+1]*mat[2*retu+0]*mat[3*retu+3]
		-mat[0*retu+1]*mat[1*retu+3]*mat[2*retu+0]*mat[3*retu+2]
		-mat[0*retu+1]*mat[1*retu+2]*mat[2*retu+3]*mat[3*retu+0]
		-mat[0*retu+2]*mat[1*retu+3]*mat[2*retu+1]*mat[3*retu+0]
		-mat[0*retu+3]*mat[1*retu+1]*mat[2*retu+2]*mat[3*retu+0]
		+mat[0*retu+3]*mat[1*retu+2]*mat[2*retu+1]*mat[3*retu+0]
		+mat[0*retu+2]*mat[1*retu+1]*mat[2*retu+3]*mat[3*retu+0]
		+mat[0*retu+1]*mat[1*retu+3]*mat[2*retu+2]*mat[3*retu+0];


	}












	 // make pivots non-zero numbers  
    for (int i = 0; i < retu; i++) {  
        if (abs(temp[i*retu+i]) > 0.1f && abs(temp[i*retu+i] < 10000))  
            continue;  
        int k;  
        for (k = 0; k < retu; k++)  
            if (temp[i*retu+k] != 0)  
                break;  
  
        if (k == retu)          // all numbers in a column is zero  
            return 0;  
  
		if (abs(temp[i*retu+i]) <= 0.1f) {
		float unko=1;
		while(unko > abs(temp[i*retu+k])) {
			unko *=0.1f;
		}


        for (int j = 0; j < retu; j++)  
            temp[j*retu+i] += 1/*/unko*/ * temp[j*retu+k];  
		} else {
			float unko=1;
			while(unko < abs(temp[i*retu+k])) {
				unko *=10;
			}


	        for (int j = 0; j < retu; j++)  
	            temp[retu*j+i] += 1/*/unko*/ * temp[retu*j+k];  
		}
    }  
  
	// 0をつくる
	
	for (int i=1;i<retu;i++){
		double un = temp[retu*0+i]/temp[retu*0+0];
		for (int k=0;k<retu;k++) {
			temp[retu*k+i] -= un * temp[retu*k+0];
		}
	}
	
	// 0をつくるとそのぶん他の要素が０にちかくなるはず？なので桁をよくする
	double det = 1;

	/*
	for (int i=0;i<retu;i++) {
		for (int j=0;j<retu;j++) {
		if (abs(temp[i*retu+i]) <= 0.1f) {
		float unko=1;
		if (abs(temp[i*retu+j]) < 0.0000001) continue;
		while(unko > abs(temp[i*retu+j])) {
			unko *=0.1f;
		}
		for (int k=0;k<retu;k++) {
			temp[k*retu+j] /=unko;
		}
		det *= unko;
		}
		}
	}


	for (int i=0;i<retu;i++) {
		for (int j=i+1;j<retu;j++) {
		if (abs(temp[j*retu+i]) > 100000) {
			// 桁が大きいので小さくする
			float te = abs(temp[j*retu+i]);
			for (int k=0;k<retu;k++) {
				temp[j*retu+k] /= te;
			}
			det *= te;
		}
		}
	}

	*/

	
	double sum_deet=0;
	for (int i=0;i<retu;i++){
		float tem[GYOURETU_MAX*GYOURETU_MAX];
		memset(tem,0,sizeof(tem));
		int temp_count=0;
		for (int x=0;x<retu;x++) {
			for (int y=0;y<retu;y++) {
				if (x==i) continue;
				if (y==i) continue;
				tem[temp_count] = (float)temp[x*retu+y];
				temp_count++;
			}
		}	

		double deet = 0;
		if (abs(temp[0*retu+i]) > 0.00000001) {
		  deet = temp[0*retu+i] * MyGyouretuKeisan::det(tem,retu-1);
		}
		if ((i%2) == 1) {
			deet *=-1;
		}
		sum_deet += deet;
	}
	det = sum_deet;
    return (float)det;  
}

void MyGyouretuKeisan::getMatrixInverse(float* mat, int retu, float* out_mat) {
	float temp[GYOURETU_MAX*GYOURETU_MAX];
	memset(temp,0,sizeof(temp));
	if (retu >= GYOURETU_MAX) {
		
		return;
	}

	float detA = det(mat,retu);
	if (detA == 0) {
		// なし
		detA = 1;
	}
	

	for (int i=0;i<retu;i++) {
		for (int j=0;j<retu;j++) {
			float temp2[GYOURETU_MAX*GYOURETU_MAX];
			memset(temp2,0,sizeof(temp2));
			// ij以外で作った行列を作る
			int temp_count=0;
			for (int k=0;k<retu;k++) {
				for (int l=0;l<retu;l++) {
					if (k == i && l == j) continue;
					if (k == i) continue;
					if (l == j) continue;

						temp2[temp_count] = mat[k*retu+l];
						temp_count++;
				}
			}

			// ij 以外で作った行列の行列式を求める
			float detdayo = det(temp2, retu-1);
			if (((i+j) %2) == 1) {
				temp[i*retu+j] = detdayo * (-1) / detA;
			} else {
				temp[i*retu+j] = detdayo / detA;
			}
		}
	}

	// 転置を取ったものが逆行列

	float temp3[GYOURETU_MAX*GYOURETU_MAX];
	memset(temp3,0,sizeof(temp3));
	for (int i=0;i<retu;i++) {
		for (int j=0;j<retu;j++) {
			temp3[j*retu+i] = temp[i*retu+j];
		}
	}

	for (int i=0;i<retu*retu;i++) {
		out_mat[i] = temp3[i];
	}
}




