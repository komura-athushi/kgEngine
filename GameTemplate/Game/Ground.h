/*****************************************************************//**
 * \file   Ground.h
 * \brief  Groundクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"

/**
 * \brief ステージの地面.
 */
class Ground:public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Ground();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Ground() override final;
	/**
	 * \brief 初期化処理.
	 * 
	 * \return 初期化完了したらtrue
	 */
	bool Start() override final;
	/**
	 * \brief 更新処理.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief 座標を設定.
	 * 
	 * \param pos 座標
	 */
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	/**
	 * \brief 回転を設定.
	 * 
	 * \param rot 回転
	 */
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	CSkinModelRender m_skinModelRender;								//スキンモデルレンダー
	PhysicsStaticObject m_staticObject;								//静的オブジェクト
	CVector3 m_position = CVector3::Zero();							//座標
	CQuaternion m_rotation = CQuaternion::Identity();				//回転
};

