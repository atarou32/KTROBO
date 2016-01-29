#include "KTRoboTexture.h"
using namespace KTROBO;

int Texture::getTexture(char* tex_name, int index_count) {
	// ���łɃ��[�h����Ă����ꍇ�̓��[�h�͍s���Ȃ�
	CS::instance()->enter(CS_RENDERDATA_CS, "gettex");
	if (this->texturepart_index.find(tex_name) != texturepart_index.end()) {
		CS::instance()->leave(CS_RENDERDATA_CS, "gettex");
		return texturepart_index[tex_name];
	}
	// �Ȃ��̂ō��
	TexturePart* p = new TexturePart(index_count);
	try {
	p->loadClass(this->loader, tex_name);
	}catch(GameError* err) {
		delete p;
		CS::instance()->leave(CS_RENDERDATA_CS, "gettex");
		throw err;
	}
	int size = this->parts.size();
	texturepart_index.insert(pair<string,int>(string(tex_name), size));
	parts.push_back(p); // �܂�p��indexbuffer������Ă��Ȃ����Ƃɒ���
	CS::instance()->leave(CS_RENDERDATA_CS, "gettex");
	return size;
}





int Texture::getRenderTex(int tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height) {
	// �󂢂Ă���ꏊ�����邩�ǂ������ׂ�
	CS::instance()->enter(CS_RENDERDATA_CS, "rendertex");
	int size = unuse_render_texs.size();
	RenderTex* use_tex;
	if (tex_index < this->parts.size()) {
		// �Y���̃e�N�X�`����is_need_load��true�ɂ���
		// loadtovertextexture�ł�is_need_load�̑O��is_use������悤�ɂ��邱��

		parts[tex_index]->setIsNeedLoad(true);
	} else {
		// ��������
		CS::instance()->leave(CS_RENDERDATA_CS, "rendertex");
		throw new GameError(KTROBO::WARNING, "there is no tex in getRenderTex");
	}
	if (size >0) {
		use_tex  =*unuse_render_texs.begin();
		unuse_render_texs.erase(use_tex);
	} else {
		// �󂢂Ă���ID���Ȃ����ǂ������ׂ�
		int s = unuse_render_tex_ids.size();
		use_tex = new RenderTex();
		if (s > 0) {
			int id = *unuse_render_tex_ids.begin();
			unuse_render_tex_ids.erase(id);
			use_tex->id = id;
			int rentexindex = render_texs.size();
			render_tex_indexs.insert(pair<int,int>(id,rentexindex));
		} else {
			// ����render_tex�̃T�C�Y��ID�ƂȂ�͂�
			int id = render_texs.size();
			use_tex->id = id;
			render_tex_indexs.insert(pair<int,int>(id,id));
		}
		render_texs.push_back(use_tex);
	}	
	use_tex->color = color;
	use_tex->height = height;
	// id�͂��̂܂�
	use_tex->is_need_load = true;
	use_tex->is_render = false;
	use_tex->is_use = true;
	use_tex->tex_height = tex_height;
	use_tex->tex_index = tex_index;
	use_tex->tex_width = tex_width;
	use_tex->tex_x = tex_x;
	use_tex->tex_y = tex_y;
	use_tex->width = width;
	use_tex->x = x;
	use_tex->y = y;
	int ans = use_tex->id;
	parts[tex_index]->setRenderTexId(use_tex->id);
	CS::instance()->leave(CS_RENDERDATA_CS, "rendertex");
	return ans;
}
int Texture::getRenderBillBoard(int tex_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height) {

	// �󂢂Ă���ꏊ�����邩�ǂ������ׂ�
	CS::instance()->enter(CS_RENDERDATA_CS, "renderbillb");
	int size = unuse_render_texs.size();
	RenderBillBoard* use_bill;
	if (tex_index < this->parts.size()) {
		// �Y���̃e�N�X�`����is_need_load��true�ɂ���
		// loadtovertextexture�ł�is_need_load�̑O��is_use������悤�ɂ��邱��

		parts[tex_index]->setIsNeedLoad(true);
	} else {
		// ��������
		CS::instance()->leave(CS_RENDERDATA_CS, "renderbillb");
		throw new GameError(KTROBO::WARNING, "there is no tex in getRenderBillBoard");
	}
	if (size >0) {
		// unuse_bill_boards�̂Ȃ��̂��̂𗘗p����ꍇ��ID���ė��p����
		// index�����̂܂� is_use����false�ɂȂ��Ă銴��
		use_bill  =*unuse_bill_boards.begin();
		unuse_bill_boards.erase(use_bill);
	} else {
		// �󂢂Ă���ID���Ȃ����ǂ������ׂ�
		int s = unuse_bill_board_ids.size();
		use_bill = new RenderBillBoard();
		if (s > 0) {

			int id = *unuse_bill_board_ids.begin();
			unuse_bill_board_ids.erase(id);
			use_bill->id = id;
			int bill_index = bill_boards.size();
			bill_board_indexs.insert(pair<int,int>(id, bill_index));
		} else {
			// ����render_tex�̃T�C�Y��ID�ƂȂ�͂�
			int id = bill_boards.size();
			use_bill->id = id;
			bill_board_indexs.insert(pair<int,int>(id,id));

		}
		bill_boards.push_back(use_bill);
	}	
	use_bill->color = color;
	use_bill->height = height;
	// id�͂��̂܂�
	use_bill->is_need_load = true;
	use_bill->is_render = false;
	use_bill->is_use = true;
	use_bill->tex_height = tex_height;
	use_bill->tex_index = tex_index;
	use_bill->tex_width = tex_width;
	use_bill->tex_x = tex_x;
	use_bill->tex_y = tex_y;
	use_bill->width = width;
	use_bill->world = *world;
	
	int ans = use_bill->id;
	parts[tex_index]->setBillBoardId(use_bill->id);
	CS::instance()->leave(CS_RENDERDATA_CS, "renderbillb");
	return ans;
}

