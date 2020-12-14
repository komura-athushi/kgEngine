#include "stdafx.h"
#include "Result.h"
#include "GameData.h"
#include "Player.h"
#include "GameCamera.h"
#include "Object/Obj.h"
#include "StageSelect.h"
#include "sound/SoundSource.h"
#include "SoundDirector.h"
#include "sound\SoundSource.h"
#include "Fade.h"
#include "Ranking.h"

namespace {
	const float time = 2.0f;
	const float multiply = 2.0f;
	const float disance = 15.0f;
	const float time2 = 1.0f;
	const float time3 = 0.3f;
	const float SeVolume = 3.0f;
}

Result::Result()
{

}

Result::~Result()
{

}

void Result::OnDestroy()
{
	DeleteGO(m_player);
	DeleteGO(m_gameCamera);
	QueryGOs<Obj>(nullptr, [&](Obj* object) {
		DeleteGO(object);
		return true;
    });
	GetObjModelDataFactory().DeleteAllData();
	

}

bool Result::Start()
{
	m_sprite.Init(L"Resource/sprite/space.dds", false);
	m_gameData = &GetGameData();
	m_isBattle = m_gameData->GetisBattle();
	m_gameData->SetScene(enScene_Result);
	m_player = FindGO<Player>();
	m_player->SetPosition(CVector3::Zero());
	m_gameCamera = FindGO<GameCamera>();
	m_gameCamera->SetPosition({ 0.0f,0.0f ,150.0f });
	m_gameCamera->SetTarget(CVector3::Zero());

	SoundData().SetBGM(enBGM_Result);

	m_gameOver.Init(L"Resource/sprite/gameover.dds");
	m_stageClear.Init(L"Resource/sprite/stageclear.dds");
	m_winner1.Init(L"Resource/sprite/winner1.dds");
	m_winner2.Init(L"Resource/sprite/winner2.dds");
	m_draw.Init(L"Resource/sprite/draw.dds");

	m_fade = &Fade::GetInstance();
	m_fade->StartFadeIn();

	if (!m_isBattle) {
		m_ranking = &Ranking::GetInstance();
		m_ranking->Load();
		m_ranking->Sort();
		m_ranking->Save();
	}

	Engine().GetGraphicsEngine().SetisSplit(false);
	return true;
}

void Result::Update()
{
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			NewGO<StageSelect>(0);
			DeleteGO(this);
		}
	}
	else {
		//シーンの状態によって処理を分ける
		switch (m_resultScene)
		{
		case EnResultScene_FadeIn:
			if (m_fade->IsFade()) {
				return;
			}
			else {
				CSoundSource* se = new CSoundSource();
				se->Init(L"Assets/sound/drum-roll.wav");
				se->Play(false);
				m_resultScene = EnResultScene_MovePlayer;
			}
			break;
		case EnResultScene_MovePlayer:
			MovePlayer();
			break;
		case EnResultScene_MoveGoal:
			MoveGoal();
			break;
		case EnResultScene_MoveResult:
			MoveResult();
			break;
		case EnResultScene_TransScene:
			TransScene();
			break;
		}
	}

	
}

void Result::MovePlayer()
{
	//プレイヤー動かす
	if (m_timer <= time) {
		m_timer += GameTime().GetFrameDeltaTime();
		CVector3 position = m_player->GetCharaconPosition();
		position.x += disance * GameTime().GetFrameDeltaTime() * multiply;
		m_player->SetPosition(position);
	}
	else {
		m_resultScene = EnResultScene_MoveGoal;
		m_timer = 0.0f;
		CSoundSource* se = new CSoundSource();
		se->Init(L"Assets/sound/don.wav");
		se->Play(false);
		se->SetVolume(7.0f);
	}
}

void Result::MoveGoal()
{
	
	if (m_timer <= time2) {
		m_timer += GameTime().GetFrameDeltaTime();
	}
	else {
		m_resultScene = EnResultScene_MoveResult;
		CSoundSource* se = new CSoundSource();
		se->Init(L"Assets/sound/ddon.wav");
		se->Play(false);
		se->SetVolume(7.0f);
	}
}

void Result::MoveResult()
{

	m_timer2 += GameTime().GetFrameDeltaTime();
	if (m_timer2 >= time3) {
		if (m_isBattle) {
			m_se.InitStreaming(L"Assets/sound/clear.wav");
		}
		else {
			if (m_gameData->GetisGameClear()) {
				m_se.InitStreaming(L"Assets/sound/clear.wav");
			}
			else {
				m_se.InitStreaming(L"Assets/sound/gameover.wav");
			}
		}
		m_se.Play(false);
		m_se.SetVolume(SeVolume);
		m_resultScene = EnResultScene_TransScene;
	}
	
}

