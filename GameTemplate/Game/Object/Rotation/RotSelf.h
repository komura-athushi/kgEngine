/*****************************************************************//**
 * \file   RotSelf.h
 * \brief  RotSelf�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IRot.h"
/**
 * \brief�I�u�W�F�N�g�����]������N���X.
 */
class RotSelf :public IRotate
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 *
	 */
	RotSelf();
	/**
	 * \brief �f�X�g���N�^.
	 *
	 */
	~RotSelf() override final;
	/**
	 * ��]���v�Z.
	 * 
	 * \param move �ړ��x�N�g��
	 * \return �v�Z��̉�]
	 */
	const CQuaternion Rot(const CVector3& move) override final;
	/**
	 * \brief ��]�X�e�[�g��ݒ�.
	 * 
	 */
	void SetRotState() override final
	{
		IRotate::SetRotStateBasic(enRot_Rot);
	}
	/**
	 * \brief ����������.
	 * 
	 * \param rot ������]
	 * \param speed ��]���x
	 */
	void Init(const CQuaternion& rot, const float speed = 0.0f) override final;
private:
	float m_speed = 0.0f;												//��]�X�s�[�h	
	float m_degree = 0.0f;												//�p�x

};

