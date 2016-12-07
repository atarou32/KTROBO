#include "KTRoboKendo.h"
#include "KTRoboMeshInstanced.h"
#include "KTRoboAtari.h"
#include "MyGyouretuKeisan.h"

using namespace KTROBO;

Kendo::Kendo(void)
{
}


Kendo::~Kendo(void)
{
}


Sinai::Sinai() {

	mesh = new Mesh();
	umesh_unit = new UMeshUnit();
}

Sinai::~Sinai() {
	
	if (umesh_unit) {
		delete umesh_unit;
		umesh_unit = 0;
	}
	
	if (mesh) {
		delete mesh;
		mesh = 0;
	}

}

void Sinai::init(Graphics* g, MyTextureLoader* loader, AtariHantei* hantei) {

	mesh->readMesh(g,"resrc/model/kendo/sinai/sinai.MESH",loader);
	mesh->readAnime("resrc/model/kendo/sinai/sinai.ANIME");
	mesh->animate(0,true);
	MYMATRIX idenmat;
	MyMatrixIdentity(idenmat);
	UMesh* um = new UMesh(g,"resrc/model/kendo/sinai/sinai.MESH",
		loader,mesh,false,&idenmat,0,KTROBO_MESH_BONE_NULL,false);
	umesh_unit->setUMesh(um);
	umesh_unit->setDT(0);
	umesh_unit->setROTXYZ(0,0,0);
	umesh_unit->setSCALEXYZ(10,10,10);
	umesh_unit->setV(&MYVECTOR3(0,0,0));
	umesh_unit->setXYZ(0,0,10);
	float frame_anime = 0;
	bool ob=true;
	umesh_unit->calcAnimeFrame(1,&frame_anime,&ob);
	umesh_unit->calcJyusinAndR();
	hantei->setUMeshUnit(umesh_unit,AtariUnit::AtariType::ATARI_OBJECT);
}

void Sinai::draw(Graphics* g, MYMATRIX* view, MYMATRIX* proj) {
	mesh->drawWithObbs(g,&umesh_unit->world,view,proj);
}


MYVECTOR3 Sinai::getMigitePos() {
	MYVECTOR3 ans(0,0,0);

	MYMATRIX a;

	MyMatrixMultiply(a,umesh_unit->meshs[0]->mesh->Bones[umesh_unit->meshs[0]->mesh->BoneIndexes["migiteBone"]]->matrix_local,umesh_unit->meshs[0]->mesh->Bones[umesh_unit->meshs[0]->mesh->BoneIndexes["migiteBone"]]->combined_matrix);
	MyMatrixMultiply(a,a,umesh_unit->world);
	MyVec3TransformCoord(ans,ans,a);

	return ans;



}

