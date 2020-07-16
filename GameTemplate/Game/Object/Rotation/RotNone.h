#pragma once
#include "IRot.h"

class RotNone :public IRotate
{
public:
	RotNone() {}
	~RotNone() override 
	{
	}
	//�v�Z��̉�]��Ԃ�
	CQuaternion Rot(const CVector3& move) override
	{
		return m_rotation;
	}
	//�X�e�[�g��ݒ�A�h���N���X�Őݒ肷��p
	void SetRotState() override
	{
		IRotate::SetRotStateBasic(enRot_No);
	}
	/*!
	* @brief	�������B
	*@param[in] rot				������]
	*@param[in]	speed			��]���x
	*/
	void Init(const CQuaternion& rot, const float& speed = 0.0f) override
	{
		m_rotation = rot;
	}
private:
};


