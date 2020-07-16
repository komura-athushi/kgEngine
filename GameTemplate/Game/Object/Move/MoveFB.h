#pragma once
#include "IMove.h"
//�O��Ɉړ�����Move�N���X
class MoveFB :public IMove
{
public:
	MoveFB();
	~MoveFB() override;
	/*!
	* @brief	�������B
	*@param[in] pos				���W
	*@param[in]	move			�ړ����x
	*@param[in]	movelimit		�ړ��͈�
	*@param[in]	rot				��]
	*/
	void Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity()) override;
	void SetMoveState() override
	{
		IMove::SetMoveStateBasic(enMove_Lr);
	}
	//�ړ��x�N�g�����擾
	CVector3 GetMoveVector() const override
	{
		return m_movevector;
	}
	//�ړ���̍��W��Ԃ�
	CVector3 Move() override;
private:
	float m_movespeed = 0.0f;								//�ړ����x
	float m_movelimit = 0.0f;								//�ړ��͈�
	CVector3 m_movelimitFront = CVector3::Zero();			//�㑤�̐���������W
	CVector3 m_movelimitBack = CVector3::Zero();			//�����̐���������W
	bool m_isaccessfront = true;							//�㑤�̍��W�ɋ߂Â����ǂ���
	bool m_isculcmovevector = false;						//�ړ��x�N�g�����v�Z�������ǂ���
	float m_timer = 0.0f;									//�^�C�}�[
	float m_limittimer = 0.0f;								//�^�C�}�[�̐���
	bool m_isstart = true;									//�ŏ��̋������ǂ���
};

