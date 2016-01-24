#include "KTRoboTextFromLua.h"
#include "stringconverter.h"

using namespace KTROBO;
TextFromLua::TextFromLua(Graphics* gg)
{
	g  = gg;
}


TextFromLua::~TextFromLua(void)
{
}

void TextFromLua::setDebugText(char* str) {
	WCHAR test[512];
	memset(test,0,sizeof(WCHAR)*512);
	stringconverter sc;
	sc.charToWCHAR(str, test);
	DebugTexts::instance()->setText(g, wcslen(test), test);

}


