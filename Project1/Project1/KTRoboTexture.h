#ifndef KTROBOTEXTURE_H
#define KTROBOTEXTURE_H

#pragma once
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "MyDefine.h"
#include "KTRoboGraphics.h"
#include "MyTextureLoader.h"
#include "KTRoboCS.h"
#include <set>

namespace KTROBO {

interface ITexture{ 
	// ���ӁFlua�̃R���[�`�����炽�����񂱂̊֐����Ă΂Ȃ�����
	// ���Ԃ񍡂̎d�g�݂��ƃ��b�N��������Ȃ��܂܁i�X���b�h�������Ȃ̂ŁE�E�E�j
	// �A�N�Z�X����̂ŋ��������܂�Ă��܂�
	// lua���ŃR���[�`������֐��Ăяo�����X�g�b�N���Ă����āA��ӏ��ŏ��X�ɌĂяo���悤�Ȏd�g�݂�lua���ɕK�v

public:
	TO_LUA int getTexture(char* tex_name, int index_count); // ���łɃ��[�h����Ă����ꍇ�̓��[�h�͍s���Ȃ�
	// ������RENDERDATA_CS�����b�N���邱��
	TO_LUA int getRenderTex(int tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	TO_LUA int getRenderBillBoard(int tex_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	TO_LUA void setRenderTexParam(int render_tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	TO_LUA void setRenderBillBoardParam(int bill_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);
	
	TO_LUA void setRenderTexColor(int render_tex_index, unsigned int color);
	TO_LUA void setRenderBillBoardColor(int bill_index, unsigned int color);

	TO_LUA void setRenderTexPos(int render_tex_index, int x, int y);
	TO_LUA void setRenderBillBoardPos(int bill_index, YARITORI MYMATRIX* world);

	TO_LUA void setRenderTexWH(int render_tex_index, int width, int height);
	TO_LUA void setRenderBillBoardWH(int bill_index, float width, float height);

	TO_LUA void setRenderTexTexPos(int render_tex_index, int tex_x, int tex_y, int tex_width, int tex_height);
	TO_LUA void setRenderBillBoardTexPos(int bill_index, int tex_x, int tex_y, int tex_width, int tex_height);

	TO_LUA void setRenderTexIsRender(int render_tex_index, bool t);
	TO_LUA void setRenderBillBoardIsRender(int bill_index, bool t);
	
	TO_LUA void setRenderTexChangeTex(int render_tex_index, int tex_index, int new_tex_index);
	TO_LUA void setRenderBillBoardChangeTex(int bill_index, int tex_index, int new_tex_index);

	TO_LUA void deleteRenderTex(int render_tex_index); // vector��������@delete�͑��̃X���b�h�������Ă��Ȃ����Ƃ��m�F�����ق���������������Ȃ�
	TO_LUA void deleteRenderBillBoard(int bill_index); // vector�������  create��vector�̒������ς��Ȃ瑼�̃X���b�h���Ƃ߂�K�v������ is_use ��false����true�ɂ���ꍇ
	// �̂Ƃ��̓��b�N���ԂɋC�����邱�Ɓi���b�N����is_use���擾���Ă���is_use��true�ɂ��Ă��烍�b�N��؂邱��
	TO_LUA void lightdeleteRenderTex(int render_tex_index); // vector��������Ȃ��@index�o�b�t�@�͍X�V����is_use is_render is_need_load ��false�ɂ��� tex��is_need_load��true�ɂ���
	TO_LUA void lightdeleteRenderBillBoard(int bill_index); // vector��������Ȃ�  ���ۂ�index�o�b�t�@�̍X�V��vertextotexture�̒��ōs��
	TO_LUA void deleteAll();
};





// ���̃N���X�ɃA�N�Z�X����Ƃ���RENDERDATA_CS�����b�N���邱��
class RenderTex
{
public:
	unsigned short id;
	unsigned short tex_index; // ���_���Ɋ܂߂�K�v������̂��H
	unsigned int color;
	short x;
	short y;
	unsigned short width;
	unsigned short height;
	short tex_x;
	short tex_y;
	unsigned short tex_width;
	unsigned short tex_height;
	bool is_need_load;
	bool is_render;
	bool is_use;
	RenderTex() {
		id = 0;
		tex_index = 0;
		color = 0;
		x = 0;
		y = 0;
		width = 0;
		height = 0;
		tex_x = 0;
		tex_y = 0;
		tex_width = 0;
		tex_height = 0;
		is_need_load = false;
		is_render = false;
		is_use = true;
	};
};

// ���̃N���X�ɃA�N�Z�X����Ƃ���RENDERDATA_CS�����b�N���邱��
class RenderBillBoard 
{
public:
	unsigned short id; // 
	unsigned short tex_index;
	unsigned int color;
	MYMATRIX world;
	float width;
	float height;
	short tex_x;
	short tex_y;
	unsigned short tex_width;
	unsigned short tex_height;
	bool is_need_load;
	bool is_render;
	bool is_use;
	RenderBillBoard() {
		id = 0;
		tex_index = 0;
		color = 0;
		width = 0;
		height = 0;
		tex_x = 0;
		tex_y = 0;
		tex_width = 0;
		tex_height = 0;
		is_need_load = false;
		is_render = false;
		is_use = true;
		MyMatrixIdentity(world);
	};
};

#define KTROBO_TEXTURE_INDEXBUFFER_MAX 1024*3
#define KTROBO_TEXTURE_INDEXBUFFER_DEFAULT 128*3 // ���Ȃ�����
#define KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT 512

class Texture
{
private:
	static MYSHADERSTRUCT mss_for_render_tex;
	static MYSHADERSTRUCT mss_for_render_bill;
	static MYSHADERSTRUCT mss_for_vertextex;
	static ID3D11Buffer* vertexbuffer;

public:
	static void Init(Graphics* g);
	static void Del();
	static void loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename, char* vs_func_name, char* gs_func_name,
								char* ps_func_name, unsigned int ds_width,unsigned int ds_height,
								D3D11_INPUT_ELEMENT_DESC* layout, int numoflayout, bool blend_enable);


private:
	RenderTex dummy_rendertex; // vector�̃C���f�b�N�X������Ȃ����߂ɂ�������悤����
	RenderBillBoard dummy_billboard;
	MyTextureLoader* loader;
private:
	vector<RenderBillBoard*> bill_boards;
	vector<RenderTex*> render_texs;
private:
	vector<TexturePart*> parts;
	map<string, int> texturepart_index;
	set<RenderTex*> unuse_render_texs; // �g���Ă��Ȃ�render_tex
	set<RenderBillBoard*> unuse_bill_boards;
	set<int> unuse_render_tex_ids; // �폜����Ďg���ĂȂ�render_tex��ID
	set<int> unuse_bill_board_ids;
public:
	map<int,int> render_tex_indexs; // id index �̃y�A
	map<int,int> bill_board_indexs;
private:
	set<int> erase_render_tex_ids; // �폜����\���render_tex
	set<int> erase_bill_board_ids; // �폜����\���billboard
	bool is_all_delete;
	MyTextureLoader::MY_TEXTURE_CLASS* vtex_tex;
	MyTextureLoader::MY_TEXTURE_CLASS* vtex_bill;

	void _render(Graphics* g, int part_size, int p_index);
	void _createIndexBuffer(Graphics* g, int psize, int p_index);
public:
	Texture(MyTextureLoader* l) {
		loader = l;
		is_all_delete = false;
		vtex_tex = loader->makeClass(KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT, KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT);
		vtex_bill = loader->makeClass(KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT, KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT);
	}

	~Texture(void){};
	void render(Graphics* g); // ������RENDERDATA_CS, DEVICECON_CS���א؂�Ƀ��b�N���邱�� // �`��X���b�h�ŌĂ�
	void sendinfoToVertexTexture(Graphics* g);// ������RENDERDATA_CS, DEVICECON_CS���א؂�Ƀ��b�N���邱�� // �`��⏕�X���b�h�ŌĂ�
	void updateIndexBuffer(Graphics* g);//�`��⏕�X���b�h�ŌĂ�
	void createIndexBuffer(Graphics* g);// ���[�h�X���b�h�ŌĂ� �K���ȏ����l��create����
	void deletedayo(Graphics* g); // delete�������s���@���[�h�X���b�h�ŌĂ� �א؂��delete����

	int getTexture(char* tex_name, int index_count=KTROBO_TEXTURE_INDEXBUFFER_DEFAULT); // ���łɃ��[�h����Ă����ꍇ�̓��[�h�͍s���Ȃ�
	int getRenderTex(int tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	int getRenderBillBoard(int tex_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexParam(int render_tex_id, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	void setRenderBillBoardParam(int bill_id, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexColor(int render_tex_id, unsigned int color);
	void setRenderBillBoardColor(int bill_id, unsigned int color);

	void setRenderTexPos(int render_tex_id, int x, int y);
	void setRenderBillBoardPos(int bill_id, YARITORI MYMATRIX* world);

	void setRenderTexWH(int render_tex_id, int width, int height);
	void setRenderBillBoardWH(int bill_id, float width, float height);

	void setRenderTexTexPos(int render_tex_id, int tex_x, int tex_y, int tex_width, int tex_height);
	void setRenderBillBoardTexPos(int bill_id, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexIsRender(int render_tex_id, bool t);
	void setRenderBillBoardIsRender(int bill_id, bool t);
	
	void setRenderTexChangeTex(int render_tex_id, int tex_index, int new_tex_index);
	void setRenderBillBoardChangeTex(int bill_id, int tex_index, int new_tex_index);

	void deleteRenderTex(int render_tex_id); // ���b�N���ǂ����邩�l���ǂ���
	void deleteRenderBillBoard(int bill_id); // ���b�N���ǂ����邩�l���ǂ���
	void lightdeleteRenderTex(int render_tex_id);
	void lightdeleteRenderBillBoard(int bill_id);

	void deleteAll();

};

// �e�N�X�ƃr���{�[�h�ō\�����قȂ�܂��A�Ă΂��G�t�F�N�g�t�@�C�����قȂ�
// ���x���Ȃ��ݒ��_�e�N�X�`���Ƀe�N�X�ƃr���{�[�h�����i�[����
// �قȂ�e�N�X�`���̃e�N�X�ƃr���{�[�h�����邱�Ƃɂǂ�����đΉ�����΂����̂�
// ��1 ���͒��_���͌Œ�Œ��_�e�N�X�`����tex_index���܂߂�if (tex_index == now_index)�̂Ƃ�����
// �`�悷��@����ȊO�̂Ƃ���pixel shader��discard����
// ��2 �`��֐�����΂�邻�̓s�x���͒��_��CPU���Ńe�N�X�`�����Ƃɍ�蒼����GPU���ɑ����ăe�N�X�`���̎�ނ��Ƃɕ`����s��
// ��3 �e�N�X�`�����Ƃɏ\���ȑ傫���̃C���f�b�N�X�o�b�t�@��p�ӂ��Ă����@id�̏����ɕ��񂾔z�� ����͒��_���Ƃ��� 
// �C���f�b�N�X�o�b�t�@,���_�e�N�X�`�����X�V�����̂�
// �e�N�X�ƃr���{�[�h���A�j���[�V�����������񂪕ύX�ɂȂ����Ƃ�
// ��3�������悤�ȋC������ �ł��A�ق��̃e�N�X�`���ɕς��ꍇ�͂ǂ�����΂����̂��E�E�E�E
// �ς��O�̃e�N�X�`���ƕς���̃e�N�X�`���̃C���f�b�N�X�Ɋւ��Ē��_�e�N�X�`���A�C���f�b�N�X�o�b�t�@�̍X�V���s���΂悢

// ��3�ł�����
// ���_�e�N�X�`���ɏ����i�[���邽�߂̒��_����
// ���\��V��������Ȃ�
// struct{ 
// id
// tex_index
// color
// x
// y
// width
// height
// tex_x
// tex_y
// tex_width
// tex_height
// } ���X�V�܂��͐����j�����ꂽrendertex�Ɋւ��č쐬����
// drawinstanced �Ŋi�[����l�̃C���f�b�N�X���w�肷��
// geometry shader�ŕ����g�p�Ǝv�������ǁE�E�E�E
// struct {
// id
// value
// offset (color x+y width+height tex_x+tex_y tex_width+tex_height)
// geometryshader�ł������̂ق����������₷����
// drawinstanced�͕K�v�Ȃ�// �C���f�b�N�X�o�b�t�@������Ȃ��悤�ȋC������

// �i�[����l
// color x y width height tex_x tex_y tex_width tex_height
// 4   2   2  2     2       2      2     2       2  = 20
// R8G8B8A8 ���ƂP�e�N�Z���ɂS������@�ЂƂ̃e�N�X�ɂ�5�e�N�Z���K�v
// 512*512 �̃e�N�X�`������  ���悻50000�̃e�N�X��o�^�ł��邽�Ԃ񂱂�ȂɎg���Ȃ�
// id tex_index �Ɋւ��Ă�CPU���ŏ����������ĊǗ�����
// �r���{�[�h�̏ꍇ�͒��_����
// id
// tex_index
// color
// world
// width
// height
// tex_x
// tex_y
// tex_width
// tex_height
// �i�[����l��
// struct {
// id
// valueint
// valuefloat
// offset
// isint
// geometry�ŕ�������H
// color world width height tex_x tex_y tex_width tex_height
// 4    4* 16    4     4      2     2    2          2  = 84  21 �e�N�Z���K�v
// 512*512�̃e�N�X�`�����Ƃ��悻10000�̃r���{�[�h��o�^�ł���Ȃ�Ƃ����v����

// �j���Ɋւ��ā@�C���f�b�N�X�o�b�t�@���X�V����Ε`��͂���Ȃ��Ȃ�
// �ė��p�Ɋւ��� ���Ԃ�e�N�X����Ƀr���{�[�h�͐����j�����͂�����
// �󂢂Ă���id���������ē��ꍞ�߂�悤�Ȏd�g�݂��K�v�ȋC������
// 




class TexturePart {
private:
	bool is_use;
	MyTextureLoader::MY_TEXTURE_CLASS* tex_class;
	bool is_need_load;
	bool is_index_load;// indexbuffer���N���G�C�g����Ă��邩�ǂ���
	
	int index_count_max; // �ő�C���f�b�N�X�� buffer �͂����3�{
public:
	int index_count_tex; // ���݂̃C���f�b�N�X��
	int index_count_bill;
	ID3D11Buffer* indexbuffer_tex; // 
	ID3D11Buffer* indexbuffer_bill;
public:
	set<int> render_tex_ids;
	set<int> bill_board_ids;
public:

	TexturePart(int index_c) {
		is_use =true;
		tex_class = 0;
		is_need_load = false;
		is_index_load = false;
		indexbuffer_tex = 0;
		indexbuffer_bill = 0;
		index_count_max = index_c;
		index_count_bill = 0;
		index_count_tex = 0;
	}

	void createIndexBuffer(Graphics* g);

	// RENDERDATA�����b�N���邱��
	void del() {
		if (is_index_load) {
			if (indexbuffer_tex) {
				indexbuffer_tex->Release();
				indexbuffer_tex = 0;
			}
			if (indexbuffer_bill) {
				indexbuffer_bill->Release();
				indexbuffer_bill = 0;
			}
		}
		render_tex_ids.clear();
		bill_board_ids.clear();
		// tex_class�̓��[�_���Ǘ�����
	}

	void eraseRenderTexId(int render_tex_id) {
		CS::instance()->enter(CS_RENDERDATA_CS, "eraserendertexid");
		render_tex_ids.erase(render_tex_id);
		CS::instance()->leave(CS_RENDERDATA_CS, "eraserendertexid");
	}

	void eraseBillBoardId(int bill_id) {
		CS::instance()->enter(CS_RENDERDATA_CS, "erasebillboardid");
		bill_board_ids.erase(bill_id);
		CS::instance()->leave(CS_RENDERDATA_CS, "erasebillboardid");
	}


	void setRenderTexId(int render_tex_id) {
		CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexid");
		render_tex_ids.insert(render_tex_id);
		CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexid");
	}


	void setBillBoardId(int bill_id) {
		CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardid");
		bill_board_ids.insert(bill_id);
		CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardid");
	}



	int getIndexCount() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		int ans = index_count_max;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}
	//�킩��Ȃ��Ȃ�ɂ����߂Ă���
	void setIsIndexLoad(bool t) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		is_index_load = t;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
	}

	bool getIsIndexLoad() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		bool ans = is_index_load;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}



	void setIsNeedLoad(bool t) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		is_need_load = t;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
	}

	bool getIsNeedLoad() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		bool ans = is_need_load;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}


	void setIsUse(bool t) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		is_use = t;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
	}


	bool getIsUse() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		bool ans = is_use;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}

	MyTextureLoader::MY_TEXTURE_CLASS* getClass() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		MyTextureLoader::MY_TEXTURE_CLASS* ans = tex_class;
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
		return ans;
	}

	void loadClass(MyTextureLoader* loader, char* filename) {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		try {
		tex_class = loader->loadClass(filename);
		}catch (GameError* err) {
			CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
			throw err;
		}
		CS::instance()->leave(CS_RENDERDATA_CS, "leave texturepart");
	}

};











}
#endif