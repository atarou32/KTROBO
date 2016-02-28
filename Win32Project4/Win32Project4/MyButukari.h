#ifndef MYBUTUKARI_H
#define MYBUTUKARI_H

#pragma once


#define BUTUKARI_RIGHT 0x0001
#define BUTUKARI_LEFT 0x0002
#define BUTUKARI_TOP 0x0004
#define BUTUKARI_BOTTOM 0x0008
#define BUTUKARI_IN 0x0010
#define BUTUKARI_OUT 0x0020
#define BUTUKARI_BUTUKARI 0x0040
#define BUTUKARI_TATEMID 0x0080
#define BUTUKARI_YOKOMID 0x0100
#define BUTUKARI_TATE 0x0200
#define BUTUKARI_YOKO 0x0400

#define BUTUKARI_YOKOOUT 0x0800
#define BUTUKARI_TATEOUT 0x1000

struct MYRECT{
	float left;
	float right;
	float top;
	float bottom;
	MYRECT& operator=(const MYRECT &obj) {
		left = obj.left;
		right = obj.right;
		top = obj.top;
		bottom = obj.bottom;
		return (*this);
	}
	MYRECT(){};
	MYRECT(float l, float r, float t ,float b) {
		left = l;
		right = r;
		top = t;
		bottom = b;
	};
};

bool IS_MYRECT_ZERO(MYRECT* rect);

unsigned int getButukariStatus(MYRECT* rect,MYRECT* korenitaisite);


#define BUTUKARIPOINT_TOP 0x0001
#define BUTUKARIPOINT_BOTTOM 0x0002
#define BUTUKARIPOINT_LEFT 0x0004
#define BUTUKARIPOINT_RIGHT 0x0008
#define BUTUKARIPOINT_IN 0x0010

unsigned int getButukariStatusPoint(float x,float y,MYRECT* korenitaisite);
double getKyori(float x,float y,float x2,float y2);

#endif