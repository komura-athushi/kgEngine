#pragma once
class Ground;
class Player;
//ゲームクラス
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
private:
	Ground* m_ground;
	Player* m_player;
	float m_timer = 0.0f;
	bool m_is = false;
};

