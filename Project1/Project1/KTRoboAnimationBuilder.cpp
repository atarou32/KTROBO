#include "KTRoboAnimationBuilder.h"
#include "KTRoboCS.h"
#include "MyTokenAnalyzer.h"
#include "KTRoboLuaCollection.h"
#include "KTRoboInput.h"
#include "tolua_glue/tolua_glue.h"
#include "MyGyouretuKeisan.h"

using namespace KTROBO;

AnimationBuilder::AnimationBuilder(char* n,int len, MyTextureLoader* lo) : Scene(n,len)
{
	loader = lo;
	MYVECTOR3 from(-10,-10,10);
	MYVECTOR3 at(0,0,0);
	MYVECTOR3 up(0,0,1);
	now_index = 0;
	bone_index = 0;
	MyMatrixLookAtRH(view,from,at,up);
	MyMatrixPerspectiveFovRH(proj, 1, gs[TASKTHREADS_AIDECISION]->getScreenWidth() / (float)gs[TASKTHREADS_AIDECISION]->getScreenHeight(), 1, 1000);
	
	kuru = new kurukuru();
//	MYVECTOR3 bone_poss[KTROBO_MESH_BONE_MAX];
//	int bone_bills[KTROBO_MESH_BONE_MAX];

	for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
		bone_bills[i] = 0;
	}


}

int AnimationBuilder::createAnimationBuilderImpl(char* hon_filepath) {
	AnimationBuilderImpl* impl = new AnimationBuilderImpl(hon_filepath);

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	impls.push_back(impl);
	now_index = impls.size()-1;
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return now_index;
}


void  AnimationBuilder::setOnajiMesh(int impl_id, char* onaji_filepath) {

	char onaji_meshpath[128];
	char onaji_animepath[128];
	memset(onaji_meshpath,0,128);
	memset(onaji_animepath,0,128);
	sprintf_s(onaji_meshpath,128,"%s.MESH", onaji_filepath);
	sprintf_s(onaji_animepath,128,"%s.ANIME", onaji_filepath);
	if (impls.size() > impl_id && impl_id >= 0) {
		AnimationBuilderMesh* mesh = new AnimationBuilderMesh(onaji_filepath, onaji_meshpath,onaji_animepath);	
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		impl->onaji_mesh.push_back(mesh);
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	}
}

void  AnimationBuilder::setKoMesh(int impl_id, char* ko_filepath, char* oya_filepath, char* parent_bone_name, bool is_connect_without_material_local, YARITORI MYMATRIX* matrix_kakeru) {
	char ko_meshpath[128];
	char ko_animepath[128];
	memset(ko_meshpath,0,128);
	memset(ko_animepath,0,128);
	sprintf_s(ko_meshpath,128,"%s.MESH", ko_filepath);
	sprintf_s(ko_animepath, 128, "%s.ANIME", ko_filepath);
	if (impls.size() > impl_id && impl_id >=0) {
		AnimationBuilderMesh* mesh = new AnimationBuilderMesh(ko_filepath, ko_meshpath,ko_animepath);
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		mesh->setOyaMesh(oya_filepath, parent_bone_name, is_connect_without_material_local, matrix_kakeru);
		impl->ko_mesh.push_back(mesh);
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	}

}

int  AnimationBuilder::getHonMeshBoneNum(int impl_id) {
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		int num = impl->hon_mesh->mesh->Bones.size();
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return num;
	}
	return 0;
}

char*  AnimationBuilder::getHonMeshBoneName(int impl_id, int bone_index) {
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			char* bone_name = impl->hon_mesh->mesh->Bones[bone_index]->bone_name;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return bone_name;
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return "nullbone";
	}
	return "nullbone";


}

void  AnimationBuilder::setHonMeshBoneRotX(int impl_id, int bone_index, float rotx) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
	
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isrotx[bone_index]) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_rotx[bone_index] = rotx;
				impl->setIsAnimate(false);
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}

void  AnimationBuilder::setHonMeshBoneRotY(int impl_id, int bone_index, float roty) {
CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isroty[bone_index]) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_roty[bone_index] = roty;
			impl->setIsAnimate(false);
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}


void  AnimationBuilder::setHonMeshBoneRotZ(int impl_id, int bone_index, float rotz) {
CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera&& impl->now_kakera->mesh_bone_isrotz[bone_index]) {
				// ロードされているので設定する
				impl->setIsAnimate(false);
				impl->now_kakera->mesh_bone_rotz[bone_index] = rotz;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}
