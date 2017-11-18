#include "KTRoboEffect.h"
#include "KTRoboLuaCollection.h"

using namespace KTROBO;

Effect::Effect(int effect_id)
{
	this->effect_id = effect_id;
	texture_id = 0;
	for (int i=0;i<64;i++) {
		effect_name[i] = 0;
	}
	for (int i=0;i<256;i++) {
		texture_name[i] = 0;
		file_name[i] = 0;
	}
}

void Effect::Release() {
	vector<EffectPart*>::iterator it= parts.begin();
	while(it != parts.end()) {
		EffectPart* pp = *it;
		delete pp;
		pp =0;
		it++;
	}
	parts.clear();
}


Effect::~Effect(void)
{
	Release();	
}

bool EffectPartPart::isInTime(float time) {
	
	if (this->dtime_start <= time) {
		if (this->dtime_end >= time) {
			return true;
		}
	}
	return false;
}


bool EffectPartPart::isStartedTime(float time) {

	if (this->dtime_start <= time) {
		return true;
	}

	return false;
}



float EffectPartPos::getXOfT(float time) {

	if (dtime_end - dtime_start > 0.001f) {

		return this->start_x + (this->end_x - this->start_x) * (time - dtime_start) / (dtime_end - dtime_start);
	}

	return 0;
}

float EffectPartPos::getYOfT(float time) {
	if (dtime_end - dtime_start > 0.001f) {

		return this->start_y + (this->end_y - this->start_y) * (time - dtime_start) / (dtime_end - dtime_start);
	}

	return 0;

}


float EffectPartPos::getZOfT(float time) {
	if (dtime_end - dtime_start > 0.001f) {

		return this->start_z + (this->end_z - this->start_z) * (time - dtime_start) / (dtime_end - dtime_start);
	}
	return 0;
}


void EffectPartTexPos::setTexPos(Texture* t, int billboard_id) {

	if (t) {
	t->setRenderBillBoardTexPos(billboard_id, this->tex_x, this->tex_y, tex_width, tex_height);
	}
}


float EffectPartWH::getWidthOfT(float time) {

	if (dtime_end - dtime_start > 0.001f) {

		return this->start_width + (this->end_width - this->start_width) * (time - dtime_start) / (dtime_end - dtime_start);
	}
	return 1;



}



float EffectPartWH::getHeightOfT(float time) {

	if (dtime_end - dtime_start > 0.001f) {

		return this->start_height + (this->end_height - this->start_height) * (time - dtime_start) / (dtime_end - dtime_start);
	}
	return 1;




}




float EffectPartRot::getRotXOfT(float time) {
	if (dtime_end - dtime_start > 0.001f) {

		return this->start_rotx + (this->end_rotx - this->start_rotx) * (time - dtime_start) / (dtime_end - dtime_start);
	}
	return 0;




}


float EffectPartRot::getRotYOfT(float time) {

	if (dtime_end - dtime_start > 0.001f) {

		return this->start_roty + (this->end_roty - this->start_roty) * (time - dtime_start) / (dtime_end - dtime_start);
	}
	return 0;

}


float EffectPartRot::getRotZOfT(float time) {
	if (dtime_end - dtime_start > 0.001f) {

		return this->start_rotz + (this->end_rotz - this->start_rotz) * (time - dtime_start) / (dtime_end - dtime_start);
	}
	return 0;


}


