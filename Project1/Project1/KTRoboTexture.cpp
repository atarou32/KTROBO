#include "KTRoboTexture.h"
using namespace KTROBO;

void Texture::render(Graphics* g){} // ������RENDERDATA_CS, DEVICECON_CS���א؂�Ƀ��b�N���邱�� // �`��X���b�h�ŌĂ�
void Texture::sendinfoToVertexTexture(Graphics* g){}// ������RENDERDATA_CS, DEVICECON_CS���א؂�Ƀ��b�N���邱�� // �`��⏕�X���b�h�ŌĂ�
void Texture::createIndexBuffer(Graphics* g){}// ���[�h�X���b�h�ŌĂ�

int Texture::getTexture(char* tex_name, int index_count=KTROBO_TEXTURE_INDEXBUFFER_DEFAULT) {
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
		} else {
			// ����render_tex�̃T�C�Y��ID�ƂȂ�͂�
			int id = render_texs.size();
			use_tex->id = id;
		}
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
	CS::instance()->leave(CS_RENDERDATA_CS, "rendertex");
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
		} else {
			// ����render_tex�̃T�C�Y��ID�ƂȂ�͂�
			int id = bill_boards.size();
			use_bill->id = id;
		}
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
	CS::instance()->leave(CS_RENDERDATA_CS, "renderbillb");

}

void Texture::setRenderTexParam(int render_tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height) {





}
void Texture::setRenderBillBoardParam(int bill_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

void Texture::setRenderTexColor(int render_tex_index, unsigned int color);
void Texture::setRenderBillBoardColor(int bill_index, unsigned int color);

void Texture::setRenderTexPos(int render_tex_index, int x, int y);
void Texture::setRenderBillBoardPos(int bill_index, YARITORI MYMATRIX* world);

void Texture::setRenderTexWH(int render_tex_index, int width, int height);
void Texture::setRenderBillBoardWH(int bill_index, float width, float height);

void Texture::setRenderTexTexPos(int render_tex_index, int tex_x, int tex_y, int tex_width, int tex_height);
void Texture::setRenderBillBoardTexPos(int bill_index, int tex_x, int tex_y, int tex_width, int tex_height);

void Texture::setRenderTexIsRender(int render_tex_index, bool t);
void Texture::setRenderBillBoardIsRender(int bill_index, bool t);
	
void Texture::setRenderTexChangeTex(int render_tex_index, int tex_index, int new_tex_index);
void Texture::setRenderBillBoardChangeTex(int bill_index, int tex_index, int new_tex_index);

void Texture::deleteRenderTex(int render_tex_index); // ���b�N���ǂ����邩�l���ǂ���
void Texture::deleteRenderBillBoard(int bill_index); // ���b�N���ǂ����邩�l���ǂ���
void Texture::lightdeleteRenderTex(int render_tex_index);
void Texture::lightdeleteRenderBillBoard(int bill_index);

void Texture::deleteAll();