void  AnimationBuilder::toggleHonMeshBoneRotXIsChange(int impl_id, int bone_index) {
CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	// falseにすると回転が0になりIKでも回転しなくなる
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				impl->setIsAnimate(false);
				impl->now_kakera->mesh_bone_rotx[bone_index] = 0;
				impl->now_kakera->mesh_bone_transx[bone_index] = 0;
				impl->now_kakera->mesh_bone_isrotx[bone_index] = !impl->now_kakera->mesh_bone_isrotx[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}
	
	
	
void  AnimationBuilder::toggleHonMeshBoneRotYIsChange(int impl_id, int bone_index) {
CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	// falseにすると回転が0になりIKでも回転しなくなる
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				impl->setIsAnimate(false);
				impl->now_kakera->mesh_bone_roty[bone_index] = 0;
				impl->now_kakera->mesh_bone_transy[bone_index] = 0;
				impl->now_kakera->mesh_bone_isroty[bone_index] = !impl->now_kakera->mesh_bone_isroty[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}


void  AnimationBuilder::toggleHonMeshBoneRotZIsChange(int impl_id, int bone_index) {
	// falseにすると回転が0になりIKでも回転しなくなる
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				impl->setIsAnimate(false);
				impl->now_kakera->mesh_bone_rotz[bone_index] = 0;
				impl->now_kakera->mesh_bone_transz[bone_index] = 0;
				impl->now_kakera->mesh_bone_isrotz[bone_index] = !impl->now_kakera->mesh_bone_isrotz[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}


void  AnimationBuilder::setHonMeshBoneTransX(int impl_id, int bone_index, float dx) {
CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isrotx[bone_index]) {
				// ロードされているので設定する
				impl->setIsAnimate(false);
				impl->now_kakera->mesh_bone_transx[bone_index] = dx;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}

void  AnimationBuilder::setHonMeshBoneTransY(int impl_id, int bone_index, float dy) {
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isrotx[bone_index]) {
				// ロードされているので設定する
				impl->setIsAnimate(false);
				impl->now_kakera->mesh_bone_transy[bone_index] = dy;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}

void  AnimationBuilder::setHonMeshBoneTransZ(int impl_id, int bone_index, float dz) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
	
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isrotx[bone_index]) {
				// ロードされているので設定する
				impl->setIsAnimate(false);
				impl->now_kakera->mesh_bone_transz[bone_index] = dz;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}

void AnimationBuilderImpl::setNowKakeraKakeraFrame(int frame) {
	
	// 一番近いフレームかけらを見つけてくる
	int now_index = 0;
	int now_frame = 0;
	int bsize = kakeras.size();
	for (int i=0;i<bsize;i++) {
		int mframe = kakeras[i]->frame;
		if (mframe <= frame && now_frame < mframe) {
			now_frame = mframe;
			now_index = i;
		}
	}

	if (bsize) {
		now_kakera->copy(kakeras[now_index]);
	}
	this->setIsAnimate(false);
}

void AnimationBuilderImpl::setNowKakeraFrame(int frame) {
	
	int bsize = now_kakera->mesh_bone_name_index.size();
	for (int b=0;b<bsize;b++) {
		now_kakera->mesh_bone_default_anime_frame[b] = frame;
//		now_kakera->mesh_bone_isrotx[b] = true; isrotに関しては変えない
//		now_kakera->mesh_bone_isroty[b] = true;
//		now_kakera->mesh_bone_isrotz[b] = true;
		now_kakera->mesh_bone_rotx[b] = 0;
		now_kakera->mesh_bone_roty[b] = 0;
		now_kakera->mesh_bone_rotz[b] = 0;
		now_kakera->mesh_bone_transx[b] = 0;
		now_kakera->mesh_bone_transy[b] = 0;
		now_kakera->mesh_bone_transz[b] = 0;
		now_kakera->frame = frame;
		this->setIsAnimate(false);
	}
}

void  AnimationBuilder::setAnimePoseFrameKakera(int impl_id, int frame) {
	// 姿勢を本来のポーズに戻す
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {		
	
		AnimationBuilderImpl *impl = impls[impl_id];
			if (impl->now_kakera) {
				impl->setNowKakeraKakeraFrame(frame);
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}	
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}	

void  AnimationBuilder::setAnimePoseFrame(int impl_id, int frame) {
	// 姿勢を本来のポーズに戻す
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {		
	
		AnimationBuilderImpl *impl = impls[impl_id];
			if (impl->now_kakera) {
				impl->setNowKakeraFrame(frame);
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}	
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}	


void  AnimationBuilder::saveAnimePoseFrame(int impl_id, int frame) {
	// 現在のとっている姿勢を指定したアニメフレームとして保存する
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {		
	
		AnimationBuilderImpl *impl = impls[impl_id];
			if (impl->now_kakera) {

            // すでに同じフレームのかけらがあるかどうか調べる
				int size = impl->kakeras.size();
				for (int i=0;i<size;i++) {
					if (impl->kakeras[i]->frame == frame) {
						impl->kakeras[i]->copy(impl->now_kakera);
						CS::instance()->leave(CS_RENDERDATA_CS, "leave");
						return;
					}
				}

			AnimationMeshKakera* new_kakera = new AnimationMeshKakera();
			new_kakera->copy(impl->now_kakera);
			new_kakera->frame = frame;
		
			impl->kakeras.push_back(new_kakera);
			impl->frame_to_kakera_index.insert(pair<int,int>(frame,size));

			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}	
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}	
	
	
	
	
int  AnimationBuilder::createFrameExe(int impl_id, char* frameexe_name, bool is_loop) {
	int aa = 0;
	AnimationMesh* am = new AnimationMesh();
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		AnimationBuilderImpl *impl = impls[impl_id];
		am->anime_name = string(frameexe_name);
		am->is_loop = is_loop;
		am->all_time = 0;
		am->anime_index = impl->animes.size();
		aa = am->anime_index;
		impl->animes.push_back(am);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return aa;
}

void AnimationBuilder::changeFrameExe(int impl_id, int frameexe_id, int frame_id, int frame, float time) {
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		AnimationBuilderImpl *impl = impls[impl_id];
		AnimationMesh* mm = impl->animes[frameexe_id];
		if (frame_id >=0 && frame_id < mm->frames.size()) {
			mm->frames[frame_id]->frame = frame;
			mm->frames[frame_id]->time = time;
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}


void AnimationBuilder::setFrameToExe(int impl_id, int frameexe_id, int pose_id, int frame, float time) {
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		AnimationBuilderImpl *impl = impls[impl_id];
		AnimationMesh* mm = impl->animes[frameexe_id];
		mm->all_time += time;
		AnimationMeshFrame* fra= new AnimationMeshFrame();
		fra->time = time;
		fra->kakera_index = (impl->frame_to_kakera_index)[pose_id];
		fra->kakera = impl->kakeras[fra->kakera_index];
		fra->frame = frame;
		mm->frames.push_back(fra);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}

void AnimationMesh::write(char* filename, int impl_id) {
	KTROBO::mylog::writelog(filename, "IMPL_ID=%d;\n",impl_id);
	KTROBO::mylog::writelog(filename, "ALL_TIME=%f;\n", this->all_time);
	KTROBO::mylog::writelog(filename, "ANIME_INDEX=%d;\n", this->anime_index);
	KTROBO::mylog::writelog(filename, "ANIME_NAME=\"%s\"\n", this->anime_name);
	if (is_loop) {
		KTROBO::mylog::writelog(filename, "IS_LOOP=1");
	} else {
		KTROBO::mylog::writelog(filename, "IS_LOOP=0");
	}
	int num = this->frames.size();
	KTROBO::mylog::writelog(filename, "FRAME_NUM=%d\n",num);
	for (int i=0;i<num;i++) {
		KTROBO::mylog::writelog(filename, "FRAME{\n");
		
		KTROBO::mylog::writelog(filename, "FR=%d;\n",frames[i]->frame);
		KTROBO::mylog::writelog(filename, "KI=%d;\n", frames[i]->kakera_index);
		KTROBO::mylog::writelog(filename, "KAKERAFRAME=%d", frames[i]->kakera->frame);
		KTROBO::mylog::writelog(filename, "TI=%f;\n", frames[i]->time);

		KTROBO::mylog::writelog(filename, "}\n");
	}
}


bool AnimationBuilder::force_saveNowToFile(char* filename) {
	// すべての今の状態を保存する
	FILE* fp;
	// いったん消去する
	fopen_s(&fp, filename, "w");
	if (fp != NULL) {
		
		return false;
	}
	fclose(fp);

	// 書き出しはじめ

	KTROBO::mylog::writelog(filename, "AB{\n");
	CS::instance()->enter(CS_RENDERDATA_CS, "savenowtofile");
	int num = impls.size();
	KTROBO::mylog::writelog(filename, "IMPLNUM=%d;\n",num);
	CS::instance()->leave(CS_RENDERDATA_CS, "savenowtofile");
	
	for (int i=0;i<num;i++) {
	CS::instance()->enter(CS_RENDERDATA_CS, "savenowtofile");
	AnimationBuilderImpl* im = impls[i];
	KTROBO::mylog::writelog(filename,"IMPL{\n");

	if (im->hon_mesh) {
		KTROBO::mylog::writelog(filename, "HON_MESH{\n");

		im->hon_mesh->write(filename,i);

		KTROBO::mylog::writelog(filename, "}//honmesh\n");
	}

	if (im->onaji_mesh.size()) {
		KTROBO::mylog::writelog(filename, "ONAJI_MESHS{\n");
		int s = im->onaji_mesh.size();
		KTROBO::mylog::writelog(filename, "NUM=%d;\n", s);
		for (int i=0;i<s;i++) {
			KTROBO::mylog::writelog(filename, "ONAJI_MESH{\n");

			im->onaji_mesh[i]->write(filename,i);


			KTROBO::mylog::writelog(filename, "}//onajimesh\n");
		}

		KTROBO::mylog::writelog(filename, "}// onajimeshs\n");
	}

	if (im->ko_mesh.size()) {
		KTROBO::mylog::writelog(filename, "KO_MESHS{\n");
		int s = im->ko_mesh.size();
		KTROBO::mylog::writelog(filename, "NUM=%d;\n", s);
		for (int i=0;i<s;i++) {
			KTROBO::mylog::writelog(filename, "KO_MESH{\n");

			im->ko_mesh[i]->write(filename,i);


			KTROBO::mylog::writelog(filename, "}//komesh\n");
		}

		KTROBO::mylog::writelog(filename, "}// komeshs\n");
	}
	
	if (im->now_kakera) {
		KTROBO::mylog::writelog(filename, "NOW_KAKERA{\n");
		im->now_kakera->write(filename,i);

		KTROBO::mylog::writelog(filename, "}\n");
	}

	int num = im->kakeras.size();
	if (num) {
		KTROBO::mylog::writelog(filename, "KAKERAS{\n");
		KTROBO::mylog::writelog(filename, "NUM=%d;\n", num);
		for (int i=0;i<num;i++) {
			KTROBO::mylog::writelog(filename,"KAKERA {\n");
			im->kakeras[i]->write(filename,i);
			KTROBO::mylog::writelog(filename, "}\n");
		}
		KTROBO::mylog::writelog(filename, "}\n");
	}


	num = im->animes.size();
	if (num) {
			KTROBO::mylog::writelog(filename, "ANIMES{\n");
		KTROBO::mylog::writelog(filename, "NUM=%d;\n", num);
		for (int i=0;i<num;i++) {
			KTROBO::mylog::writelog(filename,"ANIME {\n");
			im->animes[i]->write(filename,i);
			KTROBO::mylog::writelog(filename, "}\n");
		}
		KTROBO::mylog::writelog(filename, "}\n");
	}

	KTROBO::mylog::writelog(filename,"}//impl\n");
	CS::instance()->leave(CS_RENDERDATA_CS, "savenowtofile");
	}


	KTROBO::mylog::writelog(filename, "}//file\n");
}



/*
	vector<float> mesh_bone_rotx;
	vector<float> mesh_bone_roty;
	vector<float> mesh_bone_rotz;
	vector<float> mesh_bone_transx;
	vector<float> mesh_bone_transy;
	vector<float> mesh_bone_transz;
	vector<bool> mesh_bone_isrotx;
	vector<bool> mesh_bone_isroty;
	vector<bool> mesh_bone_isrotz;

	string mesh_filepathname;
	int frame;
	map<string,int> mesh_bone_name_index;

	vector<int> mesh_bone_default_anime_frame;

	vector<MYMATRIX> mesh_offset_matrix;
*/
void AnimationMeshKakera::write(char* filename, int impl_id) {
	KTROBO::mylog::writelog(filename, "IMPL_ID=%d;\n",impl_id);
	KTROBO::mylog::writelog(filename, "ROTTRANSISPARAM{\n");
	int mesh_bone_rotx_size = mesh_bone_rotx.size();
	KTROBO::mylog::writelog(filename, "%d;",mesh_bone_rotx_size);
	for (int i=0;i<mesh_bone_rotx_size;i++) {
		KTROBO::mylog::writelog(filename, "%f;",mesh_bone_rotx[i]);
	}
	KTROBO::mylog::writelog(filename, "\n");

	int mesh_bone_roty_size = mesh_bone_roty.size();
	KTROBO::mylog::writelog(filename, "%d;",mesh_bone_roty_size);
	for (int i=0;i<mesh_bone_roty_size;i++) {
		KTROBO::mylog::writelog(filename, "%f;",mesh_bone_roty[i]);
	}
	KTROBO::mylog::writelog(filename, "\n");

	int mesh_bone_rotz_size = mesh_bone_rotz.size();
	KTROBO::mylog::writelog(filename, "%d;",mesh_bone_rotz_size);
	for (int i=0;i<mesh_bone_rotz_size;i++) {
		KTROBO::mylog::writelog(filename, "%f;",mesh_bone_rotz[i]);
	}
	KTROBO::mylog::writelog(filename, "\n");

	int mesh_bone_transx_size = mesh_bone_transx.size();
	KTROBO::mylog::writelog(filename, "%d;",mesh_bone_transx_size);
	for (int i=0;i<mesh_bone_transx_size;i++) {
		KTROBO::mylog::writelog(filename, "%f;",mesh_bone_transx[i]);
	}
	KTROBO::mylog::writelog(filename, "\n");

	int mesh_bone_transy_size = mesh_bone_transy.size();
	KTROBO::mylog::writelog(filename, "%d;",mesh_bone_transy_size);
	for (int i=0;i<mesh_bone_transy_size;i++) {
		KTROBO::mylog::writelog(filename, "%f;",mesh_bone_transy[i]);
	}
	KTROBO::mylog::writelog(filename, "\n");

	int mesh_bone_transz_size = mesh_bone_transz.size();
	KTROBO::mylog::writelog(filename, "%d;",mesh_bone_transz_size);
	for (int i=0;i<mesh_bone_transz_size;i++) {
		KTROBO::mylog::writelog(filename, "%f;",mesh_bone_transz[i]);
	}
	KTROBO::mylog::writelog(filename, "\n");


	int mesh_bone_isrotx_size = mesh_bone_isrotx.size();
	KTROBO::mylog::writelog(filename, "%d;",mesh_bone_isrotx_size);
	for (int i=0;i<mesh_bone_isrotx_size;i++) {
		if (mesh_bone_isrotx[i]) {
			KTROBO::mylog::writelog(filename, "1;");
		} else {
			KTROBO::mylog::writelog(filename, "0;");
		}
	}
	KTROBO::mylog::writelog(filename, "\n");

	int mesh_bone_isroty_size = mesh_bone_isroty.size();
	KTROBO::mylog::writelog(filename, "%d;",mesh_bone_isroty_size);
	for (int i=0;i<mesh_bone_isroty_size;i++) {
		if (mesh_bone_isroty[i]) {
			KTROBO::mylog::writelog(filename, "1;");
		} else {
			KTROBO::mylog::writelog(filename, "0;");
		}
	}
	KTROBO::mylog::writelog(filename, "\n");

	int mesh_bone_isrotz_size = mesh_bone_isrotz.size();
	KTROBO::mylog::writelog(filename, "%d;",mesh_bone_isrotz_size);
	for (int i=0;i<mesh_bone_isrotz_size;i++) {
		if (mesh_bone_isrotz[i]) {
			KTROBO::mylog::writelog(filename, "1;");
		} else {
			KTROBO::mylog::writelog(filename, "0;");
		}
	}
	KTROBO::mylog::writelog(filename, "\n");
	KTROBO::mylog::writelog(filename, "}\n");
	/*
	string mesh_filepathname;
	int frame;
	map<string,int> mesh_bone_name_index;

	vector<int> mesh_bone_default_anime_frame;

	vector<MYMATRIX> mesh_offset_matrix;
	*/

	KTROBO::mylog::writelog(filename, "MESH_FILEPATHNAME=\"%s\";\n", mesh_filepathname.c_str());
	KTROBO::mylog::writelog(filename, "FRAME=%d;\n", frame);
	KTROBO::mylog::writelog(filename, "MESHBONENAMEINDEX{\n");
	int num = mesh_bone_name_index.size();
	KTROBO::mylog::writelog(filename, "%d;\n", num);
	map<string,int>::iterator itt = mesh_bone_name_index.begin();
	while( itt  != mesh_bone_name_index.end()) {
		pair<string,int> p = *itt;
		KTROBO::mylog::writelog(filename, "\"%s\"=%d;\n",p.first,p.second);
	}
	KTROBO::mylog::writelog(filename, "}\n");

	KTROBO::mylog::writelog(filename, "DEFAULTFRAMEANDMATRIX{\n");

	num = mesh_bone_default_anime_frame.size();
	KTROBO::mylog::writelog(filename, "%d;",num);
	for (int i=0;i<num;i++) {
		KTROBO::mylog::writelog(filename, "%d;",mesh_bone_default_anime_frame[i]);
	}
	KTROBO::mylog::writelog(filename, "\n");

	num = mesh_offset_matrix.size();
	KTROBO::mylog::writelog(filename, "%d;\n",num);
	for (int i=0;i<num;i++) {
		KTROBO::mylog::writelog(filename, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;\n",
			mesh_offset_matrix[i]._11,mesh_offset_matrix[i]._12,mesh_offset_matrix[i]._13,mesh_offset_matrix[i]._14,
				mesh_offset_matrix[i]._21,mesh_offset_matrix[i]._22,mesh_offset_matrix[i]._23,mesh_offset_matrix[i]._24,
					mesh_offset_matrix[i]._31,mesh_offset_matrix[i]._32,mesh_offset_matrix[i]._33,mesh_offset_matrix[i]._34,
						mesh_offset_matrix[i]._41,mesh_offset_matrix[i]._42,mesh_offset_matrix[i]._43,mesh_offset_matrix[i]._44);
	}
	KTROBO::mylog::writelog(filename, "\n");


	KTROBO::mylog::writelog(filename, "}\n");
}




void AnimationBuilderMesh::write(char* filename, int impl_id) {
	
	KTROBO::mylog::writelog(filename, "IMPL_ID=%d\n", impl_id);
	KTROBO::mylog::writelog(filename, "MFILEPATH=\"%s\"\n", mesh_filepath);
	KTROBO::mylog::writelog(filename, "OFILEPATH=\"%s\"\n", oya_filepath);
	if (is_connect_without_material_local) {
		KTROBO::mylog::writelog(filename, "ICWML=1;\n");
	} else {
		KTROBO::mylog::writelog(filename, "ICWML=0;\n");
	}
	KTROBO::mylog::writelog(filename, "OMBNAME=\"%s\"\n", oya_mesh_bone_name);
	KTROBO::mylog::writelog(filename, "KMATRIX=");
	for (int i=0;i<16;i++) {
		KTROBO::mylog::writelog(filename, "%f,",matrix_kakeru.m[i/4][i%4]);
	}
	KTROBO::mylog::writelog(filename, ";\n");
}

bool AnimationBuilder::force_saveAnimeAndFrameToFile(int impl_id, char* filename) {
	// .anime  ファイル　と　.akat ファイルを吐き出す
	char animefile[128];
	char akatfile[128];
	memset(animefile,0,128);
	memset(akatfile,0,128);
	sprintf_s(animefile,128,"%s.anime",filename);
	sprintf_s(akatfile,128,"%s.akat",filename);
	FILE* animef;
	FILE* akatf;
	fopen_s(&animef,animefile, "w");
	if (animef != NULL) {
		//fclose(animef);
		return false;
	}
	fclose(animef);
	fopen_s(&akatf, akatfile, "w");
	if (akatf != NULL) {
//		fclose(animef);
		return false;
	}
	fclose(akatf);

	// 書き出しはじめ

	// animefile の書き込む

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	
	if (impl_id < 0 || impl_id >= impls.size()) {
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return false;
	}
	if (!impls[impl_id]->now_kakera) {
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return false;
	}
	KTROBO::mylog::writelog(animefile, "animsets{\n");
	AnimationBuilderImpl* im = impls[impl_id];
	int bone_num = im->now_kakera->mesh_bone_name_index.size();
	KTROBO::mylog::writelog(animefile, "%d;\n",bone_num);
	map<string,int>::iterator it = im->now_kakera->mesh_bone_name_index.begin();// ほかのkakeraでも変わらない
	while (it != im->now_kakera->mesh_bone_name_index.end()) {
		pair<string,int> p = *it;
		KTROBO::mylog::writelog(animefile, "anim{\n");
		KTROBO::mylog::writelog(animefile, "%s;\n",p.first);
		KTROBO::mylog::writelog(animefile, "keys{\n");
		int key_num = im->kakeras.size();
		int bone_index = p.second;

		for (int i=0;i<key_num;i++) {
			AnimationMeshKakera* ka = im->kakeras[i];
			KTROBO::mylog::writelog(animefile, "%d;\n",ka->frame);
			KTROBO::mylog::writelog(animefile, "matrix;0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;\n");
			KTROBO::mylog::writelog(animefile, "matrix_basis;");
			for (int k=0;k<16;k++) {
				KTROBO::mylog::writelog(animefile, "%f,",ka->mesh_offset_matrix[bone_index].m[k/4][k%4]);
			}
			KTROBO::mylog::writelog(animefile, ";\n");
		}
		KTROBO::mylog::writelog(animefile, "}\n");
		KTROBO::mylog::writelog(animefile, "}\n");
		it++;
	}


	CS::instance()->leave(CS_RENDERDATA_CS, "leave");


	// akat ファイルの書き込み
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	KTROBO::mylog::writelog(akatfile, "AKATS{\n");
	int n = im->animes.size();
	KTROBO::mylog::writelog(akatfile, "%d;",n);
	for (int i=0;i<n;i++) {
		KTROBO::mylog::writelog(akatfile, "AKAT{\n");
		im->animes[i]->write(akatfile,impl_id);
		KTROBO::mylog::writelog(akatfile, "}\n");
	}
	KTROBO::mylog::writelog(akatfile, "}\n");

	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return true;
}

bool  AnimationBuilder::saveNowToFile(char* filename) {
	// すべての今の状態を保存する
	FILE* fp;
	fopen_s(&fp, filename, "r");
	if (fp != NULL) {
		fclose(fp);
		return false;
	}
	return force_saveNowToFile(filename);
}

bool AnimationBuilder::forceLoadFromFile(char* filename) {

	deleteAll();

	MyTokenAnalyzer a;
	a.load(filename);
	while(!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "HON_MESH")==0) {
			a.GetToken("{");
			while (strcmp(a.Toke(), "}")!=0 && !a.enddayo()) {
				a.GetToken();
				if (strcmp(a.Toke(), "MFILEPATH")==0) {
					a.GetToken();
					CS::instance()->enter(CS_RENDERDATA_CS, "enter");
					this->createAnimationBuilderImpl(a.Toke());
					CS::instance()->leave(CS_RENDERDATA_CS, "leave");
				}
			}
		}
	}

	a.resetPointer();
	while(!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "ONAJI_MESH")==0) {
			a.GetToken("{");
			a.GetToken("IMPL_ID");
			int impl_id = a.GetIntToken();
			while (strcmp(a.Toke(), "}")!=0 && !a.enddayo()) {
				a.GetToken();
				if (strcmp(a.Toke(), "MFILEPATH")==0) {
					a.GetToken();
					CS::instance()->enter(CS_RENDERDATA_CS, "enter");
					this->setOnajiMesh(impl_id, a.Toke());
					CS::instance()->leave(CS_RENDERDATA_CS, "leave");
				}
			}
		}
	}
	a.resetPointer();
	while(!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "KO_MESH")==0) {
			a.GetToken("{");
			a.GetToken("IMPL_ID");
			int impl_id = a.GetIntToken();
			char mfilepath[128];
			char ofilepath[128];
			char obonename[128];
			bool icwml=false;
			memset(mfilepath,0,128);
			memset(ofilepath,0,128);
			memset(obonename,0,128);
			MYMATRIX matrix_kakeru;
			MyMatrixIdentity(matrix_kakeru);
			while (strcmp(a.Toke(), "}")!=0 && !a.enddayo()) {
				a.GetToken();

				if (strcmp(a.Toke(), "MFILEPATH")==0) {
					a.GetToken();
					hmystrcpy(mfilepath,128,0,a.Toke());
				}

				if (strcmp(a.Toke(), "OFILEPATH")==0) {
					a.GetToken();
					hmystrcpy(ofilepath,128,0,a.Toke());
				}

				if (strcmp(a.Toke(), "OMBNAME")==0) {
					a.GetToken();
					hmystrcpy(obonename,128,0,a.Toke());
				}

				
				if (strcmp(a.Toke(), "ICWML")==0) {
					int b = a.GetIntToken();
					if (b) {
						icwml = true;
					}
				}
				if (strcmp(a.Toke(), "KMATRIX")==0) {
					for (int i=0;i<16;i++) {
						matrix_kakeru.m[i/4][i%4] = a.GetFloatToken();
					}
				}
			}
			CS::instance()->enter(CS_RENDERDATA_CS, "enter");
			this->setKoMesh(impl_id, mfilepath,ofilepath,obonename, icwml, &matrix_kakeru);
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		}
	}

	a.resetPointer();
	while(!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "KAKERA")==0) {
			a.GetToken("{");
			a.GetToken("IMPL_ID");
			int impl_id = a.GetIntToken();
			a.GetToken("ROTTRANSISPARAM");
			a.GetToken("{");
			int rotxsize = a.GetIntToken();
			AnimationMeshKakera* kakera = new AnimationMeshKakera();
			for (int i=0;i<rotxsize;i++) {
				kakera->mesh_bone_rotx.push_back(a.GetFloatToken());
			}
			int rotysize = a.GetIntToken();
			for (int i=0;i<rotysize;i++) {
				kakera->mesh_bone_roty.push_back(a.GetFloatToken());
			}
			int rotzsize = a.GetIntToken();
			for (int i=0;i<rotzsize;i++) {
				kakera->mesh_bone_rotz.push_back(a.GetFloatToken());
			}
			int transxsize = a.GetIntToken();
			for (int i=0;i<transxsize;i++) {
				kakera->mesh_bone_transx.push_back(a.GetFloatToken());
			}
			int transysize = a.GetIntToken();
			for (int i=0;i<transysize;i++) {
				kakera->mesh_bone_transy.push_back(a.GetFloatToken());
			}
			int transzsize = a.GetIntToken();
			for (int i=0; i < transzsize; i++) {
				kakera->mesh_bone_transz.push_back(a.GetFloatToken());
			}

			int isrotxsize = a.GetIntToken();
			for (int i=0;i<isrotxsize;i++) {
				int t = a.GetIntToken();
				if (t) {
					kakera->mesh_bone_isrotx.push_back(true);
				} else {
					kakera->mesh_bone_isrotx.push_back(false);
				}
			}
			int isrotysize = a.GetIntToken();
			for (int i=0;i<isrotysize;i++) {
				int t = a.GetIntToken();
				if (t) {
					kakera->mesh_bone_isroty.push_back(true);
				} else {
					kakera->mesh_bone_isroty.push_back(false);
				}
			}
			int isrotzsize = a.GetIntToken();
			for (int i=0;i<isrotzsize;i++) {
				int t = a.GetIntToken();
				if (t) {
					kakera->mesh_bone_isrotz.push_back(true);
				} else {
					kakera->mesh_bone_isrotz.push_back(false);
				}
			}
			a.GetToken("}");
			a.GetToken("MESHFILEPATHNAME");
			a.GetToken();
			kakera->mesh_filepathname = string(a.Toke());
			a.GetToken("FRAME");
			kakera->frame = a.GetIntToken();
			a.GetToken("MESHBONENAMEINDEX");
			a.GetToken("{");
			int mn = a.GetIntToken();
			for (int i=0;i<mn;i++) {
				a.GetToken();
				string s = string(a.Toke());
				int inde = a.GetIntToken();
				kakera->mesh_bone_name_index.insert(pair<string,int>(s,inde));
			}
			a.GetToken("}");
			a.GetToken("DEFAULTFRAMEANDMATRIX");
			a.GetToken("{");
			mn = a.GetIntToken();
			for (int i=0;i<mn;i++) {
				int d = a.GetIntToken();
				kakera->mesh_bone_default_anime_frame.push_back(d);
			}
			mn = a.GetIntToken();
			for (int i=0;i<mn;i++) {
				MYMATRIX m;
				for (int k=0;k<16;k++) {
					
					m.m[k/4][k%4] = a.GetFloatToken();
				}
				kakera->mesh_offset_matrix.push_back(m);
			}
			a.GetToken("}");
			impls[impl_id]->kakeras.push_back(kakera);
			impls[impl_id]->frame_to_kakera_index.insert(pair<int,int>(kakera->frame, impls[impl_id]->kakeras.size()-1));

		}
	}

	// now_kakeraについてはhon_meshが読み込まれた後と同時にロードされるためロードファイルからは読み込まない
	// kakeras と animesについて
	// animesをロードする


	a.resetPointer();
	while(!a.enddayo()) {
		a.GetToken();
		if (strcmp(a.Toke(), "ANIME")==0) {
			a.GetToken("{");
			a.GetToken("IMPL_ID");
			int impl_id = a.GetIntToken();
			a.GetToken("ALLTIME");
			AnimationMesh* am = new AnimationMesh();
			am->all_time = a.GetFloatToken();
			a.GetToken("ANIME_INDEX");
			am->anime_index = a.GetIntToken();
			a.GetToken("ANIME_NAME");
			a.GetToken();
			am->anime_name = string(a.Toke());
			a.GetToken("IS_LOOP");
			int t = a.GetIntToken();
			if (t) {
				am->is_loop = true;
			} else {
				am->is_loop = false;
			}
			a.GetToken("FRAME_NUM");
			int fm = a.GetIntToken();
			for (int k=0;k<fm;k++) {
				AnimationMeshFrame* frame = new AnimationMeshFrame();
				a.GetToken("FRAME");
				a.GetToken("{");
				a.GetToken("FR");
				frame->frame = a.GetIntToken();
				a.GetToken("KI");
				frame->kakera_index = a.GetIntToken();
				
				a.GetToken("KAKERAFRAME");
				int kakeraframe = a.GetIntToken();
				frame->kakera = impls[impl_id]->kakeras[impls[impl_id]->frame_to_kakera_index[kakeraframe]];
				a.GetToken("FI");
				frame->time = a.GetFloatToken();
				a.GetToken("}");
				am->frames.push_back(frame);
			}
			impls[impl_id]->animes.push_back(am);

		}
	}




	a.deletedayo();
	return true;
}


