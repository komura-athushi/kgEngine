/*****************************************************************//**
 * \file   LineSegment.h
 * \brief  LineSegmentクラス
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
 * \brief 塊のガタガタ処理をするクラス.
 */
class LineSegment
{
public:
	/**
	 * \brief コンストラクタ.
	 */
	LineSegment() {};
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~LineSegment() 
	{
		if (m_isInitialize) {
			Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
		}
	};
	/**
	 * \brief 初期化.
	 * 
	 * \param position 座標
	 */
	void Init(const CVector3& position);
	/**
	 * \brief 更新.
	 * 
	 * \param position 座標
	 * \param linesegment 線分
	 */
	void Execute(const CVector3& position, const CVector3& linesegment);
	/**
	 * \brief 剛体を取得.
	 * 
	 * \return 剛体
	 */
	RigidBody* GetRigidBody() 
	{
		return &m_rigidBody;
	}
	/**
	 * \brief プレイヤーを取得.
	 * 
	 * \param player
	 */
	void SetPlayer(Player* player)
	{
		m_player = player;
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

