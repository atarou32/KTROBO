#ifndef KTROBOTEXTURE_H
#define KTROBOTEXTURE_H

#pragma once
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "MyDefine.h"
#include "KTRoboGraphics.h"
#include "MyTextureLoader.h"
#include "KTRoboCS.h"

namespace KTROBO {

interface ITexture{ 
	// ���ӁFlua�̃R���[�`�����炽�����񂱂̊֐����Ă΂Ȃ�����
	// ���Ԃ񍡂̎d�g�݂��ƃ��b�N��������Ȃ��܂܁i�X���b�h�������Ȃ̂ŁE�E�E�j
	// �A�N�Z�X����̂ŋ��������܂�Ă��܂�
	// lua���ŃR���[�`������֐��Ăяo�����X�g�b�N���Ă����āA��ӏ��ŏ��X�ɌĂяo���悤�Ȏd�g�݂�lua���ɕK�v

public:
	TO_LUA int getTexture(char* tex_name); // ���łɃ��[�h����Ă����ꍇ�̓��[�h�͍s���Ȃ�
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


class TexturePart {
private:
	bool is_use;
	MyTextureLoader::MY_TEXTURE_CLASS* tex_class;
	bool is_need_load;
	bool is_index_load;// indexbuffer���N���G�C�g����Ă��邩�ǂ���
	ID3D11Buffer* indexbuffer; // 
	int index_count;
public:
	TexturePart(int index_c) {
		is_use =true;
		tex_class = 0;
		is_need_load = false;
		is_index_load = false;
		indexbuffer = 0;
		index_count = index_c;
	}

	int getIndexCount() {
		CS::instance()->enter(CS_RENDERDATA_CS, "enter texturepart");
		int ans = index_count;
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
};

class Texture
{
private:
	MyTextureLoader* loader;
	vector<RenderBillBoard*> bill_boards;
	vector<RenderTex*> render_texs;
	vector<TexturePart*> parts;
	map<string, int> texturepart_index;

public:
	Texture(void);
	~Texture(void);
	void render(Graphics* g); // ������RENDERDATA_CS, DEVICECON_CS���א؂�Ƀ��b�N���邱�� // �`��X���b�h�ŌĂ�
	void sendinfoToVertexTexture(Graphics* g);// ������RENDERDATA_CS, DEVICECON_CS���א؂�Ƀ��b�N���邱�� // �`��⏕�X���b�h�ŌĂ�
	void createIndexBuffer(Graphics* g);// ���[�h�X���b�h�ŌĂ�

	int getTexture(char* tex_name); // ���łɃ��[�h����Ă����ꍇ�̓��[�h�͍s���Ȃ�
	int getRenderTex(int tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	int getRenderBillBoard(int tex_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexParam(int render_tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height);
	void setRenderBillBoardParam(int bill_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexColor(int render_tex_index, unsigned int color);
	void setRenderBillBoardColor(int bill_index, unsigned int color);

	void setRenderTexPos(int render_tex_index, int x, int y);
	void setRenderBillBoardPos(int bill_index, YARITORI MYMATRIX* world);

	void setRenderTexWH(int render_tex_index, int width, int height);
	void setRenderBillBoardWH(int bill_index, float width, float height);

	void setRenderTexTexPos(int render_tex_index, int tex_x, int tex_y, int tex_width, int tex_height);
	void setRenderBillBoardTexPos(int bill_index, int tex_x, int tex_y, int tex_width, int tex_height);

	void setRenderTexIsRender(int render_tex_index, bool t);
	void setRenderBillBoardIsRender(int bill_index, bool t);
	
	void setRenderTexChangeTex(int render_tex_index, int tex_index, int new_tex_index);
	void setRenderBillBoardChangeTex(int bill_index, int tex_index, int new_tex_index);

	void deleteRenderTex(int render_tex_index); // ���b�N���ǂ����邩�l���ǂ���
	void deleteRenderBillBoard(int bill_index); // ���b�N���ǂ����邩�l���ǂ���
	void lightdeleteRenderTex(int render_tex_index);
	void lightdeleteRenderBillBoard(int bill_index);

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


#endif