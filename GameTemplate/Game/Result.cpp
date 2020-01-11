#include "stdafx.h"
#include "Result.h"
#include "GameData.h"
#include "Player.h"
#include "GameCamera.h"
#include "Object/Obj.h"
#include "StageSelect.h"
#include "sound/SoundSource.h"

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


	return true;
}

void Result::Update()
{
	switch(m_resultScene)
	{
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
	}
}

void Result::MoveGoal()
{
	m_resultScene = EnResultScene_MoveResult;
}

void Result::MoveResult()
{
	m_resultScene = EnResultScene_TransScene;
}

void Result::TransScene()
{
	//Aボタン押したらステージセレクトに遷移
	if (Engine().GetPad(0).IsTrigger(enButtonA)) {
		NewGO<StageSelect>(0);
		DeleteGO(this);
		CSoundSource* se = new CSoundSource();
		se->Init(L"Assets/sound/kettei.wav");
		se->Play(false);
		if (m_gameData->GetisGameClear()) {
			m_gameData->SetStageClear();
		}
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
		wchar_t output2[256];
		if (m_gameData->GetisGameClear()) {
			swprintf_s(output2, L"GAMECLEAR");
		}

		else {
			swprintf_s(output2, L"GAMEOVER");
		}
		m_font.DrawScreenPos(output2, CVector2(400.0f, 600.0f), CVector4::White(), { 2.3f,2.0f });
	case EnResultScene_MoveGoal:
		wchar_t output[256];
		swprintf_s(output, L"ケッカ    %.f", m_gameData->GetResultPlayerSize());
		m_font.DrawScreenPos(output, CVector2(700.0f, 300.0f), CVector4::White(), { 1.5f,1.5f });
	}
}