unsigned int EffectPartColor::getColorOfT(float time) {

	unsigned char red=0;
	unsigned char green=0;
	unsigned char blue=0;
	unsigned char alpha=0;

	unsigned int scolor = start_color;
	unsigned int ecolor = end_color;
	unsigned char sred = (scolor & 0xFF000000) >> 24;
	unsigned char ered = (ecolor & 0xFF000000) >> 24;
	unsigned char sgreen = (scolor & 0x00FF0000) >> 16;
	unsigned char egreen = (ecolor & 0x00FF0000) >> 16;
	unsigned char sblue = (scolor & 0x0000FF00) >> 8;
	unsigned char eblue = (ecolor & 0x0000FF00) >> 8;
	unsigned char salpha = (scolor & 0x000000FF);
	unsigned char ealpha = (ecolor & 0x000000FF);

	if (dtime_end - dtime_start > 0.001f) {
		red = sred + (ered - sred)* (time - dtime_start) / (dtime_end - dtime_start);
		green = sgreen + (egreen - sgreen) * (time- dtime_start) / (dtime_end - dtime_start);
		blue = sblue + (eblue - sblue) * (time- dtime_start) / (dtime_end - dtime_start);
		alpha = salpha + (ealpha - salpha) * (time- dtime_start) / (dtime_end - dtime_start);
		unsigned int new_color = (red << 24) + (green << 16) + (blue << 8) + alpha;
		return new_color;
	}

	return 0;
}


void EffectPart::update(EffectImpl* effect_impl, Texture* texture, float loop_time, int billboard_id, float dtime, int stamp) {
	// 該当するエフェクトパートの状態を更新する

	float tt = effect_impl->getTime();// time だがループするかどうかによって変わってくる
	float moto_tt = tt;
	tt = tt - loop_time;

	if (effect_impl->getIsActivated()) {
	if (effect_impl->isActivateJyunbiOK()) {
		
	} else {
		texture->setRenderBillBoardIsRender(billboard_id,false);
		effect_impl->incActivateJyunbi();
		if (effect_impl->isActivateJyunbiOK()) {
			texture->setRenderBillBoardIsRender(billboard_id,true);
		}
	}
	}

	if (tt >= end_time) {


		// is_render をfalseにしてリターン
		texture->setRenderBillBoardIsRender(billboard_id,false);
		effect_impl->setIsActivated(false);
		return; // 何もしないでリターンする
	}
	float dx = 0;
	float dy = 0;
	float dz = 0;
	float dw = 1;
	float dh = 1;
	float drotx = 0;
	float droty = 0;
	float drotz = 0;
	unsigned int new_color= 0x0000000;

	// texpos
	vector<EffectPartTexPos*>::iterator it_texpos = this->tex_poss.begin();
	while (it_texpos != tex_poss.end()) {
		EffectPartTexPos* texp = *it_texpos;
		if (texp->isInTime(tt)) {
			texp->setTexPos(texture, billboard_id);
			// 最後にあてはまったものになる
		}
		it_texpos++;
	}

	// pos
	vector<EffectPartPos*>::iterator it_pos = this->poss.begin();
	while(it_pos != poss.end()) {
		EffectPartPos* posp = *it_pos;

		if (posp->isInTime(moto_tt)) {
			dx = posp->getXOfT(tt);
			dy = posp->getYOfT(tt);
			dz = posp->getZOfT(tt);
			// みつかったらbreakする
			break;
		}
		it_pos++;
	}

	// wh
	vector<EffectPartWH*>::iterator it_wh = this->whs.begin();
	while(it_wh != whs.end()) {
		EffectPartWH* whp = *it_wh;
		if (whp->isInTime(tt)) {
			dw = whp->getWidthOfT(tt);
			dh = whp->getHeightOfT(tt);
			// みつかったらbreakする
			break;
		}

		it_wh++;
	}

	// rot
	vector<EffectPartRot*>::iterator it_rot = this->rots.begin();
	while(it_rot != rots.end()) {
		EffectPartRot* rotp = *it_rot;
		if (rotp->isInTime(tt)) {
			drotx = rotp->getRotXOfT(tt);
			droty = rotp->getRotYOfT(tt);
			drotz = rotp->getRotZOfT(tt);
			// みつかったらbreakする
			break;
		}
		it_rot++;
	}



	// color
	vector<EffectPartColor*>::iterator it_color = this->colors.begin();
	while(it_color != colors.end()) {
		EffectPartColor* colorp = *it_color;
		if (colorp->isInTime(tt)) {
			new_color = colorp->getColorOfT(tt);
			// みつかったらbreakする
			break;
		}
		it_color++;
	}


	MYMATRIX ww = *effect_impl->getWorld();
	MYVECTOR3 x_vec(1,0,0);
	MYVECTOR3 y_vec(0,1,0);
	MYVECTOR3 z_vec(0,0,1);
	MyVec3TransformNormal(x_vec,x_vec,ww);
	MyVec3TransformNormal(y_vec,y_vec,ww);
	MyVec3TransformNormal(z_vec,z_vec,ww);
	MYVECTOR3 pp(0,0,0);
	pp = x_vec * dx + y_vec * dy + z_vec * dz;
	MYMATRIX trans;

	MYMATRIX ans_mat;
	MYVECTOR3 moto_pp(0,0,0);
	MyVec3TransformCoord(moto_pp,moto_pp,ww);
	pp = pp + moto_pp;
	MyMatrixTranslation(trans, pp.float3.x,pp.float3.y, pp.float3.z);
	// 後はrotを入れる
	MYMATRIX mat_rotx;
	MYMATRIX mat_roty;
	MYMATRIX mat_rotz;
	MyMatrixRotationX(mat_rotx, drotx);
	MyMatrixRotationY(mat_roty, droty);
	MyMatrixRotationZ(mat_rotz, drotz);
	MyMatrixMultiply(ans_mat,mat_rotz,mat_rotx);
	MyMatrixMultiply(ans_mat,ans_mat,mat_roty);
	MyMatrixMultiply(ans_mat,ans_mat, trans);
	texture->setRenderBillBoardPos(billboard_id, &ans_mat);
	texture->setRenderBillBoardColor(billboard_id, new_color);
	texture->setRenderBillBoardWH(billboard_id,dw,dh);

}

