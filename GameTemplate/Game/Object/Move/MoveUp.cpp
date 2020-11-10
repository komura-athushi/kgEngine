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
	//�����
	CVector3 parallel = { 0.0f,1.0f,0.0f };
	m_position = pos;
	m_moveSpeed = move;
	m_moveLimit = movelimit;
	CQuaternion qrot = rot;
	//�N�H�[�^�j�I���������x�N�g������]������
	qrot.Multiply(parallel);
	parallel *= m_moveLimit / speed;
	m_moveLimitUp = m_position + parallel;
	m_moveLimitDown = m_position - parallel;
	m_limitTimer = movelimit / move;
	SetMoveState();
}

const CVector3& MoveUp::Move()
{
	//�����ړ��x�N�g�����v�Z���Ă��Ȃ�������v�Z����
	if (!m_isCulcMoveVector) {
		//�㑤
		if (m_isAccessUp) {
			m_moveVector = m_moveLimitUp - m_position;
			m_moveVector.Normalize();
			m_isCulcMoveVector = true;
		}
		//����
		else {
			m_moveVector = m_moveLimitDown - m_position;
			m_moveVector.Normalize();
			m_isCulcMoveVector = true;
		}
	}
	//���W���v�Z
	m_position += m_moveVector * GameTime().GetFrameDeltaTime() * m_moveSpeed;
	if (m_isStart) {
		m_timer += GameTime().GetFrameDeltaTime() * speed;
	}
	else {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	//�����ɋ߂Â��Ȃ�
	if (m_isAccessUp) {
		if (m_timer >= m_limitTimer) {
			m_isAccessUp = false;
			m_isCulcMoveVector = false;
			m_timer = 0.0f;
			m_isStart = false;
		}
	}
	//�E���ɋ߂Â��Ȃ�
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
