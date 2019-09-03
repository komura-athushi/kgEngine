#pragma once
class Game;
class GameData;
#include "graphics/2D/kgFont.h"
//ステージの制限時間を管理する
class Time:public IGameObject
{
public:
	Time();
	~Time();
	bool Start() override;
	void PostRender() override;
	//制限時間を設定する
	void SetTime(const float& time) 
	{
		m_time = time;
		m_timer = time;
	}
private:
	CSprite m_sprite1,m_sprite2;								//スプライト
	CFont m_font;												//フォント
	Game* m_game = nullptr;										//ゲーム
	GameData* m_gamedata = nullptr;								//ゲームデータ
	float m_time = 0.0f;										//制限時間
	float m_timer = 0.0f;										//タイマー
	float m_degree = 0.0f;										//円形ゲージの角度
};

