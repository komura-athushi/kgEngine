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
private:
	Camera m_camera;
	CVector3 m_position = {0.0f,0.0f,100.0f};
	CVector3 m_target = { 0.0f,0.0f,0.0f };
};

