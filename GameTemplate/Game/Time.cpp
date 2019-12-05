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
	m_degree = (m_timer / m_time) * 180.0f;
	m_sprite1.DrawScreenPos(CVector2(1180.0f, 130.0f), CVector2(0.6f, 0.6f), CVector2(0.5f, 0.5f), 0.0f, CVector4(1.0f, 1.0f, 1.0f, 0.5f),
		DirectX::SpriteEffects_None, 0.5f);
	m_sprite2.DrawScreenPos(CVector2(1180.0f, 130.0f), CVector2(0.6f, 0.6f), CVector2(0.5f, 0.5f), 0.0f, CVector4(1.0f, 1.0f, 1.0f, 0.8f),
		DirectX::SpriteEffects_None, 0.5f, m_degree);
	int minutes = m_timer / 60;
	int seconds = m_timer - minutes * 60;
	wchar_t output[256];
	if (minutes < 1) {
		swprintf_s(output, L"あと%d秒\n", seconds);
	}
	else {
		swprintf_s(output, L"あと%d分\n", minutes);
	}
	
	m_font.DrawScreenPos(output, CVector2(1000.0f, 30.0f), CVector4::Blue(), CVector2(0.8f, 0.8f));
}
