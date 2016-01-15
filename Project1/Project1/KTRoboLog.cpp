#include "KTRoboLog.h"
#include "KTRoboCS.h"

using namespace KTROBO;


mylog::mylog(void)
{
}


mylog::~mylog(void)
{
}


char* mylog::writelog(int error_num, const char* buffer, ...) 
{

	va_list ap;
	va_start(ap, buffer);
	static char log[4096];
	memset(log,0,4096);
	vsnprintf_s(log,4096, buffer, ap);
	log[4095] = '\0';

	va_end(ap);

	FILE* file;
	CS::instance()->enter(CS_LOG_CS, log);
	if (0 != fopen_s(&file,getLogFileName(error_num),"a")) {
		OutputDebugString(L"errordayo");
		CS::instance()->leave(CS_LOG_CS, log);
		return log;
	}
	fwrite(log,1,strlen(log),file);
	fclose(file);
	CS::instance()->leave(CS_LOG_CS, log);
	return log;











}
char* mylog::writelog(char* filename, const char* buffer, ...)
{
	va_list ap;
	va_start(ap, buffer);
	static char log[4096];
	memset(log,0,4096);
	vsnprintf_s(log,4096, buffer, ap);
	log[4095] = '\0';

	va_end(ap);

	FILE* file;
	CS::instance()->enter(CS_LOG_CS, log);
	if (0 != fopen_s(&file,filename,"a")) {
		OutputDebugString(L"errordayo");
		CS::instance()->leave(CS_LOG_CS, log);
		return log;
	}
	fwrite(log,1,strlen(log),file);
	fclose(file);
	CS::instance()->leave(CS_LOG_CS, log);
	return log;
}


