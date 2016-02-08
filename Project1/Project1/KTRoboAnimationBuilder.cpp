#include "KTRoboAnimationBuilder.h"
#include "KTRoboCS.h"
#include "MyTokenAnalyzer.h"


using namespace KTROBO;

AnimationBuilder::AnimationBuilder(char* n,int len) : Scene(n,len)
{
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

void  AnimationBuilder::setKoMesh(int impl_id, char* ko_filepath, char* oya_filepath, char* parent_bone_name, bool is_connect_without_material_local) {
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
		mesh->setOyaMesh(oya_filepath, parent_bone_name, is_connect_without_material_local);
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
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isrotx[bone_index]) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_rotx[bone_index] = rotx;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
}

void  AnimationBuilder::setHonMeshBoneRotY(int impl_id, int bone_index, float roty) {
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isroty[bone_index]) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_roty[bone_index] = roty;
			
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
}


void  AnimationBuilder::setHonMeshBoneRotZ(int impl_id, int bone_index, float rotz) {
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera&& impl->now_kakera->mesh_bone_isrotz[bone_index]) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_rotz[bone_index] = rotz;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}

}
void  AnimationBuilder::setHonMeshBoneRotXIsChange(int impl_id, int bone_index, bool t) {
	// falseにすると回転が0になりIKでも回転しなくなる
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_rotx[bone_index] = 0;
				impl->now_kakera->mesh_bone_transx[bone_index] = 0;
				impl->now_kakera->mesh_bone_isrotx[bone_index] = t;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
}
	
	
	
void  AnimationBuilder::setHonMeshBoneRotYIsChange(int impl_id, int bone_index, bool t) {
	// falseにすると回転が0になりIKでも回転しなくなる
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_roty[bone_index] = 0;
				impl->now_kakera->mesh_bone_transy[bone_index] = 0;
				impl->now_kakera->mesh_bone_isroty[bone_index] = t;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}

}


void  AnimationBuilder::setHonMeshBoneRotZIsChange(int impl_id, int bone_index, bool t) {
	// falseにすると回転が0になりIKでも回転しなくなる
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_rotz[bone_index] = 0;
				impl->now_kakera->mesh_bone_transz[bone_index] = 0;
				impl->now_kakera->mesh_bone_isrotz[bone_index] = t;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}

}


void  AnimationBuilder::setHonMeshBoneTransX(int impl_id, int bone_index, float dx) {
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isrotx[bone_index]) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_transx[bone_index] = dx;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}

}

void  AnimationBuilder::setHonMeshBoneTransY(int impl_id, int bone_index, float dy) {
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isrotx[bone_index]) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_transy[bone_index] = dy;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
}

void  AnimationBuilder::setHonMeshBoneTransZ(int impl_id, int bone_index, float dz) {
	if (impls.size() > impl_id && impl_id >=0) {
		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
		if (impl->hon_mesh->mesh->Bones.size() > bone_index && bone_index >=0) {
			if (impl->now_kakera && impl->now_kakera->mesh_bone_isrotx[bone_index]) {
				// ロードされているので設定する
				impl->now_kakera->mesh_bone_transz[bone_index] = dz;
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}
}


void  AnimationBuilder::setAnimePoseFrame(int impl_id, int frame) {
	// 現在のとっている姿勢を指定したアニメフレームとして保存する
	if (impls.size() > impl_id && impl_id >=0) {		
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		AnimationBuilderImpl *impl = impls[impl_id];
			if (impl->now_kakera) {
			AnimationMeshKakera* new_kakera = new AnimationMeshKakera();
			new_kakera->copy(impl->now_kakera);
			new_kakera->frame = frame;
			int size = impl->kakeras.size();
			impl->kakeras.push_back(new_kakera);
			impl->frame_to_kakera_index.insert(pair<int,int>(frame,size));

			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			
			return;
			}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return;
	}	
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
			}
			CS::instance()->enter(CS_RENDERDATA_CS, "enter");
			this->setKoMesh(impl_id, mfilepath,ofilepath,obonename, icwml);
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

	forceLoadFromFile(filename);
	


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

	force_saveAnimeAndFrameToFile(impl_id, filename);

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



void AnimationMeshKakera::setOffsetMatrixToMesh(Mesh* mesh) {





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
		this->frame = kakera_moto->frame;

	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}