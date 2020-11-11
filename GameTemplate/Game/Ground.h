/*****************************************************************//**
 * \file   Ground.h
 * \brief  Ground�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"

/**
 * \brief �X�e�[�W�̒n��.
 */
class Ground:public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Ground();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Ground() override final;
	/**
	 * \brief ����������.
	 * 
	 * \return ����������������true
	 */
	bool Start() override final;
	/**
	 * \brief �X�V����.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief ���W��ݒ�.
	 * 
	 * \param pos ���W
	 */
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	/**
	 * \brief ��]��ݒ�.
	 * 
	 * \param rot ��]
	 */
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	CSkinModelRender m_skinModelRender;								//�X�L�����f�������_�[
	PhysicsStaticObject m_staticObject;								//�ÓI�I�u�W�F�N�g
	CVector3 m_position = CVector3::Zero();							//���W
	CQuaternion m_rotation = CQuaternion::Identity();				//��]
};

