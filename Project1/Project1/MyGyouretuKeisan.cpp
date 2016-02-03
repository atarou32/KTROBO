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




void MyIKMODOKI::setFreeBone() {




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

	MeshBone* moku_bone = mesh->Bones[mesh->BoneIndexes[mokuhyou_bonename.c_str()]];
	MeshBone* temp_bone = moku_bone->parent_bone;
	int temp_retu = 0;
	while ((strcmp(temp_bone->bone_name, moto_bonename.c_str()) !=0) && (temp_bone != NULL) && (temp_retu < n)) {
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
		MyMatrixMultiply(mymat, temp_bone->combined_matrix, temp_bone->matrix_local);
		MyVec3TransformNormal(x_h,x,mymat);
		MyVec3TransformNormal(y_h,y,mymat);
		MyVec3TransformNormal(z_h,z,mymat);
		MyVec3TransformCoord(gen_h,gen,mymat);
		
		
	




		// ワールド座標系におけるボーン座標の原点から目標地点までの方向ベクトルを求める




	}

}


void MyIKMODOKI::updateStep() {



}


void MyIKMODOKI::updateMesh() {




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

	if (XM < NTHETA) {
		// M > N のとき　[JT J ]-1 JT
		getMatrixTranspose(mat,gyou,retu,temp);
		getMatrixMultiply(retu,gyou,temp,gyou,retu,mat,temp2);
		getMatrixInverse(temp2,retu,temp2);
		getMatrixMultiply(retu,retu,temp2,retu,gyou,temp,out_mat);

	} else if(XM == NTHETA) {
		// M= N のとき　J-1

		getMatrixInverse(mat, retu, out_mat);

	} else {
		// M < N のとき　JT [J JT]-1 
		getMatrixTranspose(mat,gyou,retu,temp);
		getMatrixMultiply(gyou,retu,mat,retu,gyou,temp,temp2);
		getMatrixInverse(temp2,gyou,temp2);
		getMatrixMultiply(retu,gyou,temp,gyou,gyou,temp2,out_mat);
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
	memcpy(temp,mat,sizeof(float)*retu*retu);

	 // make pivots non-zero numbers  
    for (int i = 0; i < retu; i++) {  
        if (temp[i*retu+i] != 0)  
            continue;  
        int k;  
        for (k = 0; k < retu; k++)  
            if (temp[k*retu+i] != 0)  
                break;  
  
        if (k == retu)          // all numbers in a column is zero  
            return 0;  
  
        for (int j = 0; j < retu; j++)  
            temp[i*retu+j] += temp[k*retu+j];  
    }  
  
    // make a triangular matrix  
    for (int i = 0; i < retu; i++) {  
        for (int j = 0; j < retu; j++) {  
            if (i == j)  
                continue;  
            float c = temp[j*retu+i] / temp[i*retu+i];  
            for (int k = 0; k < retu; k++)  
                temp[j*retu+k] -=  c * temp[i*retu+k];  
        }  
    }  
  
    float det = 1;  
    for (int i = 0; i < retu; i++)  
        det *= temp[i*retu+i];  
  
    return det;  
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




