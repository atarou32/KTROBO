#ifndef KTROBOINPUT_H
#define KTROBOINPUT_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "KTRoboClock.h"
#include "winuser.h"


namespace KTROBO {


#define KTROBO_INPUT_BUTTON_DOWN 0x01
#define KTROBO_INPUT_BUTTON_PRESSED 0x02
#define KTROBO_INPUT_BUTTON_UP 0x02
#define KTROBO_INPUT_RENSYA_MAX_MILLISECOND 30 // 1フレームのミリセコンド
#define KTROBO_INPUT_RENSYA_START_MILLISECOND 500
#define KTROBO_INPUT_RENSYA_PER_MILLISECOND 200 // ながおし時にDOWNメッセージが送られる周期のミリセコンド


class INPUTSHORICLASS {
public:
	virtual bool handleMessage(int msg, void* data, DWORD time)=0;
};


struct INPUTGETBYMESSAGESTRUCT {
public:
	INPUTGETBYMESSAGESTRUCT* parent;
	INPUTGETBYMESSAGESTRUCT* child;
	bool is_use;
	INPUTSHORICLASS* impl;

	INPUTGETBYMESSAGESTRUCT* getParent() {
		return parent;
	}
	INPUTGETBYMESSAGESTRUCT* getChild() {
		return child;
	}

	void setParent(INPUTGETBYMESSAGESTRUCT* p) {
		parent = p;
	}

	void setChild(INPUTGETBYMESSAGESTRUCT* p) {
		child = p;
	}

	bool handleMessage(int msg, void* data, DWORD time) {
		// メッセージを受け取って処理を行い、そこでチェインをとめるならばtrueを返す
		return impl->handleMessage(msg, data, time);
	}

	INPUTGETBYMESSAGESTRUCT() {
		this->impl = 0;
		this->parent = 0;
		this->child = 0;
		this->is_use = false;
	}
	void setIsUse(bool t) {
		is_use = t;
	}

	bool getIsUse() {return is_use;}

	void setParam(INPUTSHORICLASS* impl, INPUTGETBYMESSAGESTRUCT* parent, INPUTGETBYMESSAGESTRUCT* child) {
		this->impl = impl;
		this->parent = parent;
		this->child = child;
		this->is_use = true;
	}

};


struct MOUSE_STATE {
public:
	char mouse_r_button;
	char mouse_l_button;
	char mouse_m_button;

	DWORD mouse_x;
	DWORD mouse_y;
	DWORD mouse_dx;
	DWORD mouse_dy;
	MOUSE_STATE() {
		mouse_r_button = 0;
		mouse_l_button = 0;
		mouse_m_button = 0;
		mouse_x = 0;
		mouse_y = 0;
		mouse_dx = 0;
		mouse_dy = 0;
	}

	MOUSE_STATE& operator=(MOUSE_STATE& value) {
		this->mouse_dx = value.mouse_dx;
		this->mouse_dy = value.mouse_dy;
		this->mouse_l_button = value.mouse_l_button;
		this->mouse_m_button = value.mouse_m_button;
		this->mouse_r_button = value.mouse_r_button;
		this->mouse_x = value.mouse_x;
		this->mouse_y = value.mouse_y;
        return *this;
    }
};

class MYINPUTMESSAGESTRUCT : public MYMESSAGESTRUCT{
public:
	float X;
	float Y;
	float DX;
	float DY;
	volatile char keystate[256];
	volatile MOUSE_STATE mousestate;
	MYINPUTMESSAGESTRUCT() : MYMESSAGESTRUCT() {
		X = 0;
		Y = 0;
		DX = 0;
		DY = 0;
		for (int i=0;i<256;i++) {
			keystate[i] = 0;
		}
		mousestate;
	}

	float getX() {return X;}
	float getY() {return Y;}
	float getDX() {return DX;}
	float getDY() {return DY;}
	volatile char* getKEYSTATE() {return keystate;}
	volatile MOUSE_STATE* getMOUSESTATE() {return &mousestate;}

