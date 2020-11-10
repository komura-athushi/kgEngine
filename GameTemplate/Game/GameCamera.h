/*****************************************************************
 * \file   GameCamera.h
 * \brief  ゲームカメラ
 * 
 * \author komura atushi
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/SpringCamera.h"
class Player;
class GameData;
//どっちのスティックが回転か

enum StateEnterStick {
	enStick_EnterStickR,					//右スティックだけ押されている
	enStick_EnterStickL,					//左スティックだけ押されている
	enStick_EnterStickBoth,					//どちらのスティックも入力されている
	enStick_NoEnterStick,					//どちらのスティックも入力されていない
	enStick_EnterStickBothOppositeLeft,		//左右のスティックがそれぞれ逆方向に入力されている、左が前方向
	enStick_EnterStickBothOppositeRight		//左右のスティックがそれぞれ逆方向に入力されている、右が前方向
};

/**
 * \brief	ゲームカメラクラス
 */
class GameCamera:public IGameObject
{
public:
	/**
	 * \brief	コンストラクタ.
	 */
	GameCamera();
	/**
	 * \brief	デストラクタ.
	 */
	~GameCamera();
	/**
	 * \brief Start関数.
	 * 
	 * \return	trueを返したら初期化終了。
	 */
	bool Start() override final;
	/**
	 * \brief 更新処理.
	 */
	void Update() override final; 
	/**
	 * \brief スティックの入力状態を取得.
	 * 
	 * \return スティックの入力状態
	 */
	StateEnterStick GetStateStick() const
	{
		return m_state;
	}
	/**
	 * \brief 座標を設定.
	 * 
	 * \param position	座標。
	 */
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	/**
	 * \brief 注視点.
	 * 
	 * \param target 注視点
	 */
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	/**
	 * \brief 注視点と座標の距離を取得.
	 * 
	 * \return 注視点と座標の距離
	 */
	float GetRadius() const
	{
		return m_radius;
	}
	/**
	 * \brief プレイヤーを設定.
	 * 
	 * \param player プレイヤー
	 */
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	/**
	 * \brief プレイヤーの番号(1Pか2Pか)を設定.
	 * 
	 * \param number プレイヤーの番号
	 */
	void SetPlayerNumber(const int number)
	{
		m_playerNumber = number;
	}
private:
	/**
	 * \brief パッドの入力を考慮してカメラの視点と注視点を計算.
	 * 
	 */
	void Calculation();
	/**
	 * \brief プレイヤーの大きさを考慮してカメラの半径を変える.
	 * 
	 */
	void TransRadius();
	/**
	 * 	L3とR3を同時押しした時に視点を変更する.
	 * 
	 */
	void TransView();
private:
	CVector3 m_position = CVector3::Zero();				//!視点
	CVector3 m_target = CVector3::One();				//!注視点
	float m_degreey = 0.0f, m_degreexz = 20.0f;			//!それぞれカメラの角度
	Player* m_player = nullptr;							//プレイヤーのポインタ
	float m_radius = 0.0f;								//カメラの半径
	const float m_protradius = 170.0f;					//カメラの最初の半径
	StateEnterStick m_state = enStick_NoEnterStick;		//スティックの入力について
	float m_Multiply = 1.0f;							//カメラと塊の距離の倍率
	float m_timer = 0.0f;								//L3とR3が同時に押された時に時間を計る
	bool m_transView = false;							//L3とR3が同時に押された時のカメラを遷移しているか
	GameData* m_gamedata = nullptr;						//ゲームデータ
	SpringCamera m_springCamera;						//ばねカメラ
	int m_playerNumber = 0;								//プレイヤーの番号
};