bool  AnimationBuilder::loadFromFile(char* filename) {
	// すべての状態が保存されたファイルから状態を復元する現在のものは消してしまう
	if (impls.size()) return false;

	return forceLoadFromFile(filename);
	
	

}

bool  AnimationBuilder::saveAnimeAndFrameToFile(int impl_id, char* filename) {

	// .anime  ファイル　と　.akat ファイルを吐き出す
	char animefile[128];
	char akatfile[128];
	memset(animefile,0,128);
	memset(akatfile,0,128);
	sprintf_s(animefile,128,"%s.anime",filename);
	sprintf_s(akatfile,128,"%s.akat",filename);
	FILE* animef;
	FILE* akatf;
	fopen_s(&animef,animefile, "r");
	if (animef != NULL) {
		fclose(animef);
		return false;
	}
	fopen_s(&akatf, akatfile, "r");
	if (akatf != NULL) {
		fclose(akatf);
		return false;
	}

	return force_saveAnimeAndFrameToFile(impl_id, filename);

}

void  AnimationBuilder::deleteAll() {

	CS::instance()->enter(CS_RENDERDATA_CS, "renderdata");
	vector<AnimationBuilderImpl*>::iterator it = impls.begin();
	while(it != impls.end()) {

		AnimationBuilderImpl* i = *it;
		if (i) {
			i->Release();
			delete i;
			i = 0;
		}
		it++;
	}
	impls.clear();
	CS::instance()->leave(CS_RENDERDATA_CS, "renderdata");
}

