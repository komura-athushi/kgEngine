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
	//�����
	CVector3 Parallel = { 0.0f,1.0f,0.0f };
	m_position = pos;
	m_movespeed = move;
	m_movelimit = movelimit;
	CQuaternion qrot = rot;
	//�N�H�[�^�j�I���������x�N�g������]������
	qrot.Multiply(Parallel);
	Parallel *= m_movelimit / 2;
	m_movelimitUp = m_position + Parallel;
	m_movelimitDown = m_position - Parallel;
	m_limittimer = movelimit / move;
	SetMoveState();
}

CVector3 MoveUp::Move()
{
	//�����ړ��x�N�g�����v�Z���Ă��Ȃ�������v�Z����
	if (!m_isculcmovevector) {
		//����
		if (m_isaccessup) {
			m_movevector = m_movelimitUp - m_position;
			m_movevector.Normalize();
			m_movevector *= m_movespeed;
			m_isculcmovevector = true;
		}
		//�E��
		else {
			m_movevector = m_movelimitDown - m_position;
			m_movevector.Normalize();
			m_movevector *= m_movespeed;
			m_isculcmovevector = true;
		}
	}
	//���W���v�Z
	m_position += m_movevector * GameTime().GetFrameDeltaTime();
	if (m_isstart) {
		m_timer += GameTime().GetFrameDeltaTime() * 2;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//�����ɋ߂Â��Ȃ�
	if (m_isaccessup) {
		if (m_timer >= m_limittimer) {
			m_isaccessup = false;
			m_isculcmovevector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	//�E���ɋ߂Â��Ȃ�
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