MYVECTOR3 Sinai::getHidaritePos() {

	MYVECTOR3 ans(0,0,0);

	MYMATRIX a;

	MyMatrixMultiply(a,umesh_unit->meshs[0]->mesh->Bones[umesh_unit->meshs[0]->mesh->BoneIndexes["hidariteBone"]]->matrix_local,umesh_unit->meshs[0]->mesh->Bones[umesh_unit->meshs[0]->mesh->BoneIndexes["hidariteBone"]]->combined_matrix);
	MyMatrixMultiply(a,a,umesh_unit->world);
	MyVec3TransformCoord(ans,ans,a);

	return ans;



}
void SinaiNigiru::nigiraseru(Graphics* g, MYMATRIX *view, MYMATRIX* proj) {
	MYVECTOR3 a = sinai->getMigitePos();
	MyIKMODOKI ik(this->nigiruhito->meshs[0]->mesh,&nigiruhito->world,&a,"migiSakotuBone", "migiArmTekubiBone");

	MeshBone* saki = nigiruhito->meshs[0]->mesh->Bones[nigiruhito->meshs[0]->mesh->BoneIndexes["migiArmTekubiBone"]];
	MeshBone* moto = nigiruhito->meshs[0]->mesh->Bones[nigiruhito->meshs[0]->mesh->BoneIndexes["migiSakotuBone"]];
	MeshBone* sakip = saki->parent_bone;
	MeshBone* bsakip = saki->parent_bone;
	int bsize = nigiruhito->meshs[0]->mesh->Bones.size();
	
	MYMATRIX mat;
	MyMatrixMultiply(mat, saki->matrix_local,saki->combined_matrix);
	MyMatrixMultiply(mat,mat,nigiruhito->world);
	MYVECTOR3 vec(0,0,0);
	MyVec3TransformCoord(vec,vec,mat);

//	
	MYVECTOR3 sa = a-vec;
	if (MyVec3Length(sa) < 0.04f) {
//		return;
	}
	//sa.float3.y = 0;
	MYVECTOR3 vv = (vec+ sa);
//	vv.float3.y = 0;
	
	ik.setMokuhyou(&a);
	OBB b;
	b.c = vec;
	MYMATRIX idenmat;
	MyMatrixIdentity(idenmat);
	g->drawOBB(g,0xFFFF4444,&idenmat,view,proj,&b);
	b.c = a;
	g->drawOBB(g,0xFF444444,&idenmat,view,proj,&b);
	while (sakip && strcmp(bsakip->bone_name , moto->bone_name )!=0) {

		ik.setXFreeBone(bsakip->bone_name);
			ik.setYFreeBone(bsakip->bone_name);
			ik.setZFreeBone(bsakip->bone_name);
		/*	
		if (strcmp(bsakip->bone_name, "migiSakotuBone")==0) {
		} else if(strcmp(bsakip->bone_name, "migiArmBone")==0) {
			ik.setXFreeBone(bsakip->bone_name);
			ik.setZFreeBone(bsakip->bone_name);

			ik.setYFreeBone(bsakip->bone_name);

		} else if(strcmp(bsakip->bone_name, "migiArmSitaBone")==0) {

			ik.setZFreeBone(bsakip->bone_name);
		}else {
			ik.setXFreeBone(bsakip->bone_name);
			ik.setYFreeBone(bsakip->bone_name);
			ik.setZFreeBone(bsakip->bone_name);
		}
		*/
		bsakip = sakip;
		sakip = sakip->parent_bone;
	}
	int retry_count = 0;
	if (MyVec3Length(sa) > 0.1f) {

	for (int s=0;s<16;s++) {
		if (retry_count > 16) {
			break;
		}

		this->setOffsetMatrixToNigiru();
		ik.updateStep();
		bool is_updateddayo = false;
		int kawaranai_count = 0;
		for (int i=0;i<bsize;i++) {

			MeshBone* bone = nigiruhito->meshs[0]->mesh->Bones[i];
			float drotx = ik.getdthetaXBone(bone->bone_name)/3.14/60;
			float droty = ik.getdthetaYBone(bone->bone_name)/3.14/60;
			float drotz = ik.getdthetaZBone(bone->bone_name)/3.14/60;
			if (fabsf(drotx) > 0.0001f) {
				float nrotx = this->getRotX(i);
				float hon_rotx = this->getHonRotX(bone->bone_name,nrotx+drotx);
				this->setRotX(i, hon_rotx);

				if (fabsf(nrotx - hon_rotx) < 0.0001f) {
					kawaranai_count++;
				}

		//		this->setRotX(i, nrotx+drotx);
				is_updateddayo = true;
			}
			if (fabsf(droty) > 0.0001f) {
				float nroty = this->getRotY(i);
				float hon_roty = this->getHonRotY(bone->bone_name,nroty+droty);
				this->setRotY(i, hon_roty);
		//		this->setRotY(i,nroty+droty);
				if (fabsf(nroty - hon_roty) < 0.0001f) {
					kawaranai_count++;
				}
					is_updateddayo = true;
			}
			if (fabsf(drotz) > 0.0001f) {
				float nrotz = this->getRotZ(i);
				float hon_rotz = this->getHonRotZ(bone->bone_name, nrotz+drotz);
				this->setRotZ(i, hon_rotz);
		//		this->setRotZ(i, nrotz + drotz);
				if (fabsf(nrotz - hon_rotz) < 0.0001f) {
					kawaranai_count++;
				}
					is_updateddayo = true;
			}
		}

	

		if (!is_updateddayo) {
			break;
		}

		if (kawaranai_count > ik.getfreenum()/2) {
			// XV‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚é‚Ì‚É‚³‚ê‚Ä‚¢‚È‚¢‚Æ‚¢‚¤ó‘Ô‚Åˆø‚Á‚©‚©‚Á‚Ä‚éê‡‚ª‚ ‚é


			saki = nigiruhito->meshs[0]->mesh->Bones[nigiruhito->meshs[0]->mesh->BoneIndexes["migiArmTekubiBone"]];
			MyMatrixMultiply(mat, saki->matrix_local,saki->combined_matrix);
			MyMatrixMultiply(mat,mat,nigiruhito->world);
			MYVECTOR3 vec(0,0,0);
			MyVec3TransformCoord(vec,vec,mat);	
			MYVECTOR3 sa = a-vec;
			if (MyVec3Length(sa) > 1) {




				for (int k=0;k<bsize;k++) {
					int test = rand()%256;

					if (ik.hasFreeBone(nigiruhito->meshs[0]->mesh->Bones[k]->bone_name)) {


					setRotX(k,test/256.00f * 1.57 -0.77);
					test = rand()%256;
					setRotY(k,test/256.00f * 1.57-0.77);
					test = rand()%256;
					setRotZ(k,test/256.0f*1.57-0.77);
					retry_count++;
					s = 0;
					}
				}
				// V‚µ‚¢Žp¨‚©‚ç‚â‚è‚È‚¨‚·
			} else {
				// ŠK‚ª‚Å‚½
				break;
			}

		}


		this->setOffsetMatrixToNigiru();
	}

	}






	
	a = sinai->getHidaritePos();
	MyIKMODOKI ik2(this->nigiruhito->meshs[0]->mesh,&nigiruhito->world,&a,"hidariSakotuBone", "hidariArmTekubiBone");

	 saki = nigiruhito->meshs[0]->mesh->Bones[nigiruhito->meshs[0]->mesh->BoneIndexes["hidariArmTekubiBone"]];
	 moto = nigiruhito->meshs[0]->mesh->Bones[nigiruhito->meshs[0]->mesh->BoneIndexes["hidariSakotuBone"]];
	 sakip = saki->parent_bone;
	 bsakip = saki->parent_bone;
	bsize = nigiruhito->meshs[0]->mesh->Bones.size();
	//MyMatrixInverse(mat,NULL,nigiruhito->world);
	//MyVec3TransformCoord(a,a,mat);
	ik2.setMokuhyou(&a);
	
	
	mat;
	MyMatrixMultiply(mat, saki->matrix_local,saki->combined_matrix);
	MyMatrixMultiply(mat,mat,nigiruhito->world);
	MYVECTOR3 vec2(0,0,0);
	MyVec3TransformCoord(vec2,vec2,mat);

//	
	sa = a-vec2;
	//}
//	MYMATRIX mat;
//	MyMatrixMultiply(mat, saki->combined_matrix, saki->matrix_local);
//	MYVECTOR3 vec(0,0,0);
//	MyVec3TransformCoord(vec,vec,mat);
//	vec.float3.z -= 10.0f;
//	vec = MYVECTOR3(0,-15,11);
//	ik.setMokuhyou(&vec);
	
	while (sakip && strcmp(bsakip->bone_name , moto->bone_name )!=0) {

		ik2.setXFreeBone(bsakip->bone_name);
			ik2.setYFreeBone(bsakip->bone_name);
			ik2.setZFreeBone(bsakip->bone_name);
			/*
		if (strcmp(bsakip->bone_name, "hidariSakotuBone")==0) {
			//ik2.setYFreeBone(bsakip->bone_name);
			//ik2.setZFreeBone(bsakip->bone_name);
		} else if(strcmp(bsakip->bone_name, "hidariArmBone")==0) {
			ik2.setXFreeBone(bsakip->bone_name);
			ik2.setZFreeBone(bsakip->bone_name);
			ik2.setYFreeBone(bsakip->bone_name);
		} else if(strcmp(bsakip->bone_name, "hidariArmSitaBone")==0) {

			ik2.setZFreeBone(bsakip->bone_name);
		}else {
			ik2.setXFreeBone(bsakip->bone_name);
			ik2.setYFreeBone(bsakip->bone_name);
			ik2.setZFreeBone(bsakip->bone_name);
		}
		*/
		bsakip = sakip;
		sakip = sakip->parent_bone;
	}

	retry_count =0;

	if (MyVec3Length(sa) > 0.1f) {
	for (int s=0;s<16;s++) {
		if (retry_count > 16) {
			break;
		}

		this->setOffsetMatrixToNigiru();
		ik2.updateStep();
		bool is_updateddayo = false;
		int kawaranai_count = 0;
		for (int i=0;i<bsize;i++) {

			MeshBone* bone = nigiruhito->meshs[0]->mesh->Bones[i];
			float drotx = ik2.getdthetaXBone(bone->bone_name)/3.14/60;
			float droty = ik2.getdthetaYBone(bone->bone_name)/3.14/60;
			float drotz = ik2.getdthetaZBone(bone->bone_name)/3.14/60;
			if (abs(drotx) > 0.0001f) {
				float nrotx = this->getRotX(i);
				float hon_rotx = this->getHonRotX(bone->bone_name,nrotx+drotx);
				this->setRotX(i, hon_rotx);
//				this->setRotX(i, nrotx+drotx);
				is_updateddayo = true;
				if (fabsf(nrotx - hon_rotx) < 0.0001f) {
					kawaranai_count++;
				}
			}
			if (abs(droty) > 0.0001f) {
				float nroty = this->getRotY(i);
				float hon_roty = this->getHonRotY(bone->bone_name,nroty+droty);
				this->setRotY(i, hon_roty);
//				this->setRotY(i, nroty+droty);
				is_updateddayo = true;
				if (fabsf(nroty - hon_roty) < 0.0001f) {
					kawaranai_count++;
				}
			}
			if (abs(drotz) > 0.0001f) {
				float nrotz = this->getRotZ(i);
				float hon_rotz = this->getHonRotZ(bone->bone_name, nrotz+drotz);
				this->setRotZ(i, hon_rotz);
	//			this->setRotZ(i, nrotz+drotz);
				is_updateddayo = true;
				if (fabsf(nrotz - hon_rotz) < 0.0001f) {
					kawaranai_count++;
				}
			}
		}
		if (!is_updateddayo) break;




		if (kawaranai_count == ik2.getfreenum()) {
			// XV‚µ‚æ‚¤‚Æ‚µ‚Ä‚¢‚é‚Ì‚É‚³‚ê‚Ä‚¢‚È‚¢‚Æ‚¢‚¤ó‘Ô‚Åˆø‚Á‚©‚©‚Á‚Ä‚éê‡‚ª‚ ‚é


			saki = nigiruhito->meshs[0]->mesh->Bones[nigiruhito->meshs[0]->mesh->BoneIndexes["hidariArmTekubiBone"]];
			MyMatrixMultiply(mat, saki->matrix_local,saki->combined_matrix);
			MyMatrixMultiply(mat,mat,nigiruhito->world);
			MYVECTOR3 vec(0,0,0);
			MyVec3TransformCoord(vec,vec,mat);	
			MYVECTOR3 sa = a-vec;
			if (MyVec3Length(sa) > 1) {




				for (int k=0;k<bsize;k++) {
					int test = rand()%256;

					if (ik2.hasFreeBone(nigiruhito->meshs[0]->mesh->Bones[k]->bone_name)) {


					setRotX(k,test/256.00f * 1.57 -0.77);
					test = rand()%256;
					setRotY(k,test/256.00f * 1.57-0.77);
					test = rand()%256;
					setRotZ(k,test/256.0f*1.57-0.77);
					retry_count++;
					s = 0;
					}
				}
				// V‚µ‚¢Žp¨‚©‚ç‚â‚è‚È‚¨‚·
			} else {
				// ŠK‚ª‚Å‚½
				break;
			}

		}









		this->setOffsetMatrixToNigiru();
	}
	}
	
}

