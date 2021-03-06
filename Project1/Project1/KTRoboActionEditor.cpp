#include "KTRoboActionEditor.h"
#include "KTRoboLuaCollection.h"
#include "tolua_glue/tolua_glue.h"
#include "MyTokenAnalyzer.h"


using namespace KTROBO;
ActionEditor::ActionEditor(void) : Scene("action_editor", 13) 
{
	akat_play_time = 0;
	anime_action = false;

	action_play_time = 0;
	anime_akat = false;
	now_character_index = 0;
	now_character_text = 0;

	kuru = new ae_kurukuru();


	do_force_save = false;
	save_done = true;
	do_force_load = false;
	load_done = true;
	save_result = true;
	load_result = true;
}


ActionEditor::~ActionEditor(void)
{
	if (kuru) {
		delete kuru;
		kuru = 0;
	}
}

ActionCharacter* ActionEditor::getNowImpl() {

	if (characters.size()) {
		return characters[now_character_index];
	}

	return 0;
}


 bool ae_kurukuru::handleMessage(int msg, void* data, DWORD time) {

	 a = 0.01f;
	 CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	 if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSEMOVE ) {
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
	 if (!ae->getNowImpl()) {
			 CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			 return true;
     }

	 ActionCharacter* ac = ae->getNowImpl();

	 if (msg == KTROBO_INPUT_MESSAGE_ID_KEYDOWN) {
		 MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*)data;
		 unsigned char* keystate = input->getKEYSTATE();
		 if (keystate[VK_TAB] & KTROBO_INPUT_BUTTON_DOWN) {
		
		 }

		 if (keystate['Q'] & KTROBO_INPUT_BUTTON_DOWN) {
			
		 }
	 } else if (msg == KTROBO_INPUT_MESSAGE_ID_MOUSERAWSTATE) {
		 MYINPUTMESSAGESTRUCT* input = (MYINPUTMESSAGESTRUCT*)data;

		

		 if ((input->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_DOWN)&& !(input->getKEYSTATE()[VK_LSHIFT] & KTROBO_INPUT_BUTTON_PRESSED)) {
			 // ボタンが押されたので
			 MYMATRIX mat;
			 char bb[512];
			 WCHAR buf[512];
			 stringconverter sc;
	


		 }





		 if (input->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_R_DOWN) {
		 }

		 if ((input->getMOUSESTATE()->mouse_button & KTROBO_MOUSESTATE_L_DOWN) && (input->getKEYSTATE()[VK_LSHIFT] & KTROBO_INPUT_BUTTON_PRESSED)) {
		 }

	 }
	 CS::instance()->leave(CS_RENDERDATA_CS, "leave");

	 return true;
 }

void ActionEditor::enter() {
	akat_play_time = 0;
	anime_action = false;

	action_play_time = 0;
	anime_akat = false;
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	now_character_index = 0;
	now_character_text = tex->getRenderText("defaultcharacter",0,0,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT,
		KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT*32,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT);
	LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, "resrc/script/AE_enter.lua");
	Scene::enter();
	kuru->setAE(this);
	InputMessageDispatcher::registerImpl(kuru,NULL,NULL);
}

void ActionEditor::leave() {
	action_play_time = 0;
	anime_action = false;
	InputMessageDispatcher::unregisterImpl(kuru);
	akat_play_time = 0;
	anime_akat = false;
	Scene::leave();
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	now_character_index = 0;
	if (now_character_text) {
		tex->lightdeleteRenderText(now_character_text);
		now_character_text = 0;
	}
	LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, "resrc/script/AE_leave.lua");
}

void ActionEditor::mainrenderIMPL(bool is_focused, Graphics* g, Game* game) {

	MYMATRIX world;
	MyMatrixIdentity(world);
	OBB rec;

	Graphics::drawOBB(g,0xFFFFFFFF,&world, &kuru->view, g->getProj(), &rec);

}
void CharacterMesh::setSiseiAkat(int max_frame, float now_frame) {

	// 一番近いフレームかけらを見つけてくる
	CharacterMeshSkeleton* skel  = this->skeletons[this->now_skeleton];
	Akat* aka = skel->akats[skel->now_akat_index];
	setSiseiAkatSitei(aka, max_frame, now_frame);

}
void CharacterMesh::setSiseiAkatSitei(Akat* aka, int max_frame, float now_frame) {



	// 一番近いフレームかけらを見つけてくる
	CharacterMeshSkeleton* skel  = this->skeletons[aka->skeleton_index];
//	Akat* aka = skel->akats[skel->now_akat_index];
	float fframe = now_frame/(float)max_frame * aka->all_frame;

	AkatFrame* frame = aka->root_akat_frame;
	if (!frame) return;

	AkatFrame* min_frame = aka->root_akat_frame;
	AkatFrame* maxx_frame = aka->root_akat_frame;
	while(frame != 0) {
		if (frame->akat_frame < fframe) {
			min_frame = frame;
			frame = frame->right;

		} else if(frame->akat_frame > fframe) {
			frame = frame->left;
		} else {
			min_frame = frame;
			break;
		}
	}
	
	frame = aka->root_akat_frame;
	while(frame != 0) {
		if (frame->akat_frame < fframe) {
			frame = frame->right;

		} else if(frame->akat_frame > fframe) {
			maxx_frame = frame;
			frame = frame->left;
		} else {
			maxx_frame = frame;
			break;
		}
	}

	float now_frame_max = maxx_frame->akat_frame;
	float now_frame_min = min_frame->akat_frame;


	
	float weightb = now_frame_max - now_frame_min;
	float weight;
	if(weightb ==0) {
		weight = 1;
	}else {
	weight = (float)(now_frame_max - fframe)/(float)weightb;
	}
	
	float instance_frame_max = maxx_frame->mesh_instanced_frame;
	float instance_frame_min = min_frame->mesh_instanced_frame;

	// 各meshinstancedに適用させる
	int msize = mesh_instanceds.size();
	unsigned short bones_anime_first_index[KTROBO_MESH_BONE_MAX];
	unsigned short bones_anime_last_index[KTROBO_MESH_BONE_MAX];
	float bones_anime_first_weight[KTROBO_MESH_BONE_MAX];
	for (int i=0;i<KTROBO_MESH_BONE_MAX;i++) {
		bones_anime_first_index[i] = instance_frame_min;
		bones_anime_last_index[i] = instance_frame_max;
		bones_anime_first_weight[i] = weight;
	}
	for (int i=0;i<msize;i++) {
		MeshInstanced* mmm = mesh_instanceds[i];
		mmm->setBoneIndexInfo(bones_anime_first_index,bones_anime_last_index,bones_anime_first_weight);
	}
}




