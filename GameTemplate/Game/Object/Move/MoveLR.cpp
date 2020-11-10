#include "stdafx.h"
#include "MoveLR.h"


namespace {
	const float speed = 2.0f;
}

MoveLR::MoveLR()
{

}

MoveLR::~MoveLR()
{

}

void MoveLR::Init(const CVector3& pos, const float move, const float movelimit,const CQuaternion& rot)
{
	//オブジェクトに平行なベクトル
	CVector3 parallel = { 1.0f,0.0f,0.0f };
	m_position = pos;
	m_moveSpeed = move;
	m_moveLimit = movelimit;
	CQuaternion qrot = rot;
	//クォータニオン分だけベクトルを回転させて
	qrot.Multiply(parallel);
	parallel *= m_moveLimit / 2;
	m_moveLimitLeft = m_position - parallel;
	m_moveLimitRight = m_position + parallel;
	m_limitTimer = movelimit / move;
	SetMoveState();
}

const CVector3& MoveLR::Move()
{
	//もし移動ベクトルを計算していなかったら計算する
	if (!m_isCulcMoveVector) {
		//左側
		if (m_isAccessLeft) {
			m_moveVector = m_moveLimitLeft - m_position;
			m_moveVector.Normalize();
			m_isCulcMoveVector = true;
		}
		//右側
		else {
			m_moveVector = m_moveLimitRight - m_position;
			m_moveVector.Normalize();
			m_isCulcMoveVector = true;
		}
	}
	//座標を計算
	m_position += m_moveVector * GameTime().GetFrameDeltaTime() * m_moveSpeed * speed;
    if (m_isstart) {
		m_timer += GameTime().GetFrameDeltaTime() * speed;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//左側に近づくなら
	if (m_isAccessLeft) {
		if (m_timer >= m_limitTimer) {
			m_isAccessLeft = false;
			m_isCulcMoveVector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	//右側に近づくなら
	else {
		if (m_timer >= m_limitTimer) {
			m_isAccessLeft = true;
			m_isCulcMoveVector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	return m_position;
}
