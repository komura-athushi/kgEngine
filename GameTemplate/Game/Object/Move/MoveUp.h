#pragma once
#include "IMove.h"
//上下に移動
class MoveUp:public IMove
{
public:
	MoveUp();
	~MoveUp() override;
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
		IMove::SetMoveStateBasic(enMove_Up);
	}
	//移動ベクトルを取得
	CVector3 GetMoveVector() const override
	{
		return m_movevector;
	}
	//移動後の座標を返す
	CVector3 Move() override;
private:
	CVector3 m_position = CVector3::Zero();					//座標
	float m_movespeed = 0.0f;								//移動速度
	CVector3 m_movevector = CVector3::Zero();				//移動ベクトル
	float m_movelimit = 0.0f;								//移動範囲
	CVector3 m_movelimitUp = CVector3::Zero();				//上側の制限する座標
	CVector3 m_movelimitDown = CVector3::Zero();			//下側の制限する座標
	bool m_isaccessup = true;								//上側の座標に近づくかどうか
	bool m_isculcmovevector = false;						//移動ベクトルを計算したかどうか
	float m_timer = 0.0f;									//タイマー
	float m_limittimer = 0.0f;								//タイマーの制限
	bool m_isstart = true;									//最初の挙動かどうか
};

