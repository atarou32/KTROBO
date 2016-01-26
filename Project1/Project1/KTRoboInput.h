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
		// ���b�Z�[�W���󂯎���ď������s���A�����Ń`�F�C�����Ƃ߂�Ȃ��true��Ԃ�
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
	// input���󂯎���ē��삷�镔���Ƃ����̂͂���Ȃɑ����Ȃ��C������
	// ����𗘗p������C���v�b�g�����C���ɂ����Q�[��������悤�ɂȂ邩������Ȃ�
	// �Ƃ肠����96�Őݒ肵�Ă݂�
	// ���͏����Ńo�b�t�@���ǂ̂��炢�ɐݒ肷�邩
	// �܂����͂��ǂ�ǂ񂭂�Ƃ��܂��́@���̃X���b�h���x���Ȃ��Ă���Ƃ���
	// ���̓o�b�t�@�ɂ��܂������͂͂ǂ�����̂�
	// ���͂ɂ��Ă͏�������悤�ɂ���
	// �܂�㏑������
	// ��������Βx���Ȃ��Ă���Ƃ��Ƀ{�^���������Ă��������Ȃ����Ƃ����邪
	// ��x�����Ă��炦�Α��v�Ȃ͂��E�E�E
	// �o�b�t�@�ɂ��񂵂Ă�128�݂��Ă݂�

	static INPUTGETBYMESSAGESTRUCT message_getter_structs[KTROBO_INPUTGETMESSAGESTRUCT_SIZE];
	static DWORD now_time;
	static MYINPUTMESSAGESTRUCT message_structs[KTROBO_INPUTMESSAGESTRUCT_SIZE];
	static int now_message_index; 
	// ���݂̃o�b�t�@�̃C���f�b�N�X ���̃C���f�b�N�X+1�̃C���f�b�N�X����C���f�b�N�X�̈ʒu�܂ł̃I�u�W�F�N�g��is_use��true�̂���
	// �����������
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