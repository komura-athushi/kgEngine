#pragma once
enum EnMove {
	enMove_Lr,									//���E�Ɉړ�
	enMove_Up,									//�㉺�Ɉړ�
	enMove_Rot,									//��]
	enMove_No,									//�ړ����Ȃ�
	enMove_MoveHit,								//�v���C���[(���̂Ɋ������܂ꂽ)
};

//Move�N���X�̊�{�N���X
class IMove
{
public:
	IMove() {}
	virtual ~IMove() {}
	//�ړ���̍��W��Ԃ�
	virtual CVector3 Move() { return CVector3::Zero(); }
	//�X�e�[�g��ݒ�A�h���N���X�Őݒ肷��p
	virtual void SetMoveState() {}
	/*!
	* @brief	�������B
	*@param[in] pos				���W
	*@param[in]	move			�ړ����x	
	*@param[in]	movelimit		�ړ��͈�
	*@param[in]	rot				��]
	*/
	virtual void Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity()) {}
	//�ړ��x�N�g�����擾
	virtual CVector3 GetMoveVector() const { return CVector3::Zero(); }
	//�X�e�[�g���擾
	EnMove GetMoveState()
	{
		return m_state;
	}
	//�X�e�[�g��ݒ�
	void SetMoveStateBasic(EnMove state)
	{
		m_state = state;
	}
private:
	EnMove m_state;
};

