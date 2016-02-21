#include "KTRoboActionEditor.h"
#include "KTRoboLuaCollection.h"
#include "tolua_glue/tolua_glue.h"
#include "MyTokenAnalyzer.h"


using namespace KTROBO;
ActionEditor::ActionEditor(void) : Scene("action_editor", 13) 
{
}


ActionEditor::~ActionEditor(void)
{
}

void ActionEditor::enter() {
	LuaTCBMaker::makeTCB(TASKTHREADS_AIDECISION, true, "resrc/script/AE_enter.lua");
	Scene::enter();
}

void ActionEditor::leave() {
	Scene::leave();
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

	return 0;

}


char* ActionEditor::getAkatName(int character_id, int hon_mesh_id, int skeleton_id, int akat_index) {

	return "";

}


int ActionEditor::makeAction(int character_id, char* action_name) {
	return 0;
}

int ActionEditor::setAkatToAction(int character_id, int action_id, int hon_mesh_id, int skeleton_id, int akat_index) {
	return 0;
}

void ActionEditor::setNowCharacterId(int character_id) {


}
int ActionEditor::getNowCharacterId() {

	return 0;
}

void ActionEditor::setNowAkat(int character_id, int hon_mesh_id, int skeleton_id,  int akat_index) {



}


void ActionEditor::getNowAkatIndex(int character_id,  int hon_mesh_id, int skeleton_id,  OUT_ int* akat_index) {



}


void ActionEditor::setNowAction(int character_id, int action_id) {



}


void ActionEditor::togglePlayNowAction() {




}


void ActionEditor::togglePlayNowAkat() {



}

void ActionEditor::setCommandToCharacter(int character_id, int command_id, char* command, char* name, int priority) {



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