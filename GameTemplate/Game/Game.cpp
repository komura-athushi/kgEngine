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
#include "Fade.h"

namespace {
	const float time = 2.0f;
	const CVector2 fontPosition = CVector2(600.0f, 320.0f);
}

Game::Game()
{

}

Game::~Game()
{
	
}

void Game::OnDestroy()
{
	//色々削除
	DeleteGO(m_ground);
	DeleteGO(m_time);
	for (Obj* obj : m_objList) {
		if (m_isBattle) {
			obj->SetNoDraw();
		}
		else {
			if (!obj->GetisHitPlayer()) {
				obj->SetNoDraw();
			}
		}
	}
	DeleteGO(m_offScreen);
	if (m_isBattle) {
		m_player[0]->GetCSkinModelRender().SetActive(false);
		DeleteGO(m_player[1]);
		DeleteGO(m_gameCamera[1]);
	}

}

void Game::InitLevel()
{
	wchar_t filePath[256];
	m_gameCamera[0] = NewGO<GameCamera>(0);
	m_gameCamera[0]->SetPlayerNumber(0);
	if (m_isBattle) {
		m_gameCamera[1] = NewGO<GameCamera>(0);
		m_gameCamera[1]->SetPlayerNumber(1);
		//ステージの番号によって読み込むレベルファイルを設定する
		swprintf_s(filePath, L"Assets/level/level01.tkl");
	}
	else {
		//ステージの番号によって読み込むレベルファイルを設定する
		swprintf_s(filePath, L"Assets/level/level0%d.tkl", int(m_gameData->GetStageNumber()));
	}

	//レベルを読み込む
	m_level.Init(filePath, [&](LevelObjectData& objdata) {
		if (objdata.ForwardMatchName(L"o")) {
			for (int i = 0; i < GetObjectData().GetListSize(); i++) {
				if (objdata.ForwardMatchName(GetObjectData().GetObjectData(i)->s_name)) {
					Obj* obj = NewGO<Obj>(1);
					obj->SetObjData(GetObjectData().GetObjectData(i));
					MoveStatus ms = FindMove(objdata.name);
					if (ms.s_state == enMove_Path) {
						obj->ReadMovePath(ms.s_pathnumber);
					}
					obj->InitMove(ms.s_state, objdata.position, ms.s_move, ms.s_limit, objdata.rotation);
					RotStatus rs = FindRot(objdata.name);
					obj->InitRot(rs.s_state, rs.s_speed);
					obj->SetFilePath(GetObjectData().GetObjectData(i)->s_name);
					m_objList.push_back(obj);
					return true;
				}
			}
			return true;
		}
		else if (objdata.ForwardMatchName(L"sphere")) {
			if (m_gameData->GetisBattle() && m_player[0] != nullptr) {
				m_player[1] = NewGO<Player>(0);
				m_player[1]->SetFirstPosition(objdata.position);
				m_player[1]->SetPlayerNumber(1);
			}
			else if (m_player[0] == nullptr) {
				m_player[0] = NewGO<Player>(0);
				m_player[0]->SetFirstPosition(objdata.position);
				m_player[0]->SetPlayerNumber(0);
			}

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
}

bool Game::Start()
{
	m_gameData = &GetGameData();
	m_isBattle = m_gameData->GetisBattle();

	//レベルデータを読み込む
	InitLevel();
	
	m_time = NewGO<Time>(0);
	if (m_isBattle) {
		m_time->SetTime(m_gameData->GetBattleLimitTime());
	}
	else {
		m_time->SetTime(m_gameData->GetStageLimitTime());
	}
	GetObjModelDataFactory().InitInstancingData();
	m_gameData->SetPoseCancel();
	m_gameData->SetScene(enScene_Stage);
	m_offScreen = NewGO<OffScreen>(3);
	if (m_gameData->GetisBattle()) {
		SoundData().SetBGM(enBGM_Battle);
	}
	else {
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
	}

	//色んな画像読み込んで
	m_pause.Init(L"Resource/sprite/pause.dds");
	m_end.Init(L"Resource/sprite/end.dds");
	m_start.Init(L"Resource/sprite/start.dds");

	m_fade = &Fade::GetInstance();
	m_fade->StartFadeIn();

	m_player[0]->SetGameCamera(m_gameCamera[0]);
	m_gameCamera[0]->SetPlayer(m_player[0]);

	if (m_isBattle) {
		m_player[1]->SetGameCamera(m_gameCamera[1]);
		m_gameCamera[1]->SetPlayer(m_player[1]);
		Engine().GetGraphicsEngine().SetisSplit(true);

		m_player[0]->SetPlayer(m_player[1]);
		m_player[1]->SetPlayer(m_player[0]);
	}

	m_gameData->SetisStart(false);
	return true;
}
void Game::UpdateReadyGo()
{
	if (m_fade->IsIdle()) {
		m_startTime = int(m_timer3);
		m_timer3 -= GameTime().GetFrameVariableDeltaTime();
		if (m_startTime != int(m_timer3)) {
			if (m_startTime <= 1) {
				CSoundSource* se = new CSoundSource();
				se->Init(L"Assets/sound/start.wav");
				se->Play(false);
			}
			else {
				CSoundSource* se = new CSoundSource();
				se->Init(L"Assets/sound/pin.wav");
				se->Play(false);
			}
		}
		if (m_timer3 <= 0.0f) {
			m_isStart = true;
			m_gameData->SetPoseCancel();
			SoundData().SetPlayBGM();
			m_gameData->SetisStart(true);
		}

	}
	else if (!m_fade->IsFadeIn()) {
		SoundData().SetStopBGM();
		m_gameData->SetPose();
	}
}
void Game::UpdateInGame()
{

	if (m_owaOwari) {
		SoundData().SetStopBGM();
		m_gameData->SetPose();
		m_timer2 += GameTime().GetFrameDeltaTime();
		m_gameData->SetReusltPlayerSsize(m_player[0]->GetRadius());
		if (m_isBattle) {
			m_gameData->SetReusltPlayerSsize(m_player[1]->GetRadius(), 1);
		}
		if (m_timer2 >= time) {
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}
	else {
		if (!m_isBattle) {
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
}
void Game::UpdateEndGame()
{
	if (!m_fade->IsFade()) {
		NewGO<Result>(0);
		DeleteGO(this);
	}
}
void Game::Update()
{
	//ステージが終了したら
	if (m_isWaitFadeout) {
		UpdateEndGame();
	}
	else {
		if (!m_isStart) {
			//レディーゴー演出中の更新処理。
			UpdateReadyGo();
		}
		else {
			//ゲーム中の更新処理。
			UpdateInGame();
		}
	}

}

void Game::PostRender()
{
	//終了
	if (m_owaOwari) {
		m_end.Draw();
	}
	//ポーズ
	else if (m_gameData->GetisPose() && m_gameData->GetisStart()) {
		m_pause.Draw();
	}
	else if (!m_isStart) {
		if (m_startTime < 4) {
			float hoge = m_timer3 - m_startTime;
			hoge = 1.0f - hoge;
			hoge *= 1.5f;
			//スタート前の3、1、1表示
			if (m_startTime >= 1) {
				wchar_t output[10];
				swprintf_s(output, L"%d", m_startTime);
				
				m_font.DrawScreenPos(output, fontPosition, CVector4::Red(), CVector2(1.0f + hoge, 1.0f + hoge));
			}
			//START表示
			else {
				CVector2 pos;
				CVector2 scale;
				if (Engine().GetGraphicsEngine().GetisSplit()) {
					pos = CVector2(FRAME_BUFFER_W / 2, FRAME_BUFFER_H);
					scale = CVector2(0.0f + hoge, (0.0f + hoge) * 2);
				}
			
				else {
					pos = CVector2(FRAME_BUFFER_W / 2, FRAME_BUFFER_H / 2);
					scale = CVector2(0.0f + hoge, (0.0f + hoge));
				}
				m_start.DrawScreenPos(pos, scale);
			}
		}
	}
}