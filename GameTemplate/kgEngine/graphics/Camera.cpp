#include "KGstdafx.h"
#include "Camera.h"



void Camera::Update()
{
	//�J�����̑O���������߂�
	m_front = m_target - m_position;
	m_front.y = 0.0f;
	m_front.Normalize();
	//�J�����̉E���������߂�
	m_right.Cross(m_up, m_front);
	m_right.y = 0.0f;
	m_right.Normalize();
	//�r���[�s����v�Z�B
	m_viewMatrix.MakeLookAt(
		m_position,
		m_target,
		m_up
	);
	//�v���W�F�N�V�����s����v�Z�B
	m_projMatrix.MakeProjectionMatrix(
		m_viewAngle,					//��p�B
		FRAME_BUFFER_W / FRAME_BUFFER_H,	//�A�X�y�N�g��B
		m_near,
		m_far
	);
}