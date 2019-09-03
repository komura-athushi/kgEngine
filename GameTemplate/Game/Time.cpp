#include "stdafx.h"
#include "Time.h"
#include "Game.h"
#include "GameData.h"
Time::Time()
{

}

Time::~Time()
{

}

bool Time::Start()
{
	//黒
	m_sprite1.Init(L"Resource/sprite/clockkari.dds", false);
	//黄色
	m_sprite2.Init(L"Resource/sprite/clockkari2.dds", true);
	//制限時間から円形ゲージの角度を決定する
	m_degree = (m_time / 60.0f) * 360.0f / 60.0f;
	m_game = FindGO<Game>();
	m_gamedata = &GetGameData();
	return true;
}

void Time::PostRender()
{

	//タイマーが0になればステージ終了フラグを建てる
	if (m_timer <= 0.0000001f) {
		m_timer = 0.0f;
		m_game->SetOwaOwari();
	}
	else {
		if (!m_gamedata->GetisPose()) {
			//タイマーを減らす
			m_timer -= GameTime().GetFrameDeltaTime();
		}
	}
	m_degree = (m_timer / 60.0f) * 360.0f / 60.0f;
	m_sprite1.DrawScreenPos(CVector2(200.0f, 200.0f), CVector2(0.8f, 0.8f), CVector2(0.5f, 0.5f), 0.0f, CVector4(1.0f, 1.0f, 1.0f, 0.5f),
		DirectX::SpriteEffects_None, 0.5f);
	m_sprite2.DrawScreenPos(CVector2(200.0f, 200.0f), CVector2(0.8f, 0.8f), CVector2(0.5f, 0.5f), 0.0f, CVector4(1.0f, 1.0f, 1.0f, 0.8f),
		DirectX::SpriteEffects_None, 0.5f, m_degree);
	int minutes = m_timer / 60;
	int seconds = m_timer - minutes * 60;
	wchar_t output[256];
	swprintf_s(output, L"あと%d分%d秒だしん！\n", minutes, seconds);
	m_font.DrawScreenPos(output, CVector2(55.0f, 335.0f), CVector4::White(), CVector2(0.8f, 0.8f));
}
