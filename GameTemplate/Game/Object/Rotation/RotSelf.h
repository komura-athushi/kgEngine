#pragma once
#include "IRot.h"
//���]
class RotSelf:public IRotate
{
public:
	RotSelf();
	~RotSelf() override;
	//�v�Z��̉�]��Ԃ�
	CQuaternion Rot(const CVector3& move) override; 
	//�X�e�[�g��ݒ�A�h���N���X�Őݒ肷��p
	void SetRotState() override
	{
		IRotate::SetRotStateBasic(enRot_Rot);
	}
	/*!
	* @brief	�������B
	*@param[in] rot				������]
	*@param[in]	speed			��]���x
	*/
	void Init(const CQuaternion& rot, const float& speed = 0.0f) override;
private:
	CQuaternion m_rotation = CQuaternion::Identity();					//��]
	float m_speed = 0.0f;
	float m_degree = 0.0f;

};

