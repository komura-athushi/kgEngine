#pragma once
class Ground;
class Player;
class GameCamera;
class Object;
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
	Ground* m_ground = nullptr;
	Player* m_player = nullptr;
	GameCamera* m_gamecamera = nullptr;
	Object* m_object = nullptr;
	float m_timer = 0.0f;
};

