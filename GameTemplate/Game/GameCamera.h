/*****************************************************************
 * \file   GameCamera.h
 * \brief  �Q�[���J����
 * 
 * \author komura atushi
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/SpringCamera.h"
class Player;
class GameData;
//�ǂ����̃X�e�B�b�N����]��

enum StateEnterStick {
	enStick_EnterStickR,					//�E�X�e�B�b�N����������Ă���
	enStick_EnterStickL,					//���X�e�B�b�N����������Ă���
	enStick_EnterStickBoth,					//�ǂ���̃X�e�B�b�N�����͂���Ă���
	enStick_NoEnterStick,					//�ǂ���̃X�e�B�b�N�����͂���Ă��Ȃ�
	enStick_EnterStickBothOppositeLeft,		//���E�̃X�e�B�b�N�����ꂼ��t�����ɓ��͂���Ă���A�����O����
	enStick_EnterStickBothOppositeRight		//���E�̃X�e�B�b�N�����ꂼ��t�����ɓ��͂���Ă���A�E���O����
};

/**
 * \brief	�Q�[���J�����N���X
 */
class GameCamera:public IGameObject
{
public:
	/**
	 * \brief	�R���X�g���N�^.
	 */
	GameCamera();
	/**
	 * \brief	�f�X�g���N�^.
	 */
	~GameCamera() override final;
	/**
	 * \brief Start�֐�.
	 * 
	 * \return	true��Ԃ����珉�����I���B
	 */
	bool Start() override final;
	/**
	 * \brief �X�V����.
	 */
	void Update() override final; 
	/**
	 * \brief �X�e�B�b�N�̓��͏�Ԃ��擾.
	 * 
	 * \return �X�e�B�b�N�̓��͏��
	 */
	const StateEnterStick GetStateStick() const
	{
		return m_state;
	}
	/**
	 * \brief ���W��ݒ�.
	 * 
	 * \param position	���W�B
	 */
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	/**
	 * \brief �����_.
	 * 
	 * \param target �����_
	 */
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	/**
	 * \brief �����_�ƍ��W�̋������擾.
	 * 
	 * \return �����_�ƍ��W�̋���
	 */
	const float GetRadius() const
	{
		return m_radius;
	}
	/**
	 * \brief �v���C���[��ݒ�.
	 * 
	 * \param player �v���C���[
	 */
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	/**
	 * \brief �v���C���[�̔ԍ�(1P��2P��)��ݒ�.
	 * 
	 * \param number �v���C���[�̔ԍ�
	 */
	void SetPlayerNumber(const int number)
	{
		m_playerNumber = number;
	}
private:
	/**
	 * \brief �p�b�h�̓��͂��l�����ăJ�����̎��_�ƒ����_���v�Z.
	 * 
	 */
	void Calculation();
	/**
	 * \brief �v���C���[�̑傫�����l�����ăJ�����̔��a��ς���.
	 * 
	 */
	void TransRadius();
	/**
	 * \brief L3��R3�𓯎������������Ɏ��_��ύX����.
	 * 
	 */
	void TransView();
	/**
	 * \brief �X�e�B�b�N����.
	 * 
	 */
	void Stick();
private:
	CVector3 m_position = CVector3::Zero();				//!���_
	CVector3 m_target = CVector3::One();				//!�����_
	float m_degreeY = 0.0f, m_degreeXZ = 20.0f;			//!���ꂼ��J�����̊p�x
	Player* m_player = nullptr;							//�v���C���[�̃|�C���^
	float m_radius = 0.0f;								//�J�����̔��a
	const float m_protRadius = 170.0f;					//�J�����̍ŏ��̔��a
	StateEnterStick m_state = enStick_NoEnterStick;		//�X�e�B�b�N�̓��͂ɂ���
	float m_multiply = 1.0f;							//�J�����Ɖ�̋����̔{��
	float m_timer = 0.0f;								//L3��R3�������ɉ����ꂽ���Ɏ��Ԃ��v��
	bool m_transView = false;							//L3��R3�������ɉ����ꂽ���̃J������J�ڂ��Ă��邩
	GameData* m_gameData = nullptr;						//�Q�[���f�[�^
	SpringCamera m_springCamera;						//�΂˃J����
	int m_playerNumber = 0;								//�v���C���[�̔ԍ�
	CVector3 m_stickR = CVector3::Zero();
	CVector3 m_stickL = CVector3::Zero();	
	CVector3 m_degree = CVector3::Zero();
};

