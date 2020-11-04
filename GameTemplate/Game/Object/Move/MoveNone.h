/*****************************************************************//**
 * \file   MoveNone.h
 * \brief MoveNone�N���X  
 * 
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IMove.h"
/**
 * \brief �ړ��������������Ȃ��N���X.
 */
class MoveNone :public IMove
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	MoveNone();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~MoveNone() override final;
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
		IMove::SetMoveStateBasic(enMove_No);
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
	const CVector3& Move() override final
	{
		return m_position;
	} 
private:
};