void Texture::setRenderTexParam(int render_tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexparam");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		e->color = color;
		e->x = x;
		e->y = y;
		e->height = height;
		e->is_need_load = true;
		e->tex_height = tex_height;
		e->tex_width = tex_width;
		e->tex_x = tex_x;
		e->tex_y = tex_y;
		e->width = width;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexparam");
}
void Texture::setRenderBillBoardParam(int bill_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardparam");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		e->color = color;
		e->world = *world;
		e->height = height;
		e->is_need_load = true;
		e->tex_height = tex_height;
		e->tex_width = tex_width;
		e->tex_x = tex_x;
		e->tex_y = tex_y;
		e->width = width;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardparam");

}

void Texture::setRenderTexColor(int render_tex_index, unsigned int color) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexcolor");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		e->color = color;
		e->is_need_load = true;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexcolor");
}
void Texture::setRenderBillBoardColor(int bill_index, unsigned int color) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardcolor");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		e->color = color;	
		e->is_need_load = true;	
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardcolor");

}

void Texture::setRenderTexPos(int render_tex_index, int x, int y) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexpos");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		e->x = x;
		e->y = y;
		e->is_need_load = true;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexpos");


}
void Texture::setRenderBillBoardPos(int bill_index, YARITORI MYMATRIX* world) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardpos");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		e->world = *world;	
		e->is_need_load = true;	
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardpos");


}

void Texture::setRenderTexWH(int render_tex_index, int width, int height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexwh");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		e->width = width;
		e->height = height;
		e->is_need_load = true;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexwh");



}


void Texture::setRenderBillBoardWH(int bill_index, float width, float height) {
	
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardwh");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		e->width = width;
		e->height = height;
		e->is_need_load = true;	
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardwh");
}

void Texture::setRenderTexTexPos(int render_tex_index, int tex_x, int tex_y, int tex_width, int tex_height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertextpos");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		e->tex_x = tex_x;
		e->tex_y = tex_y;
		e->tex_width = tex_width;
		e->tex_height = tex_height;
		e->is_need_load = true;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertextpos");
}



