#pragma once
enum EnMove {
	enMove_Lr,									//���E�Ɉړ�
	enMove_Fb,
	enMove_Up,									//�㉺�Ɉړ�
	enMove_Rot,									//��]
	enMove_Path,
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
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="move">�ړ����x</param>
	/// <param name="movelimit">�ړ��͈�</param>
	/// <param name="rot">��]</param>
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
protected:
	CVector3 m_position = CVector3::Zero();					//���W
	CVector3 m_movevector = CVector3::Zero();				//�ړ��x�N�g��
private:
	EnMove m_state;							//�X�e�[�g
};
//IMove��unique_ptr�̕ʖ���`�B
using IMovePtr = std::unique_ptr<IMove>;