void Effect::setName(char* name) {
	if (strlen(name) < 64) {
		strcpy_s(this->effect_name, name);
	}
}

void Effect::setFileName(char* filename) {
	if (strlen(filename) < 256) {
		strcpy_s(this->file_name, filename);
	}
}

void EffectManager::loadFileFromLua(int task_index, char* filename) {
	// ここにロードファイルのルーアを呼び出す
	LuaTCBMaker::doTCBnow(task_index,true, filename);

}
void Effect::setTextureIDAndName(int texture_id, char* texture_name) {

	if (strlen(texture_name) <256) {
		strcpy_s(this->texture_name,texture_name);
	}
	this->texture_id = texture_id;
}




void Effect::update(EffectImpl* effect_impl, Texture* texture, float dtime, int stamp) {
	
	// 該当するエフェクトの実体の状態を更新する
	if (effect_impl->getIsActivated()) {
		effect_impl->setTime(effect_impl->getTime()+dtime);
		// looptimeの更新

		int siz = parts.size();
		float time = effect_impl->getTime();
		for (int i=0;i<siz;i++) {
			float tt = parts[i]->getTimeWhenLoop();
			if (time - effect_impl->looptime_effectpart[i] >= tt) {
				if (parts[i]->isLoop()) {
					effect_impl->looptime_effectpart[i] += parts[i]->getPlusTimeLoop();
				}
			}
		}

		for (int i=0;i<siz;i++) {
			int billboard_id = effect_impl->effectpart_billboard_ids[i];
			int loop_time = effect_impl->looptime_effectpart[i];
			parts[i]->update(effect_impl,texture,loop_time, billboard_id, dtime,stamp);
		}
	}
}


