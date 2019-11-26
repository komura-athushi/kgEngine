#pragma once
#include "graphics/Camera.h"
class OffScreenCamera
{
public:
	OffScreenCamera();
	~OffScreenCamera();
	void Update();
	//�v���W�F�N�V�����s����擾
	CMatrix GetProjectionMatrix()
	{
		return m_camera.GetProjectionMatrix();
	}
	//�r���[�s����擾
	CMatrix GetViewMatrix()
	{
		return m_camera.GetViewMatrix();
	}
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
private:
	Camera m_camera;
	CVector3 m_position = {0.0f,0.0f,20.0f};
	CVector3 m_target = { 0.0f,0.0f,0.0f };
};