void Texture::setRenderBillBoardTexPos(int bill_index, int tex_x, int tex_y, int tex_width, int tex_height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardtpos");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		e->tex_x = tex_x;
		e->tex_y = tex_y;
		e->tex_width = tex_width;
		e->tex_height = tex_height;
		e->is_need_load = true;	
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardtpos");

}

void Texture::setRenderTexIsRender(int render_tex_index, bool t) {

	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexisrender");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		TexturePart* p = parts[e->tex_index];
		e->is_render = t;
//		e->is_need_load = true;
		p->setIsNeedLoad(true);
		p->setRenderTexId(e->id);

	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexisrender");
}


void Texture::setRenderBillBoardIsRender(int bill_index, bool t) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardisrender");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		TexturePart* p = parts[e->tex_index];
		e->is_render = t;
//		e->is_need_load = true;	
		p->setIsNeedLoad(true);
		p->setBillBoardId(e->id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardisrender");
}
	
void Texture::setRenderTexChangeTex(int render_tex_index, int tex_index, int new_tex_index) {
	// �`�悳���e�N�X�`����ς���ɂ́E�E�E
	// render_tex��is_need_load�͕ς��Ȃ��Ă悢�itex_index�͒��_�e�N�X�`���̒��ɂȂ��̂�)
	// �Y������e�N�X�`����is_need_load��true�ɂ��Ȃ��Ă͂Ȃ�Ȃ�
	
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexchangetex");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* t = render_texs[render_tex_indexs[render_tex_index]];
		TexturePart* p = parts[tex_index];
		TexturePart* p2 = parts[new_tex_index];
		t->tex_index = new_tex_index;
		p->setIsNeedLoad(true);
		p2->setIsNeedLoad(true);
		p->eraseRenderTexId(t->id);
		p2->setRenderTexId(t->id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexchangetex");
}


void Texture::setRenderBillBoardChangeTex(int bill_index, int tex_index, int new_tex_index) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardchangetex");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* t = bill_boards[bill_board_indexs[bill_index]];
		TexturePart* p = parts[tex_index];
		TexturePart* p2 = parts[new_tex_index];
		t->tex_index = new_tex_index;
		p->setIsNeedLoad(true);
		p2->setIsNeedLoad(true);
		p->eraseBillBoardId(t->id);
		p2->setBillBoardId(t->id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardchangetex");
}


void Texture::deleteRenderTex(int render_tex_index) {
	
	// ���b�N���ǂ����邩�l���ǂ��� RENDERDATA�����ő��v�����E�E�H
	CS::instance()->enter(CS_RENDERDATA_CS, "deleterendertex");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* t = render_texs[render_tex_indexs[render_tex_index]];
		TexturePart* p = parts[t->tex_index];
		// p ��is_need_load��true�ɂ���̂�delete���������ۂɍs���Ă���
		erase_render_tex_ids.insert(t->id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "deleterendertex");
}


void Texture::deleteRenderBillBoard(int bill_id) {
	// ���b�N���ǂ����邩�l���ǂ���
	CS::instance()->enter(CS_RENDERDATA_CS, "deletebillboard");
	if (bill_board_indexs.find(bill_id) != bill_board_indexs.end()) {
		RenderBillBoard* t = bill_boards[bill_board_indexs[bill_id]];
		erase_bill_board_ids.insert(t->id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "deletebillboard");

}




void Texture::lightdeleteRenderTex(int render_tex_index) {
	// ���b�N���ǂ����邩�l���ǂ��� RENDERDATA�����ő��v�����E�E�H
	CS::instance()->enter(CS_RENDERDATA_CS, "lightdeleterendertex");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* t = render_texs[render_tex_indexs[render_tex_index]];
		TexturePart* p = parts[t->tex_index];
		// vector����폜���Ȃ��̂Ńt���O�𗧂Ă邾��
		t->is_use = false;
		p->eraseRenderTexId(t->id);
		p->setIsNeedLoad(true);
		unuse_render_texs.insert(t);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "lightdeleterendertex");

}
void Texture::lightdeleteRenderBillBoard(int bill_id) {
	// ���b�N���ǂ����邩�l���ǂ���
	CS::instance()->enter(CS_RENDERDATA_CS, "deletebillboard");
	if (bill_board_indexs.find(bill_id) != bill_board_indexs.end()) {
		RenderBillBoard* t = bill_boards[bill_board_indexs[bill_id]];
		TexturePart* p = parts[t->tex_index];
		p->eraseBillBoardId(t->id);
		p->setIsNeedLoad(true);
		t->is_use = false;
		unuse_bill_boards.insert(t);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "deletebillboard");


}

void Texture::deleteAll() {
	// ���ԂƂ��C�ɂ��Ȃ��őS��delete����

	CS::instance()->enter(CS_RENDERDATA_CS, "deleteall");
	
	is_all_delete = true;

	CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");

}

void Texture::_render(Graphics* g, int part_size, int p_index) {


	CS::instance()->enter(CS_DEVICECON_CS, "texture render");
	CS::instance()->enter(CS_RENDERDATA_CS, "texture render");
	int psize = parts.size();
	if (psize != part_size) {
		// ���傤�ǕύX���������^�C�~���O
		// ���̏ꍇ�͂ǂ����邩
		if (psize > part_size) {
			// �����������Ȃ̂ŋC�ɂ��Ȃ��ł܂킷
		} else {
			// �������̂ŋ����̂Ń��^�[������@����ŕ`�悳��Ȃ��^�C�~���O���łĂ��邯�ǂ��傤���Ȃ�

			CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
			CS::instance()->leave(CS_DEVICECON_CS, "texture render");
			return;
		}
	}
	TexturePart* p = parts[p_index];
	if (p->getIsUse()) {
		if (p->getIsIndexLoad()) {
			// need load��truefalse�ɂ�����炸�`�悷��
		
			// tex�̕`��
			g->getDeviceContext()->OMSetBlendState();
			g->getDeviceContext()->OMSetRenderTargets();
			g->getDeviceContext()->IASetIndexBuffer();
			g->getDeviceContext()->VSSetShader();
			g->getDeviceContext()->GSSetShader();
			g->getDeviceContext()->PSSetShader();
			g->getDeviceContext()->PSSetShaderResources(0,1,&p->getClass()->view);
			g->getDeviceContext()->VSSetShaderResources(0,1,&this->vtex_tex->view);
			g->getDeviceContext()->VSSetConstantBuffers(0,1,&this->cbuf_1);

			g->getDeviceContext()->DrawIndexed();


			// bill�̕`��
			g->getDeviceContext()->OMSetBlendState();
			g->getDeviceContext()->OMSetRenderTargets();
			g->getDeviceContext()->IASetIndexBuffer();
			g->getDeviceContext()->VSSetShader();
			g->getDeviceContext()->GSSetShader();
			g->getDeviceContext()->PSSetShader();
			g->getDeviceContext()->PSSetShaderResources(0,1,&p->getClass()->view);
			g->getDeviceContext()->VSSetShaderResources(0,1,&this->vtex_tex->view);
			g->getDeviceContext()->VSSetConstantBuffers(0,1,&this->cbuf_1);

			g->getDeviceContext()->DrawIndexed();

			// unset



		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
	CS::instance()->leave(CS_DEVICECON_CS, "texture render");


}
void Texture::render(Graphics* g) {
	// ������RENDERDATA_CS, DEVICECON_CS���א؂�Ƀ��b�N���邱�� // �`��X���b�h�ŌĂ�
	// tex���ƂɃ����_�[���Ă�
	CS::instance()->enter(CS_RENDERDATA_CS, "texture render");
	// texture�����擾���Ă���`��ɂ��悤����܂Ń��b�N�͊O���Ȃ����Ƃ肠�����擾����
	int psize = parts.size();
	CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
	for (int i=0;i<psize;i++) {
		_render(g, psize, i);
	}
}	
	
	
void Texture::sendinfoToVertexTexture(Graphics* g) {
	// ������RENDERDATA_CS, DEVICECON_CS���א؂�Ƀ��b�N���邱�� // �`��⏕�X���b�h�ŌĂ�

	
	
	
	
}
	
void Texture::createIndexBuffer(Graphics* g) {
	// ���[�h�X���b�h�ŌĂ�





}

#define KTROBO_TEXTURE_DELETEDAYO_ONELOOP_SAKUJYOKAISUU 50

void Texture::deletedayo(Graphics* g){
	// delete�������s���@���[�h�X���b�h�ŌĂ� �א؂��delete����?���Ԃ�`�悪����Ă��Ȃ��Ƃ��Ƃ��i���[�h��ʁH��ʑJ�ډ�ʂȂǂŌĂԂ��ƂɂȂ肻��)



	CS::instance()->enter(CS_RENDERDATA_CS, "deleteall");
	if (is_all_delete) {
		// �S�����������Ă�

		// billboard�֌W����������
	
		int bsize = bill_boards.size();
		unuse_bill_boards.clear();
		unuse_bill_board_ids.clear();
		erase_bill_board_ids.clear();
		bill_board_indexs.clear();
		for (int i=0;i<bsize;i++) {
			RenderBillBoard * b = bill_boards[i];
			if (b != &dummy_billboard) {
				delete b;
				b = 0;
			}
		}
		bill_boards.clear();
		// billboard��������

		// rendertex�֌W����������
		int rsize = render_texs.size();
		unuse_render_texs.clear();
		unuse_render_tex_ids.clear();
		erase_render_tex_ids.clear();
		render_tex_indexs.clear();
		for(int i=0;i<rsize;i++) {
			RenderTex* c = render_texs[i];
			if (c != &dummy_rendertex) {
				delete c;
				c = 0;
			}
		}
		render_texs.clear();

		// texture�֌W����������
		int psize = parts.size();
		for (int i=0;i<psize;i++) {
			TexturePart* pp = parts[i];
			pp->del();
			delete pp;
			pp = 0;
		}
		parts.clear();
		texturepart_index.clear();
		//����ł���
		CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
		return;
	} 
	CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");

	for (int i=0;i < KTROBO_TEXTURE_DELETEDAYO_ONELOOP_SAKUJYOKAISUU;i++) {
		CS::instance()->enter(CS_RENDERDATA_CS, "deleteall");

		int size = erase_render_tex_ids.size();
		if (size > 0) {
			int render_tex_id = *erase_render_tex_ids.begin();
			erase_render_tex_ids.erase(render_tex_id);
			if (render_tex_indexs.find(render_tex_id) != render_tex_indexs.end()) {
				RenderTex* t = render_texs[render_tex_indexs[render_tex_id]];
				render_texs[render_tex_indexs[render_tex_id]] = &dummy_rendertex;
		
				// �폜�������s��
				TexturePart* p = parts[t->tex_index];
				p->eraseRenderTexId(t->id);
				render_tex_indexs.erase(render_tex_id);
				unuse_render_tex_ids.insert(render_tex_id);

				delete t;
				t = 0;
			}
		} else {
			CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
			break;
		}

		CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
	}

	for (int i=0;i < KTROBO_TEXTURE_DELETEDAYO_ONELOOP_SAKUJYOKAISUU;i++) {
		CS::instance()->enter(CS_RENDERDATA_CS, "deleteall");

		int size = erase_bill_board_ids.size();
		if (size > 0) {
			int bill_id = *erase_bill_board_ids.begin();
			erase_bill_board_ids.erase(bill_id);
			if (bill_board_indexs.find(bill_id) != bill_board_indexs.end()) {
				RenderBillBoard* t = bill_boards[bill_board_indexs[bill_id]];
				bill_boards[bill_board_indexs[bill_id]] = &dummy_billboard;
		
				// �폜�������s��
				TexturePart* p = parts[t->tex_index];
				p->eraseRenderTexId(t->id);
				bill_board_indexs.erase(bill_id);
				unuse_bill_board_ids.insert(bill_id);
				delete t;
				t = 0;
			}
		} else {
			CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
			break;
		}

		CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
	}
}
