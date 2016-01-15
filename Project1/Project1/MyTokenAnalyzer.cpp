#include "MyTokenAnalyzer.h"

CRITICAL_SECTION MyTokenAnalyzer::cs;

MyTokenAnalyzer::MyTokenAnalyzer(void)
{
	Buffer = 0;
	memset(Token,0,1024);
	Pointer = 0;
}


MyTokenAnalyzer::~MyTokenAnalyzer(void)
{
}

void MyTokenAnalyzer::deletedayo() {
	if (Buffer) {
		delete[] Buffer;
		Buffer = 0;
	}
}


int SJISMultiCheck(unsigned char c){
   if(((c>=0x81)&&(c<=0x9f))||((c>=0xe0)&&(c<=0xfc)))return 1;
   else return 0;
}
int SJISlength(unsigned char* c) {
	int count = 0;
	while(*c != 0 && count < 512) {
		if (SJISMultiCheck(*c)) {
			c +=2;
			count++;
		} else {
			c+=1;
			count++;
		}
	}
	return count;
}
bool MyTokenAnalyzer::load(const char* filename) {
	FILE* file;
	EnterCriticalSection(&cs);

	if (strlen(filename) > 1024) {
		KTROBO::mylog::writelog(KTROBO::WARNING, "��������t�@�C����");
		LeaveCriticalSection(&cs);
		return false;
	}

	if(0 != fopen_s(&file,filename,"r")) {
		KTROBO::mylog::writelog(KTROBO::WARNING, "%s �̓ǂݍ��݂Ɏ��s", filename);
		LeaveCriticalSection(&cs);
		return false;
	}

	/* �t�@�C���T�C�Y�𒲍� */
	fpos_t fsize;
	fseek(file,0,SEEK_END); 
	fgetpos(file,&fsize);

	if (fsize > 10000000) {
		KTROBO::mylog::writelog(KTROBO::WARNING, "%s�@�͑傫������", filename);
		fclose(file);
		LeaveCriticalSection(&cs);
		return false;
	}
	char* buffer=new char[(unsigned int)fsize+1];
	char** pbuffer = &buffer;

	fseek(file,0,SEEK_SET);
	buffer[fsize]='\0';
	size_t readsize = fread_s(buffer,(size_t)fsize,1,(size_t)fsize,file);
	if (readsize > fsize) {
		KTROBO::mylog::writelog(KTROBO::WARNING, "%s �ŕs� %d %d",filename,readsize,fsize);
		KTROBO::mylog::writelog(KTROBO::WARNING, "%c,%c,%c",buffer[0],buffer[1],buffer[2]);
		fclose(file);
		LeaveCriticalSection(&cs);
		return false;
	}
	buffer[readsize]='\0';
	fclose(file);
	LeaveCriticalSection(&cs);
	// ���b�V��
	Pointer=buffer;
	Buffer = buffer;
	return true;
}