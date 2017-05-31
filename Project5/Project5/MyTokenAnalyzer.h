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


int hstrpos(char *haystack, char *needle);
void hmystrcpy(char* dst, int max, int pos, const char* src);
void hmystrcpy2(char *dst, int max, int mpos, const  char* src);


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

		if ((Pointer ==0) && ((Token==0) || (strcmp(Token,"")==0))) {
			return;
		}

	char* p = Pointer;
	char* q = Token;
	int offset = 0;

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

	if (*p=='{' || *p =='}') {
		*q++ = *p++;
	} else {
		while (*p!='\0' && offset < 1023) {
			if (strchr(" \t\r\n,;\"{}=", *p)) {
				break;
			}
			
			if (SJISMultiCheck(*p)) {
			*q = *p;
			q++;
			p++;
			offset++;
			*q = *p;
			q++;
			p++;
			offset++;
			} else {
				*q = *p;
				q++;
				p++;
				offset++;
			}
		}
	}

	Pointer=p;
	*q = '\0';
	}

	int GetIntToken() {
		GetToken();
		if (Token == 0) return 0;

		return atoi(Token);
	}

	unsigned int GetUIntToken() {
		GetToken();
		if (Token == 0) return 0;
		return (unsigned int)atol(Token);
	}
	float GetFloatToken() {
		GetToken();
		if (Token == 0) return 0;
		return float(atof(Token));
	}

	void GetToken(const char* token) {
		GetToken();

		if (strcmp(Token, token) != 0) {
			KTROBO::mylog::writelog(KTROBO::WARNING,"想定トークン:%s\n,読み込みトークン:%s",token,Token);
		}
	}

	char* Toke() {
		if (Token == 0) return "";
		
		return Token;}

	void SkipNode() {
		while(*Pointer != '\0') {
			GetToken();
			if (strchr(Token,'{')) break;
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