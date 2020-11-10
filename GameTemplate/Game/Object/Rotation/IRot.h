/*****************************************************************//**
 * \file   IRot.h
 * \brief  IRot�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma  once
enum EnRotate{
	enRot_Rot,									//���]
	enRot_DirectionRot,							//�ړ������ɉ�]
	enRot_No,									//��]���Ȃ�
};

/**
 * \brief �I�u�W�F�N�g�̉�]�𐧌䂷���{�N���X.
 */
class IRotate
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	IRotate() {}
	/**
	 * \brief �f�X�g���N�^�@��{�N���X�Ȃ̂�virtual.
	 * 
	 */
	virtual ~IRotate() {}
	/**
	 * \brief ��]���v�Z.
	 * 
	 * \param move �ړ��x�N�g��
	 * \return �v�Z��̉�]
	 */
	virtual const CQuaternion Rot(const CVector3& move) { return CQuaternion::Identity(); }
	/**
	 * \brief �ړ��X�e�[�g��ݒ�.
	 * 
	 */
	virtual void SetRotState() {}
	/**
	 * \brief ����������.
	 * 
	 * \param rot ������]
	 * \param speed ��]���x(1.0f��360�x����b�ŉ��A-�ɂ���Ɣ��Ε����ɉ��)
	 */
	virtual void Init(const CQuaternion& rot, const float speed = 0.0f) {}
	/**
	 * \brief �ړ��X�e�[�g���擾.
	 * 
	 * \return 
	 */
	EnRotate GetRotState() const
	{
		return m_state;
	}
	/**
	 * \brief �ړ��X�e�[�g��ݒ�.
	 * 
	 * \param state �ړ��X�e�[�g
	 */
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
