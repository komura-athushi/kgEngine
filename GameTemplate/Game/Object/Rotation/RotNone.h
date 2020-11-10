/*****************************************************************//**
 * \file   RotNone.h
 * \brief  RotNone�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IRot.h"

/**
 * \brief ������]���Ȃ��N���X.
 */
class RotNone :public IRotate
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	RotNone() {}
	/**
	 * \brief .�f�X�g���N�^
	 * 
	 */
	~RotNone() override final
	{
	}
	/**
	 * \brief ��]���v�Z.
	 * 
	 * \param move �ړ��x�N�g��
	 * \return �v�Z��̉�]
	 */
	const CQuaternion Rot(const CVector3& move) override final
	{
		return m_rotation;
	}
	/**
	 * \brief ��]�X�e�[�g��ݒ�.
	 * 
	 */
	void SetRotState() override final
	{
		IRotate::SetRotStateBasic(enRot_No);
	}
	/**
	 * \brief ����������.
	 * 
	 * \param rot ������]
	 * \param speed ��]���x
	 */
	void Init(const CQuaternion& rot, const float speed = 0.0f) override final
	{
		m_rotation = rot;
	}
private:
};



