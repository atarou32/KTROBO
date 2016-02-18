#include "KTRoboActionEditor.h"
#include "KTRoboLuaCollection.h"
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


}


int ActionEditor::setHonMesh(int character_id, char* mesh_filename, char* oya_mesh_filename, bool is_connect_without_matrial_local, YARITORI MYMATRIX* mat) {



}


int ActionEditor::setOnajiMesh(int character_id, int hon_mesh_id, char* mesh_filename) {



}


void ActionEditor::toggleMeshOptional(int character_id, int hon_mesh_id) {
	// そのキャラクターにとってメッシュが特定の状態でつけられるかどうかをトグルする

	
	
	
	
	
}	
	
	
int ActionEditor::setAnimeAndAkatFile(int character_id, int hon_mesh_id, char* anime_filename, char* akat_filename) {





}


int ActionEditor::getAkatID(int character_id, int hon_mesh_id, char* anime_filename, char* akat_filename) {



}

int ActionEditor::getAkatNum(int character_id, int akat_id) {



}


char* ActionEditor::getAkatName(int character_id, int akat_id, int akat_index) {



}


int ActionEditor::makeAction(int character_id, char* action_name) {

}

int ActionEditor::setAkatToAction(int character_id, int action_id, int akat_id, int akat_index) {

}

void ActionEditor::setNowCharacterId(int character_id) {


}
int ActionEditor::getNowCharacterId() {


}

void ActionEditor::setNowAkat(int character_id, int akat_id, int akat_index) {



}


void ActionEditor::getNowAkatIndex(int character_id, OUT_ int* akat_id, OUT_ int* akat_index) {



}


void ActionEditor::setNowAction(int character_id, int action_id) {



}


void ActionEditor::togglePlayNowAction() {




}


void ActionEditor::togglePlayNowAkat() {



}

void ActionEditor::setCommandToCharacter(int character_id, int command_id, char* command) {



}

int ActionEditor::makeActiontoAction(int character_id, char* action_name_moto, char* action_name_saki, int command_id, bool is_modoru) {


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