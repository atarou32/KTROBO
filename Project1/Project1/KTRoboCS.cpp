#include "KTRoboCS.h"

using namespace KTROBO;

CS::CS(void)
{
	is_log = false;
	is_main_enter = false;
}
CS::~CS(void)
{
}

char* CS::getCSName(int index) {
	if (index == CS_LOG_CS) {
		return "LOG:";
	}else if( index == CS_TASK_CS) {
		return "TASK:";
	}else if( index == CS_DEVICECON_CS) {
		return "DEVICECON:";
	}

	return "UNKNOWN:";
}

void CS::log(int index, char* p, int length) {

	if (is_log && length < 4096) {
#ifdef _DEBUG
		FILE* f = 0;
		fopen_s(&f,"mycs.txt", "a");
		if (f) {
			char buf[128];
			sprintf_s(buf,128,"%s:",getCSName(index));
			fwrite(p,length,1,f);
			fwrite("\n",1,1,f);
			fclose(f);
		}

		// ‚½‚­‚³‚ñ‘‚©‚ê‚é‚Ì‚ð‘jŽ~‚·‚é‚½‚ß‚ÉƒXƒŠ[ƒv‚ð“ü‚ê‚é
	//	Sleep(500);

#endif
	}
}