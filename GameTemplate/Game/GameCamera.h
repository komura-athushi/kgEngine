#pragma once
class Player;

//どっちのスティックが回転か
enum StateEnterStick {
	enStick_EnterStickR,					//右スティックだけ押されている
	enStick_EnterStickL,					//左スティックだけ押されている
	enStick_EnterStickBoth,					//どちらのスティックも入力されている
	enStick_NoEnterStick					//どちらのスティックも入力されていない
};

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start() override;
	void Update() override;
	//パッドの入力を考慮してカメラの視点と注視点を計算
	void Calculation();
	//プレイヤーの大きさを考慮してカメラの半径を変える
	void TransRadius();
	//スティックの入力状態を取得
	StateEnterStick GetStateStick() const
	{
		return m_state;
	}
private:
	CVector3 m_position = CVector3::Zero();				//視点
	CVector3 m_target = CVector3::Zero();				//注視点
	float m_degreey = 0.0f, m_degreexz = 20.0f;			//それぞれカメラの角度
	Player* m_player = nullptr;							//プレイヤーのポインタ
	float m_radius = 0.0f;								//カメラの半径
	const float m_protradius = 260.0f;					//カメラの最初の半径
	StateEnterStick m_state = enStick_NoEnterStick;		//スティックの入力について
	float m_Multiply = 1.0f;							//カメラと塊の距離の倍率
};

