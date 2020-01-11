#pragma once
#include "graphics/Camera.h"
class OffScreenCamera
{
public:
	OffScreenCamera();
	~OffScreenCamera();
	//アップデート
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
	//注視点を設定
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	//注視点を取得
	CVector3 GetTarget()
	{
		return m_target;
	}
	//座標を設定
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	//座標を取得
	CVector3 GetPosition()
	{
		return m_position;
	}
	//画角を設定
	void SetViewAngle(float angle)
	{
		m_camera.SetViewAngle(angle);
	}
	//カメラを取得
	Camera* GetCamera()
	{
		return &m_camera;
	}
private:
	Camera m_camera;									//カメラ
	CVector3 m_position = {0.0f,200.0f,500.0f};			//座標
	CVector3 m_target = { 0.0f,0.0f,0.0f };				//注視点
};

