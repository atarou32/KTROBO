#include "KTRoboEffectSuuji.h"


void EffectSuuji::update(MYVECTOR3* lookfromtoat) {

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

	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM_ZERO;i++) {
		if (manager->getEffectImplIsActivated(zeros[i].effect_impl_id)) {
			MYVECTOR3 temp_pos = zeros[i].pos + yoko * zeros[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * zeros[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&zeros[i], &mat);
		}
	}

	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		if (manager->getEffectImplIsActivated(ones[i].effect_impl_id)) {
			MYVECTOR3 temp_pos = ones[i].pos + yoko * ones[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * ones[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&ones[i], &mat);
		}
	}

	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		if (manager->getEffectImplIsActivated(twos[i].effect_impl_id)) {
			MYVECTOR3 temp_pos = twos[i].pos + yoko * twos[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * twos[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&twos[i], &mat);
		}
	}


	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		if (manager->getEffectImplIsActivated(threes[i].effect_impl_id)) {
			MYVECTOR3 temp_pos = threes[i].pos + yoko * threes[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * threes[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&threes[i], &mat);
		}
	}
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		if (manager->getEffectImplIsActivated(fours[i].effect_impl_id)) {
			MYVECTOR3 temp_pos = fours[i].pos + yoko * fours[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * fours[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&fours[i], &mat);
		}
	}
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		if (manager->getEffectImplIsActivated(fives[i].effect_impl_id)) {
			MYVECTOR3 temp_pos = fives[i].pos + yoko * fives[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * fives[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&fives[i], &mat);
		}
	}
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		if (manager->getEffectImplIsActivated(sixs[i].effect_impl_id)) {
			MYVECTOR3 temp_pos = sixs[i].pos + yoko * sixs[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * sixs[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&sixs[i], &mat);
		}
	}
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		if (manager->getEffectImplIsActivated(sevens[i].effect_impl_id)) {
			MYVECTOR3 temp_pos = sevens[i].pos + yoko * sevens[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * sevens[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&sevens[i], &mat);
		}
	}
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		if (manager->getEffectImplIsActivated(eights[i].effect_impl_id)) {
			MYVECTOR3 temp_pos =eights[i].pos + yoko * eights[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * eights[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&eights[i], &mat);
		}
	}
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		if (manager->getEffectImplIsActivated(nines[i].effect_impl_id)) {
			MYVECTOR3 temp_pos = nines[i].pos + yoko * nines[i].offset;
			temp_pos = temp_pos + yoko * 2.1 * nines[i].jyunban;
			MYMATRIX mat;
			MyMatrixTranslation(mat,temp_pos.float3.x,temp_pos.float3.y, temp_pos.float3.z);
			updatePart(&nines[i], &mat);
		}
	}


}

void EffectSuuji::updatePart(EffectSuujiPart* part, MYMATRIX* world) {


	manager->setEffectImplWorld(part->effect_impl_id, world);
}

EffectSuuji::EffectSuuji(int task_index,EffectManager* manager)
{
	manager->loadFileFromLua(task_index, "resrc/script/effect/EFFECT_suuji.lua");
	this->manager = manager;
	// zeros... たちにimplを入れていく
	zeros_index = 0;
	int zeros_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_ZERO);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM_ZERO;i++) {
		zeros[i].effect_impl_id = manager->getEffectImpl(zeros_id);
	}

	ones_index = 0;
	int ones_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_ONE);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		ones[i].effect_impl_id = manager->getEffectImpl(ones_id);
	}

	twos_index = 0;
	int twos_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_TWO);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		twos[i].effect_impl_id = manager->getEffectImpl(twos_id);
	}

	threes_index = 0;
	int threes_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_THREE);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		threes[i].effect_impl_id = manager->getEffectImpl(threes_id);
	}

	fours_index = 0;
	int fours_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_FOUR);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		fours[i].effect_impl_id = manager->getEffectImpl(fours_id);
	}


	fives_index = 0;
	int fives_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_FIVE);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		fives[i].effect_impl_id = manager->getEffectImpl(fives_id);
	}

	sixs_index = 0;
	int sixs_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_SIX);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		sixs[i].effect_impl_id = manager->getEffectImpl(sixs_id);
	}

	sevens_index = 0;
	int sevens_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_SEVEN);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		sevens[i].effect_impl_id = manager->getEffectImpl(sevens_id);
	}

	eights_index = 0;
	int eights_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_EIGHT);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		eights[i].effect_impl_id = manager->getEffectImpl(eights_id);
	}

	nines_index = 0;
	int nines_id = manager->getEffectFromName(KTROBO_EFFECT_NAME_SUUJI_NINE);
	for (int i=0;i<KTROBO_EFFECT_SUUJI_NUM;i++) {
		nines[i].effect_impl_id = manager->getEffectImpl(nines_id);
	}




}


EffectSuuji::~EffectSuuji(void)
{
	// ロードしたものを消しはしないので注意
}




void EffectSuuji::setEffect(int suuji_part, MYVECTOR3* pos, MYVECTOR3* moto_pos, float offset, int keta, int jyunban) {
	EffectSuujiPart* efe = getEffectSuuji(suuji_part, pos);
	if (efe) {
		efe->jyunban = jyunban;
		efe->keta = keta;
		efe->offset = offset;
		efe->pos = *moto_pos;
	}
}

