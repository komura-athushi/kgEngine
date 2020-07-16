#pragma once
#include "IRot.h"
//�ړ������ɉ�]
class RotDirection:public IRotate
{
public:
	RotDirection();
	~RotDirection() override;
	//�v�Z��̉�]��Ԃ�
	CQuaternion Rot(const CVector3& move) override;
	//�X�e�[�g��ݒ�A�h���N���X�Őݒ肷��p
	void SetRotState() override
	{
		IRotate::SetRotStateBasic(enRot_DirectionRot);
	}
	/*!
	* @brief	�������B
	*@param[in] rot				������]
	*@param[in]	speed			��]���x
	*/
	void Init(const CQuaternion& rot, const float& speed = 0.0f) override;
private:
};

