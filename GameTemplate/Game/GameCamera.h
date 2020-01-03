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

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start() override;
	void Update() override;
	//スティックの入力状態を取得
	StateEnterStick GetStateStick() const
	{
		return m_state;
	}
	//座標を設定
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	//注視点を設定
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	//カメラの座標から注視点までの距離を取得
	float GetRadius() const
	{
		return m_radius;
	}
private:
	//パッドの入力を考慮してカメラの視点と注視点を計算
	void Calculation();
	//プレイヤーの大きさを考慮してカメラの半径を変える
	void TransRadius();
	//L3とR3を同時押しした時に視点を変更する
	void TransView();
private:
	CVector3 m_position = CVector3::Zero();				//視点
	CVector3 m_target = CVector3::One();				//注視点
	float m_degreey = 0.0f, m_degreexz = 20.0f;			//それぞれカメラの角度
	Player* m_player = nullptr;							//プレイヤーのポインタ
	float m_radius = 0.0f;								//カメラの半径
	const float m_protradius = 170.0f;					//カメラの最初の半径
	StateEnterStick m_state = enStick_NoEnterStick;		//スティックの入力について
	float m_Multiply = 1.0f;							//カメラと塊の距離の倍率
	float m_timer = 0.0f;								//L3とR3が同時に押された時に時間を計る
	bool m_transView = false;							//L3とR3が同時に押された時のカメラを遷移しているか
	GameData* m_gamedata = nullptr;						//ゲームデータ
	SpringCamera m_springCamera;						//ばねカメラ
};

