/*****************************************************************//**
 * \file   RotSelf.h
 * \brief  RotSelfクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IRot.h"
/**
 * \briefオブジェクトを自転させるクラス.
 */
class RotSelf :public IRotate
{
public:
	/**
	 * \brief コンストラクタ.
	 *
	 */
	RotSelf();
	/**
	 * \brief デストラクタ.
	 *
	 */
	~RotSelf() override final;
	/**
	 * 回転を計算.
	 * 
	 * \param move 移動ベクトル
	 * \return 計算後の回転
	 */
	const CQuaternion Rot(const CVector3& move) override final;
	/**
	 * \brief 回転ステートを設定.
	 * 
	 */
	void SetRotState() override final
	{
		IRotate::SetRotStateBasic(enRot_Rot);
	}
	/**
	 * \brief 初期化処理.
	 * 
	 * \param rot 初期回転
	 * \param speed 回転速度
	 */
	void Init(const CQuaternion& rot, const float speed = 0.0f) override final;
private:
	float m_speed = 0.0f;												//回転スピード	
	float m_degree = 0.0f;												//角度

};

