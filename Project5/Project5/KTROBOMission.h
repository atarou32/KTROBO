#ifndef KTROBOMISSION_H
#define KTROBOMISSION_H

/*
	�~�b�V�����̊T�v

	�~�b�V������I�����A�~�b�V�������J�n�����
	�P�D�@�̂�I��
	�Q�D�~�b�V�����u���[�t�B���O
	�R�D�~�b�V�����J�n
	�S�D�~�b�V�����I���i�B���A���s�j
	�T�D�~�b�V������V�̎擾

	�~�b�V�����̎��
	�P�D�ʏ�~�b�V����
	�Q�D�X�g�[���[�~�b�V����
	�R�D�ً}�~�b�V����
	
	�~�b�V�����̒B������
	�P�D����̓G�̌��j
	�Q�D����|�C���g�̎擾
	�R�D���莞�Ԃ̐���

	�~�b�V�����̑���
	
	�~�b�V����ID
	�~�b�V������ރ^�C�v
	�~�b�V������
	�~�b�V�����R�[�h�l�[��
	�~�b�V��������
	�~�b�V�����B������
	�~�b�V�����n�`ID

	


*/

namespace KTROBO {
#pragma once
class Mission
{
public:
	Mission(void);
	~Mission(void);

	void start();
	void end();
	void success();
	void failure();
	void gift();
	int get_now_state();

};

class MissionBreefing {
public:
	void waiting();
	void ready(); 
	void startMission();
	void breakMission();
	void roboChange();
	int get_now_state();
};

}
#endif