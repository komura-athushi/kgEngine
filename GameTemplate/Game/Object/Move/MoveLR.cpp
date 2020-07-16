#include "stdafx.h"
#include "MoveLR.h"




MoveLR::MoveLR()
{

}

MoveLR::~MoveLR()
{

}

void MoveLR::Init(const CVector3& pos, const float& move, const float& movelimit,const CQuaternion& rot)
{
	//オブジェクトに平行なベクトル
	CVector3 Parallel = { 1.0f,0.0f,0.0f };
	m_position = pos;
	m_movespeed = move;
	m_movelimit = movelimit;
	CQuaternion qrot = rot;
	//クォータニオン分だけベクトルを回転させて
	qrot.Multiply(Parallel);
	Parallel *= m_movelimit / 2;
	m_movelimitLeft = m_position - Parallel;
	m_movelimitRight = m_position + Parallel;
	m_limittimer = movelimit / move;
	SetMoveState();
}

CVector3 MoveLR::Move()
{
	//もし移動ベクトルを計算していなかったら計算する
	if (!m_isculcmovevector) {
		//左側
		if (m_isaccessleft) {
			m_movevector = m_movelimitLeft - m_position;
			m_movevector.Normalize();
			m_isculcmovevector = true;
		}
		//右側
		else {
			m_movevector = m_movelimitRight - m_position;
			m_movevector.Normalize();
			m_isculcmovevector = true;
		}
	}
	//座標を計算
	m_position += m_movevector * GameTime().GetFrameDeltaTime() * m_movespeed * 2.0f;
    if (m_isstart) {
		m_timer += GameTime().GetFrameDeltaTime() * 2;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//左側に近づくなら
	if (m_isaccessleft) {
		if (m_timer >= m_limittimer) {
			m_isaccessleft = false;
			m_isculcmovevector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	//右側に近づくなら
	else {
		if (m_timer >= m_limittimer) {
			m_isaccessleft = true;
			m_isculcmovevector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	return m_position;
}
