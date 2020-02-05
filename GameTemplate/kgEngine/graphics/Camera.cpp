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
	if(Engine().GetGraphicsEngine().GetisSplit()) {

		//�v���W�F�N�V�����s����v�Z�B
		m_projMatrix.MakeProjectionMatrix(
			m_splitViewAngle,					//��p�B
			FRAME_BUFFER_W / ( FRAME_BUFFER_H / 2) ,	//�A�X�y�N�g��B
			m_near,
			m_far
		);
	}
	else {
		//�v���W�F�N�V�����s����v�Z�B
		m_projMatrix.MakeProjectionMatrix(
			m_viewAngle,					//��p�B
			FRAME_BUFFER_W / FRAME_BUFFER_H,	//�A�X�y�N�g��B
			m_near,
			m_far
		);
	}
	//�r���[�s��̋t�s����v�Z�B
	CMatrix ViewMatrixInv;
	ViewMatrixInv.Inverse(m_viewMatrix);
	m_front.Set(ViewMatrixInv.m[2][0], ViewMatrixInv.m[2][1], ViewMatrixInv.m[2][2]);
	m_right.Set(ViewMatrixInv.m[0][0], ViewMatrixInv.m[0][1], ViewMatrixInv.m[0][2]);
}