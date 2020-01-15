#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/RigidBody.h"
class Player;
//����
class LineSegment
{
public:
	LineSegment() {};
	~LineSegment() 
	{
		if (m_isInitialize) {
			Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
		}
	};
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="position">���W</param>
	void Init(const CVector3& position);
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="linesegment">����</param>
	void Execute(const CVector3& position, const CVector3& linesegment);
	//���̂��擾
	RigidBody* GetRigidBody()
	{
		return &m_rigidBody;
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