EffectManager::EffectManager(Texture* tex) {

	this->tex = tex;

	// まず最初に作るのはdummyeffectとdummy_impl

	dummy_effect = new Effect(KTROBO_EFFECT_DUMMY_ID);
	dummy_effect_impl = new EffectImpl(KTROBO_EFFECT_IMPL_DUMMY_ID,KTROBO_EFFECT_DUMMY_ID);
	effects.push_back(dummy_effect);
	effect_impls.push_back(dummy_effect_impl);
	effect_id_indexs.insert(std::pair<int,int>(KTROBO_EFFECT_DUMMY_ID,KTROBO_EFFECT_DUMMY_INDEX));
	effect_impl_id_indexs.insert(std::pair<int,int>(KTROBO_EFFECT_IMPL_DUMMY_ID, KTROBO_EFFECT_IMPL_DUMMY_INDEX));
	effect_name_indexs.insert(std::pair<string,int>(string("dummy"), KTROBO_EFFECT_DUMMY_INDEX));
	
}



int EffectManager::getEffect(char* effect_name, char* texture_name, char* lua_file_name) {
	// 同じnameを指定した場合は同じIDが返る

	CS::instance()->enter(CS_RENDERDATA_CS, "geteffect");
	if (this->effect_name_indexs.find(effect_name) != effect_name_indexs.end()) {
		
		int index =  effect_name_indexs[effect_name];
		CS::instance()->leave(CS_RENDERDATA_CS, "geteffect");
		return index;
	} else {
		// 作る
		int effect_index = effects.size();
		int effect_id = effect_index;
		Effect* efe = new Effect(effect_id);
		efe->setName(effect_name);
		efe->setFileName(lua_file_name);
		int texture_id = tex->getTexture(texture_name, 4096);
		efe->setTextureIDAndName(texture_id, texture_name);
		
		// 次はインデックスに入れ込む
		
		effects.push_back(efe);
		effect_id_indexs.insert(std::pair<int,int>(effect_id,effect_index));
		effect_name_indexs.insert(std::pair<string,int>(string(effect_name),effect_index));
		CS::instance()->leave(CS_RENDERDATA_CS, "geteffect");
		return effect_id;



	}
	CS::instance()->leave(CS_RENDERDATA_CS, "geteffect");
	return KTROBO_EFFECT_NONE;
}	



