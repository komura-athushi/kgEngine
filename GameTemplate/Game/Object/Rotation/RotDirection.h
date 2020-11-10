/*****************************************************************//**
 * \file   RotDirection.h
 * \brief  RotDirection�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IRot.h"
/**
 * \brief �I�u�W�F�N�g���ړ������ɉ�]������N���X.
 */
class RotDirection:public IRotate
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	RotDirection();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~RotDirection() override final;
	/**
	 * \brief �I�u�W�F�N�g�̉�]���v�Z.
	 * 
	 * \param move �ړ��x�N�g��
	 * \return �v�Z��̉�]
	 */
	const CQuaternion Rot(const CVector3& move) override final;
	/**
	 * \brief �ړ��X�e�[�g��ݒ�.
	 * 
	 */
	void SetRotState() override final
	{
		IRotate::SetRotStateBasic(enRot_DirectionRot);
	}
	/**
	 * \brief ����������.
	 * 
	 * \param rot ������]
	 * \param speed ��]���x
	 */
	void Init(const CQuaternion& rot, const float speed = 0.0f) override final;
private:
};

