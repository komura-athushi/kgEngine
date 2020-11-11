/*****************************************************************//**
 * \file   StageSelectPoint.h
 * \brief  StageSelectPoint�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
class StageSelectGround;

enum enPoint {
	enPoint_nun,
	enPoint_Stage1 = 1,
	enPoint_Stage2 = 2,
	enPoint_Collection,
	enPoint_Battle
};

/**
 * \brief �X�e�[�W�Z���N�g�̃|�C���g.
 */
class StageSelectPoint:public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	StageSelectPoint();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~StageSelectPoint() override final;
	/**
	 * \brief ������.
	 * 
	 * \return true�ŏ���������
	 */
	bool Start() override final;
	/**
	 * \brief �X�V����.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief ���W��ݒ�.
	 * 
	 * \param pos ���W
	 */
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	/**
	 * ^\brief ���W���擾.
	 * 
	 * \return ���W
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/**
	 * \brief ��]��ݒ�.
	 * 
	 * \param rot ��]
	 */
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	/**
	 * \brief �傫����ݒ�.
	 * 
	 * \param scale �傫��
	 */
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
	/**
	 * \brief �|�C���g�̑�����ݒ�.
	 * 
	 * \param point �|�C���g�̑���
	 */
	void SetPoint(enPoint point)
	{
		m_enPoint = point;
	}
	/**
	 * \brief �|�C���g�̑������擾.
	 * 
	 * \return �|�C���g�̑���
	 */
	 const enPoint GetPoint() const
	{
		return m_enPoint;
	}
private:
	/**
	 * \brief ���[�J���s����v�Z.
	 * 
	 */
	void ClcLocalMatrix();
private:
	CSkinModelRender m_model;								//�X�L�����f�������_�[
	CVector3 m_position;									//���W		
	CQuaternion m_rotation;									//��]
	CVector3 m_scale;										//�傫��
	CMatrix m_localMatrix;									//���[�J���s��
	CMatrix m_worldMatrix;									//���[���h�s��
	StageSelectGround* m_stageSelectGround = nullptr;		//������
	bool m_isFind = false;									//���������������H
	enPoint m_enPoint = enPoint_nun;
};

