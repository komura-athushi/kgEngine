/*****************************************************************//**
 * \file   IMove.h
 * \brief  IMove�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
enum EnMove {
	enMove_Lr,									//���E�Ɉړ�
	enMove_Fb,									//�O��Ɉړ�
	enMove_Up,									//�㉺�Ɉړ�
	enMove_Rot,									//��]
	enMove_Path,								//�p�X�ړ�
	enMove_No,									//�ړ����Ȃ�
	enMove_MoveHit,								//�v���C���[(���̂Ɋ������܂ꂽ)
};

/**
 * \brief  �I�u�W�F�N�g�̈ړ��̐���������{�N���X.
 */
class IMove
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	IMove() {}
	/**
	 * \brief �f�X�g���N�^�A��{�N���X�Ȃ̂�virtual.
	 * 
	 */
	virtual ~IMove() {}
	/**
	 * \brief �ړ�����.
	 * 
	 * \return �ړ�������̍��W
	 */
	virtual const CVector3& Move() { return CVector3::Zero(); } 
	/**
	 * \brief �ړ��X�e�[�g��ݒ�.
	 * 
	 */
	virtual void SetMoveState() {}
	/**
	 * \brief ����������.
	 * 
	 * \param pos ���W
	 * \param move �ړ����x
	 * \param movelimit �ړ��͈�
	 * \param rot ��]
	 */
	virtual void Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot = CQuaternion::Identity()) {}
	/**
	 * �ړ��x�N�g�����擾.
	 * 
	 * \return �ړ��x�N�g��
	 */
	virtual const CVector3& GetMoveVector() const { return CVector3::Zero(); }
	/**
	 * \brief �ړ��X�e�[�g���擾.
	 * 
	 * \return �ړ��X�e�[�g
	 */
	const EnMove GetMoveState() const
	{
		return m_state;
	}
	/**
	 * �ړ��X�e�[�g��ݒ�.
	 * 
	 * \param state �ړ��X�e�[�g
	 */
	void SetMoveStateBasic(EnMove state)
	{
		m_state = state;
	}
protected:
	CVector3 m_position = CVector3::Zero();					//���W
	CVector3 m_moveVector = CVector3::Zero();				//�ړ��x�N�g��
	float m_moveSpeed = 0.0f;								//�ړ����x
	bool m_isCulcMoveVector = false;						//�ړ��x�N�g�����v�Z�������ǂ���
private:
	EnMove m_state;							//�X�e�[�g
};
//IMove��unique_ptr�̕ʖ���`�B
using IMovePtr = std::unique_ptr<IMove>;

