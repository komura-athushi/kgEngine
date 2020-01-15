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
		if (m_isInitialize) {
			Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
		}
	};
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	void Init(const CVector3& position);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="linesegment">線分</param>
	void Execute(const CVector3& position, const CVector3& linesegment);
	//剛体を取得
	RigidBody* GetRigidBody()
	{
		return &m_rigidBody;
	}
private:

	bool 				m_isJump = false;					//ジャンプ中？
	bool				m_isOnGround = true;				//地面の上にいる？
	SphereCollider		m_collider;							//コライダー。
	float				m_radius = 0.01f;					//半径
	bool				m_isSphere = false;					//球体かどうか
	RigidBody			m_rigidBody;						//剛体。		
	Player* m_player = nullptr;								//プレイヤー
	bool m_isInitialize = false;							//初期化したかどうか
};

