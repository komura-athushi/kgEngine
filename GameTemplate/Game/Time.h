#pragma once
class Game;
#include "graphics/2D/kgFont.h"
class Time:public IGameObject
{
public:
	Time();
	~Time();
	bool Start() override;
	void PostRender() override;
	void SetTime(const float& time) 
	{
		m_time = time;
		m_timer = time;
	}
private:
	CSprite m_sprite1,m_sprite2;
	CFont m_font;
	Game* m_game;
	float m_time = 0.0f;
	float m_timer = 0.0f;
	float m_degree = 0.0f;
};