int EffectManager::getEffectFromName(char* effect_name) {
	// 同じnameを指定した場合は同じIDが返る

	CS::instance()->enter(CS_RENDERDATA_CS, "geteffect");
	if (this->effect_name_indexs.find(effect_name) != effect_name_indexs.end()) {
		
		int index =  effect_name_indexs[effect_name];
		CS::instance()->leave(CS_RENDERDATA_CS, "geteffect");
		return index;
	}

	CS::instance()->leave(CS_RENDERDATA_CS, "geteffect");
	return KTROBO_EFFECT_NONE;
}	
	
	
	
	
/*	
int EffectManager::getEffectFromFileName(char* file_name) {
	// effect_nameはファイル中の設定の値が入る　基本的に１エフェクト１ファイル

	if (this->effect_name_indexs.find(file_name) != effect_name_indexs.end()) {
		return effect_name_indexs[file_name];
	}

	return KTROBO_EFFECT_NONE;
}
*/
bool EffectManager::hasEffectPartIndex(int effect_id, int index) {

	if (effect_id_indexs.find(effect_id) != effect_id_indexs.end()) {
		Effect* efe = effects[effect_id_indexs[effect_id]];
		int p = efe->getPartsSize();
		if (p > index) return true;
	}

	return false;
}
	
	
void EffectManager::makeEffectPart(int effect_id,int index, float endtime, bool is_loop, float time_when_loop, float plustime_loop) {
	if (effect_id == KTROBO_EFFECT_NONE) return;
	// indexは　0 1 2 3 4 5 の順にゲットすること　急に１０とかゲットしようとしてもindexは増えない
	CS::instance()->enter(CS_RENDERDATA_CS, "makeeffectpart");
	if (effect_id_indexs.find(effect_id) != effect_id_indexs.end()) {
		Effect* efe = effects[effect_id_indexs[effect_id]];
		
	
		int p = effects[effect_id_indexs[effect_id]]->getPartsSize();
		if (p == index) {
			EffectPart* pt = new EffectPart(is_loop, endtime,time_when_loop, plustime_loop);
			efe->setParts(pt);
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "makeeffectpart");
}	
	
	
	
	
void EffectManager::setEffectPartPos(int effect_id, int index,  float dtime_start, float dtime_end, float start_x, float start_y, float start_z, float end_x, float end_y, float end_z) {
	if (effect_id == KTROBO_EFFECT_NONE) return;
	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectpart pos");

	if (hasEffectPartIndex(effect_id, index)) {
		Effect* efe = effects[effect_id_indexs[effect_id]];
		EffectPart* efep = efe->getPart(index);
		EffectPartPos* ppp = new EffectPartPos(dtime_start,dtime_end, start_x,start_y,start_z,end_x,end_y,end_z);
		efep->poss.push_back(ppp);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectpart pos");
}

void EffectManager::setEffectPartTexPos(int effect_id, int index,  float dtime_start, float dtime_end, int tex_x,int tex_y, int tex_width, int tex_height) {
	if (effect_id == KTROBO_EFFECT_NONE) return;
	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectpart texpos");

	if (hasEffectPartIndex(effect_id, index)) {
		Effect* efe = effects[effect_id_indexs[effect_id]];
		EffectPart* efep = efe->getPart(index);
		EffectPartTexPos* ppp = new EffectPartTexPos(dtime_start,dtime_end, tex_x,tex_y,tex_width,tex_height);
		efep->tex_poss.push_back(ppp);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectpart texpos");


}

void EffectManager::setEffectPartWH(int effect_id, int index, float dtime_start, float dtime_end,float start_width, float start_height, float end_width, float end_height) {
	if (effect_id == KTROBO_EFFECT_NONE) return;
	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectpart wh");

	if (hasEffectPartIndex(effect_id, index)) {
		Effect* efe = effects[effect_id_indexs[effect_id]];
		EffectPart* efep = efe->getPart(index);
		EffectPartWH* ppp = new EffectPartWH(dtime_start,dtime_end, start_width,start_height, end_width, end_height);
		efep->whs.push_back(ppp);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectpart wh");


}

void EffectManager::setEffectPartRot(int effect_id, int index, float dtime_start, float dtime_end, float start_rotx, float start_roty, float start_rotz, float end_rotx, float end_roty, float end_rotz) {
	if (effect_id == KTROBO_EFFECT_NONE) return;
	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectpart rot");

	if (hasEffectPartIndex(effect_id, index)) {
		Effect* efe = effects[effect_id_indexs[effect_id]];
		EffectPart* efep = efe->getPart(index);
		EffectPartRot* ppp = new EffectPartRot(dtime_start,dtime_end, start_rotx,start_roty,start_rotz,end_rotx,end_roty,end_rotz);
		efep->rots.push_back(ppp);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectpart rot");
}

void EffectManager::setEffectPartColor(int effect_id,int index, float dtime_start, float dtime_end, unsigned int start_color, unsigned int end_color) {
	if (effect_id == KTROBO_EFFECT_NONE) return;
	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectpart color");

	if (hasEffectPartIndex(effect_id, index)) {
		Effect* efe = effects[effect_id_indexs[effect_id]];
		EffectPart* efep = efe->getPart(index);
		EffectPartColor* ppp = new EffectPartColor(dtime_start,dtime_end, start_color,end_color);
		efep->colors.push_back(ppp);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectpart color");

}

void EffectManager::deleteEffect(int effect_id) {
	// effect_part_idを開放する　effect自体は残す
	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectpart color");

	if (effect_id_indexs.find(effect_id) != effect_id_indexs.end()) {
		Effect* efe = effects[effect_id_indexs[effect_id]];
		efe->Release();
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectpart color");
}


void EffectManager::release() {
	
	// すべてを開放する
	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectpart color");

	effect_id_indexs.clear();
	effect_impl_id_indexs.clear();
	effect_name_indexs.clear();
	vector<Effect*>::iterator it = effects.begin();
	while(it != effects.end()) {
		Effect* efe = *it;
		delete efe;
		efe = 0;
		it++;
	}
	effects.clear();

	vector<EffectImpl*>::iterator itt = effect_impls.begin();
	while(itt != effect_impls.end()) {
		EffectImpl* efe = *itt;
		delete efe;
		efe = 0;
		itt++;
	}
	effect_impls.clear();
	dummy_effect = 0;// ループで消去されているはず
	dummy_effect_impl = 0;
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectpart color");


}

void EffectImpl::changeEffectID(Texture* texture, Effect* mae_effect, Effect* new_effect) {

	if (mae_effect->getEffectID() != new_effect->getEffectID()) {
		// 違うエフェクトなので

		int mae_psiz = mae_effect->getPartsSize();
		int new_psiz = new_effect->getPartsSize();
		int impl_psiz = this->effectpart_billboard_ids.size();

		if (impl_psiz < new_psiz) {
			// 新しいビルボードを取得して　また　元のものはテクスチャをチェンジする
			// impl_psiz > mae_psiz だったとしても　mae_effectのテクスチャになってるようにする
			int sa = new_psiz - impl_psiz;
			int tex_index = new_effect->getTextureID();
			MYMATRIX mat;
			MyMatrixIdentity(mat);
			for (int i=0;i<impl_psiz;i++) {
				int bid = effectpart_billboard_ids[i];
				texture->setRenderBillBoardChangeTex(bid, mae_effect->getTextureID(), tex_index);
			}
			

			for (int i=0;i<sa;i++) {

				int bid = texture->getRenderBillBoard(tex_index, 0xFFFFFFFF, &mat,1,1,0,0,1,1);
				this->effectpart_billboard_ids.push_back(bid);
			//	this->looptime_effectpart.push_back(0);
			}

			//looptimeをクリアする
			looptime_effectpart.clear();
			for (int i=0;i<new_psiz;i++) {
				looptime_effectpart.push_back(0);
			}
		} else {
			// 新しいビルボードはいらない

			int tex_index = new_effect->getTextureID();
			MYMATRIX mat;
			MyMatrixIdentity(mat);
			for (int i=0;i<impl_psiz;i++) {
				int bid = effectpart_billboard_ids[i];
				texture->setRenderBillBoardChangeTex(bid, mae_effect->getTextureID(), tex_index);
			}
			//looptimeをクリアする
			looptime_effectpart.clear();
			for (int i=0;i<impl_psiz;i++) {
				looptime_effectpart.push_back(0);
			}


		}

		this->effect_id = new_effect->getEffectID();
	} else {
		int impl_psiz = this->effectpart_billboard_ids.size();

		//looptimeをクリアする
		looptime_effectpart.clear();
		for (int i=0;i<impl_psiz;i++) {
			looptime_effectpart.push_back(0);
		}
	}
}

EffectImpl* EffectManager::getUnUsedEffectImpl(Effect* efe) {
	
	set<int>::iterator it = unuse_effect_impl_indexs.begin();
	while(it != unuse_effect_impl_indexs.end()) {
		int ii = *it;
		if (used_unuse_effect_impl_indexs.find(ii) != used_unuse_effect_impl_indexs.end()) {



		} else {
			// あったので
			EffectImpl* imp =  effect_impls[ii];
			
			MYMATRIX mat;
			MyMatrixIdentity(mat);
			
			if (effect_id_indexs.find(imp->getEffectID()) != effect_id_indexs.end()) {
				Effect* mae_efe = effects[effect_id_indexs[imp->getEffectID()]];
				imp->changeEffectID(tex, mae_efe,efe);
			}
			imp->setIsActivated(false);
			imp->setTime(0);
			imp->setWorld(&mat);
			used_unuse_effect_impl_indexs.insert(ii);
			if (used_unuse_effect_impl_indexs.size() == unuse_effect_impl_indexs.size()) {
				// どちらもクリアする
				used_unuse_effect_impl_indexs.clear();
				unuse_effect_impl_indexs.clear();

			}
			return imp;
		}
		it++;
	}
	return NULL;

}







int EffectManager::getEffectImpl(int effect_id) {
	
	// effectの実体を作る
	// まずエフェクトがない場合はnoneを返す
	if (effect_id == KTROBO_EFFECT_NONE) return KTROBO_EFFECT_NONE;

	CS::instance()->enter(CS_RENDERDATA_CS, "geteffectimpl");
	if (effect_id_indexs.find(effect_id) != effect_id_indexs.end()) {
		// あるので
		Effect* efe = effects[effect_id_indexs[effect_id]];
		EffectImpl* impl = getUnUsedEffectImpl(efe);
		int impl_id= KTROBO_EFFECT_NONE;
		if (!impl) {
			
			int impl_index = effect_impls.size();
			impl_id = impl_index;
			impl = new EffectImpl(impl_id, effect_id);
			int psiz = impl->effectpart_billboard_ids.size();
			int psii = efe->getPartsSize();
			int sii = psii - psiz;
			int tex_index = efe->getTextureID();
			MYMATRIX mat;
			MyMatrixIdentity(mat);
			for (int i=0;i<sii;i++) {
				int bid = tex->getRenderBillBoard(tex_index, 0xFFFFFFFF, &mat,1,1,0,0,1,1);
				impl->effectpart_billboard_ids.push_back(bid);
			}

			impl->looptime_effectpart.clear();
			for (int i=0;i<psii;i++) {
				impl->looptime_effectpart.push_back(0);
			}


			effect_impls.push_back(impl);
			effect_impl_id_indexs.insert(std::pair<int,int>(impl_id,impl_index));
		} else {
			impl_id = impl->getEffectImplID();
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "geteffectimpl");
		return impl_id;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "geteffectimpl");
	return KTROBO_EFFECT_NONE;
}
void EffectManager::setEffectImplWorld(int effect_impl_id, MYMATRIX* world) {
	if (effect_impl_id == KTROBO_EFFECT_NONE) return;

	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectimpl world");
	if (effect_impl_id_indexs.find(effect_impl_id) != effect_impl_id_indexs.end()) {
		// あるので
		EffectImpl* imp = effect_impls[effect_impl_id_indexs[effect_impl_id]];
		imp->setWorld(world);
		CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl world");
		return;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl world");
}

void EffectManager::setEffectImplTime(int effect_impl_id, float time) {
if (effect_impl_id == KTROBO_EFFECT_NONE) return;

	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectimpl time");
	if (effect_impl_id_indexs.find(effect_impl_id) != effect_impl_id_indexs.end()) {
		// あるので
		EffectImpl* imp = effect_impls[effect_impl_id_indexs[effect_impl_id]];
		imp->setTime(time);
		CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl time");
		return;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl time");

}

void EffectManager::setEffectImplIsRender(int effect_impl_id, bool t) {
	if (effect_impl_id == KTROBO_EFFECT_NONE) return;

	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectimpl isrender");
	if (effect_impl_id_indexs.find(effect_impl_id) != effect_impl_id_indexs.end()) {
		// あるので
		EffectImpl* imp = effect_impls[effect_impl_id_indexs[effect_impl_id]];
		int siz = imp->effectpart_billboard_ids.size();
		for (int i=0;i<siz;i++) {
			tex->setRenderBillBoardIsRender(imp->effectpart_billboard_ids[i],t);
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl isrender");
		return;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl isrender");

}
bool EffectManager::getEffectImplIsActivated(int effect_impl_id) {
	if (effect_impl_id == KTROBO_EFFECT_NONE) return false;

//	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectimpl isstart");
	if (effect_impl_id_indexs.find(effect_impl_id) != effect_impl_id_indexs.end()) {
		// あるので
		EffectImpl* imp = effect_impls[effect_impl_id_indexs[effect_impl_id]];
		bool t = imp->getIsActivated();
	//	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl isstart");
		return t;
	}
//	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl isstart");
	return false;
}

void EffectManager::setEffectImplIsStart(int effect_impl_id, bool t) {
	if (effect_impl_id == KTROBO_EFFECT_NONE) return;

	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectimpl isstart");
	if (effect_impl_id_indexs.find(effect_impl_id) != effect_impl_id_indexs.end()) {
		// あるので
		EffectImpl* imp = effect_impls[effect_impl_id_indexs[effect_impl_id]];
		imp->setIsActivated(t);
		CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl isstart");
		return;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl isstart");

}

void EffectManager::lightdeleteEffectImpl(int effect_impl_id) {
	// effect_impl を未使用にする 消しはしない（再利用する）
	if (effect_impl_id == KTROBO_EFFECT_NONE) return;

	CS::instance()->enter(CS_RENDERDATA_CS, "seteffectimpl isstart");
	if (effect_impl_id_indexs.find(effect_impl_id) != effect_impl_id_indexs.end()) {
		// あるので
		// まずbillboardを非表示にする
		EffectImpl* impl = effect_impls[effect_impl_id_indexs[effect_impl_id]];

		int siz = impl->effectpart_billboard_ids.size();
		for (int i=0;i<siz;i++) {
			tex->setRenderBillBoardIsRender(impl->effectpart_billboard_ids[i],false);
		}
		unuse_effect_impl_indexs.insert(effect_impl_id_indexs[effect_impl_id]);
		CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl isstart");
		return;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "seteffectimpl isstart");
}	
	
	
	
	
	
void EffectManager::deleteEffectImpl() {
	// effect_implを消去する
	CS::instance()->enter(CS_RENDERDATA_CS, "delete effect impl");
	effect_impl_id_indexs.clear();
	unuse_effect_impl_indexs.clear();
	used_unuse_effect_impl_indexs.clear();
	vector<EffectImpl*>::iterator it = effect_impls.begin();
	dummy_effect_impl = 0;
	while(it != effect_impls.end()) {
		EffectImpl* ii = *it;
		ii->release(tex);
		delete ii;
		ii = 0;
		it++;
	}
	effect_impls.clear();

	// dummy を入れ込む
	dummy_effect_impl = new EffectImpl(KTROBO_EFFECT_IMPL_DUMMY_ID, KTROBO_EFFECT_DUMMY_ID);
	effect_impls.push_back(dummy_effect_impl);	
	effect_impl_id_indexs.insert(std::pair<int,int>(KTROBO_EFFECT_IMPL_DUMMY_ID, KTROBO_EFFECT_IMPL_DUMMY_INDEX));
	CS::instance()->leave(CS_RENDERDATA_CS, "delete effect impl");


}

void EffectManager::update(float dtime, int stamp) {


	if (dtime > 200) return;

	// 管理しているエフェクトの実体の状態を更新する
	CS::instance()->enter(CS_RENDERDATA_CS, "update effect impl");
	// effectimpl の更新
	vector<EffectImpl*>::iterator it = effect_impls.begin();
	while(it != effect_impls.end()) {

		EffectImpl* eimp = *it;
		if (eimp->getIsActivated()) {
			if (effect_id_indexs.find(eimp->getEffectID()) != effect_id_indexs.end()) {
				effects[effect_id_indexs[eimp->getEffectID()]]->update(eimp, tex,dtime,stamp);
			}
		}
		it++;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "update effect impl");

}

EffectManagers::EffectManagers(Texture* tex) {
	this->tex = tex;

	EffectManager* manag = new EffectManager(tex);
	managers.push_back(manag);
}

EffectManagers::~EffectManagers() {
	vector<EffectManager*>::iterator it = managers.begin();
	while( it != managers.end()) {
		EffectManager* m = *it;
		m->release();
		delete m;
		m = 0;
		it++;
	}
	managers.clear();
}

void EffectManagers::update(float dtime, int stamp) {
	if (managers.size()) {
		managers[0]->update(dtime, stamp);
	}
}
