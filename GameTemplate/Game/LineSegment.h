/*****************************************************************//**
 * \file   LineSegment.h
 * \brief  LineSegment�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/RigidBody.h"
class Player;
/**
 * \brief ��̃K�^�K�^����������N���X.
 */
class LineSegment
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 */
	LineSegment() {};
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~LineSegment() 
	{
		if (m_isInitialize) {
			Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
		}
	};
	/**
	 * \brief ������.
	 * 
	 * \param position ���W
	 */
	void Init(const CVector3& position);
	/**
	 * \brief �X�V.
	 * 
	 * \param position ���W
	 * \param linesegment ����
	 */
	void Execute(const CVector3& position, const CVector3& linesegment);
	/**
	 * \brief ���̂��擾.
	 * 
	 * \return ����
	 */
	RigidBody* GetRigidBody() 
	{
		return &m_rigidBody;
	}
	/**
	 * \brief �v���C���[���擾.
	 * 
	 * \param player
	 */
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
private:

	bool 				m_isJump = false;					//�W�����v���H
	bool				m_isOnGround = true;				//�n�ʂ̏�ɂ���H
	SphereCollider		m_collider;							//�R���C�_�[�B
	float				m_radius = 0.01f;					//���a
	bool				m_isSphere = false;					//���̂��ǂ���
	RigidBody			m_rigidBody;						//���́B		
	Player* m_player = nullptr;								//�v���C���[
	bool m_isInitialize = false;							//�������������ǂ���
};

