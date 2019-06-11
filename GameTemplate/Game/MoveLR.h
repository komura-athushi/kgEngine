#pragma once
#include "IMove.h"
class MoveLR:public IMove
{
public:
	MoveLR();
	~MoveLR() override;
	/*!
	* @brief	�������B
	*@param[in] pos				���W
	*@param[in]	move			�ړ����x
	*@param[in]	movelimit		�ړ��͈�
	*@param[in]	rot				��]
	*/
	void Init(const CVector3& pos, const float& move, const float& movelimit,const CQuaternion& rot = CQuaternion::Identity()) override;
	void SetMoveState() override
	{
		IMove::SetMoveStateBasic(enMove_Lr);
	}
	//�ړ���̍��W��Ԃ�
	CVector3 Move() override;
private:
	CVector3 m_position = CVector3::Zero();					//���W
	float m_movespeed = 0.0f;								//�ړ����x
	CVector3 m_movevector = CVector3::Zero();				//�ړ��x�N�g��
	float m_movelimit = 0.0f;								//�ړ��͈�
	CVector3 m_movelimitLeft = CVector3::Zero();			//�����̐���������W
	CVector3 m_movelimitRight = CVector3::Zero();			//�E���̐���������W
	bool m_isaccessleft = true;								//�����̍��W�ɋ߂Â����ǂ���
	bool m_isculcmovevector = false;						//�ړ��x�N�g�����v�Z�������ǂ���
	float m_timer = 0.0f;									//�^�C�}�[
	float m_limittimer = 0.0f;								//�^�C�}�[�̐���
	bool m_isstart = true;									//�ŏ��̋������ǂ���
};

