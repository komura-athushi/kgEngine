#pragma once
#include "graphics/Camera.h"
class OffScreenCamera
{
public:
	OffScreenCamera();
	~OffScreenCamera();
	void Update();
	//プロジェクション行列を取得
	CMatrix GetProjectionMatrix()
	{
		return m_camera.GetProjectionMatrix();
	}
	//ビュー行列を取得
	CMatrix GetViewMatrix()
	{
		return m_camera.GetViewMatrix();
	}
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	CVector3 GetTarget()
	{
		return m_target;
	}
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	CVector3 GetPosition()
	{
		return m_position;
	}
	//画角を設定
	void SetViewAngle(float angle)
	{
		m_camera.SetViewAngle(angle);
	}
private:
	Camera m_camera;
	CVector3 m_position = {0.0f,200.0f,500.0f};
	CVector3 m_target = { 0.0f,0.0f,0.0f };
};

