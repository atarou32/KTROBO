#ifndef MYLUAGLUEMAKECOMMON_H
#define MYLUAGLUEMAKECOMMON_H
#pragma once
#include "stdio.h"
#include "string.h"



#define MAKEGLUEINPUT_FILEPATH "FILEPATH"
#define MAKEGLUEINPUT_INTERFACE "INTERFACE"
#define MAKEGLUEINPUT_COLLECTION "COLLECTION"
#define MAKEGLUEINPUT_CONSTRUCTOR "CONSTRUCTOR"
#define MAKEGLUEINPUT_DESTRUCTOR "DESTRUCTOR"
#define MAKEGLUEINPUT_COLLECTED "COLLECTED"








#define TEMPHAIRETU_SIZE 32
#define TEMPSTRING_SIZE 1024
#define HIKISUU_MAX 16
#define IN_
#define OUT_
#define INOUT_
#define TO_LUA
#define COLLECTED
#define YARITORI

// �R���N�V�����N���X�̖��O�̓R���N�V���������N���X��s���������O�ɂ��邱��
// TO_LUA virtual INOUT COLLECTED YARITORI  �^�� �̏��ɏ�������
// COLLECTED ���g�p����ۂɂ͐e�q�֌W��o�^����Ȃǂ̏ꍇ�ɕʂ̐e�R���N�g�N���X�C���f�b�N�X��ݒ肵�Ȃ��悤��Lua���ŋC�����邱��
// collection�N���X�ɂ́@collect�����C���^�[�t�F�[�X��Ԃ����\�b�h IMesh* getInterface(int index); ����������Ă��邱��
// collection�N���X�ɂ́@collect�����C���X�^���X��Ԃ����\�b�h�@Mesh* getInstance(int index); ����`����Ă��邱��
// void �̏ꍇ lua���̊֐��̕Ԃ�l�͈����Ɠ��������Ԃ�
// ������Ԃ��ꍇ�� �擪�����ɖ{���̕Ԃ�l���Ԃ�
// IN_ �����Ă���΂��̂܂ܕԂ�
// OUT_ �����Ă���΍X�V����ĕԂ� lua���Ŕz��̃T�C�Y���m�ۂ��邱��
// INOUT_ �����Ă���΍X�V����ĕԂ�
// COLLECTED �����Ă����collection�N���X�̃C���f�b�N�X�Acollect�����N���X�̃C���f�b�N�X�������ƂȂ�
// YARITORI �͌��󂃁{�{���̃N���X��lua���̃N���X��p�ӂł��Ă��Ȃ��̂�
//          ����̊|���n���ƂȂ�֐��������ŏ������Ƃɂ���(lua���ł͘A�z�z����N���X�̕\���Ƃ��Ďg�����Ǝv��)
//          �N���X�̃|�C���^�������ꍇ��COLLECTED�łȂ��ꍇ��YARITORI ������K�v������
// COLLECTED �����Ă���ꍇ�@lua���ł̈����̓e�[�u���ƂȂ�Acollection_index class_index ���L�[�ɂ��Ēl�̓C���f�b�N�X������ 
//                             collection_name, class_name������
// �N���X�|�C���^�������ƂȂ�ꍇ �����̗v�f�����z��ł͂Ȃ��ЂƂ����ƍl���ď������s���B
// float �Ƃ��@int �̃|�C���^���������ƂȂ�ꍇ�@�����̗v�f�����z��Ƃ��ď������s���BTEMPHAIRETU_SIZE�@���g�p����B
// �R���N�V���������N���X�̃R���N�V�����Ȃǂ̒�`�͂ł��Ȃ�
// �C���^�[�t�F�[�X��Ԃ�l�Ƃ���������Ƃ����炾�߁@�R���N�g�����N���X�������ɂ���
// ���{�{���ł�null ���N���X�����Ɏw�肵�����ꍇ�͋�̘A�z�z���lua������w�肷�邱��
// ��@makeInstanced(Mesh* mesh, Mesh* skeleton, MeshInstanced* parent_mesh); �̂悤�Ȃ��̂�����parent_mesh��NULL�ɂ������ꍇ��
// Lua���Ł@MeshInstanceds_makeInstanced(collection_index,collected_index,  mesh, skeleton, {}); �̂悤�Ɏw�肷��
// Lua���ł͈����̍ŏ���collection_index ���w�肷�邱�Ƃ��K�v�ɂȂ遨Lua���ł̃N���X��`���l���邱�Ƃł�����B������\��
// collection�N���X��collect�����N���X�̃R���X�g���N�^������
// �� int makeInstanced(Mesh* mesh, Mesh* skeleton, MeshInstanced* parent_mesh);
// MeshInstanceds_makeInstanced() �֐��iLUA���j��int�ł͂Ȃ��@�N���X�A�z�z���Ԃ��悤�ɂ��� 
// �N���X�|�C���^�������̏ꍇOUT_ ���w�肵�Ă����f����Ȃ� YARITORI�̏ꍇ�͔��f�����H
// �Ԃ�l�̓|�C���^�������̂̓N���X�̂݁@COLLECTED�@���@YARITORI ���w�肷��K�v������
// COLLECTED ���Ԃ�l�̊֐��͓o�^�ł��Ȃ�
// YARITORI �̃R���X�g���N�^��void�������̂��̂����݂��Ă��邱�� // ��FMYVECTOR4 pos;

#define COLLECTION_INDEX_LUA_FIELD "collection_index"
#define COLLECTION_CLASS_LUA_FIELD "collection_class_name"
#define COLLECTED_INDEX_LUA_FIELD "collected_index"
#define COLLECTED_CLASS_LUA_FIELD "collected_class_name"
#define COLLECTED_INTERFACE_LUA_FIELD "collected_interface_name"

int strpos(char *haystack, char *needle)
{

   if (strlen(haystack) ==0) return -1;

   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;   // Not found = -1.
}

void mystrcpy(char* dst, int max, int pos, char* src) {
	int count = 0;
	int mypos = pos;
	while (count < max && (src[mypos] != '\0')) {
		dst[count] = src[mypos];
		mypos++;
		count++;
	}
	dst[max-1] = '\0';
}

void mystrcpy2(char *dst, int max, int mpos,  char* src) {
	int count = 0;
	int mypos = mpos;
	while (count < max && mypos > count && (src[count] != '\0')) {
		dst[count] = src[count];
		count++;
	}
	dst[max-1] = '\0';

}





#endif

