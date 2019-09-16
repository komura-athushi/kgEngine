#include "stdafx.h"
#include "MovePath.h"

MovePath::MovePath()
{

}

MovePath::~MovePath()
{

}

void MovePath::Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot)
{
	m_position = pos;
	m_movespeed = move;
	m_point = m_path.GetNearPoint(m_position);
	SetMoveState();
}

void MovePath::ReadPath(const wchar_t* filePath)
{
	m_path.Load(filePath);
}

CVector3 MovePath::Move()
{
	const float Molecule = 5.0f;
	const float AddDegree = 180.0f / 2 / 180.0f * CMath::PI;

	if (m_isstart) {
		m_movevector = m_point->s_vector - m_position;
		m_movevector.Normalize();
		m_isstart = false;
	}
	else {
		CVector3 Distance = m_point->s_vector - m_position;
		if (!m_isstop) {
			if (Distance.LengthSq() <= m_movespeed * m_movespeed * GameTime().GetFrameDeltaTime()) {
				//���݂̐��ʂ̃x�N�g���ƁA���̃p�X�Ɍ����Ă̈ړ��x�N�g�������߂�
				m_point = m_path.GetPoint(m_point->s_number);
				CVector3 Distance = m_point->s_vector - m_position;
				Distance.y = 0;
				Distance.Normalize();
				CVector3 MoveVector = m_movevector;
				MoveVector.y = 0.0f;
				MoveVector.Normalize();
				//�e�x�N�g���̊p�x�����߂�(0�`PI)
				float Degree1 = acosf(MoveVector.x);
				float Degree2 = acosf(Distance.x);
				//2PI�܂őΉ�������
				if (MoveVector.z <= 0.0f) {
					Degree1 = Degree1 + (CMath::PI - Degree1) * 2;
				}
				if (Distance.z <= 0.0f) {
					Degree2 = Degree2 + (CMath::PI - Degree2) * 2;
				}
				//�p�x�̍������߂�
				float Degree = 0.0f;
				if (Degree1 >= Degree2) {
					if (Degree1 - Degree2 >= CMath::PI) {
						Degree = CMath::PI * 2 - Degree1 + Degree2;
						m_isadddegree = false;
					}
					else {
						Degree = Degree1 - Degree2;
						m_isadddegree = true;
					}
				}
				else {
					if (Degree2 - Degree1 >= CMath::PI) {
						Degree = CMath::PI * 2 - Degree2 + Degree1;
						m_isadddegree = true;
					}
					else {
						Degree = Degree2 - Degree1;
						m_isadddegree = false;
					}
				}
				m_time = Degree / AddDegree;
				//�ꎞ��~
				m_isstop = true;
			}
		}
		else {
			//�^�C�}�[�����ȏ�ɂȂ�����ړ�����
			if (m_timer >= m_time) {
				m_movevector = m_point->s_vector - m_position;
				m_movevector.Normalize();
				m_isstop = false;
				m_timer = 0.0f;
			}
			//�x�N�g������]������
			else {
				CQuaternion rot;
				if (m_isadddegree) {
					rot.SetRotation(CVector3::AxisY(), AddDegree * GameTime().GetFrameDeltaTime());
				}
				else {
					rot.SetRotation(CVector3::AxisY(), -AddDegree * GameTime().GetFrameDeltaTime());
				}
				rot.Multiply(m_movevector);
				m_timer += GameTime().GetFrameDeltaTime();
			}
		}
	}
	if (!m_isstop) {
		m_position += m_movevector * GameTime().GetFrameDeltaTime() * m_movespeed;
	}
	return m_position;
}