/*****************************************************************//**
 * \file   StageSelectGround.h
 * \brief  StageSelectGround�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
/**
 * \brief �X�e�[�W�Z���N�g��ʂ̒n��.
 */
class StageSelectGround : public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	StageSelectGround();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~StageSelectGround() override final;
	/**
	 * \brief ����������.
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
	 * \param position ���W
	 */
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	/**
	 * \brief �n�ʂ̈ړ����x���擾.
	 * 
	 * \return �ړ����x
	 */
	const CVector3& GetMoveSpeed() const
	{
		return m_currentPosition - m_beforePosition;
	}
	/**
	 * \brief �X�L�����f�������_�[���擾.
	 * 
	 * \return �X�L�����f�������_���[
	 */
	CSkinModelRender& GetModel()
	{
		return  m_skinModelRender;
	}
private:
	/**
	 * \brief ��].
	 * 
	 */
	void Turn();
	/**
	 * \brief �ړ�.
	 * 
	 */
	void Move();
private:
	CSkinModelRender m_skinModelRender;									//�������̃��f��
	CVector3 m_position;												//���W
	CQuaternion m_rotation = CQuaternion::Identity();					//��]
	CVector3 m_currentPosition, m_beforePosition = CVector3::Zero();	//�ړ�����O�ƌ�̍��W

};

