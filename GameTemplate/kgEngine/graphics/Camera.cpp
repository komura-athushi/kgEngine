#include "KGstdafx.h"
#include "Camera.h"



void Camera::Update()
{
	//カメラの前方向を求める
	m_front = m_target - m_position;
	m_front.y = 0.0f;
	m_front.Normalize();
	//カメラの右方向を求める
	m_right.Cross(m_up, m_front);
	m_right.y = 0.0f;
	m_right.Normalize();
	//ビュー行列を計算。
	m_viewMatrix.MakeLookAt(
		m_position,
		m_target,
		m_up
	);
	//プロジェクション行列を計算。
	m_projMatrix.MakeProjectionMatrix(
		m_viewAngle,					//画角。
		FRAME_BUFFER_W / FRAME_BUFFER_H,	//アスペクト比。
		m_near,
		m_far
	);
}