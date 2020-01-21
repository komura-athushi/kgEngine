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
	m_gameData->SetScene(enScene_Result);
	m_player = FindGO<Player>();
	m_player->SetPosition(CVector3::Zero());
	m_gameCamera = FindGO<GameCamera>();
	m_gameCamera->SetPosition({ 0.0f,0.0f ,150.0f });
	m_gameCamera->SetTarget(CVector3::Zero());

	SoundData().SetBGM(enBGM_Result);

	m_gameOver.Init(L"Resource/sprite/gameover.dds");
	m_stageClear.Init(L"Resource/sprite/stageclear.dds");

	m_fade = &Fade::GetInstance();
	m_fade->StartFadeIn();

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
	const float Time = 2.0f;
	const float Multiply = 2.0f;
	const float Disance = 15.0f;

	//プレイヤー動かす
	if (m_timer <= Time) {
		m_timer += GameTime().GetFrameDeltaTime();
		CVector3 position = m_player->GetCharaconPosition();
		position.x += Disance * GameTime().GetFrameDeltaTime() * Multiply;
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
	const float Time = 1.0f;
	if (m_timer <= Time) {
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
	const float Time2 = 0.3f;
	const float SeVolume = 3.0f;

	m_timer2 += GameTime().GetFrameDeltaTime();
	if (m_timer2 >= Time2) {
		if (m_gameData->GetisGameClear()) {
			m_se.InitStreaming(L"Assets/sound/clear.wav");
		}
		else {
			m_se.InitStreaming(L"Assets/sound/gameover.wav");
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
	if (!m_se.IsPlaying()) {
		if (m_gameData->GetisGameClear()) {
			m_gameData->SetStageClear();
		}

		m_fade->StartFadeOut();
		m_isWaitFadeout = true;
	}
}

void Result::PrePostRender()
{
	m_sprite.DrawScreenPos();
}

void Result::PostRender()
{
	switch(m_resultScene) {
	case EnResultScene_TransScene:
	case EnResultScene_MoveResult:
		if (m_gameData->GetisGameClear()) {
			m_stageClear.Draw();
		}
		else {
			m_gameOver.Draw();
		}
	case EnResultScene_MoveGoal:
		wchar_t output[256];
		swprintf_s(output, L"ケッカ    %.f", m_gameData->GetResultPlayerSize());
		m_font.DrawScreenPos(output, CVector2(700.0f, 300.0f), CVector4::White(), { 1.5f,1.5f });
		m_font.DrawScreenPos(L"cm\n", CVector2(963.0f, 325.0f), CVector4::White(), { 0.8f,0.8f });
	}
}