#pragma once
#include "IRot.h"

class RotNone :public IRotate
{
public:
	RotNone() {}
	~RotNone() override 
	{
	}
	//計算後の回転を返す
	CQuaternion Rot(const CVector3& move) override
	{
		return m_rotation;
	}
	//ステートを設定、派生クラスで設定する用
	void SetRotState() override
	{
		IRotate::SetRotStateBasic(enRot_No);
	}
	/*!
	* @brief	初期化。
	*@param[in] rot				初期回転
	*@param[in]	speed			回転速度
	*/
	void Init(const CQuaternion& rot, const float& speed = 0.0f) override
	{
		m_rotation = rot;
	}
private:
};



