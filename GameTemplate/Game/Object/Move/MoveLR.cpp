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
	//�I�u�W�F�N�g�ɕ��s�ȃx�N�g��
	CVector3 Parallel = { 1.0f,0.0f,0.0f };
	m_position = pos;
	m_movespeed = move;
	m_movelimit = movelimit;
	CQuaternion qrot = rot;
	//�N�H�[�^�j�I���������x�N�g������]������
	qrot.Multiply(Parallel);
	Parallel *= m_movelimit / 2;
	m_movelimitLeft = m_position - Parallel;
	m_movelimitRight = m_position + Parallel;
	m_limittimer = movelimit / move;
	SetMoveState();
}

CVector3 MoveLR::Move()
{
	//�����ړ��x�N�g�����v�Z���Ă��Ȃ�������v�Z����
	if (!m_isculcmovevector) {
		//����
		if (m_isaccessleft) {
			m_movevector = m_movelimitLeft - m_position;
			m_movevector.Normalize();
			m_isculcmovevector = true;
		}
		//�E��
		else {
			m_movevector = m_movelimitRight - m_position;
			m_movevector.Normalize();
			m_isculcmovevector = true;
		}
	}
	//���W���v�Z
	m_position += m_movevector * GameTime().GetFrameDeltaTime() * m_movespeed * 2.0f;
    if (m_isstart) {
		m_timer += GameTime().GetFrameDeltaTime() * 2;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//�����ɋ߂Â��Ȃ�
	if (m_isaccessleft) {
		if (m_timer >= m_limittimer) {
			m_isaccessleft = false;
			m_isculcmovevector = false;
			m_timer = 0.0f;
			m_isstart = false;
		}
	}
	//�E���ɋ߂Â��Ȃ�
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
