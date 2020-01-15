#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Ground.h"
#include "GameCamera.h"
#include "Object/Obj.h"
#include "Object/Move/IMove.h"
#include "Object/Rotation/IRot.h"
#include "Object/ObjectData.h"
#include "Moji.h"
#include "Time.h"
#include "Fade.h"
#include "GameData.h"
#include "Result.h"
#include "Title.h"
#include "OffScreen.h"
#include "SoundDirector.h"
#include "sound/SoundSource.h"


Game::Game()
{

}

Game::~Game()
{
	
}

void Game::OnDestroy()
{
	DeleteGO(m_ground);
	DeleteGO(m_time);
	for (Obj* obj : m_objList) {
		if (!obj->GetisHitPlayer()) {
			obj->SetNoDraw();
		}
	}
	DeleteGO(m_offScreen);
	
}

bool Game::Start()
{
	m_gameData = &GetGameData();
	wchar_t filePath[256];
	//ステージの番号によって読み込むレベルファイルを設定する
	swprintf_s(filePath, L"Assets/level/level0%d.tkl", int(m_gameData->GetStageNumber() ));
	m_gamecamera = NewGO<GameCamera>(0);
	//レベルを読み込む
	m_level.Init(filePath, [&](LevelObjectData& objdata) {
		if (objdata.ForwardMatchName(L"o")) {
			for (int i = 0; i < ObjectData::GetInstance().GetListSize(); i++ ) {
				if (objdata.ForwardMatchName(ObjectData::GetInstance().GetObjectData(i)->s_name)) {
					Obj* obj = NewGO<Obj>(1);
					obj->SetObjData(ObjectData::GetInstance().GetObjectData(i));
					MOVESTATUS ms = FindMove(objdata.name);
					if (ms.s_state == enMove_Path) {
						obj->ReadMovePath(ms.s_pathnumber);
					}
					obj->InitMove(ms.s_state, objdata.position, ms.s_move, ms.s_limit, objdata.rotation);
					ROTSTATUS rs = FindRot(objdata.name);
					obj->InitRot(rs.s_state, rs.s_speed);
					obj->SetFilePath(ObjectData::GetInstance().GetObjectData(i)->s_name);
					m_objList.push_back(obj);
					return true;
				}
			}
			return true;
		}
		else if (objdata.ForwardMatchName(L"sphere")) {
			m_player = NewGO<Player>(0);
			m_player->SetPosition(objdata.position);
			return true;
		}
		else if (objdata.ForwardMatchName(L"ground")) {
			m_ground = NewGO<Ground>(0);
			m_ground->SetPosition(objdata.position);
			m_ground->SetRotation(objdata.rotation);
			return true;
		}
		return true;
	});
	m_time = NewGO<Time>(0);
	m_time->SetTime(m_gameData->GetStageLimitTime());
	GetObjModelDataFactory().InitInstancingData();
	m_gameData->SetPoseCancel();
	m_gameData->SetScene(enScene_Stage);
	m_offScreen = NewGO<OffScreen>(3);
	//ステージによって再生するBGMを再生する
	switch (m_gameData->GetStageNumber())
	{
	case enState_Stage1:
		SoundData().SetBGM(enBGM_Stage1);
		break;
	case enState_Stage2:
		SoundData().SetBGM(enBGM_Stage2);
		break;
	case enState_Stage3:
		SoundData().SetBGM(enBGM_Stage3);
		break;
	default:
		break;
	}

	m_pause.Init(L"Resource/sprite/pause.dds");
	m_end.Init(L"Resource/sprite/end.dds");

	return true;
}

void Game::Update()
{	
	const float Time = 2.0f;

	//ステージが終了したら
	if (m_owaOwari) {
		SoundData().SetStopBGM();
		m_gameData->SetPose();
		m_timer2 += GameTime().GetFrameDeltaTime();
		m_gameData->SetReusltPlayerSsize(m_player->GetRadius());
		if (m_timer2 >= Time) {
			NewGO<Result>(0);
			DeleteGO(this);
		}
	}
	else {
		//スタートボタンが押されたらポーズする、もっかい押したら解除
		if (Engine().GetPad(0).IsTrigger(enButtonStart)) {
			if (m_gameData->GetisPose()) {
				m_gameData->SetPoseCancel();
				SoundData().SetPlayBGM();
			}
			else {
				m_gameData->SetPose();
				SoundData().SetStopBGM();
			}
		}
	}
}

void Game::PostRender()
{

	if (m_owaOwari) {
		m_end.Draw();
	}
	else if (m_gameData->GetisPose()) {
		m_pause.Draw();
	}
}