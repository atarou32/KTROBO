#ifndef MYTOKENANALYZER_H
#define MYTOKENANALYZER_H

#pragma once
#include "string.h"
#include "KTRoboLog.h"
#include "stdlib.h"
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

int SJISMultiCheck(unsigned char c);
int SJISlength(unsigned char* c);

class MyTokenAnalyzer
{
private:
	char* Buffer;
	char* Pointer;
	char Token[1024];

public:
	MyTokenAnalyzer(void);
	~MyTokenAnalyzer(void);
	static CRITICAL_SECTION cs;
	static void Init() {
		InitializeCriticalSection(&cs);
	}
	static void Del() {
		DeleteCriticalSection(&cs);
	}
	bool load(const char* filename);
	void resetPointer() {Pointer = Buffer;}
	void deletedayo();
	bool enddayo() {
		if (Pointer ==0) {
			return true;
		}
		if (*Pointer == '\0') {
			return true;
		}
		return false;
	}

	void GetToken() {
	char* p = Pointer;
	char* q = Token;

	while(*p != '\0') {
		if (!strchr(" \t\r\n,;\"=", *p)) {
			break;
		}
		
		if (SJISMultiCheck(*p)) {
			p += 2;
		} else {
			p++;
		}
	}

	if (*p=='{' || *p =='}' || *p=='(' || *p ==')') {
		*q++ = *p++;
	} else {
		while (*p!='\0') {
			if (strchr(" \t\r\n,;\"{}=()", *p)) {
				break;
			}
			
			if (SJISMultiCheck(*p)) {
			*q = *p;
			q++;
			p++;
			*q = *p;
			q++;
			p++;
			} else {
				*q = *p;
				q++;
				p++;
			}
		}
	}

	Pointer=p;
	*q = '\0';
	}

	int GetIntToken() {
		GetToken();
		return atoi(Token);
	}

	unsigned int GetUIntToken() {
		GetToken();
		return (unsigned int)atol(Token);
	}
	float GetFloatToken() {
		GetToken();
		return float(atof(Token));
	}

	void GetToken(const char* token) {
		GetToken();

		if (strcmp(Token, token) != 0) {
			KTROBO::mylog::writelog(KTROBO::WARNING,"想定トークン:%s\n,読み込みトークン:%s",token,Token);
		}
	}

	char* Toke() {return Token;}

	void SkipNode() {
		while(*Pointer != '\0') {
			if (strchr(Token,'{')) break;
			GetToken();
		}
		int count = 1;

		while(*Pointer != '\0' && count >0) {
			GetToken();
			if (strchr(Token, '{')) count++; else
				if (strchr(Token, '}')) count--;
		}

		if (count >0) {
			KTROBO::mylog::writelog(KTROBO::WARNING, "括弧の非対応");
			return;
		}
	}
};



#endif