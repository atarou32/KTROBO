#ifndef KTROBOINPUT_H
#define KTROBOINPUT_H

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "KTRoboClock.h"



namespace KTROBO {


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
	char* keystate;
	MOUSE_STATE mousestate;
	MYINPUTMESSAGESTRUCT() : MYMESSAGESTRUCT() {
		X = 0;
		Y = 0;
		DX = 0;
		DY = 0;
		keystate = 0;
		mousestate;
	}

	float getX() {return X;}
	float getY() {return Y;}
	float getDX() {return DX;}
	float getDY() {return DY;}
	char* getKEYSTATE() {return keystate;}
	MOUSE_STATE* getMOUSESTATE() {return &mousestate;}

	void setX(float x) {X=x;}
	void setY(float y) {Y=y;}
	void setDX(float dx) {DX=dx;}
	void setDY(float dy) {DY=dy;}
	void setKEYSTATE(char* y) {keystate=y;}
	void setMOUSESTATE(MOUSE_STATE* m) {mousestate=*m;}

};

class MYMESSAGESTRUCT {
public:
	int msg_id;
	int sender;
	void* data;
	bool is_use;
	DWORD time;
	MYMESSAGESTRUCT() {
		msg_id = 0;
		sender = 0;
		data = 0;
		is_use = false;
		time = 0;
	}
	DWORD getTIME() {return time;}
	int getMSGID() {return msg_id;}
	int getSENDER() {return sender;}
	void* getData() {return data;}
	bool getISUSE() {return is_use;}

	void setTIME(DWORD t) {time =t;}
	void setMSGID(int m) {msg_id=m;}
	void setSENDER(int s) {sender=s;}
	void setData(void* d) {data=d;}
	void setISUSE(bool i) {is_use=i;}
};

#define KTROBO_INPUTMESSAGESTRUCT_SIZE 128
#define KTROBO_INPUTGETMESSAGESTRUCT_SIZE 96
class InputMessageDispatcher {
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
	static DWORD now_time;
	static MYINPUTMESSAGESTRUCT message_structs[KTROBO_INPUTMESSAGESTRUCT_SIZE];
	static int now_message_index; 
	// 現在のバッファのインデックス このインデックス+1のインデックスからインデックスの位置までのオブジェクトでis_useがtrueのもの
	// が処理される
	static void Init();
	static void messageDispatch();
	static void messageMake();
	static void _messageDispatch(MYINPUTMESSAGESTRUCT* message, INPUTGETBYMESSAGESTRUCT* get_input, DWORD time);
	static void registerImpl(INPUTSHORICLASS* cl, INPUTSHORICLASS* parent, INPUTSHORICLASS* child);
	static void unregisterImpl(INPUTSHORICLASS* cl);
	static INPUTGETBYMESSAGESTRUCT* getGetMessageStruct(INPUTSHORICLASS* cl);
};



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

public:
	Input(void);
	~Input(void);
	static void Init(void);
	
	static LRESULT CALLBACK myWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
};

}

#endif