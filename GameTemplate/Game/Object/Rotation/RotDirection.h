/*****************************************************************//**
 * \file   RotDirection.h
 * \brief  RotDirectionクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IRot.h"
/**
 * \brief オブジェクトを移動方向に回転させるクラス.
 */
class RotDirection:public IRotate
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	RotDirection();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~RotDirection() override final;
	/**
	 * \brief オブジェクトの回転を計算.
	 * 
	 * \param move 移動ベクトル
	 * \return 計算後の回転
	 */
	const CQuaternion Rot(const CVector3& move) override final;
	/**
	 * \brief 移動ステートを設定.
	 * 
	 */
	void SetRotState() override final
	{
		IRotate::SetRotStateBasic(enRot_DirectionRot);
	}
	/**
	 * \brief 初期化処理.
	 * 
	 * \param rot 初期回転
	 * \param speed 回転速度
	 */
	void Init(const CQuaternion& rot, const float speed = 0.0f) override final;
private:
};

