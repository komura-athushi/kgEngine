/*****************************************************************//**
 * \file   MoveLR.h
 * \brief  MoveLR�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IMove.h"
/**
 * \brief �I�u�W�F�N�g�����E�Ɉړ�������.
 */
class MoveLR:public IMove
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	MoveLR();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~MoveLR() override final;
	/**
	 * \brief ����������.
	 *
	 * \param pos ���W
	 * \param move �ړ����x
	 * \param movelimit �ړ��͈�
	 * \param rot ��]
	 */
	void Init(const CVector3& pos, const float move, const float movelimit,const CQuaternion& rot = CQuaternion::Identity()) override final;
	/**
	 * \brief �ړ��X�e�[�g��ݒ�.
	 * 
	 */
	void SetMoveState() override final
	{
		IMove::SetMoveStateBasic(enMove_Lr);
	}
	//�ړ��x�N�g�����擾
	const CVector3& GetMoveVector() const override final
	{
		return m_moveVector;
	}
	//�ړ���̍��W��Ԃ�
	const CVector3& Move() override final;
private:
	float m_moveLimit = 0.0f;								//�ړ��͈�
	CVector3 m_moveLimitLeft = CVector3::Zero();			//�����̐���������W
	CVector3 m_moveLimitRight = CVector3::Zero();			//�E���̐���������W
	bool m_isAccessLeft = true;								//�����̍��W�ɋ߂Â����ǂ���
	float m_timer = 0.0f;									//�^�C�}�[
	float m_limitTimer = 0.0f;								//�^�C�}�[�̐���
	bool m_isstart = true;									//�ŏ��̋������ǂ���
};