void ActionEditor::renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	int num = characters.size();
	if (num) {

		MeshInstanceds* is = MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(0);
		MYVECTOR4 lightdir(0,0,-1,0);
		
		
			CS::instance()->leave(CS_RENDERDATA_CS, "enter");
			CS::instance()->enter(CS_DEVICECON_CS,"enter");
			CS::instance()->enter(CS_RENDERDATA_CS, "enter");
			is->setViewProj(g,&kuru->view,g->getProj(),&lightdir);
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			CS::instance()->leave(CS_DEVICECON_CS, "leave");
			CS::instance()->enter(CS_RENDERDATA_CS,"enter");





		if (anime_akat && characters[now_character_index]->meshs.size()) {
			static float mae_FRAME = 0;
			CharacterMesh* mes = characters[now_character_index]->meshs[characters[now_character_index]->now_mesh];
			if (mes->skeletons.size() && mes->skeletons[mes->now_skeleton]->skeletons_loaded) {
				DWORD dtime = timeGetTime() - akat_play_time+1;
				CharacterMeshSkeleton* skel = mes->skeletons[mes->now_skeleton];
				if (skel->akats.size()) {
					Akat* aka = skel->akats[skel->now_akat_index];
					if (aka->all_frame > 0) {
						dtime = dtime % (int)(1000);//とりあえず１秒のakatとしてしまう actionのときに長さを調節できるようにする
						float frame = dtime / (float)1000 * KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX;
						if ( abs(frame - mae_FRAME) > 1) {
							mae_FRAME = frame;
							mes->setSiseiAkat(KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX,frame);
							
						}
					}
				}
			}
		}

		if (anime_action && characters[now_character_index]->meshs.size() && characters[now_character_index]->actions.size()) {
			static float mae_FRAME = 0;
			Action* act = characters[now_character_index]->actions[characters[now_character_index]->now_action];
			
			if (act->mesh_akat_pair.size()) {

				map<CharacterMesh*, Akat*>::iterator it = act->mesh_akat_pair.begin();
				while (it != act->mesh_akat_pair.end()) {
					CharacterMesh* mes = (*it).first;
					Akat* aka = (*it).second;
					if (mes->skeletons.size()) {
						DWORD dtime = timeGetTime() - akat_play_time+1;
						CharacterMeshSkeleton* skel = mes->skeletons[aka->skeleton_index];
						if (skel->akats.size() && skel->skeletons_loaded) {
							if (act->all_max_frame > 0) {
								dtime = dtime % (int)(act->all_max_frame*RENDERTIME_SETTIME);//とりあえず１秒のakatとしてしまう actionのときに長さを調節できるようにする
								float frame = dtime / (float)(act->all_max_frame*RENDERTIME_SETTIME) * KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX;
								if ( abs(frame - mae_FRAME) > 1) {
									mae_FRAME = frame;
									mes->setSiseiAkatSitei(aka, KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX,frame);
								}
							}
						}
					}
					it++;
				}
			}
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}

void ActionEditor::aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {
	
	CS::instance()->enter(CS_RENDERDATA_CS,"leave");
	if (save_done && do_force_save) {
		do_force_save = false;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->removeScene();
		if (save_result) {
			MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->enterONEMESSAGE("セーブしました");
		} else {
			MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->enterONEMESSAGE("失敗しました");
		}
		CS::instance()->enter(CS_RENDERDATA_CS ,"ret");
	}
	CS::instance()->leave(CS_RENDERDATA_CS,"enter");

	CS::instance()->enter(CS_RENDERDATA_CS,"leave");
	if (load_done && do_force_load) {
		do_force_load = false;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");


		MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->removeScene();
		if (load_result) {
			MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->enterONEMESSAGE("ロードしました");
		} else {
			MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->enterONEMESSAGE("失敗しました");
		}
		CS::instance()->enter(CS_RENDERDATA_CS ,"ret");
	}
	CS::instance()->leave(CS_RENDERDATA_CS,"enter");

}

void ActionEditor::posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {


}



MeshInstanced* ActionCharacter::getInstanceIDOfOyaMesh(char* oya_filepath) {

	int msize = this->meshs.size();
	for (int m = 0; m<msize; m++) {
		CharacterMesh* mc = meshs[m];
		int mmsize = mc->mesh_filenames.size();
		for (int mm=0;mm<mmsize;mm++) {
			if (strcmp(oya_filepath, mc->mesh_filenames[mm].c_str()) ==0) {
				if (mm>=0 && mm < mc->mesh_instanceds.size()) {
					return mc->mesh_instanceds[mm];
				}
			}
		}
	}

	throw new GameError(KTROBO::WARNING, "couldnt find oyamesh");
}

void CharacterMeshSkeleton::loadAkat(int skeleton_index) {


	MyTokenAnalyzer a;
	if (!a.load(akat_filename)) return;
	a.GetToken("AKATS");
	a.GetToken("{");
	int num = a.GetIntToken();
	for (int i=0;i<num;i++) {
		Akat* new_akt = new Akat(skeleton_index,i);
		a.GetToken("AKAT");
		a.GetToken("{");
		a.GetToken();a.GetToken();
		a.GetToken("ALL_TIME");
		new_akt->all_frame = floor(a.GetFloatToken());
		a.GetToken();a.GetToken();
		a.GetToken("ANIME_NAME");
		a.GetToken();
		hmystrcpy(new_akt->name,32,0,a.Toke());
		a.GetToken();a.GetToken();
		a.GetToken("FRAME_NUM");
		int frame_num = a.GetIntToken();
		for (int k=0;k<frame_num;k++) {
			a.GetToken("FRAME");
			a.GetToken("{");
			a.GetToken("FR");
			int fr = floor(a.GetFloatToken());
			a.GetToken();a.GetToken();
			a.GetToken("KAKERAFRAME");
			int kakeraframe = a.GetIntToken();
			a.GetToken();a.GetToken();
			a.GetToken("}");
			AkatFrame* fre = new AkatFrame();
			fre->left = 0;
			fre->right = 0;
			fre->akat_frame = fr;
			fre->mesh_instanced_frame = kakeraframe;
			new_akt->setAkatFrame(fre);
		}
		// new_akt を登録する
		this->akats.push_back(new_akt);
		a.GetToken("}");
	}

	a.GetToken("}");
	this->skeletons_loaded = true;
	a.deletedayo();
}


void ActionEditor::loaddestructIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {
	// ここでロードとセーブを行う

	CS::instance()->enter(CS_RENDERDATA_CS,"enter");
	if (do_force_save && !save_done) {
		// セーブする
		char buf[512];
		memset(buf,0,512);
		hmystrcpy(buf,512,0,filename.c_str());
		CS::instance()->leave(CS_RENDERDATA_CS,"leave");
		bool tes = _forceSaveNowToFile(buf);
		CS::instance()->enter(CS_RENDERDATA_CS,"enter");
	//	do_force_save = false;
		save_result = tes;
		save_done = true;
		
	}
	CS::instance()->leave(CS_RENDERDATA_CS,"leave");

	CS::instance()->enter(CS_RENDERDATA_CS,"enter");
	if (do_force_load && !load_done) {
		// ロードする
		char buf[512];
		memset(buf,0,512);
		hmystrcpy(buf,512,0,load_filename.c_str());
		CS::instance()->leave(CS_RENDERDATA_CS,"leave");
		CS::instance()->leave(CS_TASK_CS, "leave",TASKTHREADS_LOADDESTRUCT);
		bool tes  = _forceLoadFromFile(buf);
		CS::instance()->enter(CS_TASK_CS, "enter",TASKTHREADS_LOADDESTRUCT);
		CS::instance()->enter(CS_RENDERDATA_CS,"enter");
	//	do_force_save = false;
		load_result = tes;
		load_done = true;
		
	}
	CS::instance()->leave(CS_RENDERDATA_CS,"leave");

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	int im = characters.size();
	if (im) {
		for (int i=0;i<im;i++) {
			bool is_loaded = false;
			ActionCharacter* ii = characters[i];
			// CharacterMesh と Actionのロードを行う
			int msize = ii->meshs.size();
			
			for (int m=0;m<msize;m++){
				CharacterMesh* mesh = ii->meshs[m];
				int usize = mesh->mesh_has_loaded.size();
				for (int u=0;u<usize;u++) {
					if (!mesh->mesh_has_loaded[u]) {
						is_loaded = true;
						Mesh* mes = mesh->meshs[u];
						string s = mesh->mesh_filenames[u];
						char mesh_filename[128];
						memset(mesh_filename,0,128);
						sprintf_s(mesh_filename, 128,"%s.MESH", s.c_str());

						CS::instance()->leave(CS_RENDERDATA_CS, "leave");
						mes->readMesh(g,mesh_filename,g->getTexLoader());
						CS::instance()->enter(CS_RENDERDATA_CS, "enter");
						mesh->mesh_has_loaded[u] = true;
					}
				}

				int skelsize = mesh->skeletons.size();
				for (int s = 0;s<skelsize;s++) {
					CharacterMeshSkeleton* skelton = mesh->skeletons[s];
					if (!skelton->skeletons_loaded) {
						is_loaded = true;
					char mesh_name[128];
					memset(mesh_name,0,128);
					sprintf_s(mesh_name,128,"%s.MESH",skelton->mesh_meshname);
					CS::instance()->leave(CS_RENDERDATA_CS, "leave");
					skelton->skeleton->readMeshWithoutVertex(g, mesh_name, g->getTexLoader());
					skelton->skeleton->readAnime(skelton->mesh_animename);
					CS::instance()->enter(CS_RENDERDATA_CS, "enter");
					skelton->loadAkat(s);
					}
				}



			}

			int asize = ii->actions.size();
			for (int a=0;a<asize;a++) {
				Action* act = ii->actions[a];
				
				if ((act->mesh_akat_pair_index_for_load.size() > act->mesh_akat_pair.size()) && act->mesh_akat_pair.size() ==0) {
					// mesh_akat_pairに入れる
					map<int,pair<int,int>>::iterator it = act->mesh_akat_pair_index_for_load.begin();
					while( it != act->mesh_akat_pair_index_for_load.end()) {
						pair<int,pair<int,int>> p = *it;
						int charactermesh_index = p.first;
						int skeleton_index = p.second.first;
						int akat_index = p.second.second;
						CharacterMesh* mes = ii->meshs[charactermesh_index];
						Akat* akat = mes->skeletons[skeleton_index]->akats[akat_index];

						act->mesh_akat_pair.insert(pair<CharacterMesh*, Akat*>(mes,akat));
						it++;
					}
					is_loaded =true;
				}

			}

			// meshinstancedsに登録する

			int mmmsize = ii->meshs.size();

			for (int mmm=0; mmm < mmmsize;mmm++) {
				CharacterMesh* mm = ii->meshs[mmm];
				int mmsize = mm->mesh_has_loaded.size();
				int misize = mm->mesh_instanceds.size();
				if (mm->skeletons.size()>mm->now_skeleton && mm->now_skeleton >=0 && mm->skeletons[mm->now_skeleton]->skeletons_loaded) {
				for (int mi=misize;mi<mmsize;mi++) {
					is_loaded =true;
					MeshInstanceds* mis = MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(0);
//	MeshInstanced* makeInstanced(Mesh* mesh, Mesh* skeleton, IMeshInstanced* iparent_instance, int parent_bone_index, bool connect_without_matrix_local, MYMATRIX* matrix_local_kakeru)
//	{
					Mesh* mess = mm->meshs[mi];
					CS::instance()->leave(CS_RENDERDATA_CS,"leave");
					CS::instance()->leave(CS_TASK_CS, "leave",TASKTHREADS_LOADDESTRUCT);
					CS::instance()->enter(CS_TASK_CS, "ai lock", 4);
					CS::instance()->enter(CS_TASK_CS, "load lock", 3);
					CS::instance()->enter(CS_TASK_CS, "render lock",2);
					CS::instance()->enter(CS_TASK_CS, "anime lock", 1);
					CS::instance()->enter(CS_TASK_CS, "atari lock", 0);
					CS::instance()->enter(CS_RENDERDATA_CS,"enter");
					try {
					if (mm->has_oya_mesh) {
						MeshInstanced* oya_instanceid = ii->getInstanceIDOfOyaMesh(mm->oya_meshfilename);
						int bone_index = mis->getInstanceSkeletonBoneIndex(oya_instanceid->getInstanceIndex(), mm->oya_meshbonename);
						MeshInstanced* instanced = mis->makeInstanced(mess,mm->skeletons[mm->now_skeleton]->skeleton,oya_instanceid, bone_index, mm->is_connect_without_material_local, &mm->matrix_kakeru);
						mm->mesh_instanceds.push_back(instanced);
					} else {
						MeshInstanced* instanced = mis->makeInstanced(mess, mm->skeletons[mm->now_skeleton]->skeleton,NULL,0,true,&mm->matrix_kakeru);
						mm->mesh_instanceds.push_back(instanced);
					}
					} catch(GameError* err) {
						CS::instance()->leave(CS_RENDERDATA_CS,"render");
						CS::instance()->leave(CS_TASK_CS, "atari lock",0);
						CS::instance()->leave(CS_TASK_CS, "anime lock",1);
						CS::instance()->leave(CS_TASK_CS, "render lock",2);
						CS::instance()->leave(CS_TASK_CS, "load lock", 3);
						CS::instance()->leave(CS_TASK_CS, "ai lock", 4);
						CS::instance()->enter(CS_TASK_CS, "load lock", 3);
						//CS::instance()->enter(CS_RENDERDATA_CS,"enter");
						throw err;
					}
					CS::instance()->leave(CS_RENDERDATA_CS,"render");
					CS::instance()->leave(CS_TASK_CS, "atari lock",0);
					CS::instance()->leave(CS_TASK_CS, "anime lock",1);
					CS::instance()->leave(CS_TASK_CS, "render lock",2);
					CS::instance()->leave(CS_TASK_CS, "load lock", 3);
					CS::instance()->leave(CS_TASK_CS, "ai lock", 4);
					CS::instance()->enter(CS_TASK_CS, "load lock", 3);
					CS::instance()->enter(CS_RENDERDATA_CS,"enter");
				}
				}
			}



			if (is_loaded) {
				CS::instance()->leave(CS_RENDERDATA_CS, "leave");
				MyLuaGlueSingleton::getInstance()->getColMessages(0)->getInstance(0)->makeMessage(
					KTROBO_MESSAGE_ID_ACTIONEDITOR_LOAD_AFTER,KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, i,0,true);
				LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,true, "resrc/script/AE_loadAfter.lua");
					CS::instance()->enter(CS_RENDERDATA_CS, "enter");
			}
		}
	}

	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}
