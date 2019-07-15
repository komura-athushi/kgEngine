#pragma once
#include "level/Level.h"
#include "Player.h"
class Ground;
class Player;
class GameCamera;
class Obj;
class Time;
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
	std::vector<Obj*> m_objList;
	float m_timer = 0.0f;
	//level
	Level m_level;
	Time* m_time;
};