EffectSuujiPart* EffectSuuji::getEffectSuuji(int suuji_part, MYVECTOR3* pos) {
	if (suuji_part == 0) {
		manager->setEffectImplTime(zeros[zeros_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(zeros[zeros_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(zeros[zeros_index].effect_impl_id,true);
		manager->setEffectImplIsStart(zeros[zeros_index].effect_impl_id,true);
		EffectSuujiPart* pp = &zeros[zeros_index];
		zeros_index++;
		if (zeros_index >= KTROBO_EFFECT_SUUJI_NUM_ZERO) {
			zeros_index = 0;
		}
		return pp;
	} else if( suuji_part == 1) {
		manager->setEffectImplTime(ones[ones_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(ones[ones_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(ones[ones_index].effect_impl_id,true);
		manager->setEffectImplIsStart(ones[ones_index].effect_impl_id,true);
		EffectSuujiPart* pp = &ones[ones_index];
		ones_index++;
		if (ones_index >= KTROBO_EFFECT_SUUJI_NUM) {
			ones_index = 0;
		}
		return pp;
	} else if( suuji_part == 2) {
		manager->setEffectImplTime(twos[twos_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(twos[twos_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(twos[twos_index].effect_impl_id,true);
		manager->setEffectImplIsStart(twos[twos_index].effect_impl_id,true);
		EffectSuujiPart* pp = &twos[twos_index];
		twos_index++;
		if (twos_index >= KTROBO_EFFECT_SUUJI_NUM) {
			twos_index = 0;
		}
		return pp;
	} else if( suuji_part == 3) {
		manager->setEffectImplTime(threes[threes_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(threes[threes_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(threes[threes_index].effect_impl_id,true);
		manager->setEffectImplIsStart(threes[threes_index].effect_impl_id,true);
		EffectSuujiPart* pp = &threes[threes_index];
		threes_index++;
		if (threes_index >= KTROBO_EFFECT_SUUJI_NUM) {
			threes_index = 0;
		}
		return pp;
	} else if( suuji_part == 4) {
		manager->setEffectImplTime(fours[fours_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(fours[fours_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(fours[fours_index].effect_impl_id,true);
		manager->setEffectImplIsStart(fours[fours_index].effect_impl_id,true);
		EffectSuujiPart* pp = &fours[fours_index];
		fours_index++;
		if (fours_index >= KTROBO_EFFECT_SUUJI_NUM) {
			fours_index = 0;
		}
		return pp;
	} else if( suuji_part == 5) {
		manager->setEffectImplTime(fives[fives_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(fives[fives_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(fives[fives_index].effect_impl_id,true);
		manager->setEffectImplIsStart(fives[fives_index].effect_impl_id,true);
		EffectSuujiPart* pp = &fives[fives_index];
		fives_index++;
		if (fives_index >= KTROBO_EFFECT_SUUJI_NUM) {
			fives_index = 0;
		}
		return pp;
	} else if( suuji_part == 6) {
		manager->setEffectImplTime(sixs[sixs_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(sixs[sixs_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(sixs[sixs_index].effect_impl_id,true);
		manager->setEffectImplIsStart(sixs[sixs_index].effect_impl_id,true);
		EffectSuujiPart* pp = &sixs[sixs_index];
		sixs_index++;
		if (sixs_index >= KTROBO_EFFECT_SUUJI_NUM) {
			sixs_index = 0;
		}
		return pp;
	} else if( suuji_part == 7) {
		manager->setEffectImplTime(sevens[sevens_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(sevens[sevens_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(sevens[sevens_index].effect_impl_id,true);
		manager->setEffectImplIsStart(sevens[sevens_index].effect_impl_id,true);
		EffectSuujiPart* pp = &sevens[sevens_index];
		sevens_index++;
		if (sevens_index >= KTROBO_EFFECT_SUUJI_NUM) {
			sevens_index = 0;
		}
		return pp;
	} else if( suuji_part == 8) {
		manager->setEffectImplTime(eights[eights_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(eights[eights_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(eights[eights_index].effect_impl_id,true);
		manager->setEffectImplIsStart(eights[eights_index].effect_impl_id,true);
		EffectSuujiPart* pp = &eights[eights_index];
		eights_index++;
		if (eights_index >= KTROBO_EFFECT_SUUJI_NUM) {
			eights_index = 0;
		}
		return pp;
	} else if( suuji_part == 9) {
		manager->setEffectImplTime(nines[nines_index].effect_impl_id,0);
		MYMATRIX mat;
		MyMatrixTranslation(mat,pos->float3.x,pos->float3.y,pos->float3.z);
		manager->setEffectImplWorld(nines[nines_index].effect_impl_id,&mat);
		manager->setEffectImplIsRender(nines[nines_index].effect_impl_id,true);
		manager->setEffectImplIsStart(nines[nines_index].effect_impl_id,true);
		EffectSuujiPart* pp = &nines[nines_index];
		nines_index++;
		if (nines_index >= KTROBO_EFFECT_SUUJI_NUM) {
			nines_index = 0;
		}
		return pp;
	}

	return NULL;

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
	int jyunban = 0;
	while ( temp != 0) {
		int suuji_part= temp % 10;

		this->setEffect(suuji_part,&temp_pos, pos, offset,keta,jyunban);
		temp_pos = temp_pos + yoko * haba;
		jyunban++;
		temp = temp / 10;
	}
}