int ActionEditor::getActionNum(int character_id) {


		int ans =0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && characters.size() > character_id) {
		ActionCharacter* ac = characters[character_id];
		ans = ac->actions.size();
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

	return ans;




}
int ActionEditor::getSkeletonNum(int character_id, int hon_mesh_id) {
	int ans =0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && characters.size() > character_id) {
		ActionCharacter* ac = characters[character_id];
		if (hon_mesh_id >= 0 && hon_mesh_id < ac->meshs.size()) {
			CharacterMesh* mm = ac->meshs[hon_mesh_id];
			ans = mm->skeletons.size();
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return ans;
}
int ActionEditor::getHonMeshNum(int character_id) {

		int ans =0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && characters.size() > character_id) {
		ActionCharacter* ac = characters[character_id];
		ans = ac->meshs.size();
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

	return ans;

}
int ActionEditor::createActionCharacter(char* name) {
	int ans;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	ans = characters.size();
	ActionCharacter* mm = new ActionCharacter(name, MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0));
	characters.push_back(mm);
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return ans;
}


int ActionEditor::setHonMesh(int character_id, char* mesh_filename, char* oya_mesh_filename, char* oya_mesh_bonename,bool is_connect_without_matrial_local, YARITORI MYMATRIX* mat) {

	int ans=0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* cha = characters[character_id];
		ans = cha->getMeshs()->size();
		Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
		CharacterMesh* mesh = new CharacterMesh(ans, tex);
		hmystrcpy(mesh->oya_meshfilename,128,0,oya_mesh_filename);
		hmystrcpy(mesh->oya_meshbonename,128,0,oya_mesh_bonename);
		string filename = mesh_filename;
		//mesh->is_akat_loaded = false;
		mesh->is_connect_without_material_local = is_connect_without_matrial_local;
		mesh->matrix_kakeru = *mat;
		// Meshの作成とフラグをオフにしておく
		Mesh* m = new Mesh();
		bool mesh_has_loaded = false;	
		if (strlen(oya_mesh_filename)) {
			// 親メッシュがいるので設定が必要
			mesh->has_oya_mesh = true;
			// meshへの実際の設定は親メッシュがロードされてから子メッシュをロードする際に行う
		}else {
			mesh->has_oya_mesh = false;
		}
		mesh->meshs.push_back(m);
		mesh->mesh_filenames.push_back(filename);
		mesh->mesh_has_loaded.push_back(mesh_has_loaded);
		cha->meshs.push_back(mesh);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return ans;
}


