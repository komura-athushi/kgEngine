#pragma once
#include "graphics/Camera.h"
class OffScreenCamera
{
public:
	OffScreenCamera();
	~OffScreenCamera();
	//�A�b�v�f�[�g
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
	//�����_��ݒ�
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	//�����_���擾
	CVector3 GetTarget()
	{
		return m_target;
	}
	//���W��ݒ�
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	//���W���擾
	CVector3 GetPosition()
	{
		return m_position;
	}
	//��p��ݒ�
	void SetViewAngle(float angle)
	{
		m_camera.SetViewAngle(angle);
	}
	//�J�������擾
	Camera* GetCamera()
	{
		return &m_camera;
	}
private:
	Camera m_camera;									//�J����
	CVector3 m_position = {0.0f,200.0f,500.0f};			//���W
	CVector3 m_target = { 0.0f,0.0f,0.0f };				//�����_
};

