#pragma once
#include "level/Level.h"
#include "Player.h"
class Ground;
class Player;
class GameCamera;
class Obj;
class Time;
class Fade;
class GameData;
//�Q�[���N���X
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	void PostRender() override;
	//�X�e�[�W���I���I�����Ɛݒ肷��
	void SetOwaOwari()
	{
		m_owaOwari = true;
	}
	//void OnDestroy() override;
private:
	Ground* m_ground = nullptr;
	Player m_player;
	GameCamera* m_gamecamera = nullptr;
	std::vector<Obj*> m_objList;
	float m_timer = 0.0f;
	//level
	Level m_level;
	Time* m_time;
	Fade* m_fade;
	GameData* m_gameData;
	bool m_owaOwari = false;
	CFont m_font;
	float m_timer2 = 0.0f;
	
};

