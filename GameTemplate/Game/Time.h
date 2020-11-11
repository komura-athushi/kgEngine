/*****************************************************************//**
 * \file   Time.h
 * \brief  Time�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
class Game;
class GameData;
#include "graphics/2D/kgFont.h"
//�X�e�[�W�̐������Ԃ��Ǘ�����
/**
 * \brief �X�e�[�W���̐������Ԃ��Ǘ�����.
 */
class Time:public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Time();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Time() override final;
	/**
	 * \brief ����������.
	 * 
	 * \return true�ŏ���������
	 */
	bool Start() override final;
	/**
	 * \brief �`�揈��.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief �������Ԃ�ݒ肷��.
	 * 
	 * \param time ��������
	 */
	void SetTime(const float time) 
	{
		m_time = time;
		m_timer = time;
	}
private:
	CSprite m_sprite1,m_sprite2;								//�X�v���C�g
	CFont m_font;												//�t�H���g
	Game* m_game = nullptr;										//�Q�[��
	GameData* m_gamedata = nullptr;								//�Q�[���f�[�^
	float m_time = 0.0f;										//��������
	float m_timer = 0.0f;										//�^�C�}�[
	float m_degree = 0.0f;										//�~�`�Q�[�W�̊p�x
	int m_beforeSeconds = 60;									//�O�t���[���̕b��
};

