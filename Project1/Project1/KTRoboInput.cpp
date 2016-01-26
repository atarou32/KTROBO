#include "KTRoboInput.h"
#include "mmsystem.h"
#include "KTRoboCS.h"

using namespace KTROBO;

DWORD Input::now_time=0;
DWORD Input::last_time=0;

Input::Input(void)
{
	
	
}


Input::~Input(void)
{
}


void Input::Init() {
	now_time = timeGetTime();
	last_time = now_time;
}

LRESULT CALLBACK Input::myWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {

   PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;
		case WM_INPUT:


			break;

		case WM_MOUSEMOVE:

			break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;








}



INPUTGETBYMESSAGESTRUCT InputMessageDispatcher::message_getter_structs[96];
DWORD InputMessageDispatcher::now_time=0;
MYINPUTMESSAGESTRUCT InputMessageDispatcher::message_structs[128];
int InputMessageDispatcher::now_message_index=0; 

void InputMessageDispatcher::Init() {
	now_time = timeGetTime();
}



void InputMessageDispatcher::_messageDispatch(MYINPUTMESSAGESTRUCT* message, INPUTGETBYMESSAGESTRUCT* get_input, DWORD time) {

	if (get_input->handleMessage(message->getMSGID(), (void*)message, time)) {
		return;
	}

	if(get_input->child) {
		InputMessageDispatcher::_messageDispatch(message, get_input->child, time);
	}


}


void InputMessageDispatcher::messageDispatch() {
	// 外部で複数スレッドをロックしてこの関数を呼び出す
	// MESSAGE_CS でロックする
	DWORD time = timeGetTime();
	CS::instance()->enter(CS_MESSAGE_CS, "enter message dispatch");
	int mes_index = (InputMessageDispatcher::now_message_index+1)% KTROBO_INPUTMESSAGESTRUCT_SIZE;
	for (int i=0;i<KTROBO_INPUTMESSAGESTRUCT_SIZE;i++) {
		int index = (mes_index + i) % KTROBO_INPUTMESSAGESTRUCT_SIZE;
		MYINPUTMESSAGESTRUCT* s = &InputMessageDispatcher::message_structs[i];
		if (s->getISUSE()) {
			// 処理を行う
			for (int i=0;i<KTROBO_INPUTGETMESSAGESTRUCT_SIZE;i++) {
				if (!InputMessageDispatcher::message_getter_structs[i].getParent() && message_getter_structs[i].setIsUse()) {					
					_messageDispatch(s, &message_getter_structs[i], time);
				}
			}

			s->setISUSE(false);
		}
	}


	CS::instance()->leave(CS_MESSAGE_CS, "leave message dispatch");
}
void InputMessageDispatcher::messageMake() {
	// 複数スレッドのロックはこの関数では必要ない
	// MESSAGE_CS でロックする

	CS::instance()->enter(CS_MESSAGE_CS, "enter message make");

	CS::instance()->leave(CS_MESSAGE_CS, "leave message make");
}


INPUTGETBYMESSAGESTRUCT* InputMessageDispatcher::getGetMessageStruct(INPUTSHORICLASS* cl) {
	for (int i=0;i<KTROBO_INPUTGETMESSAGESTRUCT_SIZE;i++) {
		if (!InputMessageDispatcher::message_getter_structs[i].getIsUse()) {					
			if (message_getter_structs[i].impl == cl) {
				return &message_getter_structs[i];
			}
		}
	}
	return NULL;
}
void InputMessageDispatcher::registerImpl(INPUTSHORICLASS* cl, INPUTSHORICLASS* parent, INPUTSHORICLASS* child) {
	

	CS::instance()->enter(CS_MESSAGE_CS, "enter message make");

	for (int i=0;i<KTROBO_INPUTGETMESSAGESTRUCT_SIZE;i++) {
		if (!InputMessageDispatcher::message_getter_structs[i].getIsUse()) {
			INPUTGETBYMESSAGESTRUCT* parent_struct=NULL;
			INPUTGETBYMESSAGESTRUCT* child_struct = NULL;
			if (parent) {
				parent_struct = getGetMessageStruct(parent);
				if (parent_struct) {
					parent_struct->setChild(&message_getter_structs[i]);
				}
			}
			if (child) {
				child_struct = getGetMessageStruct(child);
				if (child_struct) {
					child_struct->setChild(&message_getter_structs[i]);
				}
			}
			message_getter_structs[i].setParam(cl,parent_struct, child_struct);
			message_getter_structs[i].setIsUse(true);
			CS::instance()->leave(CS_MESSAGE_CS, "leave message make");
			return;
		}
	}
	CS::instance()->leave(CS_MESSAGE_CS, "leave message make");
	throw new GameError(KTROBO::FATAL_ERROR, "to many registerimpl");



	
}



void InputMessageDispatcher::unregisterImpl(INPUTSHORICLASS* cl) {

	
	CS::instance()->enter(CS_MESSAGE_CS, "enter message make");
	INPUTGETBYMESSAGESTRUCT* str = getGetMessageStruct(cl);
	if (str) {
		INPUTGETBYMESSAGESTRUCT* p = str->getParent();
		INPUTGETBYMESSAGESTRUCT* c = str->getChild();
		if (p) {
			p->setChild(c);
		}
		if (c) {
			c->setParent(p);
		}
		str->setParam(NULL,NULL,NULL);
		str->setIsUse(false);
	}
	CS::instance()->leave(CS_MESSAGE_CS, "leave message make");


}
