#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/RigidBody.h"
class Player;
//線分
class LineSegment
{
public:
	LineSegment() {};
	~LineSegment() 
	{
		Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
	};
	void Init(const CVector3& position);
	//実行
	void Execute(const CVector3& position, const CVector3& linesegment);
private:
	CVector3 			m_position = CVector3::Zero();	//座標。
	bool 				m_isJump = false;				//ジャンプ中？
	bool				m_isOnGround = true;			//地面の上にいる？
	SphereCollider		m_collider;						//コライダー。
	float				m_radius = 0.01f;				//半径
	bool				m_isSphere = false;				//球体かどうか
	RigidBody			m_rigidBody;					//剛体。		
	Player* m_player = nullptr;
};

