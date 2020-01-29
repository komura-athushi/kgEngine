#include "stdafx.h"
#include "LineSegment.h"
#include "Physics/CollisionAttr.h"
#include "Player.h"

//衝突したときに呼ばれる関数オブジェクト(地面用)
struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;									//衝突フラグ。
	CVector3 hitPos = CVector3(0.0f, -FLT_MAX, 0.0f);	//衝突点。
	CVector3 startPos = CVector3::Zero();				//レイの始点。
	CVector3 hitNormal = CVector3::Zero();				//衝突点の法線。
	btCollisionObject* me = nullptr;					//自分自身。自分自身との衝突を除外するためのメンバ。
	float dist = FLT_MAX;								//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
	float playerRadius = 0.0f;							//プレイヤーの半径
	int playerNumber = 0;								//1Pか2Pか

														//衝突したときに呼ばれるコールバック関数。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_User
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_LineSegment) {
			//自分に衝突した。or キャラクタ属性のコリジョンと衝突した。
			return 0.0f;
		}
		if (GetCompareSize(playerRadius, convexResult.m_hitCollisionObject->GetSize()) && convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Object) {
			convexResult.m_hitCollisionObject->SetisHit(playerNumber);
		}
		//衝突点の法線を引っ張ってくる。
		CVector3 hitNormalTmp = *(CVector3*)& convexResult.m_hitNormalLocal;
		//上方向と法線のなす角度を求める。
		float angle = hitNormalTmp.Dot(CVector3::Up());
		angle = fabsf(acosf(angle));
		if (angle < CMath::PI * 0.3f		//地面の傾斜が54度より小さいので地面とみなす。
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //もしくはコリジョン属性が地面と指定されている。
			) {
			//衝突している。
			isHit = true;
			CVector3 hitPosTmp = *(CVector3*)& convexResult.m_hitPointLocal;
			//衝突点の距離を求める。。
			CVector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			float distTmp = vDist.Length();
			if (dist > distTmp) {
				//この衝突点の方が近いので、最近傍の衝突点を更新する。
				hitPos = hitPosTmp;
				hitNormal = *(CVector3*)& convexResult.m_hitNormalLocal;
				dist = distTmp;
			}
		}
		return 0.0f;
	}
};

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	CVector3 hitPos = CVector3::Zero();		//衝突点。
	CVector3 startPos = CVector3::Zero();		//レイの始点。
	float dist = FLT_MAX;					//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
	CVector3 hitNormal = CVector3::Zero();	//衝突点の法線。
	btCollisionObject* me = nullptr;		//自分自身。自分自身との衝突を除外するためのメンバ。

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me || convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_User) {
			//自分に衝突した。or 地面に衝突した。
			return 0.0f;
		}
		//衝突点の法線を引っ張ってくる。
		CVector3 hitNormalTmp;
		hitNormalTmp.Set(convexResult.m_hitNormalLocal);
		//上方向と衝突点の法線のなす角度を求める。
		float angle = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up())));
		if (angle >= CMath::PI * 0.3f		//地面の傾斜が54度以上なので壁とみなす。
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character	//もしくはコリジョン属性がキャラクタなので壁とみなす。
			) {
			isHit = true;
			CVector3 hitPosTmp;
			hitPosTmp.Set(convexResult.m_hitPointLocal);
			//交点との距離を調べる。
			CVector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			vDist.y = 0.0f;
			float distTmp = vDist.Length();
			if (distTmp < dist) {
				//この衝突点の方が近いので、最近傍の衝突点を更新する。
				hitPos = hitPosTmp;
				dist = distTmp;
				hitNormal = hitNormalTmp;
			}
		}
		return 0.0f;
	}
};

void LineSegment::Init(const CVector3& position)
{
	m_collider.Create(m_radius);

	//剛体を初期化
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_collider;
	rbInfo.mass = 0.0f;
	m_rigidBody.Create(rbInfo);
	btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
	//剛体の位置を更新
	trans.setOrigin(btVector3(position.x, position.z, position.z));
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_User);
	m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	Engine().GetPhysicsEngine().AddRigidBody(m_rigidBody);
	m_isInitialize = true;
}

void LineSegment::Execute(const CVector3& position, const CVector3& linesegment)
{
	//次の移動先となる座標を計算する
	CVector3 nextPosition = position;
	//速度からこのフレームでの移動量を求める、オイラー積分
	CVector3 addPos = linesegment;
	nextPosition += addPos;
	CVector3 nextPosition2 = nextPosition;

	btRigidBody* btBody = m_rigidBody.GetBody();
	//剛体を動かす。
	btBody->setActivationState(DISABLE_DEACTIVATION);
	btTransform& trans = btBody->getWorldTransform();
	//剛体の位置を更新。
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	//下方向を調べる。
	{
		
	
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//始点はカプセルコライダーの中心。
		start.setOrigin(btVector3(position.x, position.y, position.z));
		end.setOrigin(btVector3(nextPosition.x, nextPosition.y, nextPosition.z));
		SweepResultGround callback;
		callback.me = m_rigidBody.GetBody();
		callback.startPos.Set(start.getOrigin());
		callback.playerRadius = m_player->GetRadius();
		callback.playerNumber = m_player->GetPlayerNumber();
		//衝突検出。
		if (fabsf(nextPosition.y - callback.startPos.y) > FLT_EPSILON) {
			Engine().GetPhysicsEngine().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
			if (callback.isHit) {
				m_isJump = false;
				m_isOnGround = true;
				nextPosition = callback.hitPos;
			}
		}
	}
	//@todo 未対応。 trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
	CVector3 vector = nextPosition2 - nextPosition;
	CVector3 pos = m_player->GetPosition();
	if (fabsf(vector.y) >= 0.1f) {
		m_player->SetMoveSpeedYZero();
	}
		
	pos.y -= vector.y;
	m_player->SetPosition(pos - CVector3::AxisY() * m_player->GetRadius());
}