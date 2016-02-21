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






}

void ActionEditor::setCommandToCharacter(int character_id, int command_id, char* command, char* name, int priority) {
	CS::instance()->enter(CS_RENDERDATA_CS, "enter");
	
	if (character_id >= 0 && character_id < characters.size()) {
		ActionCharacter* ac = characters[character_id];
		
		CharacterActionCommand* new_command = new CharacterActionCommand();
		new_command->priority = priority;
		new_command->command = command_id;
		hmystrcpy(new_command->name,32,0,name);
		analyzeStrCommand(command, new_command);
		ac->commands.push_back(new_command);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "leave");
}

int ActionEditor::makeActiontoAction(int character_id, char* action_name_moto, char* action_name_saki, int command_id, bool is_modoru) {

	return 0;
}

void ActionEditor::clearActionToAction(int character_id, int action_to_action_id) {


}

void ActionEditor::deleteAll() {



}

void ActionEditor::saveNowToFile(char* filename) {



}

void ActionEditor::forceSaveNowToFile(char* filename) {



}

void ActionEditor::loadFromFile(char* filename) {




}



void ActionEditor::forceLoadFromFile(char* filename) {




}


void ActionEditor::saveNowCharacterToFile(char* filename) {




}


void ActionEditor::forceSaveNowCharacterToFile(char* filename) {






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