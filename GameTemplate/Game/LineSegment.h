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
		Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
	};
	void Init(const CVector3& position);
	//���s
	void Execute(const CVector3& position, const CVector3& linesegment);
private:
	CVector3 			m_position = CVector3::Zero();	//���W�B
	bool 				m_isJump = false;				//�W�����v���H
	bool				m_isOnGround = true;			//�n�ʂ̏�ɂ���H
	SphereCollider		m_collider;						//�R���C�_�[�B
	float				m_radius = 0.01f;				//���a
	bool				m_isSphere = false;				//���̂��ǂ���
	RigidBody			m_rigidBody;					//���́B		
	Player* m_player = nullptr;
};

