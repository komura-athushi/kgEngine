#include "stdafx.h"
#include "MoveUp.h"

namespace {
	const float speed = 2.0f;
}


MoveUp::MoveUp()
{

}

MoveUp::~MoveUp()
{

}

void MoveUp::Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot)
{
	//上方向
	CVector3 parallel = { 0.0f,1.0f,0.0f };
	m_position = pos;
	m_moveSpeed = move;
	m_moveLimit = movelimit;
	CQuaternion qrot = rot;
	//クォータニオン分だけベクトルを回転させて
	qrot.Multiply(parallel);
	parallel *= m_moveLimit / speed;
	m_moveLimitUp = m_position + parallel;
	m_moveLimitDown = m_position - parallel;
	m_limitTimer = movelimit / move;
	SetMoveState();
}

const CVector3& MoveUp::Move()
{
	//もし移動ベクトルを計算していなかったら計算する
	if (!m_isCulcMoveVector) {
		//上側
		if (m_isAccessUp) {
			m_moveVector = m_moveLimitUp - m_position;
			m_moveVector.Normalize();
			m_isCulcMoveVector = true;
		}
		//下側
		else {
			m_moveVector = m_moveLimitDown - m_position;
			m_moveVector.Normalize();
			m_isCulcMoveVector = true;
		}
	}
	//座標を計算
	m_position += m_moveVector * GameTime().GetFrameDeltaTime() * m_moveSpeed;
	if (m_isStart) {
		m_timer += GameTime().GetFrameDeltaTime() * speed;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//左側に近づくなら
	if (m_isAccessUp) {
		if (m_timer >= m_limitTimer) {
			m_isAccessUp = false;
			m_isCulcMoveVector = false;
			m_timer = 0.0f;
			m_isStart = false;
		}
	}
	//右側に近づくなら
	else {
		if (m_timer >= m_limitTimer) {
			m_isAccessUp = true;
			m_isCulcMoveVector = false;
			m_timer = 0.0f;
			m_isStart = false;
		}
	}
	return m_position;
}
