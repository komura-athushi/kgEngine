/*****************************************************************//**
 * \file   MoveUp.h
 * \brief MoveUpクラス 
 * 
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IMove.h"
/**
 * \brief オブジェクトを上下に移動させる.
 */
class MoveUp:public IMove
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	MoveUp();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~MoveUp() override final;
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
		IMove::SetMoveStateBasic(enMove_Up);
	}
	/**
	 * \brief 移動ベクトルを設定.
	 * 
	 * \return 移動ベクトル
	 */
	const CVector3& GetMoveVector() const override final
	{
		return m_moveVector;
	}
	/**
	 * \brief オブジェクトを移動させる.
	 * 
	 * \return 移動後の座標
	 */
	const CVector3& Move() override final;
private:
	float m_moveLimit = 0.0f;								//移動範囲
	CVector3 m_moveLimitUp = CVector3::Zero();				//上側の制限する座標
	CVector3 m_moveLimitDown = CVector3::Zero();			//下側の制限する座標
	bool m_isAccessUp = true;								//上側の座標に近づくかどうか
	float m_timer = 0.0f;									//タイマー
	float m_limitTimer = 0.0f;								//タイマーの制限
	bool m_isStart = true;									//最初の挙動かどうか
};

