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
	float m_speed = 0.0f;												//回転スピード	
	float m_degree = 0.0f;												//角度

};

