#include "stdafx.h"
#include "StageSelect.h"
#include "Fade.h"
#include "Game.h"
#include "Title.h"
#include "StageSelectGround.h"
#include "Collection.h"
#include "Object/ObjectData.h"
#include "GameData.h"
#include "SoundDirector.h"
#include "sound/SoundSource.h"
#include "StageSelectGround.h"
#include "StageSelectPoint.h"

StageSelect::StageSelect()
{

}

StageSelect::~StageSelect()
{

}

void StageSelect::OnDestroy()
{
	for (auto itr : m_stageSelectPointList) {
		DeleteGO(itr);
	}
	DeleteGO(m_stageSelectGround);
	DeleteGO(m_player);

}

bool StageSelect::Start()
{
	//ゲームデータからステージの番号を取得
	m_gameData = &GetGameData();
	m_stageNumber = m_gameData->GetStageNumber();
	//レベルを読み込んで配置する
	m_level.Init(L"Assets/level/stageselect.tkl", [&](LevelObjectData& objdata) {
		if (objdata.ForwardMatchName(L"point")) {
			int number = _wtoi(&objdata.name[5]);
			if (number != 1 && !m_gameData->GetisStageClear(EnStageNumber(number - 1))) {
				return true;
			}
			StageSelectPoint* stageSelectPoint = NewGO<StageSelectPoint>(1);
			stageSelectPoint->SetPosition(objdata.position);
			stageSelectPoint->SetRotation(objdata.rotation);
			stageSelectPoint->SetScale(objdata.scale);
			stageSelectPoint->SetPoint(enPoint(number));
			m_stageSelectPointList.push_back(stageSelectPoint);
			return true;
		}
		else if (objdata.EqualObjectName(L"earth")) {
			m_stageSelectGround = NewGO<StageSelectGround>(0);
			m_stageSelectGround->SetPosition(objdata.position);
			return true;
		}
		else if (objdata.EqualObjectName(L"book")) {
			StageSelectPoint* stageSelectPoint = NewGO<StageSelectPoint>(1);
			stageSelectPoint->SetPosition(objdata.position);
			stageSelectPoint->SetRotation(objdata.rotation);
			stageSelectPoint->SetScale(objdata.scale);
			stageSelectPoint->SetPoint(enPoint_Collection);
			m_stageSelectPointList.push_back(stageSelectPoint);
			return true;
		}
		else if (objdata.EqualObjectName(L"battle")) {
			StageSelectPoint* stageSelectPoint = NewGO<StageSelectPoint>(1);
			stageSelectPoint->SetPosition(objdata.position);
			stageSelectPoint->SetRotation(objdata.rotation);
			stageSelectPoint->SetScale(objdata.scale);
			stageSelectPoint->SetPoint(enPoint_Battle);
			m_stageSelectPointList.push_back(stageSelectPoint);
			return true;
		}
		else if (objdata.EqualObjectName(L"zunko")) {
			m_player = NewGO<CSkinModelRender>(0);
			m_player->Init(L"Resource/modelData/zunko.cmo");
			m_player->SetPosition(objdata.position);
			m_player->SetScale(CVector3::One());
			return true;
		}
		return true;

	});
	CVector3 cameraTarget = m_player->GetPosition();
	MainCamera().SetPosition({ cameraTarget.x,cameraTarget.y + 80.0f,cameraTarget.x });
	MainCamera().SetTarget(cameraTarget);
	MainCamera().Update();
	m_backSprite.Init(L"Resource/sprite/space.dds");

	if (!m_gameData->GetisBattle()) {
		//セーブする
		ObjectData::GetInstance().SaveData();
		GetGameData().SaveDataStageClear();
	}
	SoundData().SetBGM(enBGM_StageSelect);

	m_fade = &Fade::GetInstance();
	m_fade->StartFadeIn();
	return true;
}

void StageSelect::PrePostRender()
{
	m_backSprite.Draw();
}

void StageSelect::Update()
{

	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			if (m_isTransTitle) {
				NewGO<Title>(0);
				DeleteGO(this);
			}
			else {
				switch (m_stageSelectPoint->GetPoint()) {
				case enPoint_Stage1:
				case enPoint_Stage2:
					m_gameData->SetStageNumber(EnStageNumber(m_stageSelectPoint->GetPoint()));
					m_gameData->SetisBattle(false);
					NewGO<Game>(0);
					break;
				case enPoint_Collection:
					m_gameData->SetisBattle(false);
					NewGO<Collection>(0);
					break;
				case enPoint_Battle:
					m_gameData->SetisBattle(true);
					NewGO<Game>(0);
					break;
				}
				DeleteGO(this);
			}
		}

	}
	else {

		//Aボタンが押されたら決定したステージの番号を設定する、または図鑑画面に遷移する
		if (Engine().GetPad(0).IsTrigger(enButtonA)) {
			if (m_stageSelectPoint != nullptr) {
				CSoundSource* se = new CSoundSource();
				se->Init(L"Assets/sound/kettei.wav");
				se->Play(false);
				m_isWaitFadeout = true;
				m_fade->StartFadeOut();
			}
		}
		//B押したらタイトルに戻る
		else if (Engine().GetPad(0).IsTrigger(enButtonB)) {
			CSoundSource* se = new CSoundSource();
			se->Init(L"Assets/sound/kettei.wav");
			se->Play(false);
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
			m_isTransTitle = true;
		}
		TurnPlayer();
		DistanceStagePoint();
	
	}
	

	//プレイヤーの座標を参照してシャドウマップのカメラを設定する
	Engine().GetGraphicsEngine().SetLightCameraPosition(CVector3(m_player->GetPosition().x + 300.0f, m_player->GetPosition().y + 300.0f, m_player->GetPosition().z + 300.0f));
	Engine().GetGraphicsEngine().SetLightCameraTarget(m_player->GetPosition());
}

void StageSelect::TurnPlayer()
{
	m_player->SetPosition(m_player->GetPosition());
	//ちきうの回転方向と反対の方向にモデルを回転させる
	CVector3 moveSpeed = m_stageSelectGround->GetMoveSpeed();
	moveSpeed.x = -moveSpeed.x;
	moveSpeed.z = -moveSpeed.z;
	if (moveSpeed.LengthSq() <= 0.01f) {
		CQuaternion rot;
		rot.SetRotation(CVector3::AxisY(), atan2f(0.0f, 1.0f));
		m_player->SetRotation(rot);
		return;
	}
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(moveSpeed.x, moveSpeed.z));
	m_player->SetRotation(rot);
}

void StageSelect::DistanceStagePoint()
{
	const float distance = 10.0f * 10.0f;

	m_stageSelectPoint = nullptr;
	//各ポイントとプレイヤーとの距離を調べる
	for (auto itr : m_stageSelectPointList) {
		CVector3 diff = m_player->GetPosition() - itr->GetPosition();
		if (diff.LengthSq() <= distance) {
			m_stageSelectPoint = itr;
			break;
		}
	}
}

void StageSelect::PostRender()
{
	if (m_stageSelectPoint == nullptr) {
		return;
	}

	//文字表示すっぺ
	wchar_t hoge[256];
	switch (m_stageSelectPoint->GetPoint())
	{
	case enPoint_Stage1:
	case enPoint_Stage2:
		swprintf_s(hoge, L"ステージ%d", m_stageSelectPoint->GetPoint());
		break;
	case enPoint_Collection:
		swprintf_s(hoge, L"コレクション");
		break;
	case enPoint_Battle:
		swprintf_s(hoge, L"タイセン");
		break;
	}

	m_font.DrawScreenPos(hoge, { 550.0f,100.0f });
}