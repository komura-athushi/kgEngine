/*****************************************************************//**
 * \file   RotNone.h
 * \brief  RotNoneクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IRot.h"

/**
 * \brief 何も回転しないクラス.
 */
class RotNone :public IRotate
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	RotNone() {}
	/**
	 * \brief .デストラクタ
	 * 
	 */
	~RotNone() override final
	{
	}
	/**
	 * \brief 回転を計算.
	 * 
	 * \param move 移動ベクトル
	 * \return 計算後の回転
	 */
	const CQuaternion Rot(const CVector3& move) override final
	{
		return m_rotation;
	}
	/**
	 * \brief 回転ステートを設定.
	 * 
	 */
	void SetRotState() override final
	{
		IRotate::SetRotStateBasic(enRot_No);
	}
	/**
	 * \brief 初期化処理.
	 * 
	 * \param rot 初期回転
	 * \param speed 回転速度
	 */
	void Init(const CQuaternion& rot, const float speed = 0.0f) override final
	{
		m_rotation = rot;
	}
private:
};



