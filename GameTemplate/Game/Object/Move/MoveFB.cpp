
#include "stdafx.h"
#include "MoveFB.h"

namespace {
	float speed = 2.0f;
}

MoveFB::MoveFB()
{

}

MoveFB::~MoveFB()
{

}

void MoveFB::Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot)
{
	//オブジェクトに平行なベクトル
	CVector3 Parallel = { 0.0f,0.0f,1.0f };
	m_position = pos;
	m_moveSpeed = move;
	m_moveLimit = movelimit;
	CQuaternion qrot = rot;
	//クォータニオン分だけベクトルを回転させて
	qrot.Multiply(Parallel);
	Parallel *= m_moveLimit / 2;
	m_moveLimitFront = m_position - Parallel;
	m_moveLimitBack = m_position + Parallel;
	m_limitTimer = movelimit / move;
	SetMoveState();
}

const CVector3& MoveFB::Move() 
{
	//もし移動ベクトルを計算していなかったら計算する
	if (!m_isCulcMoveVector) {
		//前側
		if (m_isAccessFront) {
			m_moveVector = m_moveLimitFront - m_position;
			m_moveVector.Normalize();
			m_isCulcMoveVector = true;
		}
		//後ろ側
		else {
			m_moveVector = m_moveLimitBack - m_position;
			m_moveVector.Normalize();
			m_isCulcMoveVector = true;
		}
	}
	//座標を計算
	m_position += m_moveVector * GameTime().GetFrameDeltaTime() * m_moveSpeed * speed;
	if (m_isStart) {
		m_timer += GameTime().GetFrameDeltaTime() * speed;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//前側に近づくなら
	if (m_isAccessFront) {
		if (m_timer >= m_limitTimer) {
			m_isAccessFront = false;
			m_isCulcMoveVector = false;
			m_timer = 0.0f;
			m_isStart = false;
		}
	}
	//後ろ側に近づくなら
	else {
		if (m_timer >= m_limitTimer) {
			m_isAccessFront = true;
			m_isCulcMoveVector = false;
			m_timer = 0.0f;
			m_isStart = false;
		}
	}
	return m_position;
}