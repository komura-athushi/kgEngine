#include "stdafx.h"
#include "MoveUp.h"




MoveUp::MoveUp()
{

}

MoveUp::~MoveUp()
{

}

void MoveUp::Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot)
{
	//上方向
	CVector3 Parallel = { 0.0f,1.0f,0.0f };
	m_position = pos;
	m_movespeed = move;
	m_movelimit = movelimit;
	CQuaternion qrot = rot;
	//クォータニオン分だけベクトルを回転させて
	qrot.Multiply(Parallel);
	Parallel *= m_movelimit / 2;
	m_movelimitUp = m_position + Parallel;
	m_movelimitDown = m_position - Parallel;
	m_limittimer = movelimit / move;
	SetMoveState();
}

CVector3 MoveUp::Move()
{
	//もし移動ベクトルを計算していなかったら計算する
	if (!m_isculcmovevector) {
		//左側
		if (m_isaccessup) {
			m_movevector = m_movelimitUp - m_position;
			m_movevector.Normalize();
			m_movevector *= m_movespeed;
			m_isculcmovevector = true;
		}
		//右側
		else {
			m_movevector = m_movelimitDown - m_position;
			m_movevector.Normalize();
			m_movevector *= m_movespeed;
			m_isculcmovevector = true;
		}
	}
	//座標を計算
	m_position += m_movevector * GameTime().GetFrameDeltaTime();
	if (m_isstart) {
		m_timer += GameTime().GetFrameDeltaTime() * 2;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//左側に近づくなら
	if (m_isaccessup) {
		if (m_timer >= m_limittimer) {
			m_isaccessup = false;
			m_isculcmovevector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	//右側に近づくなら
	else {
		if (m_timer >= m_limittimer) {
			m_isaccessup = true;
			m_isculcmovevector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	return m_position;
}
