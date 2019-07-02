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
	//�I�u�W�F�N�g�ɕ��s�ȃx�N�g��
	CVector3 Parallel = { 0.0f,0.0f,1.0f };
	m_position = pos;
	m_movespeed = move;
	m_movelimit = movelimit;
	CQuaternion qrot = rot;
	//�N�H�[�^�j�I���������x�N�g������]������
	qrot.Multiply(Parallel);
	Parallel *= m_movelimit / 2;
	m_movelimitFront = m_position - Parallel;
	m_movelimitBack = m_position + Parallel;
	m_limittimer = movelimit / move;
	SetMoveState();
}

CVector3 MoveFB::Move()
{
	//�����ړ��x�N�g�����v�Z���Ă��Ȃ�������v�Z����
	if (!m_isculcmovevector) {
		//�O��
		if (m_isaccessfront) {
			m_movevector = m_movelimitFront - m_position;
			m_movevector.Normalize();
			m_isculcmovevector = true;
		}
		//��둤
		else {
			m_movevector = m_movelimitBack - m_position;
			m_movevector.Normalize();
			m_isculcmovevector = true;
		}
	}
	//���W���v�Z
	m_position += m_movevector * GameTime().GetFrameDeltaTime() * m_movespeed;
	if (m_isstart) {
		m_timer += GameTime().GetFrameDeltaTime() * 2;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//�O���ɋ߂Â��Ȃ�
	if (m_isaccessfront) {
		if (m_timer >= m_limittimer) {
			m_isaccessfront = false;
			m_isculcmovevector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	//��둤�ɋ߂Â��Ȃ�
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