	void setX(float x) {X=x;}
	void setY(float y) {Y=y;}
	void setDX(float dx) {DX=dx;}
	void setDY(float dy) {DY=dy;}
	void setKEYSTATE(volatile char* y) {
		for (int i=0;i<256;i++) {
			keystate[i]=y[i];
		}
	}
	void setMOUSESTATE(volatile MOUSE_STATE* m) {mousestate=*m;}

};


#define KTROBO_INPUT_MESSAGE_SENDER_INPUTSYSTEM 0

#define KTROBO_INPUT_MESSAGE_ID_NONE 0
#define KTROBO_INPUT_MESSAGE_ID_UNKNOWN 1
#define KTROBO_INPUT_MESSAGE_ID_COMMAND 2
#define KTROBO_INPUT_MESSAGE_ID_KEYUP 3
#define KTROBO_INPUT_MESSAGE_ID_KEYDOWN 4

class MYMESSAGESTRUCT {
public:
	int msg_id;
	int sender;
	int data_int;// コマンドのIDに使う！
	void* data;
	bool is_use;
	DWORD time;
	MYMESSAGESTRUCT() {
		msg_id = 0;
		sender = 0;
		data = 0;
		is_use = false;
		time = 0;
		data_int = 0;
	}
	DWORD getTIME() {return time;}
	int getMSGID() {return msg_id;}
	int getSENDER() {return sender;}
	void* getData() {return data;}
	int getDataINT() {return data_int;}
	bool getISUSE() {return is_use;}

	void setTIME(DWORD t) {time =t;}
	void setMSGID(int m) {msg_id=m;}
	void setSENDER(int s) {sender=s;}
	void setData(void* d) {data=d;}
	void setDataINT(int t) {data_int = t;}
	void setISUSE(bool i) {is_use=i;}
};

#define KTROBO_INPUTMESSAGESTRUCT_SIZE 128
#define KTROBO_INPUTGETMESSAGESTRUCT_SIZE 96
class InputMessageDispatcher {
public:
	// inputを受け取って動作する部分というのはそんなに多くない気がする
	// これを利用したらインプットをメインにしたゲームが作れるようになるかもしれない
	// とりあえず96で設定してみた
	// 入力処理でバッファをどのくらいに設定するか
	// また入力がどんどんくるときまたは　他のスレッドが遅くなっているときに
	// 入力バッファにたまった入力はどうするのか
	// 入力については消失するようにする
	// つまり上書きする
	// そうすれば遅くなっているときにボタンを押しても反応しないことがあるが
	// 二度押してもらえば大丈夫なはず・・・
	// バッファにかんしては128設けてみる

	static INPUTGETBYMESSAGESTRUCT message_getter_structs[KTROBO_INPUTGETMESSAGESTRUCT_SIZE];
	static volatile DWORD now_time;
	static volatile DWORD before_time;
	static volatile DWORD dt;

	static MYINPUTMESSAGESTRUCT message_structs[KTROBO_INPUTMESSAGESTRUCT_SIZE];
	static int now_message_index; 
	// 現在のバッファのインデックス このインデックス+1のインデックスからインデックスの位置までのオブジェクトでis_useがtrueのもの
	// が処理される
	static void Init();
	static void messageDispatch();
	static void messageMake();
	static void messageMakeButtonDown(int i, DWORD ntime);
	static void messageMakeButtonUp(int i, DWORD ntime);

