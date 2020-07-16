#pragma once
#include "IMove.h"
//何もしないMoveクラス
class MoveNone :public IMove
{
public:
	MoveNone();
	~MoveNone() override;
	/*!
	* @brief	初期化。
	*@param[in] pos				座標
	*@param[in]	move			移動速度
	*@param[in]	movelimit		移動範囲
	*@param[in]	rot				回転
	*/
	void Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity()) override;
	void SetMoveState() override
	{
		IMove::SetMoveStateBasic(enMove_No);
	}
	//移動ベクトルを取得
	CVector3 GetMoveVector() const override
	{
		return m_movevector;
	}
	//移動後の座標を返す
	CVector3 Move() override
	{
		return m_position;
	} 
private:
};
