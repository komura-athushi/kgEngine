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
Game::Game()
{

}

Game::~Game()
{
	
}

void Game::OnDestroy()
{
	DeleteGO(m_ground);
	//DeleteGO(m_gamecamera);
	//DeleteGO(m_player);
	DeleteGO(m_time);
	/*QueryGOs<Obj>(nullptr, [&](Obj* object) {
		DeleteGO(object);
		return true;
	});*/
	for (Obj* obj : m_objList) {
		if (!obj->GetisHitPlayer()) {
			obj->SetNoDraw();
		}
		//DeleteGO(obj);
	}
	
}

bool Game::Start()
{
	m_gameData = &GetGameData();
	wchar_t filePath[256];
	swprintf_s(filePath, L"Assets/level/level0%d.tkl", int(m_gameData->GetStageNumber() ));
	m_gamecamera = NewGO<GameCamera>(0);
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
	return true;
}

void Game::Update()
{
	const float Time = 30.0f;
	if (m_owaOwari) {
		m_gameData->SetPose();
		m_timer2 += GameTime().GetFrameDeltaTime();
		m_gameData->SetReusltPlayerSsize(m_player->GetRadius());
		//if (m_timer2 >= Time) {
			NewGO<Result>(0);
			DeleteGO(this);
		//}
	}
	else {
		//スタートボタンが押されたら画面を切り替える
		if (Engine().GetPad(0).IsTrigger(enButtonStart)) {
			if (m_gameData->GetisPose()) {
				m_gameData->SetPoseCancel();
			}
			else {
				m_gameData->SetPose();
			}
		}
	}
}

void Game::PostRender()
{
	if (m_owaOwari) {
		wchar_t hoge[256];
		swprintf_s(hoge, L"終了");
		m_font.DrawScreenPos(hoge, { 300.0f,300.0f });
	}
}