#include "stdafx.h"
#include "MoveFB.h"

MoveFB::MoveFB()
{

}

MoveFB::~MoveFB()
{

}

void MoveFB::Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot)
{
	//オブジェクトに平行なベクトル
	CVector3 Parallel = { 0.0f,0.0f,1.0f };
	m_position = pos;
	m_movespeed = move;
	m_movelimit = movelimit;
	CQuaternion qrot = rot;
	//クォータニオン分だけベクトルを回転させて
	qrot.Multiply(Parallel);
	Parallel *= m_movelimit / 2;
	m_movelimitFront = m_position - Parallel;
	m_movelimitBack = m_position + Parallel;
	m_limittimer = movelimit / move;
	SetMoveState();
}

CVector3 MoveFB::Move()
{
	//もし移動ベクトルを計算していなかったら計算する
	if (!m_isculcmovevector) {
		//前側
		if (m_isaccessfront) {
			m_movevector = m_movelimitFront - m_position;
			m_movevector.Normalize();
			m_isculcmovevector = true;
		}
		//後ろ側
		else {
			m_movevector = m_movelimitBack - m_position;
			m_movevector.Normalize();
			m_isculcmovevector = true;
		}
	}
	//座標を計算
	m_position += m_movevector * GameTime().GetFrameDeltaTime() * m_movespeed;
	if (m_isstart) {
		m_timer += GameTime().GetFrameDeltaTime() * 2;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//前側に近づくなら
	if (m_isaccessfront) {
		if (m_timer >= m_limittimer) {
			m_isaccessfront = false;
			m_isculcmovevector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	//後ろ側に近づくなら
	else {
		if (m_timer >= m_limittimer) {
			m_isaccessfront = true;
			m_isculcmovevector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	return m_position;
}