	static void commandMake();
	static void _messageDispatch(MYINPUTMESSAGESTRUCT* message, INPUTGETBYMESSAGESTRUCT* get_input, DWORD time);
	static void registerImpl(INPUTSHORICLASS* cl, INPUTSHORICLASS* parent, INPUTSHORICLASS* child);
	static void unregisterImpl(INPUTSHORICLASS* cl);
	static INPUTGETBYMESSAGESTRUCT* getGetMessageStruct(INPUTSHORICLASS* cl);
};


// 同時押しの実装とかどうすればいいのか
// 同時押しもコマンドと実装する
// 特定のフレーム内に該当のボタンが二つあるいは三つ登場していれば判定OKとする
// コマンドの実装とかどうすればいいのか
// フレームごとにインプットの状態を保存しておき
// フレームごとにコマンドの判定を行う　ここ大事　スレッドのループ（１ｍｓぐらい）ではなく　フレームの時間単位で（30msぐらい？)　
// コマンドによって、
// 次のコマンドにつながるようなコマンドの場合はコマンドを消さない
// 基本移動や行動のコマンドはコマンドを消さない ジャンプしゃがみ、前移動後ろ移動横移動などガード
// コマンドは同じコマンドが何回も出るのを防ぐためにコマンドを全クリアする

// ためパンチの実装について
// ボタンを長く押しているかどうかの判定はどうしたらいいのか
// 特定のフレームの間押されているかどうかをコマンドとして登録すればよい
// ボタンが離されたときに発動するか、特定の時間おされたときに発動するかは　コマンドによる
// ため状態になるかどうかの判定にコマンドを使って、ニュートラルをため状態解除に使う
// ため状態時にボタン離しコマンドを検知したら技を発動させる
// コマンドの判定最大フレーム数について
// 50フレームとしてみよう (50*30 = 1500 １．５秒）


// 連打の実装について
// ボタン連打について
// ボタン連打の場合はコマンドを消さない？ いや消す
// たぶんニュートラルの状態のコマンドがボタン連打をやめたときに流れて、
// そのときにキャラクターの技だし状態が解除されるようになっているはず
// つまりキャラクターの状態遷移の問題になる

// コマンドを解釈する周期はいつにしたらいいのだろう
// １フレームごとに行う



#define INPUTJYOUTAI_KEY_DEFAULT_KOKYUU VK_LCONTROL
#define INPUTJYOUTAI_KEY_DEFAULT_LEFT 'A'
#define INPUTJYOUTAI_KEY_DEFAULT_RIGHT 'D'
#define INPUTJYOUTAI_KEY_DEFAULT_FORWARD 'W'
#define INPUTJYOUTAI_KEY_DEFAULT_BACK 'S'
#define INPUTJYOUTAI_KEY_DEFAULT_KAMAE 'Q'
#define INPUTJYOUTAI_KEY_DEFAULT_JUMP VK_SPACE
#define INPUTJYOUTAI_KEY_DEFAULT_SHAGAMI VK_LSHIFT

#define INPUTJYOUTAI_KEY_DEFAULT_PICK 'E'
#define INPUTJYOUTAI_KEY_DEFAULT_FRIEND 'C'
#define INPUTJYOUTAI_KEY_DEFAULT_JYAKU_PUNCH 'R'
#define INPUTJYOUTAI_KEY_DEFAULT_JYAKU_KICK 'F'
#define INPUTJYOUTAI_KEY_DEFAULT_KYOU_PUNCH 'T'
#define INPUTJYOUTAI_KEY_DEFAULT_KYOU_KICK 'G'

#define INPUTJYOUTAI_KEY_INDEX_KOKYUU 0
#define INPUTJYOUTAI_KEY_INDEX_LEFT 1
#define INPUTJYOUTAI_KEY_INDEX_RIGHT 2
#define INPUTJYOUTAI_KEY_INDEX_FORWARD 3
#define INPUTJYOUTAI_KEY_INDEX_BACK 4
#define INPUTJYOUTAI_KEY_INDEX_KAMAE 5
#define INPUTJYOUTAI_KEY_INDEX_JUMP 6
#define INPUTJYOUTAI_KEY_INDEX_SHAGAMI 7
#define INPUTJYOUTAI_KEY_INDEX_PICK 8
#define INPUTJYOUTAI_KEY_INDEX_FRIEND 9
#define INPUTJYOUTAI_KEY_INDEX_JYAKU_PUNCH 10
#define INPUTJYOUTAI_KEY_INDEX_JYAKU_KICK 11
#define INPUTJYOUTAI_KEY_INDEX_KYOU_PUNCH 12
#define INPUTJYOUTAI_KEY_INDEX_KYOU_KICK 13
#define INPUTJYOUTAI_KEY_INDEX_MAX 14

// 移動構造体
#define INPUTJYOUTAI_FRAME_MAX 50
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



class Input
{
private:
	static DWORD now_time;
	static DWORD last_time;
	static Clock clock;
public:
	static volatile char keystate[256];
	static volatile MOUSE_STATE mouse_state;
	static volatile char b_keystate[256];
	static volatile MOUSE_STATE b_mousestate;
	static volatile char nagaosi_keycode;// 最後に押されたボタンの仮想キーコード // inputtext用
	static volatile DWORD nagaosi_start_time; // 押され始めた時間
	static volatile DWORD nagaosi_time;// 押されてからたった時間 // inputtext用
	static volatile long input_jyoutai_idou[INPUTJYOUTAI_FRAME_MAX];
	static volatile long input_jyoutai_koudou[INPUTJYOUTAI_FRAME_MAX];
	static volatile unsigned int input_jyoutai_index;

	static volatile char command_key[INPUTJYOUTAI_KEY_INDEX_MAX];

public:
	Input(void);
	~Input(void);

	static void Init(void);
	static void setCommandKey(char* c) {
		for (int i=0;i<INPUTJYOUTAI_KEY_INDEX_MAX;i++) {
			command_key[i] = c[i];
		}
	}

	static LRESULT CALLBACK myWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
};

}

#endif