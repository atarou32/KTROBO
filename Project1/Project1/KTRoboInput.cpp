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
	nagaosi_start_time = now_time;
	for (int i=0;i<256;i++) {
		keystate[i] = 0;
		b_keystate[i] = 0;
	}

	for (int i=0;i<INPUTJYOUTAI_FRAME_MAX;i++) {
		input_jyoutai_idou[i] = 0;
		input_jyoutai_koudou[i] = 0;
	}

	command_key[INPUTJYOUTAI_KEY_INDEX_BACK] = INPUTJYOUTAI_KEY_DEFAULT_BACK;
	command_key[INPUTJYOUTAI_KEY_INDEX_FORWARD] = INPUTJYOUTAI_KEY_DEFAULT_FORWARD;
	command_key[INPUTJYOUTAI_KEY_INDEX_FRIEND] = INPUTJYOUTAI_KEY_DEFAULT_FRIEND;
	command_key[INPUTJYOUTAI_KEY_INDEX_JUMP] = INPUTJYOUTAI_KEY_DEFAULT_JUMP;
	command_key[INPUTJYOUTAI_KEY_INDEX_JYAKU_KICK] = INPUTJYOUTAI_KEY_DEFAULT_JYAKU_KICK;
	command_key[INPUTJYOUTAI_KEY_INDEX_KYOU_KICK] = INPUTJYOUTAI_KEY_DEFAULT_KYOU_KICK;
	command_key[INPUTJYOUTAI_KEY_INDEX_JYAKU_PUNCH] = INPUTJYOUTAI_KEY_DEFAULT_JYAKU_PUNCH;
	command_key[INPUTJYOUTAI_KEY_INDEX_KYOU_PUNCH] = INPUTJYOUTAI_KEY_DEFAULT_KYOU_PUNCH;
	command_key[INPUTJYOUTAI_KEY_INDEX_KAMAE] = INPUTJYOUTAI_KEY_DEFAULT_KAMAE;
	command_key[INPUTJYOUTAI_KEY_INDEX_KOKYUU] = INPUTJYOUTAI_KEY_DEFAULT_KOKYUU;

	command_key[INPUTJYOUTAI_KEY_INDEX_LEFT] = INPUTJYOUTAI_KEY_DEFAULT_LEFT;
	command_key[INPUTJYOUTAI_KEY_INDEX_PICK] = INPUTJYOUTAI_KEY_DEFAULT_PICK;
	command_key[INPUTJYOUTAI_KEY_INDEX_RIGHT] = INPUTJYOUTAI_KEY_DEFAULT_RIGHT;
	command_key[INPUTJYOUTAI_KEY_INDEX_SHAGAMI] = INPUTJYOUTAI_KEY_DEFAULT_SHAGAMI;






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


			InputMessageDispatcher::messageMake();
			break;
		case WM_KEYUP:

			InputMessageDispatcher::messageMake();
			break;
		case WM_KEYDOWN:

			InputMessageDispatcher::messageMake();
			break;

		case WM_MOUSEMOVE:

			InputMessageDispatcher::messageMake();
			break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}



INPUTGETBYMESSAGESTRUCT InputMessageDispatcher::message_getter_structs[KTROBO_INPUTGETMESSAGESTRUCT_SIZE];
volatile DWORD InputMessageDispatcher::now_time=0;
MYINPUTMESSAGESTRUCT InputMessageDispatcher::message_structs[KTROBO_INPUTMESSAGESTRUCT_SIZE];
int InputMessageDispatcher::now_message_index=0; 

