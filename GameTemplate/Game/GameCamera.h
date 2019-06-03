#pragma once
class Player;
class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start() override;
	void Update() override;
	//パッドの入力を考慮してカメラの視点と注視点を計算
	void Calculation();
private:
	CVector3 m_position = CVector3::Zero();				//視点
	CVector3 m_target = CVector3::Zero();				//注視点
	float m_degreey = 0.0f, m_degreexz = 0.0f;			//それぞれカメラの角度
	Player* m_player = nullptr;							//プレイヤーのポインタ
};

