/*****************************************************************//**
 * \file   MoveLR.h
 * \brief  MoveLRクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IMove.h"
/**
 * \brief オブジェクトを左右に移動させる.
 */
class MoveLR:public IMove
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	MoveLR();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~MoveLR() override final;
	/**
	 * \brief 初期化処理.
	 *
	 * \param pos 座標
	 * \param move 移動速度
	 * \param movelimit 移動範囲
	 * \param rot 回転
	 */
	void Init(const CVector3& pos, const float move, const float movelimit,const CQuaternion& rot = CQuaternion::Identity()) override final;
	/**
	 * \brief 移動ステートを設定.
	 * 
	 */
	void SetMoveState() override final
	{
		IMove::SetMoveStateBasic(enMove_Lr);
	}
	//移動ベクトルを取得
	const CVector3& GetMoveVector() const override final
	{
		return m_moveVector;
	}
	//移動後の座標を返す
	const CVector3& Move() override final;
private:
	float m_moveLimit = 0.0f;								//移動範囲
	CVector3 m_moveLimitLeft = CVector3::Zero();			//左側の制限する座標
	CVector3 m_moveLimitRight = CVector3::Zero();			//右側の制限する座標
	bool m_isAccessLeft = true;								//左側の座標に近づくかどうか
	float m_timer = 0.0f;									//タイマー
	float m_limitTimer = 0.0f;								//タイマーの制限
	bool m_isstart = true;									//最初の挙動かどうか
};