float AnimationBuilder::getHonRotX(char* bone_name, float rotx) {

	if (strcmp(bone_name, "migiArmBone")==0) {
		if (rotx > 1.57) {
			rotx = 1.57;
		}
		if (rotx < -0.78) {
			rotx = -0.78;
		}
		
	}

	if (strcmp(bone_name, "migiSakotuBone")==0) {
		if (rotx > 1.57) {
			rotx = 1.57;
		}
		if (rotx < -1.57) {
			rotx = -1.57;
		}
		rotx = 0;
	}
	if (strcmp(bone_name,"migiArmSitaBone")==0) {
		if (rotx > 1.57) {
			rotx = 1.57;
		}
		if (rotx < -1.57) {
			rotx = -1.57;
		}
		rotx = 0;
	}



	return rotx;
}

float AnimationBuilder::getHonRotY(char* bone_name, float roty) {

	if (strcmp(bone_name, "migiArmBone")==0) {
		if (roty > 1.57) {
			roty = 1.57;
		}
		if (roty < -1.57) {
			roty = -1.57;
		}
		roty = 0;
	}

	if (strcmp(bone_name, "migiSakotuBone")==0) {
		if (roty > 1.57) {
			roty = 1.57;
		}
		if (roty < -1.57) {
			roty = -1.57;
		}
		roty = 0;
	}

	if (strcmp(bone_name,"migiArmSitaBone")==0) {
		if (roty > 1.57) {
			roty = 1.57;
		}
		if (roty < -1.57) {
			roty = -1.57;
		}
		roty = 0;
	}






	return roty;


}
float AnimationBuilder::getHonRotZ(char* bone_name, float rotz) {
		if (strcmp(bone_name, "migiArmBone")==0) {
		if (rotz > 1.57) {
			rotz = 1.57;
		}
		if (rotz < -0.78) {
			rotz = -0.78;
		}
	}

	if (strcmp(bone_name, "migiSakotuBone")==0) {
		if (rotz > 1.57) {
			rotz = 1.57;
		}
		if (rotz < -1.57) {
			rotz = -1.57;
		}
		rotz = 0;
	}

	if (strcmp(bone_name,"migiArmSitaBone")==0) {
		if (rotz > 1.57) {
			rotz = 1.57;
		}
		if (rotz < 0) {
			rotz = 0;
		}

	}



	return rotz;
}

