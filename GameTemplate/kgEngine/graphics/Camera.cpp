#include "KGstdafx.h"
#include "Camera.h"



void Camera::Update()
{
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