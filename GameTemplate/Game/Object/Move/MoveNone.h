#pragma once
#include "IMove.h"
//�������Ȃ�Move�N���X
class MoveNone :public IMove
{
public:
	MoveNone();
	~MoveNone() override;
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
		IMove::SetMoveStateBasic(enMove_No);
	}
	//�ړ��x�N�g�����擾
	CVector3 GetMoveVector() const override
	{
		return m_movevector;
	}
	//�ړ���̍��W��Ԃ�
	CVector3 Move() override
	{
		return m_position;
	} 
private:
};
