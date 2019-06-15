#pragma once
#include "IRot.h"
//自転
class RotSelf:public IRotate
{
public:
	RotSelf();
	~RotSelf() override;
	//計算後の回転を返す
	CQuaternion Rot(const CVector3& move) override; 
	//ステートを設定、派生クラスで設定する用
	void SetRotState() override
	{
		IRotate::SetRotStateBasic(enRot_Rot);
	}
	/*!
	* @brief	初期化。
	*@param[in] rot				初期回転
	*@param[in]	speed			回転速度
	*/
	void Init(const CQuaternion& rot, const float& speed = 0.0f) override;
private:
	CQuaternion m_rotation = CQuaternion::Identity();					//回転
	float m_speed = 0.0f;
	float m_degree = 0.0f;

};

