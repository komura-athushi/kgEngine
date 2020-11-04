/*****************************************************************//**
 * \file   MoveFB.h
 * \brief MoveFBクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IMove.h"

/**
 * \brief オブジェクトを前後に動かすクラス.
 */
class MoveFB :public IMove
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	MoveFB();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~MoveFB() override final;
	/**
	 * \brief 初期化処理.
	 * 
	 * \param pos 座標
	 * \param move 移動速度
	 * \param movelimit 移動範囲
	 * \param rot 回転
	 */
	void Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot = CQuaternion::Identity()) override final;
	/**
	 * \brief 移動ステートを設定.
	 * 
	 */
	void SetMoveState() override final
	{
		IMove::SetMoveStateBasic(enMove_Lr);
	} 
	/**
	 * \brief 移動ベクトルを取得.
	 * 
	 * \return 移動ベクトル
	 */
	const CVector3& GetMoveVector() const override final
	{
		return m_moveVector;
	}
	/**
	 * \brief 移動処理.
	 * 
	 * \return 移動後の座標
	 */
	const CVector3& Move() override final;
private:
	float m_moveLimit = 0.0f;								//移動範囲
	CVector3 m_moveLimitFront = CVector3::Zero();			//上側の制限する座標
	CVector3 m_moveLimitBack = CVector3::Zero();			//下側の制限する座標
	bool m_isAccessFront = true;							//上側の座標に近づくかどうか
	float m_timer = 0.0f;									//タイマー
	float m_limitTimer = 0.0f;								//タイマーの制限
	bool m_isStart = true;									//最初の挙動かどうか
};

