#pragma once
#include "level/Level.h"
#include "Player.h"
class Ground;
class Player;
class GameCamera;
class Object;
//ÉQÅ[ÉÄÉNÉâÉX
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start() override;
	void Update() override;
	//void OnDestroy() override;
private:
	Ground* m_ground = nullptr;
	Player* m_player;
	GameCamera* m_gamecamera = nullptr;
	Object* m_object = nullptr;
	float m_timer = 0.0f;
	//level
	Level m_level;
};

