#pragma once
class Ground;
class Player;
//ゲームクラス
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	Ground* m_ground;
	Player* m_player;
	float m_timer = 0.0f;
};

