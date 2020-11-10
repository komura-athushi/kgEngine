/*****************************************************************//**
 * \file   OffScreenCamera.h
 * \brief  OffScreenCamera�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/Camera.h"
/**
 * \brief ������}�ӗp�̃J����.
 */
class OffScreenCamera
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	OffScreenCamera();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~OffScreenCamera();
	/**
	 * \brief �X�V����.
	 * 
	 */
	void Update();
	/**
	 * \brief �v���W�F�N�V�����s����擾.
	 * 
	 * \return �v���W�F�N�V�����s��
	 */
	const CMatrix& GetProjectionMatrix() const
	{
		return m_camera.GetProjectionMatrix();
	}
	/**
	 * \brief �r���[�s����擾.
	 * 
	 * \return �r���[�s��
	 */
	const CMatrix& GetViewMatrix() const
	{
		return m_camera.GetViewMatrix();
	}
	/**
	 * \brief �����_��ݒ�.
	 * 
	 * \param target �����_
	 */
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	/**
	 * \brief �����_���擾.
	 * 
	 * \return �����_
	 */
	const CVector3& GetTarget() const
	{
		return m_target;
	}
	/**
	 * \brief ���W��ݒ�.
	 * 
	 * \param position ���W
	 */
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	/**
	 * \brief ���W���擾.
	 * 
	 * \return ���W
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/**
	 * \brief ��p��ݒ�.
	 * 
	 * \param angle ��p
	 */
	void SetViewAngle(const float angle)
	{
		m_camera.SetViewAngle(angle);
	}
	/**
	 * \brief �J�������擾.
	 * 
	 * \return �J����
	 */
	Camera* GetCamera()
	{
		return &m_camera;
	}
private:
	Camera m_camera;									//�J����
	CVector3 m_position = {0.0f,200.0f,500.0f};			//���W
	CVector3 m_target = { 0.0f,0.0f,0.0f };				//�����_
};

