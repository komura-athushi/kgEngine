/*****************************************************************//**
 * \file   MoveNone.h
 * \brief MoveNoneクラス  
 * 
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IMove.h"
/**
 * \brief 移動処理を何もしないクラス.
 */
class MoveNone :public IMove
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	MoveNone();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~MoveNone() override final;
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
		IMove::SetMoveStateBasic(enMove_No);
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
	const CVector3& Move() override final
	{
		return m_position;
	} 
private:
};