void InputMessageDispatcher::Init() {
	now_time = timeGetTime();
	before_time = now_time;
	dt = 0;
	
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
void InputMessageDispatcher::commandMake() {












}


void InputMessageDispatcher::messageMakeButtonDown(int i, DWORD time) {

	InputMessageDispatcher::now_message_index = (InputMessageDispatcher::now_message_index +1) % KTROBO_INPUTMESSAGESTRUCT_SIZE;
	MYINPUTMESSAGESTRUCT* s = &InputMessageDispatcher::message_structs[now_message_index];
	s->setSENDER(KTROBO_INPUT_MESSAGE_SENDER_INPUTSYSTEM);
	s->setMSGID(KTROBO_INPUT_MESSAGE_ID_KEYDOWN);
	s->setTIME(time);
	s->setKEYSTATE(Input::keystate);
	s->setMOUSESTATE(&Input::mouse_state);
	s->setISUSE(true);
	
}


void InputMessageDispatcher::messageMakeButtonUp(int i, DWORD time) {

	InputMessageDispatcher::now_message_index = (InputMessageDispatcher::now_message_index +1) % KTROBO_INPUTMESSAGESTRUCT_SIZE;
	MYINPUTMESSAGESTRUCT* s = &InputMessageDispatcher::message_structs[now_message_index];
	s->setSENDER(KTROBO_INPUT_MESSAGE_SENDER_INPUTSYSTEM);
	s->setMSGID(KTROBO_INPUT_MESSAGE_ID_KEYUP);
	s->setTIME(time);
	s->setKEYSTATE(Input::keystate);
	s->setMOUSESTATE(&Input::mouse_state);
	s->setISUSE(true);

}


void InputMessageDispatcher::messageMake() {
	// 複数スレッドのロックはこの関数では必要ない
	// MESSAGE_CS でロックする

	CS::instance()->enter(CS_MESSAGE_CS, "enter message make");

	DWORD n_time = timeGetTime();
	if (now_time != n_time) {
		dt += n_time- now_time;

		now_time = n_time;
		if (now_time - before_time > KTROBO_INPUT_RENSYA_MAX_MILLISECOND) {
			before_time = now_time;
		
			while (dt >0) {
				// フレームを進める
				Input::input_jyoutai_index = (Input::input_jyoutai_index+1)% INPUTJYOUTAI_FRAME_MAX;
				// 該当のフラグを0にする	
				Input::input_jyoutai_idou[Input::input_jyoutai_index] = 0;
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] = 0;
				
				dt -= 1000;
				if (dt <0) {
					dt = 0;
				}
				// 何回も回る場合はフレーム落ち？になる
			}


			// フレームが進んだので　コマンド解釈処理に入る
			commandMake();

		}
	}

	// 現在のinput_jyoutai_indexのフラグを調べる
	for (int i=0;i<256;i++) {
		if (Input::keystate[i] & KTROBO_INPUT_BUTTON_PRESSED) {
			// コマンドではなくキー自体はpressedされている場合のみでは何もおきない
		} else if(Input::keystate[i] & KTROBO_INPUT_BUTTON_DOWN) {
			// 押された瞬間はメッセージを飛ばす
			messageMakeButtonDown(i, n_time);
			// 押されたのでながおし判定を始める
			Input::nagaosi_keycode = i;
			Input::nagaosi_start_time = n_time;
			Input::nagaosi_time = 0;
			break;
		} else if(Input::keystate[i] & KTROBO_INPUT_BUTTON_UP) {
			// 離された瞬間もメッセージを飛ばす
			messageMakeButtonUp(i, n_time);
			if (i == Input::nagaosi_keycode) {
				Input::nagaosi_keycode = 0;
				Input::nagaosi_start_time = 0;
				Input::nagaosi_time = 0;
			}
			break;
		}
	}

	// ながおし判定の時間を計算してメッセージを作る周期になったら作る
	if (Input::nagaosi_keycode != 0) {
		DWORD dtime = n_time - Input::nagaosi_start_time;
		if (dtime > KTROBO_INPUT_RENSYA_START_MILLISECOND) {
			if (dtime > KTROBO_INPUT_RENSYA_PER_MILLISECOND + KTROBO_INPUT_RENSYA_START_MILLISECOND) {
				Input::nagaosi_start_time = n_time - KTROBO_INPUT_RENSYA_START_MILLISECOND;
				messageMakeButtonDown(Input::nagaosi_keycode, n_time);
			}
		}
	}

	for (int i=0; i<INPUTJYOUTAI_KEY_INDEX_MAX;i++) {
		if (Input::keystate[Input::command_key[i]] & KTROBO_INPUT_BUTTON_PRESSED) {
			// pressed のときはdownのフラグを立てる
			switch(i) {
			case	INPUTJYOUTAI_KEY_INDEX_KOKYUU:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KOKYUU_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_LEFT:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_LEFT_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_RIGHT:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_RIGHT_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_FORWARD:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_FORWARD_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_BACK:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_BACK_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_KAMAE:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KAMAE_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_JUMP:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_JUMP_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_SHAGAMI:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_SHAGAMI_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_PICK:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_PICK_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_FRIEND:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_FRIEND_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_JYAKU_PUNCH:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_JYAKU_PUNCH_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_JYAKU_KICK:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_JYAKU_KICK_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_KYOU_PUNCH:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KYOU_PUNCH_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_KYOU_KICK:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KYOU_KICK_DOWN;
				break;
			default:
				break;
			}

		} else if(Input::keystate[Input::command_key[i]] & KTROBO_INPUT_BUTTON_DOWN) {
			// down のときはdownのフラグを立てる
				switch(i) {
			case	INPUTJYOUTAI_KEY_INDEX_KOKYUU:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KOKYUU_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_LEFT:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_LEFT_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_RIGHT:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_RIGHT_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_FORWARD:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_FORWARD_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_BACK:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_BACK_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_KAMAE:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KAMAE_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_JUMP:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_JUMP_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_SHAGAMI:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_SHAGAMI_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_PICK:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_PICK_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_FRIEND:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_FRIEND_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_JYAKU_PUNCH:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_JYAKU_PUNCH_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_JYAKU_KICK:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_JYAKU_KICK_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_KYOU_PUNCH:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KYOU_PUNCH_DOWN;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_KYOU_KICK:
				Input::input_jyoutai_koudou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KYOU_KICK_DOWN;
				break;
			default:
				break;
			}




		} else if(Input::keystate[Input::command_key[i]] & KTROBO_INPUT_BUTTON_UP) {
			// up のときはupのフラグを立てる
			switch(i) {
			case	INPUTJYOUTAI_KEY_INDEX_KOKYUU:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KOKYUU_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_LEFT:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_LEFT_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_RIGHT:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_RIGHT_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_FORWARD:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_FORWARD_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_BACK:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_BACK_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_KAMAE:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KAMAE_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_JUMP:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_JUMP_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_SHAGAMI:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_SHAGAMI_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_PICK:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_PICK_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_FRIEND:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_FRIEND_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_JYAKU_PUNCH:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_JYAKU_PUNCH_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_JYAKU_KICK:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_JYAKU_KICK_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_KYOU_PUNCH:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KYOU_PUNCH_UP;
				break;
			case	INPUTJYOUTAI_KEY_INDEX_KYOU_KICK:
				Input::input_jyoutai_idou[Input::input_jyoutai_index] |= INPUTJYOUTAI_KYOU_KICK_UP;
				break;
			default:
				break;
			}
		}
	}
	// b_keystate にkeystateを設定する（更新）
	for (int i=0;i<256;i++) {
		Input::b_keystate[i] = Input::keystate[i];
	}

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



volatile char Input::keystate[256];
volatile MOUSE_STATE Input::mouse_state;
volatile char Input::b_keystate[256];
volatile MOUSE_STATE Input::b_mousestate;
volatile char Input::nagaosi_keycode;// 最後に押されたボタンの仮想キーコード
volatile DWORD Input::nagaosi_time;// 押されてからたった時間 

volatile long Input::input_jyoutai_idou[INPUTJYOUTAI_FRAME_MAX];
volatile long Input::input_jyoutai_koudou[INPUTJYOUTAI_FRAME_MAX];
volatile unsigned int Input::input_jyoutai_index=0;
volatile char command_key[INPUTJYOUTAI_KEY_INDEX_MAX];

volatile DWORD InputMessageDispatcher::before_time=0;
volatile DWORD InputMessageDispatcher::dt=0;
volatile DWORD Input::nagaosi_start_time=0; // 押され始めた時間