void AnimationBuilder::hetareIK() {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size()) {
		
		static int test = 0;
	
		AnimationBuilderImpl *impl = impls[now_index];
		if (!strlen(impl->ik_bone_moto.c_str())) {
			return;
		}
		if (!strlen(impl->ik_bone_saki.c_str())) {
			return;
		}
	//	if (!test) {
//			MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0)->getRenderBillBoard(
		impl->now_kakera->setOffsetMatrixToMesh(impl->hon_mesh->mesh);
		MyIKMODOKI ik(impl->hon_mesh->mesh, &MYVECTOR3(0,-10,11),impl->ik_bone_moto.c_str(), impl->ik_bone_saki.c_str());
		int bsize = impl->hon_mesh->mesh->Bones.size();
		MeshBone* saki = impl->hon_mesh->mesh->Bones[impl->hon_mesh->mesh->BoneIndexes[impl->ik_bone_saki.c_str()]];
		MeshBone* moto = impl->hon_mesh->mesh->Bones[impl->hon_mesh->mesh->BoneIndexes[impl->ik_bone_moto.c_str()]];
		MeshBone* sakip = saki->parent_bone;
		MeshBone* bsakip = saki->parent_bone;
		MYMATRIX mat;
		MyMatrixMultiply(mat, saki->combined_matrix, saki->matrix_local);
		MYVECTOR3 vec(0,0,0);
		MyVec3TransformCoord(vec,vec,mat);
		vec.float3.z -= 10.0f;
		vec = MYVECTOR3(0,-15,11);
		ik.setMokuhyou(&vec);
		while (sakip && strcmp(bsakip->bone_name , moto->bone_name )!=0) {

			if (strcmp(bsakip->bone_name, "migiSakotuBone")==0) {
			} else if(strcmp(bsakip->bone_name, "migiArmBone")==0) {
				ik.setXFreeBone(sakip->bone_name);
			ik.setZFreeBone(sakip->bone_name);

			} else if(strcmp(bsakip->bone_name, "migiArmSitaBone")==0) {

				ik.setZFreeBone(sakip->bone_name);
			}else {
			ik.setXFreeBone(sakip->bone_name);
			ik.setYFreeBone(sakip->bone_name);
			ik.setZFreeBone(sakip->bone_name);
			}
			bsakip = sakip;
			sakip = sakip->parent_bone;
		}

		for (int s=0;s<6;s++) {
			impl->now_kakera->setOffsetMatrixToMesh(impl->hon_mesh->mesh);
			ik.updateStep();
			for (int i=0;i<bsize;i++) {

				MeshBone* bone = impl->hon_mesh->mesh->Bones[i];
				float drotx = ik.getdthetaXBone(bone->bone_name);
				float droty = ik.getdthetaYBone(bone->bone_name);
				float drotz = ik.getdthetaZBone(bone->bone_name);
				if (abs(drotx) > 0.0001f) {
				float nrotx = this->getHonMeshBoneRotX(now_index,i);
				float hon_rotx = this->getHonRotX(bone->bone_name,nrotx+drotx);
				this->setHonMeshBoneRotX(now_index, i, hon_rotx);
				}
				if (abs(droty) > 0.0001f) {
				float nroty = this->getHonMeshBoneRotY(now_index,i);
				float hon_roty = this->getHonRotY(bone->bone_name,nroty+droty);
				this->setHonMeshBoneRotY(now_index, i, hon_roty);
				}
				if (abs(drotz) > 0.0001f) {
				float nrotz = this->getHonMeshBoneRotZ(now_index,i);
				float hon_rotz = this->getHonRotZ(bone->bone_name, nrotz+drotz);
				this->setHonMeshBoneRotZ(now_index, i, hon_rotz);
				}
			}
			impl->now_kakera->setOffsetMatrixToMesh(impl->hon_mesh->mesh);
		}
		impl->setIsAnimate(false);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");


}
void AnimationMeshKakera::setOffsetMatrixToMesh(Mesh* mesh) {

	int bone_max = mesh->Bones.size();
	for (int i=0;i<bone_max;i++) {
		MeshBone* bn = mesh->Bones[i];
		float frame = this->mesh_bone_default_anime_frame[i];//this->frame;
		unsigned short ans_minmax;
		unsigned short ans_maxmin;
		float weight;
		mesh->getOffsetMatrixToGetMinMaxAndWeightIndex(bn, frame, &ans_minmax, &ans_maxmin, &weight);
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

		// 現在の姿勢のoffsetmatrixは計算できたので次は変化したあとのoffsetmatrixを求める
		// zxy の順にかける
		int bone_index = this->mesh_bone_name_index[bn->bone_name];
		MYMATRIX rotz;
	    MYMATRIX ans;
		MyMatrixRotationZ(rotz, this->mesh_bone_rotz[bone_index]);
		MYMATRIX rotx;
		MYVECTOR3 axisx(1,0,0);
//		MyVec3TransformNormal(axisx,axisx,rotz);
		MyMatrixRotationAxis(rotx, axisx,this->mesh_bone_rotx[bone_index]);
//		MyMatrixRotationX(rotx, this->mesh_bone_rotx[bone_index]);
		MyMatrixMultiply(ans, rotz, rotx);

		MYMATRIX roty;
		MYVECTOR3 axisy(0,1,0);
//		MyVec3TransformNormal(axisy,axisy,ans);
		MyMatrixRotationAxis(roty, axisy,this->mesh_bone_roty[bone_index]);

		//MyMatrixRotationY(roty, this->mesh_bone_roty[bone_index]);
		MYMATRIX trans;
		MyMatrixTranslation(trans, this->mesh_bone_transx[bone_index],  this->mesh_bone_transy[bone_index], 
			 this->mesh_bone_transz[bone_index] );
	
		MyMatrixMultiply(ans, rotz, rotx);
		MyMatrixMultiply(ans, ans, roty);
		MyMatrixMultiply(ans, ans, trans);
		MyMatrixMultiply(ans, ans, mat3);
		this->mesh_offset_matrix[bone_index] = ans;
		bn->offset_matrix = mesh_offset_matrix[bone_index];
	}
	mesh->animate(0,false);
}


