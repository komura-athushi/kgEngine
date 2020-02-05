#include "KGstdafx.h"
#include "Camera.h"



void Camera::Update()
{
	//ビュー行列を計算。
	m_viewMatrix.MakeLookAt(
		m_position,
		m_target,
		m_up
	);
	if(Engine().GetGraphicsEngine().GetisSplit()) {

		//プロジェクション行列を計算。
		m_projMatrix.MakeProjectionMatrix(
			m_splitViewAngle,					//画角。
			FRAME_BUFFER_W / ( FRAME_BUFFER_H / 2) ,	//アスペクト比。
			m_near,
			m_far
		);
	}
	else {
		//プロジェクション行列を計算。
		m_projMatrix.MakeProjectionMatrix(
			m_viewAngle,					//画角。
			FRAME_BUFFER_W / FRAME_BUFFER_H,	//アスペクト比。
			m_near,
			m_far
		);
	}
	//ビュー行列の逆行列を計算。
	CMatrix ViewMatrixInv;
	ViewMatrixInv.Inverse(m_viewMatrix);
	m_front.Set(ViewMatrixInv.m[2][0], ViewMatrixInv.m[2][1], ViewMatrixInv.m[2][2]);
	m_right.Set(ViewMatrixInv.m[0][0], ViewMatrixInv.m[0][1], ViewMatrixInv.m[0][2]);
}