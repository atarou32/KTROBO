#include "KTRoboActionEditor.h"
#include "KTRoboLuaCollection.h"
#include "tolua_glue/tolua_glue.h"
#include "MyTokenAnalyzer.h"


using namespace KTROBO;
ActionEditor::ActionEditor(void) : Scene("action_editor", 13) 
{
	frame_of_action = 0;
	anime_action = false;

	frame_of_akat = 0;
	anime_akat = false;
	now_character_index = 0;
	now_character_text = 0;




	do_force_save = false;
	save_done = true;
	do_force_load = false;
	load_done = true;
	save_result = true;
	load_result = true;
}


ActionEditor::~ActionEditor(void)
{
}

void ActionEditor::enter() {
	frame_of_action = 0;
	anime_action = false;

	frame_of_akat = 0;
	anime_akat = false;
	Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
	now_character_index = 0;
	now_character_text = tex->getRenderText("defaultcharacter",0,0,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT,
		KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT*32,KTROBO_ACTIONEDITOR_TAB_TEXT_WIDTH_HEIGHT);
	LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, "resrc/script/AE_enter.lua");
	Scene::enter();
}

void ActionEditor::leave() {
	frame_of_action = 0;
	anime_action = false;

	frame_of_akat = 0;
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


}

void ActionEditor::renderhojyoIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {

}

void ActionEditor::aiIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {


}

void ActionEditor::posbutukariIMPL(Task* task, TCB* thisTCB, Graphics* g, lua_State* l, Game* game) {


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


int ActionEditor::setHonMesh(int character_id, char* mesh_filename, char* oya_mesh_filename, bool is_connect_without_matrial_local, YARITORI MYMATRIX* mat) {

	int ans;
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	if (character_id >=0 && character_id < characters.size()) {
		ActionCharacter* cha = characters[character_id];
		int ans = cha->getMeshs()->size();
		CharacterMesh* mesh = new CharacterMesh();
		hmystrcpy(mesh->oya_meshfilename,128,0,oya_mesh_filename);
		string filename = mesh_filename;
		mesh->is_akat_loaded = false;
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
		Texture* tex = MyLuaGlueSingleton::getInstance()->getColTextures(0)->getInstance(0);
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
				if (akat_index >= 0 && akat_index < skel->akats.size()) {
					Akat* aka = skel->akats[akat_index];
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
void ActionCharacter::write(char* filename) {

	KTROBO::mylog::writelog(filename, "ACHARACTER{\n");
	KTROBO::mylog::writelog(filename, "name=\"%s\";\n",this->character_name);
	KTROBO::mylog::writelog(filename, "ACTIONS{\n");
	KTROBO::mylog::writelog(filename, "action_num=%d;\n",this->actions.size());
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

	KTROBO::mylog::writelog(filename, "MESHS{\n");
	KTROBO::mylog::writelog(filename, "num=%d;\n", meshs.size());
	int csize = meshs.size();
	for (int i=0;i<csize;i++){
		meshs[i]->write(filename);
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
	KTROBO::mylog::writelog(filename, "characters_num=%d;\n",characters.size());
	int size = characters.size();
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	for (int i=0;i<size;i++) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter");
		characters[i]->write(filename);
		CS::instance()->leave(CS_RENDERDATA_CS, "leave");
	}
	KTROBO::mylog::writelog(filename, "}\n");
	
}


bool ActionEditor::_forceLoadFromFile(char* filename) {







}


bool ActionEditor::forceSaveNowCharacterToFile(char* filename) {

	CS::instance()->enter(CS_RENDERDATA_CS, "test");

	FILE* fp;
	// いったん消去する
	fopen_s(&fp, filename, "w");
	if ((fp == NULL) || !characters.size() ) {
		CS::instance()->leave(CS_RENDERDATA_CS, "test");
		return false;
	}
	fclose(fp);

	ActionCharacter* ac = characters[now_character_index];
	ac->write(filename);

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