#pragma  once
enum EnRotate{
	enRot_Rot,									//���]
	enRot_DirectionRot,							//�ړ������ɉ�]
	enRot_No,									//��]���Ȃ�
};

//Rot�N���X�̊�{�N���X
class IRotate
{
public:
	IRotate() {}
	virtual ~IRotate() {}
	//�v�Z��̉�]��Ԃ�
	virtual CQuaternion Rot(const CVector3& move) { return CQuaternion::Identity(); }
	//�X�e�[�g��ݒ�A�h���N���X�Őݒ肷��p
	virtual void SetRotState() {}
	/*!
	* @brief	�������B
	*@param[in] rot				������]
	*@param[in]	speed			��]���x(1.0f��360�x����b�ŉ��A-�ɂ���Ɣ��Ε����ɉ��)
	*/
	virtual void Init(const CQuaternion& rot, const float& speed = 0.0f) {}
	//�X�e�[�g���擾
	EnRotate GetRotState()
	{
		return m_state;
	}
	//�X�e�[�g��ݒ�
	void SetRotStateBasic(EnRotate state)
	{
		m_state = state;
	}
protected:
	CQuaternion m_rotation = CQuaternion::Identity();					//��]
private:
	EnRotate m_state;
};
//IRotate��unique_ptr�̕ʖ���`�B
using IRotatePtr = std::unique_ptr<IRotate>;