void SinaiNigiru::setOffsetMatrixToNigiru() {
	int bone_max = nigiruhito->meshs[0]->mesh->Bones.size();
	for (int i=0;i<bone_max;i++) {
		MeshBone* bn = nigiruhito->meshs[0]->mesh->Bones[i];
		float frame = this->meshboneDefaultAnimeFrame[i];//this->frame;
		unsigned short ans_minmax;
		unsigned short ans_maxmin;
		float weight;
		nigiruhito->meshs[0]->mesh->getOffsetMatrixToGetMinMaxAndWeightIndex(bn, frame, &ans_minmax, &ans_maxmin, &weight);
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

		// Œ»Ý‚ÌŽp¨‚Ìoffsetmatrix‚ÍŒvŽZ‚Å‚«‚½‚Ì‚ÅŽŸ‚Í•Ï‰»‚µ‚½‚ ‚Æ‚Ìoffsetmatrix‚ð‹‚ß‚é
		// zxy ‚Ì‡‚É‚©‚¯‚é
		int bone_index = bn->bone_index;//this->mesh_bone_name_index[bn->bone_name];
		MYMATRIX rotz;
	    MYMATRIX ans;
		MyMatrixRotationZ(rotz, this->RotZ[bone_index]);
		MYMATRIX rotx;
		MYVECTOR3 axisx(1,0,0);
//		MyVec3TransformNormal(axisx,axisx,rotz);
		MyMatrixRotationAxis(rotx, axisx,this->RotX[bone_index]);
//		MyMatrixRotationX(rotx, this->mesh_bone_rotx[bone_index]);
		MyMatrixMultiply(ans, rotz, rotx);

		MYMATRIX roty;
		MYVECTOR3 axisy(0,1,0);
//		MyVec3TransformNormal(axisy,axisy,ans);
		MyMatrixRotationAxis(roty, axisy,this->RotY[bone_index]);

		//MyMatrixRotationY(roty, this->mesh_bone_roty[bone_index]);
		// trans ‚ÍÁ‚·
//		MYMATRIX trans;
//		MyMatrixTranslation(trans, this->mesh_bone_transx[bone_index],  this->mesh_bone_transy[bone_index], 
//			 this->mesh_bone_transz[bone_index] );
	
		MyMatrixMultiply(ans, rotz, rotx);
		MyMatrixMultiply(ans, ans, roty);
//		MyMatrixMultiply(ans, ans, trans);
		MyMatrixMultiply(ans, ans, mat3);
		this->mesh_offset_matrix[bone_index] = ans;
		bn->offset_matrix = mesh_offset_matrix[bone_index];
	}


	












	nigiruhito->meshs[0]->mesh->animate(0,false);






}



	float SinaiNigiru::getHonRotX(char* bone_name, float rotx) {

		if (strcmp(bone_name, "migiArmBone")==0) {
			if (rotx > 1.57) {
				rotx = 1.57;
			}
			if (rotx < -1.38) {
				rotx = -1.38;
			}
			
		
		}

		if (strcmp(bone_name, "migiSakotuBone")==0) {
			if (rotx > 0.57) {
				rotx = 0.57;
			}
			if (rotx < -0.57) {
				rotx = -0.57;
			}
			rotx=0;
		}
		if (strcmp(bone_name,"migiArmSitaBone")==0) {
			if (rotx > 0.57) {
				rotx = 0.57;
			}
			if (rotx < -0.57) {
				rotx = -0.57;
			}
			rotx = 0;
		}

		if (strcmp(bone_name, "hidariArmBone")==0) {
			if (rotx < -1.57) {
				rotx = -1.57;
			}
			if (rotx > 1.78) {
				rotx = 1.78;
			}
		
		}

		if (strcmp(bone_name, "hidariSakotuBone")==0) {
			if (rotx > 0.57) {
				rotx = 0.57;
			}
			if (rotx < -0.57) {
				rotx = -0.57;
			}
			rotx = 0;
		}
		if (strcmp(bone_name,"hidariArmSitaBone")==0) {
			if (rotx > 0.57) {
				rotx = 0.57;
			}
			if (rotx < -0.57) {
				rotx = -0.57;
			}
			rotx = 0;
		}



		return rotx;
	}

	float SinaiNigiru::getHonRotY(char* bone_name, float roty) {

		if (strcmp(bone_name, "migiArmBone")==0) {
			if (roty > 1.57) {
				roty = 1.57;
			}
			if (roty < -1.57) {
				roty = -1.57;
			}

		
			
		}

		if (strcmp(bone_name, "migiSakotuBone")==0) {
			if (roty > 0.57) {
				roty = 0.57;
			}
			if (roty < -0.57) {
				roty = -0.57;
			}
			roty = 0;
		}

		if (strcmp(bone_name,"migiArmSitaBone")==0) {
			if (roty > 0.57) {
				roty = 0.57;
			}
			if (roty < -0.57) {
				roty = -0.57;
			}
			roty = 0;
		}


if (strcmp(bone_name, "hidariArmBone")==0) {
			if (roty < -1.57) {
				roty = -1.57;
			}
			if (roty > 1.57) {
				roty = 1.57;
			}
			//roty = 0;
		}

		if (strcmp(bone_name, "hidariSakotuBone")==0) {
			if (roty > 0.57) {
				roty = 0.57;
			}
			if (roty < -0.57) {
				roty = -0.57;
			}
			roty = 0;
		}

		if (strcmp(bone_name,"hidariArmSitaBone")==0) {
			if (roty > 0.57) {
				roty = 0.57;
			}
			if (roty < -0.57) {
				roty = -0.57;
			}
			roty = 0;
		}




		return roty;


	}
	float SinaiNigiru::getHonRotZ(char* bone_name, float rotz) {
		if (strcmp(bone_name, "migiArmBone")==0) {
			if (rotz > 1.57) {
				rotz = 1.57;
			}
			if (rotz < -1.78) {
				rotz = -1.78;
			}

		
		}

		if (strcmp(bone_name, "migiSakotuBone")==0) {
			if (rotz > 0.57) {
				rotz = 0.57;
			}
			if (rotz < -0.57) {
				rotz = -0.57;
			}
			rotz = 0;
		}

		if (strcmp(bone_name,"migiArmSitaBone")==0) {
			if (rotz > 1.87) {
				rotz = 1.87;
			}
			if (rotz < 0.77) {
				rotz = 0.77;
			}
				
		}

		if (strcmp(bone_name, "hidariArmBone")==0) {
			if (rotz < -1.57) {
				rotz = -1.57;
			}
			if (rotz > 1.78) {
				rotz = 1.78;
			}
		}

		if (strcmp(bone_name, "hidariSakotuBone")==0) {
			if (rotz > 0.57) {
				rotz = 0.57;
			}
			if (rotz < -0.57) {
				rotz = -0.57;
			}
			rotz = 0;
		}

		if (strcmp(bone_name,"hidariArmSitaBone")==0) {
			if (rotz > -0.77) {
				rotz = -0.77;
			}
			if (rotz < -1.87) {
				rotz = -1.87;
			}
				
		}

		return rotz;
	}

