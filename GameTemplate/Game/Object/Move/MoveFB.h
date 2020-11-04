/*****************************************************************//**
 * \file   MoveFB.h
 * \brief MoveFB�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IMove.h"

/**
 * \brief �I�u�W�F�N�g��O��ɓ������N���X.
 */
class MoveFB :public IMove
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	MoveFB();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~MoveFB() override final;
	/**
	 * \brief ����������.
	 * 
	 * \param pos ���W
	 * \param move �ړ����x
	 * \param movelimit �ړ��͈�
	 * \param rot ��]
	 */
	void Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot = CQuaternion::Identity()) override final;
	/**
	 * \brief �ړ��X�e�[�g��ݒ�.
	 * 
	 */
	void SetMoveState() override final
	{
		IMove::SetMoveStateBasic(enMove_Lr);
	} 
	/**
	 * \brief �ړ��x�N�g�����擾.
	 * 
	 * \return �ړ��x�N�g��
	 */
	const CVector3& GetMoveVector() const override final
	{
		return m_moveVector;
	}
	/**
	 * \brief �ړ�����.
	 * 
	 * \return �ړ���̍��W
	 */
	const CVector3& Move() override final;
private:
	float m_moveLimit = 0.0f;								//�ړ��͈�
	CVector3 m_moveLimitFront = CVector3::Zero();			//�㑤�̐���������W
	CVector3 m_moveLimitBack = CVector3::Zero();			//�����̐���������W
	bool m_isAccessFront = true;							//�㑤�̍��W�ɋ߂Â����ǂ���
	float m_timer = 0.0f;									//�^�C�}�[
	float m_limitTimer = 0.0f;								//�^�C�}�[�̐���
	bool m_isStart = true;									//�ŏ��̋������ǂ���
};