void AnimationMeshKakera::copy(AnimationMeshKakera* kakera_moto) {
	// コピー元からコピーする
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		clear();
		mesh_bone_rotx = kakera_moto->mesh_bone_rotx;
		mesh_bone_roty = kakera_moto->mesh_bone_roty;
		mesh_bone_rotz = kakera_moto->mesh_bone_rotz;
		mesh_bone_transx = kakera_moto->mesh_bone_transx;
		mesh_bone_transy = kakera_moto->mesh_bone_transy;
		mesh_bone_transz = kakera_moto->mesh_bone_transz;
		mesh_bone_isrotx = kakera_moto->mesh_bone_isrotx;
		mesh_bone_isroty = kakera_moto->mesh_bone_isroty;
		mesh_bone_isrotz = kakera_moto->mesh_bone_isrotz;
		mesh_bone_name_index = kakera_moto->mesh_bone_name_index;
		mesh_bone_default_anime_frame = kakera_moto->mesh_bone_default_anime_frame;
		mesh_offset_matrix = kakera_moto->mesh_offset_matrix;
		this->mesh_filepathname = kakera_moto->mesh_filepathname;
	//	this->frame = kakera_moto->frame;

	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}


 bool kurukuru::handleMessage(int msg, void* data, DWORD time) {

	 a = 0.01f;
	 CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	 if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE) {
		 MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*)data;
		 if (input->getMOUSESTATE()->mouse_r_button_pressed) {
	 MYMATRIX s;
	 MyMatrixRotationZ(s,a);

	 MYVECTOR3 fromat = from - at;
	 MyVec3TransformNormal(fromat,fromat,s);
	 from = at+fromat;

	 MyMatrixLookAtRH(view,from,at,up);
		 }
	 }
	 AnimationBuilderImpl* imp = ab->getNowImpl();
	 if (imp) {
		 imp->setIsAnimate(false);
	 }
	 if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		 MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*)data;
		 unsigned char* keystate = input->getKEYSTATE();
		 if (keystate[VK_TAB] & KTROBO_INPUT_BUTTON_DOWN) {
			 int size = ab->getNowImpl()->hon_mesh->mesh->Bones.size();
			 if (size) {
				 ab->setNowBoneIndex((ab->getNowBoneIndex()+1) % size);
				// MyLuaGlueSingleton::getInstance()->getColMessages(0)->getInstance(0)->makeMessage(KTROBO_MESSAGE_ID_ANIMATIONBUILDER_BONE_PUSHED, KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, ab->getNowBoneIndex(),0, true);
				 LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,true, "resrc/script/AB_bonepushed.lua");
			 }

		 }

		 if (keystate['Q'] & KTROBO_INPUT_BUTTON_DOWN) {
			  int size = ab->getNowImpl()->hon_mesh->mesh->Bones.size();
			 if (size) {
				 ab->getNowImpl()->ik_bone_moto = string(ab->getNowImpl()->hon_mesh->mesh->Bones[ab->getNowBoneIndex()]->bone_name);
				 		 Texture* te = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
						 te->setRenderBillBoardColor(ab->bone_bills[ab->getNowBoneIndex()], 0xFF0000FF);
				
				// MyLuaGlueSingleton::getInstance()->getColMessages(0)->getInstance(0)->makeMessage(KTROBO_MESSAGE_ID_ANIMATIONBUILDER_BONE_PUSHED, KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, ab->getNowBoneIndex(),0, true);
			//	 LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,true, "resrc/script/AB_bonepushed.lua");
			 }
		 }
	 } else if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		 MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*)data;

		

		 if ((input->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_DOWN)&& !(input->getKEYSTATE()[VK_LSHIFT] & KTROBO_INPUT_BUTTON_PRESSED)) {
			 // ボタンが押されたので
			 MYMATRIX mat;
			 char bb[512];
			 WCHAR buf[512];
			 stringconverter sc;
			 sprintf_s(bb,512, "%d, %d",input->getMOUSESTATE()->mouse_x, input->getMOUSESTATE()->mouse_y);
			 sc.charToWCHAR(bb,buf);
		//	 DebugTexts::instance()->setText(ab->gs[TASKTHREADS_AIDECISION],wcslen(buf), buf);
			 MyMatrixMultiply(mat,view, ab->proj);
			// mat = view;
			 bool is_bone_osareta = false;
			 for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
				
				 MYVECTOR3 pos;
				 MyVec3TransformCoord(pos,this->ab->bone_poss[i],mat);
				 float posx = 0 + (pos.float3.x+1) * this->ab->gs[0]->getScreenWidth()/2;
				 float posy = this->ab->gs[0]->getScreenHeight() - (pos.float3.y+1) * this->ab->gs[0]->getScreenHeight()/2;
				 sprintf_s(bb,512, "%f, %f",posx, posy);
				 sc.charToWCHAR(bb,buf);
				 if (i < 16) {
			//	 DebugTexts::instance()->setText(ab->gs[TASKTHREADS_AIDECISION],wcslen(buf), buf);
				 }
				 if ((posx - input->getMOUSESTATE()->mouse_x)*(posx-input->getMOUSESTATE()->mouse_x) +
					 (posy - input->getMOUSESTATE()->mouse_y)*(posy-input->getMOUSESTATE()->mouse_y) < 25*25) {
						 // ぼねおされた
						 ab->setNowBoneIndex(i);
						 is_bone_osareta = true;
						 
				 }

			 }
			
			 if (is_bone_osareta) {
			//	 MyLuaGlueSingleton::getInstance()->getColMessages(0)->getInstance(0)->makeMessage(KTROBO_MESSAGE_ID_ANIMATIONBUILDER_BONE_PUSHED, KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, ab->getNowBoneIndex(),0, true);
				 LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,true, "resrc/script/AB_bonepushed.lua");
			 }


			






		 }





		 if (input->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_DOWN) {
			  MYMATRIX mat;
			  MyMatrixMultiply(mat,view, ab->proj);
			  for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
				
				 MYVECTOR3 pos;
				 MyVec3TransformCoord(pos,this->ab->bone_poss[i],mat);
				 float posx = 0 + (pos.float3.x+1) * this->ab->gs[0]->getScreenWidth()/2;
				 float posy = this->ab->gs[0]->getScreenHeight() - (pos.float3.y+1) * this->ab->gs[0]->getScreenHeight()/2;
				 if ((posx - input->getMOUSESTATE()->mouse_x)*(posx-input->getMOUSESTATE()->mouse_x) +
					 (posy - input->getMOUSESTATE()->mouse_y)*(posy-input->getMOUSESTATE()->mouse_y) < 25*25) {
						 // ぼねおされた
						 imp->ik_bone_moto = string(imp->hon_mesh->mesh->Bones[i]->bone_name);
						 // 色を変える
						 Texture* te = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);

						 for (int k=0;k<KTROBO_MESH_BONE_MAX;k++) {
							 te->setRenderBillBoardColor(ab->bone_bills[k], 0xFFFFFFFF);
						 }
						 te->setRenderBillBoardColor(ab->bone_bills[i],0xFF0000FF);						 
				 }

			 }
		 }

		 if ((input->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_DOWN) && (input->getKEYSTATE()[VK_LSHIFT] & KTROBO_INPUT_BUTTON_PRESSED)) {
			 	  MYMATRIX mat;
			  MyMatrixMultiply(mat,view, ab->proj);
			  for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
				
				 MYVECTOR3 pos;
				 MyVec3TransformCoord(pos,this->ab->bone_poss[i],mat);
				 float posx = 0 + (pos.float3.x+1) * this->ab->gs[0]->getScreenWidth()/2;
				 float posy = this->ab->gs[0]->getScreenHeight() - (pos.float3.y+1) * this->ab->gs[0]->getScreenHeight()/2;
				 if ((posx - input->getMOUSESTATE()->mouse_x)*(posx-input->getMOUSESTATE()->mouse_x) +
					 (posy - input->getMOUSESTATE()->mouse_y)*(posy-input->getMOUSESTATE()->mouse_y) < 25*25) {
						 // ぼねおされた
						 imp->ik_bone_saki = string(imp->hon_mesh->mesh->Bones[i]->bone_name);
						 // 色を変える
						 Texture* te = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
						 te->setRenderBillBoardColor(ab->bone_bills[i],0x0000FFFF);						 
				 }

			 }
		 }












	 }
	 CS::instance()->leave(CS_RENDERDATA_CS, "leave");

	 return true;
 }

 void AnimationBuilder::enter()
{
	LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, "resrc/script/AB_enter.lua");
	kuru->setAB(this);
	Scene::enter();
	InputMessageDispatcher::registerImpl(kuru,NULL,NULL);
	bone_index = 0;
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	int tex_id = tex->getTexture("resrc/sample/none.png",1024);
	for (int i=0;i < KTROBO_MESH_BONE_MAX; i++) {
		bone_poss[i] = MYVECTOR3(0,0,0);
		MYMATRIX world;
		MyMatrixIdentity(world);
		bone_bills[i] = tex->getRenderBillBoard(tex_id, 0xFFFFFFFF,&world, 1.0f,1.0f,0,0,1,1);
	}
 }

 