int ActionEditor::setOnajiMesh(int character_id, int hon_mesh_id, char* mesh_filename) {

	int ans;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* cha = characters[character_id];
		CharacterMesh* mesh = (*cha->getMeshs())[hon_mesh_id];
		string filename = mesh_filename;
		Mesh* m = new Mesh();
		bool mesh_has_loaded = false;	
		ans = mesh->meshs.size();
		mesh->meshs.push_back(m);
		mesh->mesh_filenames.push_back(filename);
		mesh->mesh_has_loaded.push_back(mesh_has_loaded);

	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return ans;
}
void ActionEditor::toggleMeshRender(int character_id, int hon_mesh_id) {
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* cha = characters[character_id];
		CharacterMesh* mesh = (*cha->getMeshs())[hon_mesh_id];
		mesh->is_render = !mesh->is_render;
		int msize = mesh->mesh_instanceds.size();
		for (int i=0;i<msize;i++) {
			MeshInstanced* mm = mesh->mesh_instanceds[i];
			mm->setIsRender(mesh->is_render);
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return;

}

void ActionEditor::toggleMeshOptional(int character_id, int hon_mesh_id) {
	// そのキャラクターにとってメッシュが特定の状態でつけられるかどうかをトグルする

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* cha = characters[character_id];
		CharacterMesh* mesh = (*cha->getMeshs())[hon_mesh_id];
		mesh->is_optional = !mesh->is_optional;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return;
}	
	
	
int ActionEditor::setSkeleton(int character_id, int hon_mesh_id, char* anime_filename, char* akat_filename) {

	int ans = 0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* cha = characters[character_id];
		CharacterMesh* mesh = (*cha->getMeshs())[hon_mesh_id];
		ans = mesh->skeletons.size();
		char meshfilename[128];
		memset(meshfilename,0,128);
		if (mesh->mesh_filenames.size()) {
			hmystrcpy(meshfilename, 128,0, mesh->mesh_filenames[0].c_str());
			CharacterMeshSkeleton* skle = new CharacterMeshSkeleton(meshfilename, akat_filename, anime_filename);
			mesh->skeletons.push_back(skle);
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");



	return ans;
}


int ActionEditor::getAkatNum(int character_id, int hon_mesh_id, int skeleton_id) {


	int ans = 0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* cha = characters[character_id];
		CharacterMesh* mesh = (*cha->getMeshs())[hon_mesh_id];
		if (skeleton_id >= 0 && skeleton_id < mesh->skeletons.size()) {
			CharacterMeshSkeleton* skle = mesh->skeletons[skeleton_id];
			if (skle->skeletons_loaded) {
				ans = skle->akats.size();
			} else {
				CS::instance()->leave(CS_RENDERDATA_CS, "leave");
				throw new GameError(KTROBO::WARNING, "skleton is not loaded");
			}
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return ans;
}


void ActionEditor::getAkatName(int character_id, int hon_mesh_id, int skeleton_id, int akat_index, OUT_ char* name) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* cha = characters[character_id];
		CharacterMesh* mesh = (*cha->getMeshs())[hon_mesh_id];
		if (skeleton_id >= 0 && skeleton_id < mesh->skeletons.size()) {
			CharacterMeshSkeleton* skle = mesh->skeletons[skeleton_id];
			if (skle->skeletons_loaded && akat_index >= 0 && akat_index < skle->akats.size()) {
				char* ans;
				ans = skle->akats[akat_index]->name;
				hmystrcpy(name,32,0,ans);
				CS::instance()->leave(CS_RENDERDATA_CS, "leave");
				return;
			} else {
				CS::instance()->leave(CS_RENDERDATA_CS, "leave");
				throw new GameError(KTROBO::WARNING, "skleton is not loaded");
			}
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	hmystrcpy(name, 8,0,"nullaka");
}


void ActionEditor::setNowActionSisei(int character_id, float frame) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* mm = characters[character_id];
		if (mm->actions.size()) {
			Action* act = mm->actions[mm->now_action];
			if (act->mesh_akat_pair.size()) {
				map<CharacterMesh*, Akat*>::iterator it = act->mesh_akat_pair.begin();
				while (it != act->mesh_akat_pair.end()) {
					CharacterMesh* mes = (*it).first;
					Akat* aka = (*it).second;
					if (mes->skeletons.size() && mes->skeletons[mes->now_skeleton]->skeletons_loaded) {
						mes->setSiseiAkatSitei(aka, KTROBO_MESH_INSTANCED_ANIME_MATRIX_BASIS_NUM_MAX,frame);
					}
					it++;
				}
			}
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}



void ActionEditor::setNowActionFrame(int character_id, int frame) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* mm = characters[character_id];
		if (mm->actions.size()) {
			Action* act = mm->actions[mm->now_action];
			act->all_max_frame = frame;
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}
int ActionEditor::makeAction(int character_id, char* action_name) {

	int ans = 0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ans = characters.size();
		ActionCharacter* mm = characters[character_id];
		Action* act = new Action();
		hmystrcpy(act->action_name,32,0,action_name);
		ans = mm->actions.size();
		mm->actions.push_back(act);
		act->all_max_frame = KTROBO_ACTIONEDITOR_DEFAULT_ACTION_FRAME;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		MyLuaGlueSingleton::getInstance()->getColMessages(0)->getInstance(0)->makeMessage(
				KTROBO_MESSAGE_ID_ACTIONEDITOR_LOAD_AFTER,KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, KTROBO_MESSAGE_RECEIVER_ID_SYSTEM, character_id,0,true);
		LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION,true, "resrc/script/AE_loadAfter.lua");
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return ans;

}

void ActionEditor::setAkatToAction(int character_id, int action_id, int hon_mesh_id, int skeleton_id, int akat_index) {
	
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* mm = characters[character_id];
		if (action_id >=0 && action_id < mm->actions.size()) {
			if (hon_mesh_id >= 0 && mm->meshs.size() > hon_mesh_id) {
				CharacterMesh* cm = mm->meshs[hon_mesh_id];
				if (skeleton_id >=0 && skeleton_id < cm->skeletons.size()) {
					CharacterMeshSkeleton* skel = cm->skeletons[skeleton_id];
					if (akat_index >= 0 && akat_index < skel->akats.size()) {
						Akat* aka = skel->akats[akat_index];
						Action* act = mm->actions[action_id];
						if (act->mesh_akat_pair.find(cm) != act->mesh_akat_pair.end()) {
							act->mesh_akat_pair.find(cm)->second = aka;
						} else {
							act->mesh_akat_pair.insert(pair<CharacterMesh*, Akat*>(cm,aka));
						}
						CS::instance()->leave(CS_RENDERDATA_CS, "leave");
						return;
					}
				}
			}
		} else {
			CS::instance()->leave(CS_RENDERDATA_CS, "leave");
			throw new GameError(KTROBO::WARNING, "out side vector of action");
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}

void ActionEditor::setNowCharacterId(int character_id) {
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		this->now_character_index = character_id;
		int csize = characters.size();
		Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
		for (int i=0;i<csize;i++) {
			characters[i]->setUnFocus(tex);
		}
		characters[now_character_index]->setFocus(tex);
		
		tex->setRenderTextChangeText(this->now_character_text, characters[character_id]->character_name);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}
int ActionEditor::getNowCharacterId() {
	int ans=0;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		ans = this->now_character_index;
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return ans;
}

void ActionEditor::setNowAkat(int character_id, int hon_mesh_id, int skeleton_id,  int akat_index) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* mm = characters[character_id];
		if (hon_mesh_id >= 0 && hon_mesh_id < mm->meshs.size()) {
			CharacterMesh* m = mm->meshs[hon_mesh_id];
			if (m->skeletons.size() > skeleton_id && skeleton_id >=0) {
				CharacterMeshSkeleton* skel = m->skeletons[skeleton_id];
				if (akat_index >= 0 && akat_index < skel->akats.size()) {
					Akat* aka = skel->akats[akat_index];
					mm->now_mesh = hon_mesh_id;
					Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
					char filename[256];
					memset(filename,0,256);
					hmystrcpy(filename,256,0, m->mesh_filenames[0].c_str());
					tex->setRenderTextChangeText(mm->now_mesh_text, filename);
					m->now_skeleton = skeleton_id;
					skel->now_akat_index = akat_index;
				}
			}
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}


void ActionEditor::getNowAkatIndex(int character_id,  int hon_mesh_id, int skeleton_id,  OUT_ int* akat_index) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* mm = characters[character_id];
		if (hon_mesh_id >= 0 && hon_mesh_id < mm->meshs.size()) {
			CharacterMesh* m = mm->meshs[hon_mesh_id];
			if (m->skeletons.size() > skeleton_id && skeleton_id >=0) {
				CharacterMeshSkeleton* skel = m->skeletons[skeleton_id];
				if (*akat_index >= 0 && *akat_index < skel->akats.size()) {
					Akat* aka = skel->akats[*akat_index];
					mm->now_mesh = hon_mesh_id;
					Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
					char filename[256];
					memset(filename,0,256);
					hmystrcpy(filename,256,0, m->mesh_filenames[0].c_str());
					tex->setRenderTextChangeText(mm->now_mesh_text, filename);
					m->now_skeleton = skeleton_id;
					*akat_index = skel->now_akat_index;	
				}
			}
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}


void ActionEditor::setNowAction(int character_id, int action_id) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* mm = characters[character_id];
		if (action_id >=0 && action_id < mm->actions.size()) {
			mm->now_action = action_id;
			Action* act = mm->actions[action_id];
			Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
			tex->setRenderTextChangeText(mm->now_action_text, act->action_name);
			tex->setRenderTextIsRender(mm->now_action_text, true);
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");


}


void ActionEditor::togglePlayNowAction() {

	
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (characters.size()) {
		ActionCharacter* ac = characters[now_character_index];
		this->anime_action = ! anime_action;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}


void ActionEditor::togglePlayNowAkat() {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (characters.size()) {
		ActionCharacter* ac = characters[now_character_index];
		this->anime_akat = !anime_akat;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}

void ActionEditor::analyzeStrCommand(char* command, CharacterActionCommand* new_command) {

	// 大文字が離されたとき 小文字が押されたとき
	char* strs[] = {
		"q","w","e","r","t",
		"a","s","d","f","g",
		"z","x","c","v","b",
		":"/*ジャンプのスペース*/,"-"/*しゃがみのshift*/,";"/*呼吸のコントロール*/
		"Q","W","E","R","T",
		"A","S","D","F","G",
		"Z","X","C","V","B",
		"*","=","+"};
	bool strs_is_koudou[] = {
		true,false,true,true,true,
		false,false,false,true,true,
		false,false,false,
		true,false,true,true,true,
		false,false,false,true,true,
		false,false,false
	};

	unsigned long commanddayo[] = {
		INPUTJYOUTAI_KAMAE_DOWN, INPUTJYOUTAI_FORWARD_DOWN,INPUTJYOUTAI_PICK_DOWN,INPUTJYOUTAI_JYAKU_PUNCH_DOWN, INPUTJYOUTAI_KYOU_PUNCH_DOWN,
		INPUTJYOUTAI_LEFT_DOWN, INPUTJYOUTAI_BACK_DOWN, INPUTJYOUTAI_RIGHT_DOWN, INPUTJYOUTAI_JYAKU_KICK_DOWN, INPUTJYOUTAI_KYOU_KICK_DOWN,
		0, 0, INPUTJYOUTAI_FRIEND_DOWN, 0, 0,
		INPUTJYOUTAI_JUMP_DOWN, INPUTJYOUTAI_SHAGAMI_DOWN, INPUTJYOUTAI_KOKYUU_DOWN,
		INPUTJYOUTAI_KAMAE_UP, INPUTJYOUTAI_FORWARD_UP, INPUTJYOUTAI_PICK_UP, INPUTJYOUTAI_JYAKU_PUNCH_UP, INPUTJYOUTAI_KYOU_PUNCH_UP,
		INPUTJYOUTAI_LEFT_UP, INPUTJYOUTAI_BACK_UP, INPUTJYOUTAI_RIGHT_UP, INPUTJYOUTAI_JYAKU_KICK_UP, INPUTJYOUTAI_KYOU_KICK_UP,
		0, 0, INPUTJYOUTAI_FRIEND_UP, 0, 0,
		INPUTJYOUTAI_JUMP_UP, INPUTJYOUTAI_SHAGAMI_UP, INPUTJYOUTAI_KOKYUU_UP,
	};

	int index = 0;
	memset(new_command->idou,0, sizeof(long)*INPUT_MYCOMMAND_FRAME_MAX);
	memset(new_command->koudou,0, sizeof(long)*INPUT_MYCOMMAND_FRAME_MAX);
	while(command[index] != '\0' && index < INPUT_MYCOMMAND_FRAME_MAX) {

		for (int i=0;i<36;i++) {
			char commandd = command[index];
			if (strcmp(&commandd, strs[i])==0) {
				if (strs_is_koudou[i]) {
					new_command->koudou[index] |= commanddayo[i];
				} else {
					new_command->idou[index] |= commanddayo[i];
				}
			}
		}

		index++;
	}
		/*
		#define INPUTJYOUTAI_KOKYUU_UP 0x01
#define INPUTJYOUTAI_KOKYUU_DOWN 0x02
#define INPUTJYOUTAI_LEFT_UP 0x04
#define INPUTJYOUTAI_LEFT_DOWN 0x08
#define INPUTJYOUTAI_RIGHT_UP 0x010
#define INPUTJYOUTAI_RIGHT_DOWN 0x020
#define INPUTJYOUTAI_FORWARD_UP 0x040
#define INPUTJYOUTAI_FORWARD_DOWN 0x080
#define INPUTJYOUTAI_BACK_UP 0x0100
#define INPUTJYOUTAI_BACK_DOWN 0x0200
#define INPUTJYOUTAI_KAMAE_UP 0x0400
#define INPUTJYOUTAI_KAMAE_DOWN 0x0800
#define INPUTJYOUTAI_JUMP_UP 0x1000
#define INPUTJYOUTAI_JUMP_DOWN 0x2000
#define INPUTJYOUTAI_SHAGAMI_UP 0x4000
#define INPUTJYOUTAI_SHAGAMI_DOWN 0x8000

// 行動構造体
#define INPUTJYOUTAI_MOUSEBUTTON_R_UP 0x0001
#define INPUTJYOUTAI_MOUSEBUTTON_R_DOWN 0x0002
#define INPUTJYOUTAI_MOUSEBUTTON_L_UP 0x0004
#define INPUTJYOUTAI_MOUSEBUTTON_L_DOWN 0x0008
#define INPUTJYOUTAI_PICK_UP 0x0010
#define INPUTJYOUTAI_PICK_DOWN 0x0020
#define INPUTJYOUTAI_FRIEND_UP 0x0040
#define INPUTJYOUTAI_FRIEND_DOWN 0x0080
#define INPUTJYOUTAI_JYAKU_PUNCH_UP 0x0100
#define INPUTJYOUTAI_JYAKU_PUNCH_DOWN 0x0200
#define INPUTJYOUTAI_KYOU_PUNCH_UP 0x0400
#define INPUTJYOUTAI_KYOU_PUNCH_DOWN 0x0800
#define INPUTJYOUTAI_JYAKU_KICK_UP 0x1000
#define INPUTJYOUTAI_JYAKU_KICK_DOWN 0x2000
#define INPUTJYOUTAI_KYOU_KICK_UP 0x4000
#define INPUTJYOUTAI_KYOU_KICK_DOWN 0x8000



*/



}

void ActionEditor::setCommandToCharacter(int character_id, int command_id, char* command, char* name, int priority, bool is_reset, int frame) {
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	
	if (character_id >= 0 && character_id < characters.size()) {
		ActionCharacter* ac = characters[character_id];
		
		CharacterActionCommand* new_command = new CharacterActionCommand();
		new_command->priority = priority;
		new_command->is_reset = is_reset;
		new_command->is_use = true;
		new_command->frame = frame;
		new_command->command = command_id;
		hmystrcpy(new_command->name,32,0,name);
		analyzeStrCommand(command, new_command);
		ac->commands.push_back(new_command);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}
void ActionEditor::setSakiAction(int character_id, int action_id) {


	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	
	if (character_id >= 0 && character_id < characters.size()) {
		ActionCharacter* ac = characters[character_id];
		if (action_id >=0 && action_id < ac->actions.size()) {
			ac->setSakiAction(action_id);
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");



}


void ActionEditor::setMotoAction(int character_id, int action_id) {


	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	
	if (character_id >= 0 && character_id < characters.size()) {
		ActionCharacter* ac = characters[character_id];
		if (action_id >=0 && action_id < ac->actions.size()) {
			ac->setMotoAction(action_id);
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");




}

int ActionEditor::makeActiontoAction(int character_id, int command_id) {
	int ans = 0;
	// 元のアクションと先のアクションが同じactiontoactionがすでに登録されているかどうか
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	
	if (character_id >= 0 && character_id < characters.size()) {
		ActionCharacter* ac = characters[character_id];
		bool is_has_already= false;
		bool is_has_command = false;
		ActionToAction* ans_ata = 0;
		

		int asize = ac->action_to_actions.size();
		if (asize) {
			for (int i=0;i<asize;i++) {
				ActionToAction* ata = ac->action_to_actions[i];
				if ((ata->moto_action_id == ac->moto_action) && (ata->saki_action_id == ac->saki_action)) {
					is_has_already = true;
					ans_ata = ata;
					ans = i;
					break;
				}
			}
		}
		// 次にコマンドが実際にあるものかどうか確かめる
		int csize = ac->commands.size();
		if(csize) {
			for (int i=0;i<csize;i++) {
				CharacterActionCommand* aca = ac->commands[i];
				if (aca->command == command_id) {
					is_has_command = true;
					break;
				}
			}
		}

		if (!is_has_command) {
			throw new GameError(KTROBO::WARNING, "no command");
		}

		if (!is_has_already) {
			ans_ata = new ActionToAction();
			ans_ata->moto_action_id = ac->moto_action;
			ans_ata->saki_action_id = ac->saki_action;
			ans = ac->action_to_actions.size();
			ac->action_to_actions.push_back(ans_ata);
		}
		ans_ata->command_id = command_id;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

	return ans;
}

void ActionEditor::clearActionToAction(int character_id, int action_to_action_id) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	
	if (character_id >= 0 && character_id < characters.size()) {
		ActionCharacter* ac = characters[character_id];
		if (action_to_action_id >=0 && action_to_action_id < ac->action_to_actions.size()) {
			ActionToAction* actt = ac->action_to_actions[action_to_action_id];
			actt->command_id = 0;// 不使用にする
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}

void ActionEditor::deleteAll() {
	CS::instance()->enter(CS_RENDERDATA_CS, "renderdata");
	vector<ActionCharacter*>::iterator it = characters.begin();
	while(it != characters.end()) {

		ActionCharacter* i = *it;
		if (i) {
			i->Release();
			delete i;
			i = 0;
		}
		it++;
	}
	characters.clear();
	CS::instance()->leave(CS_RENDERDATA_CS, "renderdata");
}

bool ActionEditor::saveNowToFile(char* dfilename) {

	// すべての今の状態を保存する
	CS::instance()->enter(CS_RENDERDATA_CS,"test");
	FILE* fp;
	fopen_s(&fp, dfilename, "r");
	if (fp != NULL) {
		fclose(fp);
		CS::instance()->leave(CS_RENDERDATA_CS,"test");
		return false;
	}

	this->filename = string(dfilename);
	do_force_save = true;
	save_done =false;
	MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->enterLOADTYUU();
	CS::instance()->leave(CS_RENDERDATA_CS,"test");
	
	return true;//force_saveNowToFile(filename);

}

void ActionEditor::forceSaveNowToFile(char* dfilename) {

	CS::instance()->enter(CS_RENDERDATA_CS,"enter");
	save_done = false;
	do_force_save = true;
	this->filename = string(dfilename);
	
	MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->removeScene();
	MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->enterLOADTYUU();
	CS::instance()->leave(CS_RENDERDATA_CS,"leave");
	return;

}

bool ActionEditor::loadFromFile(char* filename) {


	// すべての状態が保存されたファイルから状態を復元する現在のものは消してしまう
	if (characters.size()) return false;

	CS::instance()->enter(CS_RENDERDATA_CS,"enter");
	do_force_load = true;
	load_done = false;
	load_filename = string(filename);
	MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->enterLOADTYUU();
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return true;
}



void ActionEditor::forceLoadFromFile(char* filename) {

	CS::instance()->enter(CS_RENDERDATA_CS,"enter");
	do_force_load = true;
	load_done = false;
	load_filename = string(filename);
	MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->removeScene();
	MyLuaGlueSingleton::getInstance()->getColTextFromLuas(0)->getInstance(0)->enterLOADTYUU();
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	return;


}


bool ActionEditor::saveNowCharacterToFile(char* dfilename) {



	CS::instance()->enter(CS_RENDERDATA_CS,"test");
	FILE* fp;
	fopen_s(&fp, dfilename, "r");
	if (fp != NULL) {
		fclose(fp);
		CS::instance()->leave(CS_RENDERDATA_CS,"test");
		return false;
	}

	CS::instance()->leave(CS_RENDERDATA_CS,"test");
	
	return forceSaveNowCharacterToFile(dfilename);//force_saveNowToFile(filename);
}

void Action::write(char* filename) {
KTROBO::mylog::writelog(filename, "ACTION{\n");
KTROBO::mylog::writelog(filename, "name=\"%s\";\n", this->action_name);
KTROBO::mylog::writelog(filename, "frame=%d;\n", this->all_max_frame);

KTROBO::mylog::writelog(filename, "ATARIS{\n");
KTROBO::mylog::writelog(filename, "num=%d;\n", this->hanteis.size());
int bsize = hanteis.size();
for (int i=0;i<bsize;i++) {
	hanteis[i]->write(filename);
}
KTROBO::mylog::writelog(filename, "}\n");

int csize = mesh_akat_pair.size();
KTROBO::mylog::writelog(filename, "MAKATPAIRS{\n");
KTROBO::mylog::writelog(filename, "num=%d;\n", csize);
for (int i=0;i<csize;i++) {
	map<CharacterMesh*,Akat*>::iterator it = mesh_akat_pair.begin();
	for (int j=0;j<i;j++) {
		it++;
	}
	pair<CharacterMesh*, Akat*> p = *it;
	CharacterMesh* me = p.first;
	Akat* aka = p.second;
	KTROBO::mylog::writelog(filename, "MAKATPAIR{\n");
		KTROBO::mylog::writelog(filename, "charamesh_index=%d;\n",me->myindex);
		KTROBO::mylog::writelog(filename, "skeleton_index=%d;\n",aka->skeleton_index);
		KTROBO::mylog::writelog(filename, "akat_index=%d;\n", aka->akat_index);
	KTROBO::mylog::writelog(filename, "}\n"); 
}

KTROBO::mylog::writelog(filename, "}\n");



KTROBO::mylog::writelog(filename, "}\n");
}


void ActionEditor::setNowHonMeshId(int character_id, int hon_mesh_id) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* ac = characters[character_id];
		if (hon_mesh_id >=0 && hon_mesh_id < ac->meshs.size()) {
			CharacterMesh* cm = ac->meshs[hon_mesh_id];
			if (cm->mesh_filenames.size()) {
				ac->setNowMesh(hon_mesh_id);
			}
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}
void CharacterMesh::setNowSkeleton(int index) {
		if (index >=0 && index < skeletons.size()) {
			now_skeleton = index;
			CharacterMeshSkeleton* skel = skeletons[index];
			vector<MeshInstanced*>::iterator it = mesh_instanceds.begin();
			while(it != mesh_instanceds.end()) {
				MeshInstanced* mm = *it;
				MeshInstanceds* mmm = MyLuaGlueSingleton::getInstance()->getColMeshInstanceds(0);
//				mmm->setSkeleton(skel->skeleton);
				int skelinde = mmm->getSkeletonIndexOrSet(skel->skeleton);
				mm->setSkeletonIndex(skelinde);
				it++;
			}
			tex->setRenderTextChangeText(now_skeleton_text,skel->mesh_animename);
		} else {
			throw new GameError(KTROBO::WARNING, "out side vector of now skeleton");
		}
}

void ActionEditor::setNowSkeletonId(int character_id, int hon_mesh_id, int skeleton_id) {

	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* ac = characters[character_id];
		if (hon_mesh_id >=0 && hon_mesh_id < ac->meshs.size()) {
			CharacterMesh* cm = ac->meshs[hon_mesh_id];
			if (cm->mesh_filenames.size()) {
				cm->setNowSkeleton(skeleton_id);
			}
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");

}



void CharacterMesh::write(char* filename) {
	/*
	int myindex;
	vector<Mesh*> meshs;
	vector<bool> mesh_has_loaded;
	vector<string> mesh_filenames;// .MESHを含まない
	bool has_oya_mesh;
	char oya_meshfilename[128]; // .MESHを含まない
	bool is_connect_without_material_local;
	MYMATRIX matrix_kakeru;
	bool is_akat_loaded;
	vector<CharacterMeshSkeleton*> skeletons;
	bool is_optional;
	bool is_render;

	int now_skeleton;
	*/
	KTROBO::mylog::writelog(filename, "CHARAMESH{\n");
	KTROBO::mylog::writelog(filename, "myindex=%d;\n", myindex);
	if (has_oya_mesh) {
		KTROBO::mylog::writelog(filename, "has_oya_mesh=1;\n");
		KTROBO::mylog::writelog(filename, "oya_mesh=\"%s\";\n", oya_meshfilename);
		KTROBO::mylog::writelog(filename, "oya_bone=\"%s\";\n", oya_meshbonename);
	} else {
		KTROBO::mylog::writelog(filename, "has_oya_mesh=0;\n");
		KTROBO::mylog::writelog(filename, "oya_mesh=\"%s\";\n", "nomesh");
		KTROBO::mylog::writelog(filename, "oya_bone=\"%s\";\n", "nobone");
	}

	if (is_connect_without_material_local) {
		KTROBO::mylog::writelog(filename, "is_connect_without_material_local=1;\n");
	} else {
		KTROBO::mylog::writelog(filename, "is_connect_without_mateiral_local=0;\n");
	}

	if (is_optional) {
		KTROBO::mylog::writelog(filename, "is_optional=1;\n");
	} else {
		KTROBO::mylog::writelog(filename, "is_optional=0;\n");
	}

	if (is_render) {
		KTROBO::mylog::writelog(filename, "is_render=1;\n");
	} else {
		KTROBO::mylog::writelog(filename, "is_render=0;\n");
	}
	KTROBO::mylog::writelog(filename, "kakeru=\n");
	for (int i=0;i<16;i++) {
		KTROBO::mylog::writelog(filename, "%f;", matrix_kakeru.m[i/4][i%4]);
	}
	KTROBO::mylog::writelog(filename, "\n");
	KTROBO::mylog::writelog(filename, "FILENAMES{\n");
	KTROBO::mylog::writelog(filename, "num=%d;\n", mesh_filenames.size());
	int bsize = mesh_filenames.size();
	for (int i=0;i<bsize;i++) {
		KTROBO::mylog::writelog(filename, "string=\"%s\";\n",mesh_filenames[i].c_str());
	}
	KTROBO::mylog::writelog(filename, "}\n");


	KTROBO::mylog::writelog(filename, "SKELETONS{\n");
	KTROBO::mylog::writelog(filename, "num=%d;\n", skeletons.size());
	int xsize = skeletons.size();
	for (int i=0;i<xsize;i++) {
		KTROBO::mylog::writelog(filename, "SKELETON{\n");
		KTROBO::mylog::writelog(filename, "akat_filename=\"%s\";\n", skeletons[i]->akat_filename);
		KTROBO::mylog::writelog(filename, "mesh_meshname=\"%s\";\n", skeletons[i]->mesh_meshname);
		KTROBO::mylog::writelog(filename, "mesh_animename=\"%s\";\n", skeletons[i]->mesh_animename);
		KTROBO::mylog::writelog(filename, "}\n");
	}
	KTROBO::mylog::writelog(filename, "}\n");


	KTROBO::mylog::writelog(filename, "}\n");
}

void CharacterActionCommand::write(char* filename) {

	/*
	int command;
	int priority; // 0~15 0 のほうが優先される
	int frame; // 0~INPUTJYOUTAI_FRAME_MAX
	unsigned long idou[INPUT_MYCOMMAND_FRAME_MAX];
	unsigned long koudou[INPUT_MYCOMMAND_FRAME_MAX];
	char name[32];
	bool is_use;
	bool is_reset; // 発動したときバッファをクリアするかどうか
	*/
	KTROBO::mylog::writelog(filename, "ACCOMMAND{\n");
	KTROBO::mylog::writelog(filename, "command=%d;\n", this->command);
	KTROBO::mylog::writelog(filename, "priority=%d;\n", this->priority);
	KTROBO::mylog::writelog(filename, "frame=%d;\n", this->frame);
	KTROBO::mylog::writelog(filename, "name=\"%s\";\n", this->name);
	if (is_reset) {
		KTROBO::mylog::writelog(filename, "is_reset=1;\n");
	} else {
		KTROBO::mylog::writelog(filename, "is_reset=0;\n");
	}

	KTROBO::mylog::writelog(filename, "idou=\n");
	for (int i=0;i<INPUT_MYCOMMAND_FRAME_MAX;i++) {
		KTROBO::mylog::writelog(filename, "%ul;", idou[i]);
	}
	KTROBO::mylog::writelog(filename, "\n");
	KTROBO::mylog::writelog(filename, "koudou=\n");
	for (int i=0;i<INPUT_MYCOMMAND_FRAME_MAX;i++) {
		KTROBO::mylog::writelog(filename, "%ul;", koudou[i]);
	}
	KTROBO::mylog::writelog(filename, "\n");


	KTROBO::mylog::writelog(filename, "}\n");

}

void AtariHantei::write(char* filename) {
	/*
	int start_frame;
	int end_frame;
	float radius;
	MYMATRIX combined_matrix;
	*/
	KTROBO::mylog::writelog(filename, "ATARI{\n");
	KTROBO::mylog::writelog(filename, "start_frame=%d;\n", start_frame);
	KTROBO::mylog::writelog(filename, "end_frame=%d;\n", end_frame);
	KTROBO::mylog::writelog(filename, "radius=%f;\n", radius);
	KTROBO::mylog::writelog(filename, "matrix=\n");
	for (int i=0;i<16;i++) {
		KTROBO::mylog::writelog(filename, "%f;",combined_matrix.m[i/4][i%4]);
	}
	KTROBO::mylog::writelog(filename, "\n");

	KTROBO::mylog::writelog(filename, "}\n");
}


void ActionCharacter::write(char* filename) {

	KTROBO::mylog::writelog(filename, "ACHARACTER{\n");
	KTROBO::mylog::writelog(filename, "name=\"%s\";\n",this->character_name);

	KTROBO::mylog::writelog(filename, "MESHS{\n");
	KTROBO::mylog::writelog(filename, "num=%d;\n", meshs.size());
	int csize = meshs.size();
	for (int i=0;i<csize;i++){
		meshs[i]->write(filename);
	}

	KTROBO::mylog::writelog(filename, "}\n");




	KTROBO::mylog::writelog(filename, "ACTIONS{\n");
	KTROBO::mylog::writelog(filename, "num=%d;\n",this->actions.size());
	int asize = actions.size();
	for (int i=0;i<asize;i++) {
		actions[i]->write(filename);
	}
	KTROBO::mylog::writelog(filename, "}\n");

	KTROBO::mylog::writelog(filename, "ACTION_TO_ACTIONS{\n");
	KTROBO::mylog::writelog(filename, "num=%d;\n", action_to_actions.size());
	int bsize = action_to_actions.size();
	for (int i=0;i<bsize;i++) {
		KTROBO::mylog::writelog(filename, "ATA{\n");
		KTROBO::mylog::writelog(filename, "command_id=%d;\n", action_to_actions[i]->command_id);
		KTROBO::mylog::writelog(filename, "moto_id=%d;\n", action_to_actions[i]->moto_action_id);
		KTROBO::mylog::writelog(filename, "saki_id=%d;\n", action_to_actions[i]->saki_action_id);
		KTROBO::mylog::writelog(filename, "}\n");
	}
	KTROBO::mylog::writelog(filename, "}\n");



	KTROBO::mylog::writelog(filename, "COMMANDS{\n");
	KTROBO::mylog::writelog(filename, "num=%d;\n", commands.size());
	int dsize = commands.size();
	for (int i=0;i<dsize;i++) {
		commands[i]->write(filename);
	}
	KTROBO::mylog::writelog(filename, "}\n");

	KTROBO::mylog::writelog(filename, "}\n");
}

bool ActionEditor::_forceSaveNowToFile(char* filename) {
	
	forceSaveNowCharacterToFile(filename);
	// すべての今の状態を保存する
	FILE* fp;
	// いったん消去する
	CS::instance()->enter(CS_RENDERDATA_CS,"enter");
	fopen_s(&fp, filename, "w");
	if (fp == NULL) {
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
		return false;
	}
	fclose(fp);

	KTROBO::mylog::writelog(filename, "ACS{\n");
	KTROBO::mylog::writelog(filename, "num=%d;\n",characters.size());
	int size = characters.size();
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	for (int i=0;i<size;i++) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		characters[i]->write(filename);
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	}
	KTROBO::mylog::writelog(filename, "}\n");
	return true;
}

AtariHantei* AtariHantei::load(MyTokenAnalyzer* a) {

	a->GetToken("ATARI");
	a->GetToken("{");
	a->GetToken("start_frame");
	int s_frame = a->GetIntToken();
	a->GetToken("end_frame");
	int e_frame = a->GetIntToken();
	a->GetToken("radius");
	float ra = a->GetFloatToken();
	a->GetToken("matrix");
	MYMATRIX mat;
	for (int i=0;i<16;i++) {
		mat.m[i/4][i%4] = a->GetFloatToken();
	}
	AtariHantei* at = new AtariHantei();
	at->start_frame = s_frame;
	at->end_frame = e_frame;
	at->radius = ra;
	at->combined_matrix = mat;
	a->GetToken("}");
	return at;
}




Action* Action::load(MyTokenAnalyzer* a, ActionCharacter* ac) {
	a->GetToken("ACTION");
	a->GetToken("{");
	Action* act = new Action();
	a->GetToken("name");
	a->GetToken();
	hmystrcpy(act->action_name,32,0,a->Toke());
	a->GetToken("frame");
	act->all_max_frame = a->GetIntToken();
	a->GetToken("ATARIS");
	a->GetToken("{");
	a->GetToken("num");
	int atarinum = a->GetIntToken();
	for (int i=0;i<atarinum;i++) {
		AtariHantei* hantei = AtariHantei::load(a);
		act->hanteis.push_back(hantei);
	}
	a->GetToken("}");


	a->GetToken("MAKATPAIRS");
	a->GetToken("{");
	a->GetToken("num");
	int csize = a->GetIntToken();
	for (int i=0;i<csize;i++) {
		a->GetToken("MAKATPAIR");

		a->GetToken("{");
		a->GetToken("charamesh_index");
		int myindex = a->GetIntToken();
		a->GetToken("skeleton_index");
		int s_index = a->GetIntToken();
		a->GetToken("akat_index");
		int a_index = a->GetIntToken();
		act->mesh_akat_pair_index_for_load.insert(pair<int,pair<int,int>>(myindex,pair<int,int>(s_index,a_index)));

		a->GetToken("}");

	}
	a->GetToken("}");

	a->GetToken("}");
	return act;
}
CharacterActionCommand* CharacterActionCommand::load(MyTokenAnalyzer* a) {
	a->GetToken("ACCOMMAND");
	a->GetToken("{");
	CharacterActionCommand* c = new CharacterActionCommand();

	a->GetToken("command");
	c->command = a->GetIntToken();
	a->GetToken("priority");
	c->priority = a->GetIntToken();
	a->GetToken("frame");
	c->frame = a->GetIntToken();
	a->GetToken("name");
	a->GetToken();
	hmystrcpy(c->name,32,0,a->Toke());

	a->GetToken("is_reset");
	int is_reset = a->GetIntToken();
	if (is_reset) {
		c->is_reset = true;
		c->is_use = true;
	} else {
		c->is_reset = false;
		c->is_use = true;
	}
	a->GetToken("idou");
	for (int i=0;i<INPUT_MYCOMMAND_FRAME_MAX;i++) {
		c->idou[i] = a->GetUIntToken();
	}

	a->GetToken("koudou");
	for (int i=0;i<INPUT_MYCOMMAND_FRAME_MAX;i++) {
		c->koudou[i] = a->GetUIntToken();

	}
	a->GetToken("}");
	return c;
}

CharacterMesh* CharacterMesh::load(MyTokenAnalyzer* a, int index) {
	a->GetToken("CHARAMESH");
	a->GetToken("{");
	a->GetToken("myindex");
	Texture* teee = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	CharacterMesh* cm = new CharacterMesh(index,teee);
	cm->myindex = a->GetIntToken();
	a->GetToken("has_oya_mesh");
	int has_oya_mesh = a->GetIntToken();
	if (has_oya_mesh) {
		a->GetToken();
		a->GetToken();
		hmystrcpy(cm->oya_meshfilename,128,0,a->Toke());
		a->GetToken();
		a->GetToken();
		hmystrcpy(cm->oya_meshbonename,128,0,a->Toke());
		cm->has_oya_mesh = true;
	} else {
		a->GetToken();
		a->GetToken();
		a->GetToken();
		a->GetToken();
		cm->has_oya_mesh = false;
	}

	a->GetToken("is_connect_without_material_local");
	int is_connect_without_material_local = a->GetIntToken();
	if (is_connect_without_material_local) {
		cm->is_connect_without_material_local = true;
	} else {
		cm->is_connect_without_material_local = false;
	}

	a->GetToken("is_optional");
	int is_optional = a->GetIntToken();
	if (is_optional) {
		cm->is_optional = true;
	} else{
		cm->is_optional = false;
	}

	a->GetToken("is_render");
	int is_render = a->GetIntToken();
	if (is_render) {
		cm->is_render = true;
	} else {
		cm->is_render = false;
	}

	a->GetToken("kakeru");
	for (int i=0;i<16;i++) {
		cm->matrix_kakeru.m[i/4][i%4] = a->GetFloatToken();
	}

	a->GetToken("FILENAMES");
	a->GetToken("{");
	a->GetToken("num");
	int n = a->GetIntToken();
	for (int i=0;i<n;i++) {
		a->GetToken();
		a->GetToken();
		string s = a->Toke();
		cm->mesh_filenames.push_back(s);
		cm->meshs.push_back(new Mesh());
		cm->mesh_has_loaded.push_back(false);
	}

	a->GetToken("}");

	a->GetToken("SKELETONS");
	a->GetToken("{");
	a->GetToken("num");
	int nu = a->GetIntToken();
	for (int i=0;i<nu;i++) {
		a->GetToken("SKELETON");
		a->GetToken("{");
		a->GetToken("akat_filename");
		a->GetToken();
		char akat[128];
		memset(akat,0,128);
		char mesh[128];
		memset(mesh,0,128);
		char anime[128];
		memset(anime,0,128);
		hmystrcpy(akat,128,0,a->Toke());

		a->GetToken("mesh_meshname");
		a->GetToken();
		hmystrcpy(mesh,128,0,a->Toke());
		a->GetToken("mesh_animename");
		a->GetToken();
		hmystrcpy(anime,128,0,a->Toke());


		CharacterMeshSkeleton* skel = new CharacterMeshSkeleton(mesh,akat,anime);
		cm->skeletons.push_back(skel);
		a->GetToken("}");
	}
	a->GetToken("}");


	a->GetToken("}");
	return cm;
}


ActionCharacter* ActionCharacter::load(MyTokenAnalyzer* a) {

	a->GetToken("ACHARACTER");
	a->GetToken("{");
	a->GetToken("name");
	a->GetToken();
	char name[128];
	memset(name,0,128);
	hmystrcpy(name,128,0,a->Toke());
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	ActionCharacter* ac = new ActionCharacter(name,tex);

	a->GetToken("MESHS");
	a->GetToken("{");
	a->GetToken("num");
	int cnum = a->GetIntToken();
	for (int i=0;i<cnum;i++) {
		CharacterMesh* at = CharacterMesh::load(a,i);
		ac->meshs.push_back(at);
	}
	a->GetToken("}");


	a->GetToken("ACTIONS");
	a->GetToken("{");
	a->GetToken("num");
	int anum = a->GetIntToken();
	for (int i=0;i<anum;i++) {
		Action* act = Action::load(a, ac);
		ac->actions.push_back(act);
	}
	a->GetToken("}");

	a->GetToken("ACTION_TO_ACTIONS");
	a->GetToken("{");
	a->GetToken("num");
	int bnum = a->GetIntToken();
	for (int i=0;i<bnum;i++) {
		a->GetToken("ATA");
		a->GetToken("{");
		ActionToAction* ata = new ActionToAction();
		a->GetToken("command_id");
		ata->command_id = a->GetIntToken();
		a->GetToken("moto_id");
		ata->moto_action_id = a->GetIntToken();
		a->GetToken("saki_id");
		ata->saki_action_id = a->GetIntToken();
		a->GetToken("}");
		ac->action_to_actions.push_back(ata);
	}

	a->GetToken("}");





	a->GetToken("COMMANDS");
	a->GetToken("{");
	a->GetToken("num");
	cnum = a->GetIntToken();
	for (int i=0;i<cnum;i++) {
		CharacterActionCommand* ab = CharacterActionCommand::load(a);
		ac->commands.push_back(ab);
	}
	a->GetToken("}");







	a->GetToken("}");
	return ac;
}




bool ActionEditor::_forceLoadFromFile(char* filename) {

	CS::instance()->enter(CS_TASK_CS, "enter", TASKTHREADS_AIDECISION);
	CS::instance()->enter(CS_TASK_CS, "enter", TASKTHREADS_LOADDESTRUCT);
	CS::instance()->enter(CS_TASK_CS, "enter", TASKTHREADS_UPDATEMAINRENDER);
	CS::instance()->enter(CS_TASK_CS, "enter", TASKTHREADS_UPDATEANIMEFRAMENADO);
	CS::instance()->enter(CS_TASK_CS, "enter", TASKTHREADS_UPDATEPOSBUTUKARI);
	CS::instance()->enter(CS_DEVICECON_CS, "enter");
	deleteAll();
	// ここでGUIのデストラクトとLUAのAB_ENTERの呼び出しを済ませておく
	TCB test;
	MyLuaGlueSingleton::getInstance()->getColGUIs(0)->getInstance(0)->deleteAll();
	LuaTCBMaker::doTCBnow(TASKTHREADS_AIDECISION, true, "resrc/script/AE_enter.lua");
	CS::instance()->leave(CS_DEVICECON_CS, "leave");
	CS::instance()->leave(CS_TASK_CS, "leave", TASKTHREADS_UPDATEPOSBUTUKARI);
	CS::instance()->leave(CS_TASK_CS, "leave", TASKTHREADS_UPDATEANIMEFRAMENADO);
	CS::instance()->leave(CS_TASK_CS, "leave", TASKTHREADS_UPDATEMAINRENDER);
	
	
	CS::instance()->leave(CS_TASK_CS, "enter", TASKTHREADS_LOADDESTRUCT);
	CS::instance()->leave(CS_TASK_CS, "leave", TASKTHREADS_AIDECISION);
	CS::instance()->enter(CS_TASK_CS, "enter", TASKTHREADS_LOADDESTRUCT);
	MyTokenAnalyzer a;
	a.load(filename);

	a.GetToken("ACS");
	a.GetToken("{");
	a.GetToken("num");
	int num = a.GetIntToken();
	for (int i=0;i<num;i++) {
		ActionCharacter* ac = ActionCharacter::load(&a);
		characters.push_back(ac);
	}

	a.GetToken("}");
	a.deletedayo();

	CS::instance()->leave(CS_TASK_CS, "leave", TASKTHREADS_LOADDESTRUCT);
	return true;
}


bool ActionEditor::forceSaveNowCharacterToFile(char* filename) {

	CS::instance()->enter(CS_RENDERDATA_CS, "test");
	char filenamedayo[128];
	memset(filenamedayo,0,128);
	sprintf_s(filenamedayo,128,"%s.CHARA",filename);
	FILE* fp;
	// いったん消去する
	fopen_s(&fp, filenamedayo, "w");
	if ((fp == NULL) || !characters.size() ) {
		CS::instance()->leave(CS_RENDERDATA_CS, "test");
		return false;
	}
	fclose(fp);

	ActionCharacter* ac = characters[now_character_index];
	ac->write(filenamedayo);

	CS::instance()->leave(CS_RENDERDATA_CS, "test");
	return true;
}





int ActionEditors::makeInst() {
	if (aes.size()) {
		return 0;
	}
	ActionEditor* ae = new ActionEditor();
	aes.push_back(ae);
	return 0;

}

	
ActionEditor* ActionEditors::getInstance(int index) {
	if (aes.size()) {
		return aes[0];
	} else {
		makeInst();
		return aes[0];
	}
}

	
IActionEditor* ActionEditors::getInterface(int index) {
	if (aes.size()) {
		return aes[0];
	} else {
		makeInst();
		return aes[0];
	}
}