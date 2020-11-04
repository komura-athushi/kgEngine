/*****************************************************************//**
 * \file   MovePath.h
 * \brief MovePath�N���X  
 * 
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IMove.h"
#include "Path.h"
/**
 * \brief �I�u�W�F�N�g���p�X�ړ�������N���X.
 */
class MovePath:public IMove
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	MovePath();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~MovePath() override final;
	/**
	 * �I�u�W�F�N�g�̈ړ�����.
	 * 
	 * \return �ړ�������̍��W
	 */
	const CVector3& Move() override final;
	/**
	 * \brief �ړ��X�e�[�g��ݒ�.
	 * 
	 */
	void SetMoveState() override final
	{
		IMove::SetMoveStateBasic(enMove_Path);
	}
	/**
	 * \brief ����������.
	 *
	 * \param pos ���W
	 * \param move �ړ����x
	 * \param movelimit �ړ��͈�
	 * \param rot ��]
	 */
	void Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot = CQuaternion::Identity()) override final;
	/**
	 * \brief �p�X�f�[�^��ǂݍ���.
	 * 
	 * \param filePath �t�@�C���p�X
	 */
	void ReadPath(const wchar_t* filePath);
	/**
	 * \brief �ړ��x�N�g�����擾.
	 * 
	 * \return �ړ��x�N�g��
	 */
	const CVector3& GetMoveVector() const override final
	{
		return m_moveVector;
	}
private:
	Path m_path;											//�p�X
	Point* m_point = nullptr;								//���������Ă�|�C���g
	bool m_isStart = true;									//�X�^�[�g
	bool m_isStop = false;									//�X�g�b�v�H
	bool m_isAddDegree = true;								//�����]��
	float m_time = 0.0f;									//�����]���̎���
	float m_timer = 0.0f;									//�^�C�}�[
};