void Result::TransScene()
{
	//Aボタン押したらステージセレクトに遷移
	/*if (Engine().GetPad(0).IsTrigger(enButtonA)) {
		CSoundSource* se = new CSoundSource();
		se->Init(L"Assets/sound/kettei.wav");
		se->Play(false);
		if (m_gameData->GetisGameClear()) {
			m_gameData->SetStageClear();
		}

		m_fade->StartFadeOut();
		m_isWaitFadeout = true;
	}*/
}

void Result::PrePostRender()
{
	m_sprite.DrawScreenPos();
}

void Result::PostRender()
{
	
	const int rankingNumber = 3;
	CVector2 pos = CVector2(1000.0f, 400.0f);
	switch(m_resultScene) {
	case EnResultScene_TransScene:
		if (m_isWaitFadeout) {
			if (!m_fade->IsFade()) {
				NewGO<StageSelect>(0);
				DeleteGO(this);
			}
		}
		else if (!m_isWaitFadeout) {
			if (!m_se.IsPlaying()) {
				if (m_gameData->GetisGameClear() && !m_gameData->GetisBattle()) {
					m_gameData->SetStageClear();
				}

				m_fade->StartFadeOut();
				m_isWaitFadeout = true;
			}
		}
	case EnResultScene_MoveResult:
		if (m_isBattle) {
			int hoge = m_gameData->GetWinner();
			if (hoge == 0) {
				m_winner1.Draw();
			}
			else if (hoge == 1) {
				m_winner2.Draw();
			}
			else {
				m_draw.Draw();
			}
		}
		else {
			if (m_gameData->GetisGameClear()) {
				m_stageClear.Draw();
			}
			else {
				m_gameOver.Draw();
			}
		}
		if (!m_isBattle) {
			for (int i = 0; i < Ranking::m_rankingSize; i++) {
				wchar_t output[256];
				swprintf_s(output, L"%d", i + 1);
				m_font.DrawScreenPos(output, pos, CVector4::White(), { 1.2f,1.2f });
				m_font.DrawScreenPos(L"イ", CVector2(pos.x + 37, pos.y + 20), CVector4::White(), { 0.7f,0.7f });

				CVector4 color;
				if (m_ranking->GetThisRanking() == i + 1) {
					color = CVector4::Red();
				}
				else {
					color = CVector4::White();
				}
				CVector2 pos2 = pos;
				pos2.x += 100.0f;
				wchar_t output2[256];
				swprintf_s(output2, L"%d", m_ranking->GetRanking(i + 1));
				m_font.DrawScreenPos(output2, pos2, color, { 1.2f,1.2f });
				m_font.DrawScreenPos(L"cm", CVector2(pos2.x + 67, pos2.y + 20), color, { 0.7f,0.7f });

				pos.y += 60.0f;
			}
		}
	case EnResultScene_MoveGoal:
		if (!m_isBattle) {
			wchar_t output[256];
			swprintf_s(output, L"ケッカ    %d", m_gameData->GetResultPlayerSize());
			m_font.DrawScreenPos(output, CVector2(700.0f, 300.0f), CVector4::White(), { 1.5f,1.5f });
			m_font.DrawScreenPos(L"cm\n", CVector2(963.0f, 325.0f), CVector4::White(), { 0.8f,0.8f });
		}
		else {
			CVector2 pos = CVector2(200.0f, 300.0f);
			CVector2 plusPos = CVector2(190.0f, 25.0f);
			wchar_t output[256];
			swprintf_s(output, L"1P  %d", m_gameData->GetResultPlayerSize(0));
			m_font.DrawScreenPos(output, pos, CVector4::Red(), { 1.5f,1.5f });
			m_font.DrawScreenPos(L"cm\n", CVector2(pos.x + plusPos.x, pos.y + plusPos.y), CVector4::Red(), { 0.8f,0.8f });

			pos.x += 700.0f;

			wchar_t output2[256];
			swprintf_s(output2, L"2P  %d", m_gameData->GetResultPlayerSize(1));
			m_font.DrawScreenPos(output2, pos, CVector4::Blue(), { 1.5f,1.5f });
			m_font.DrawScreenPos(L"cm\n", CVector2(pos.x + plusPos.x, pos.y + plusPos.y), CVector4::Blue(), { 0.8f,0.8f });
		}
	}
}