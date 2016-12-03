#include "stringconverter.h"
#include "KTRoboLog.h"

stringconverter::stringconverter(void)
{
}


stringconverter::~stringconverter(void)
{
}

/*
WCHAR* stringconverter::charTowchar(const char* filename) {

	static WCHAR ans[512];
	memset(ans,0,sizeof(WCHAR)*512);
	size_t wLen = 0;
	errno_t err = 0;
	setlocale(LC_ALL,"japanese");
	err = mbstowcs_s(&wLen,ans,filename,_TRUNCATE);
	if (err != 0) {
		mylog::writelog("%s のwchar変換に失敗",filename);
		return NULL;
	}
	return ans;
}

*/

void stringconverter::charToWCHAR(const char* filename, WCHAR* ans) {

	WCHAR ans2[512];
	memset(ans2,0,sizeof(WCHAR)*511);
	size_t wLen = 0;
	errno_t err = 0;
	setlocale(LC_ALL,"japanese");
	err = mbstowcs_s(&wLen,ans2,filename,_TRUNCATE);
	if (err != 0) {
		KTROBO::mylog::writelog(KTROBO::WARNING,"%s のwchar変換に失敗",filename);
		return;
	}

	wcscpy_s(ans,511,ans2);
	return;
}
char* stringconverter::wcharTochar(const WCHAR* filename, char* buf) {
	char ans[512];
	memset(ans,0,511);
	size_t wLen = 0;
	errno_t err = 0;
	setlocale(LC_ALL,"japanese");
	err = wcstombs_s(&wLen, ans,filename,_TRUNCATE);
	if (err != 0) {
		KTROBO::mylog::writelog(KTROBO::WARNING,"%s のchar変換に失敗",filename);
		return NULL;
	}
	memcpy(buf,ans,strlen(ans));
	return buf;
}

bool stringconverter::Utf8ToUtf16(wchar_t *dest, size_t dest_size, const char *src, size_t src_size)
{
                                /**/
    UINT    uCodePage;          /* コードページ */
    DWORD   dwFlags;            /* 処理性能とマッピングのフラグ */
    LPCSTR  lpMultiByteStr;     /* 新しい文字列のバッファのアドレス */
    int     cchMultiByte;       /* バッファのサイズ */
    LPWSTR  lpWideCharStr;      /* ワイド文字の文字列のアドレス */
    int     cchWideChar;        /* 文字列の文字数 */
    int     iResult = 0;
    
    uCodePage       = CP_UTF8;
    dwFlags         = 0;
    lpMultiByteStr  = src;
    cchMultiByte    = src_size;
    lpWideCharStr   = dest;
    cchWideChar     = dest_size;
    iResult = MultiByteToWideChar(uCodePage, dwFlags, lpMultiByteStr
            , cchMultiByte, lpWideCharStr, cchWideChar);
    return iResult;
}