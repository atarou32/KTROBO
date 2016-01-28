#ifndef KTROBOTEXTURE_H
#define KTROBOTEXTURE_H

#pragma once
#include "tolua_glue/MyLuaGlueMakeCommon.h"
#include "MyDefine.h"

namespace KTROBO {

interface ITexture{
public:
	TO_LUA int getTexture(char* tex_name); // ���łɃ��[�h����Ă����ꍇ�̓��[�h�͍s���Ȃ�
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
	
	TO_LUA void deleteRenderTex(int render_tex_index);
	TO_LUA void deleteRenderBillBoard(int bill_index);
	TO_LUA void deleteAll();
};


class Texture
{
public:
	Texture(void);
	~Texture(void);
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
// 
class RenderTex
{
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
};

class RenderBillBoard {
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
};

#endif