float AnimationBuilder::getHonMeshBoneRotX(int impl_id, int bone_index) {
	float rotx = 0;
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
	
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				rotx = impl->now_kakera->mesh_bone_rotx[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return rotx;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return rotx;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return rotx;

}
float AnimationBuilder::getHonMeshBoneRotY(int impl_id, int bone_index) {
	float roty = 0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				roty = impl->now_kakera->mesh_bone_roty[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return roty;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return roty;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return roty;


}
float AnimationBuilder::getHonMeshBoneRotZ(int impl_id, int bone_index) {
		float rotz = 0;
			CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
	
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				rotz = impl->now_kakera->mesh_bone_rotz[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return rotz;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return rotz;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return rotz;


}
bool AnimationBuilder::getHonMeshBoneRotXIsChange(int impl_id, int bone_index) {
	// falseにすると回転が0になりIKでも回転しなくなる
	bool isrotx = false;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				isrotx = impl->now_kakera->mesh_bone_isrotx[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return isrotx;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return isrotx;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return isrotx;
}	
	
	
	
bool AnimationBuilder::getHonMeshBoneRotYIsChange(int impl_id, int bone_index) {

	bool isroty = false;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				isroty = impl->now_kakera->mesh_bone_isroty[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return isroty;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return isroty;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return isroty;


}

bool AnimationBuilder::getHonMeshBoneRotZIsChange(int impl_id, int bone_index) {

	bool isrotz = false;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
		
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				isrotz = impl->now_kakera->mesh_bone_isrotz[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return isrotz;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return isrotz;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return isrotz;


}
float AnimationBuilder::getHonMeshBoneTransX(int impl_id, int bone_index) {
		float transx = 0;
			CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
	
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				transx = impl->now_kakera->mesh_bone_transx[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return transx;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return transx;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return transx;


}
float AnimationBuilder::getHonMeshBoneTransY(int impl_id, int bone_index) {

		float transy = 0;
			CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
	
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				transy = impl->now_kakera->mesh_bone_transy[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return transy;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return transy;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return transy;



}

float AnimationBuilder::getHonMeshBoneTransZ(int impl_id, int bone_index) {
	float transz = 0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > impl_id && impl_id >=0) {
		
	
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				transz = impl->now_kakera->mesh_bone_transz[bone_index];
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return transz;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return transz;
	}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return transz;



}


 void AnimationBuilder::leave()
{
	InputMessageDispatcher::unregisterImpl(kuru);
	Scene::leave();
	LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, "resrc/script/AB_leave.lua");
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	for (int i=0;i < KTROBO_MESH_BONE_MAX; i++) {
		tex->lightdeleteRenderBillBoard(bone_bills[i]);//(tex_id, 0xFFFFFFFF,&world, 1.0f,1.0f,0,0,1,1);
	}
}

int AnimationBuilder::getNowIMPLIndex() {
	int index=0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	index = now_index;
	CS::instance()->leave(CS_RENDERDATA_CS, "enter");
	return index;
}
void AnimationBuilder::mainrenderIMPL(bool is_focused, Graphics* g, Game* game) {
	
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");

	MYMATRIX world;
	MyMatrixIdentity(world);
	int num = impls.size();
	if (num) {

		if (impls[now_index]->hon_mesh->mesh_loaded) {
			this->view = kuru->view;

			impls[now_index]->hon_mesh->mesh->draw(g, &world, &view, &proj);
		}

		int on = impls[now_index]->onaji_mesh.size();
		for (int n = 0;n<on;n++) {
			AnimationBuilderMesh* mm = impls[now_index]->onaji_mesh[n];
			if (mm->mesh_loaded) {
				mm->mesh->draw(g, &world, &view, &proj);
			}
		}

		int on2 = impls[now_index]->ko_mesh.size();
		for (int n=0;n<on2;n++) {
			AnimationBuilderMesh* mm = impls[now_index]->ko_mesh[n];
			if (mm->mesh_loaded) {
				mm->mesh->draw(g, &world, &view, &proj);
			}
		}
	}

	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}


void AnimationBuilder::renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {

	// このスレッドでは
	// アニメの更新を行う
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");

	int num = impls.size();
	if (num) {
		
		if (impls[now_index]->hon_mesh->mesh_loaded && !impls[now_index]->hon_mesh->is_animated) {
			impls[now_index]->now_kakera->setOffsetMatrixToMesh(impls[now_index]->hon_mesh->mesh);
			impls[now_index]->hon_mesh->is_animated = true;
			MYMATRIX mat;
			MYVECTOR3 po(0,0,0);
			MYMATRIX ansmat;
			Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
			int bsize = impls[now_index]->hon_mesh->mesh->Bones.size();
			for (int i=0;i<bsize; i++) {
				MYVECTOR3 ans;
				MyMatrixMultiply(mat, impls[now_index]->hon_mesh->mesh->Bones[i]->matrix_local,impls[now_index]->hon_mesh->mesh->Bones[i]->combined_matrix);
				MyVec3TransformCoord(ans, po, mat);
				MyMatrixTranslation(ansmat,ans.float3.x, ans.float3.y,ans.float3.z);
				bone_poss[i] = ans;
				tex->setRenderBillBoardPos(bone_bills[i],&ansmat);
			}

			for (int i =bsize; i < KTROBO_MESH_BONE_MAX;i++) {
				MyMatrixTranslation(ansmat,0,0,100);
				tex->setRenderBillBoardPos(bone_bills[i], &ansmat);
				bone_poss[i] = MYVECTOR3(0,0,100);
			}
			CS::instance()->leave(CS_RENDERDATA_CS, "enter");
			CS::instance()->enter(CS_DEVICECON_CS,"enter");
			CS::instance()->enter(CS_RENDERDATA_CS, "enter");
			tex->setViewProj(g,&kuru->view,&proj,&kuru->from,&kuru->at);
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			CS::instance()->leave(CS_DEVICECON_CS, "leave");
			CS::instance()->enter(CS_RENDERDATA_CS,"enter");

		}

		int on = impls[now_index]->onaji_mesh.size();
		for (int n = 0;n<on;n++) {
			AnimationBuilderMesh* mm = impls[now_index]->onaji_mesh[n];
			if (mm->mesh_loaded && !mm->is_animated) {
				impls[now_index]->now_kakera->setOffsetMatrixToMesh(mm->mesh);
				mm->is_animated = true;
			}
		}

		int on2 = impls[now_index]->ko_mesh.size();
		for (int n=0;n<on2;n++) {
			AnimationBuilderMesh* mm = impls[now_index]->ko_mesh[n];
			if (mm->mesh_loaded && !mm->is_animated) {
				//impls[now_index]->now_kakera->setOffsetMatrixToMesh(mm->mesh);
				mm->mesh->animate(0,true);
				mm->is_animated = true;
			}
		}
	//	impls[now_index]->setIsAnimate(true);
	}

	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}



void AnimationBuilder::aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {





}


void AnimationBuilder::posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {




}


void AnimationBuilder::loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	int im = impls.size();
	if (im) {
		for (int i=0;i<im;i++) {
			AnimationBuilderImpl* ii = impls[i];
			if (ii->hon_mesh) {
				if (!ii->hon_mesh->mesh_loaded) {
					Mesh* m = ii->hon_mesh->mesh;
					char* meshname = ii->hon_mesh->mesh_meshpath;
					char* animename = ii->hon_mesh->mesh_animepath;
					CS::instance()->leave(CS_RENDERDATA_CS, "leave");
					m->readMesh(g, meshname, loader);
					m->readAnime(animename);
					CS::instance()->enter(CS_RENDERDATA_CS, "enter");

					/// TODO now_kakera を作る
					if (!ii->now_kakera) {
						ii->now_kakera = new AnimationMeshKakera();
						int bone_size = ii->hon_mesh->mesh->Bones.size();
						for (int k = 0; k < bone_size; k++) {
							MeshBone* bn = ii->hon_mesh->mesh->Bones[k];
							ii->now_kakera->mesh_bone_name_index.insert(pair<string,int>(bn->bone_name,k));
							ii->now_kakera->mesh_bone_isrotx.push_back(true);
							ii->now_kakera->mesh_bone_isroty.push_back(true);
							ii->now_kakera->mesh_bone_isrotz.push_back(true);
							ii->now_kakera->mesh_bone_rotx.push_back(0);
							ii->now_kakera->mesh_bone_roty.push_back(0);
							ii->now_kakera->mesh_bone_rotz.push_back(0);
							ii->now_kakera->mesh_bone_transx.push_back(0);
							ii->now_kakera->mesh_bone_transy.push_back(0);
							ii->now_kakera->mesh_bone_transz.push_back(0);
							ii->now_kakera->mesh_filepathname = string(ii->hon_mesh->mesh_filepath);
							ii->now_kakera->frame=0;
							ii->now_kakera->mesh_bone_default_anime_frame.push_back(0);
							MYMATRIX mat;
							MyMatrixIdentity(mat);
							ii->now_kakera->mesh_offset_matrix.push_back(mat);
						}
					}

					ii->hon_mesh->mesh_loaded = true;
					CS::instance()->leave(CS_RENDERDATA_CS, "leave");
					MyLuaGlueSingleton::getInstance()->getColMessages(0)->getInstance(0)->makeMessage(
						KTROBO_MESSAGE_ID_ANIMATIONBUILDER_HON_MESH_AFTER,KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, this->now_index,0,true);
					LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,true, "resrc/script/AB_madeHonMeshAfter.lua");
					CS::instance()->enter(CS_RENDERDATA_CS, "enter");
				}
			}

			int onaji_meshsize = ii->onaji_mesh.size();
			if (onaji_meshsize) {
				for (int k=0;k<onaji_meshsize;k++) {
					AnimationBuilderMesh* mm = ii->onaji_mesh[k];
					if (!mm->mesh_loaded) {

						Mesh* m = mm->mesh;
						char* meshname = mm->mesh_meshpath;
						char* animename = mm->mesh_animepath;
						CS::instance()->leave(CS_RENDERDATA_CS, "leave");
						m->readMesh(g, meshname, loader);
						m->readAnime(animename);
						CS::instance()->enter(CS_RENDERDATA_CS, "enter");
						mm->mesh_loaded = true;
					}
				}
			}

			int ko_meshsize = ii->ko_mesh.size();
			if (ko_meshsize) {
				for (int k=0;k<ko_meshsize;k++) {
					AnimationBuilderMesh* mm = ii->ko_mesh[k];
					if (!mm->mesh_loaded) {

						Mesh* m = mm->mesh;
						char* meshname = mm->mesh_meshpath;
						char* animename = mm->mesh_animepath;
						MYMATRIX kakeru = mm->matrix_kakeru;
						AnimationBuilderMesh* oya_mm = ii->getOyaMesh(mm->oya_filepath);
						if (!oya_mm->mesh_loaded) continue;
							
						CS::instance()->leave(CS_RENDERDATA_CS, "leave");
						m->readMesh(g, meshname, loader);
						m->readAnime(animename);
					
						CS::instance()->enter(CS_RENDERDATA_CS, "enter");
						m->RootBone->parent_bone = oya_mm->mesh->Bones[oya_mm->mesh->BoneIndexes[mm->oya_mesh_bone_name]];
						m->RootBone_connect_without_material_local = mm->is_connect_without_material_local;
						m->rootbone_matrix_local_kakeru = kakeru;
						mm->mesh_loaded = true;
					}
				}
			}
		}
	}

	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}
void AnimationBuilder::setNowBoneIndex(int index) {
	this->bone_index = index;
	Texture* te = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
	te->setRenderBillBoardColor(bone_bills[i],0xFFFFFFFF);
	}

	te->setRenderBillBoardColor(bone_bills[bone_index],0xFFFF00FF);

}


void AnimationBuilder::setNowIMPLIndex(int index) {
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size() > index && index >=0) {
		now_index = index;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}


int AnimationBuilder::getNowBoneIndex() {
	int bonee = 0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (impls.size()) {
		bonee = bone_index;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return bonee;
}


int AnimationBuilders::makeInst() {
	if (animebs.size()) return 0;

	AnimationBuilder* ab = new AnimationBuilder("animeb",6,loader);
	animebs.push_back(ab);
	return animebs.size()-1;
}

AnimationBuilder* AnimationBuilders::getInstance(int index) {
		if (index >= 0 && index < animebs.size()) {
			return animebs[index];
		}
		if (index ==0) {
			makeInst();
			return getInstance(0);
		}
		
		throw new GameError(KTROBO::WARNING, "no ab");
}

IAnimationBuilder* AnimationBuilders::getInterface(int index) {
		if (index >= 0 && index < animebs.size()) {
			return animebs[index];
		}
		if (index ==0) {
			makeInst();
			return getInterface(0);
		}
		throw new GameError(KTROBO::WARNING, "no ab");
}


void AnimationBuilderImpl::setIsAnimate(bool t) {

	if (hon_mesh) {
		hon_mesh->is_animated = t;
	}

	int osize = onaji_mesh.size();
	for (int i=0;i<osize;i++) {
		onaji_mesh[i]->is_animated = t;
	}

	int xsize = ko_mesh.size();
	for (int i=0;i<xsize; i++) {
		ko_mesh[i]->is_animated = t;
	}
}
