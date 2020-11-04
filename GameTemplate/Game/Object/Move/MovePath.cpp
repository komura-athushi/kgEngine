#include "stdafx.h"
#include "MovePath.h"

namespace {
	const float molecule = 5.0f;
	const float addDegree = 180.0f / 2 / 180.0f * CMath::PI;
	float degreeSpeed = 2.0f;
}

MovePath::MovePath()
{

}

MovePath::~MovePath()
{

}

void MovePath::Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot)
{
	m_position = pos;
	m_moveSpeed = move;
	//���݂̍��W�����ԋ߂��|�C���g���擾����
	m_point = m_path.GetNearPoint(m_position);
	SetMoveState();
}

void MovePath::ReadPath(const wchar_t* filePath)
{
	m_path.Load(filePath);
}

const CVector3& MovePath::Move()
{
	if (m_isStart) {
		m_moveVector = m_point->s_vector - m_position;
		m_moveVector.Normalize();
		m_isStart = false;
	}
	else {
		CVector3 distance = m_point->s_vector - m_position;
		//��~������Ȃ�������
		if (!m_isStop) {
			if (distance.LengthSq() <= m_moveSpeed * m_moveSpeed * GameTime().GetFrameDeltaTime()) {
				//���݂̐��ʂ̃x�N�g���ƁA���̃p�X�Ɍ����Ă̈ړ��x�N�g�������߂�
				m_point = m_path.GetPoint(m_point->s_number);
				CVector3 distance = m_point->s_vector - m_position;
				distance.y = 0;
				distance.Normalize();
				CVector3 moveVector = m_moveVector;
				moveVector.y = 0.0f;
				moveVector.Normalize();
				//�e�x�N�g���̊p�x�����߂�(0�`PI)
				float degree1 = acosf(moveVector.x);
				float degree2 = acosf(distance.x);
				//2PI�܂őΉ�������
				if (moveVector.z <= 0.0f) {
					degree1 = degree1 + (CMath::PI - degree1) * degreeSpeed;
				}
				if (distance.z <= 0.0f) {
					degree2 = degree2 + (CMath::PI - degree2) * degreeSpeed;
				}
				//�p�x�̍������߂�
				float degree = 0.0f;
				if (degree1 >= degree2) {
					if (degree1 - degree2 >= CMath::PI) {
						degree = CMath::PI * 2 - degree1 + degree2;
						m_isAddDegree = false;
					}
					else {
						degree = degree1 - degree2;
						m_isAddDegree = true;
					}
				}
				else {
					if (degree2 - degree1 >= CMath::PI) {
						degree = CMath::PI * 2 - degree2 + degree1;
						m_isAddDegree = true;
					}
					else {
						degree = degree2 - degree1;
						m_isAddDegree = false;
					}
				}
				m_time = degree / addDegree;
				//�ꎞ��~
				m_isStop = true;
			}
		}
		//��~����������
		else {
			//�^�C�}�[�����ȏ�ɂȂ�����ړ�����
			if (m_timer >= m_time) {
				m_moveVector = m_point->s_vector - m_position;
				m_moveVector.Normalize();
				m_isStop = false;
				m_timer = 0.0f;
			}
			//�x�N�g������]������
			else {
				CQuaternion rot;
				if (m_isAddDegree) {
					rot.SetRotation(CVector3::AxisY(), addDegree * GameTime().GetFrameDeltaTime());
				}
				else {
					rot.SetRotation(CVector3::AxisY(), -addDegree * GameTime().GetFrameDeltaTime());
				}
				rot.Multiply(m_moveVector);
				m_timer += GameTime().GetFrameDeltaTime();
			}
		}
	}
	//��~������Ȃ�������
	if (!m_isStop) {
		m_position += m_moveVector * GameTime().GetFrameDeltaTime() * m_moveSpeed;
	}
	return m_position;
}