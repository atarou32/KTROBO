#include "KTRoboEffectSuuji.h"


EffectSuuji::EffectSuuji(int task_index,EffectManager* manager)
{
	manager->loadFileFromLua(task_index, "resrc/script/effect/EFFECT_suuji.lua");
	this->manager = manager;
	// zeros... たちにimplを入れていく
	zeros_index = 0;
	int zeros_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_ZERO);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM_ZERO;i++) {
		zeros[i] = manager->getEffectImpl(zeros_id);
	}

	ones_index = 0;
	int ones_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_ONE);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		ones[i] = manager->getEffectImpl(ones_id);
	}

	twos_index = 0;
	int twos_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_TWO);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		twos[i] = manager->getEffectImpl(twos_id);
	}

	threes_index = 0;
	int threes_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_THREE);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		threes[i] = manager->getEffectImpl(threes_id);
	}

	fours_index = 0;
	int fours_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_FOUR);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		fours[i] = manager->getEffectImpl(fours_id);
	}


	fives_index = 0;
	int fives_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_FIVE);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		fives[i] = manager->getEffectImpl(fives_id);
	}

	sixs_index = 0;
	int sixs_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_SIX);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		sixs[i] = manager->getEffectImpl(sixs_id);
	}

	sevens_index = 0;
	int sevens_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_SEVEN);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		sevens[i] = manager->getEffectImpl(sevens_id);
	}

	eights_index = 0;
	int eights_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_EIGHT);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		eights[i] = manager->getEffectImpl(eights_id);
	}

	nines_index = 0;
	int nines_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_NINE);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		nines[i] = manager->getEffectImpl(nines_id);
	}




}


EffectSuuji::~EffectSuuji(void)
{
	// ロードしたものを消しはしないので注意
}
void EffectSuuji::setEffect(int suuji_part, MYVECTOR3* pos) {
	if (suuji_part == 0) {
		manager->setEffectImplTime(zeros[zeros_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(zeros[zeros_index],&mat);
		manager->setEffectImplIsRender(zeros[zeros_index],true);
		manager->setEffectImplIsStart(zeros[zeros_index],true);
		zeros_index++;
		if (zeros_index >= KTROBO_EFFECT_SUUJI_NUM_ZERO) {
			zeros_index = 0;
		}
	} else if( suuji_part == 1) {
		manager->setEffectImplTime(ones[ones_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(ones[ones_index],&mat);
		manager->setEffectImplIsRender(ones[ones_index],true);
		manager->setEffectImplIsStart(ones[ones_index],true);
		ones_index++;
		if (ones_index >= KTROBO_EFFECT_SUUJI_NUM) {
			ones_index = 0;
		}
	} else if( suuji_part == 2) {
		manager->setEffectImplTime(twos[twos_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(twos[twos_index],&mat);
		manager->setEffectImplIsRender(twos[twos_index],true);
		manager->setEffectImplIsStart(twos[twos_index],true);
		twos_index++;
		if (twos_index >= KTROBO_EFFECT_SUUJI_NUM) {
			twos_index = 0;
		}
	} else if( suuji_part == 3) {
		manager->setEffectImplTime(threes[threes_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(threes[threes_index],&mat);
		manager->setEffectImplIsRender(threes[threes_index],true);
		manager->setEffectImplIsStart(threes[threes_index],true);
		threes_index++;
		if (threes_index >= KTROBO_EFFECT_SUUJI_NUM) {
			threes_index = 0;
		}
	} else if( suuji_part == 4) {
		manager->setEffectImplTime(fours[fours_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(fours[fours_index],&mat);
		manager->setEffectImplIsRender(fours[fours_index],true);
		manager->setEffectImplIsStart(fours[fours_index],true);
		fours_index++;
		if (fours_index >= KTROBO_EFFECT_SUUJI_NUM) {
			fours_index = 0;
		}
	} else if( suuji_part == 5) {
		manager->setEffectImplTime(fives[fives_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(fives[fives_index],&mat);
		manager->setEffectImplIsRender(fives[fives_index],true);
		manager->setEffectImplIsStart(fives[fives_index],true);
		fives_index++;
		if (fives_index >= KTROBO_EFFECT_SUUJI_NUM) {
			fives_index = 0;
		}
	} else if( suuji_part == 6) {
		manager->setEffectImplTime(sixs[sixs_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(sixs[sixs_index],&mat);
		manager->setEffectImplIsRender(sixs[sixs_index],true);
		manager->setEffectImplIsStart(sixs[sixs_index],true);
		sixs_index++;
		if (sixs_index >= KTROBO_EFFECT_SUUJI_NUM) {
			sixs_index = 0;
		}
	} else if( suuji_part == 7) {
		manager->setEffectImplTime(sevens[sevens_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(sevens[sevens_index],&mat);
		manager->setEffectImplIsRender(sevens[sevens_index],true);
		manager->setEffectImplIsStart(sevens[sevens_index],true);
		sevens_index++;
		if (sevens_index >= KTROBO_EFFECT_SUUJI_NUM) {
			sevens_index = 0;
		}
	} else if( suuji_part == 8) {
		manager->setEffectImplTime(eights[eights_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(eights[eights_index],&mat);
		manager->setEffectImplIsRender(eights[eights_index],true);
		manager->setEffectImplIsStart(eights[eights_index],true);
		eights_index++;
		if (eights_index >= KTROBO_EFFECT_SUUJI_NUM) {
			eights_index = 0;
		}
	} else if( suuji_part == 9) {
		manager->setEffectImplTime(nines[nines_index],0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(nines[nines_index],&mat);
		manager->setEffectImplIsRender(nines[nines_index],true);
		manager->setEffectImplIsStart(nines[nines_index],true);
		nines_index++;
		if (nines_index >= KTROBO_EFFECT_SUUJI_NUM) {
			nines_index = 0;
		}
	}

}


void EffectSuuji::render(int suuji, MYVECTOR3* lookfromtoat, MYVECTOR3* pos) {

	// 桁数を数える
	int keta = 0;
	int temp = suuji;

	if (suuji == 0) {
		keta = 1;
	} else {
		while(temp != 0) {
			temp = temp / 10;
			keta++;
		}
		
	}

	float offset = -keta/2 * 2.1;
	float haba = 2.1;
	// lookfromtoat とposから順々に場所を指定していく
	MYVECTOR3 looktoat = *lookfromtoat;
	MyVec3Normalize(looktoat,looktoat);
	MYVECTOR3 up(0,0,1);
	MYVECTOR3 yoko(1,0,0);
	if (abs(MyVec3Dot(up,looktoat) > 0.99999)) {
		if (MyVec3Dot(up,looktoat) < 0) {
		}
	} else {
		MyVec3Cross(yoko,up, looktoat);
		MyVec3Normalize(yoko,yoko);
	}
	temp = suuji;
	MYVECTOR3 temp_pos;
	temp_pos = *pos + yoko * offset;
	while ( temp != 0) {
		int suuji_part= temp % 10;

		this->setEffect(suuji_part,&temp_pos);
		temp_pos = temp_pos + yoko * haba;
		temp = temp / 10;
	}
}











