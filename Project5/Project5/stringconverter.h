#ifndef STRINGCONVERTER_H
#define STRINGCONVERTER_H


#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "locale.h"
#include "wchar.h"

class stringconverter
{
public:
	stringconverter(void);
	~stringconverter(void);
//	WCHAR* charTowchar(const char* filename);
	void charToWCHAR(const char* filename, WCHAR* buf); // buf ÇÕÇTÇPÇQÉTÉCÉYÇ…Ç∑ÇÈÇ±Ç∆
	char* wcharTochar(const WCHAR* filename, char* buf);

	bool Utf8ToUtf16(wchar_t *dest, size_t dest_size, const char *src, size_t src_size);

};

#endif