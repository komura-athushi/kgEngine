#include "stdafx.h"
#include "StageSelect.h"
#include "Fade.h"
#include "Game.h"
#include "Title.h"
#include "StageSelectGround.h"
#include "StagePoint.h"
#include "CollectionBook.h"
#include "Collection.h"
#include "Object/ObjectData.h"
#include "GameData.h"
#include "SoundDirector.h"
#include "sound/SoundSource.h"
#include "StageSelectGround.h"

StageSelect::StageSelect()
{

}

StageSelect::~StageSelect()
{

}

void StageSelect::OnDestroy()
{
	for (auto itr : m_stagePointList) {
		DeleteGO(itr.second);
	}
	DeleteGO(m_collectionBook);
	DeleteGO(m_stageSelectGround);
}

bool StageSelect::Start()
{
	//ゲームデータからステージの番号を取得
	m_gameData = &GetGameData();
	m_stageNumber = m_gameData->GetStageNumber();
	m_player.Init(L"Resource/modelData/zunko.cmo");
	//レベルを読み込んで配置する
	m_level.Init(L"Assets/level/stageselect.tkl", [&](LevelObjectData& objdata) {
		if (objdata.ForwardMatchName(L"point")) {
			int number = _wtoi(&objdata.name[5]);
			if (number != 1 && !m_gameData->GetisStageClear(EnStageNumber(number - 1))) {
				return true;
			}
			StagePoint* stagePoint = NewGO<StagePoint>(1);
			stagePoint->SetPosition(objdata.position);
			stagePoint->SetRotation(objdata.rotation);
			stagePoint->SetScale(objdata.scale);
			stagePoint->SetNumber(number);
			m_stagePointList.emplace(number, stagePoint);
			return true;
		}
		else if (objdata.EqualObjectName(L"earth")) {
			m_stageSelectGround = NewGO<StageSelectGround>(0);
			m_stageSelectGround->SetPosition(objdata.position);
			return true;
		}
		else if (objdata.EqualObjectName(L"book")) {
			m_collectionBook = NewGO<CollectionBook>(1);
			m_collectionBook->SetPosition(objdata.position);
			m_collectionBook->SetRotation(objdata.rotation);
			m_collectionBook->SetScale(objdata.scale);
			return true;
		}
		else if (objdata.EqualObjectName(L"zunko")) {
			m_player.SetPosition(objdata.position);
			return true;
		}
		return true;

	});
	CVector3 cameraTarget = m_player.GetPosition();
	MainCamera().SetPosition({ cameraTarget.x,cameraTarget.y + 80.0f,cameraTarget.x });
	MainCamera().SetTarget(cameraTarget);
	MainCamera().Update();
	m_backSprite.Init(L"Resource/sprite/space.dds");

	//セーブする
	ObjectData::GetInstance().SaveData();
	GetGameData().SaveDataStageClear();
	SoundData().SetBGM(enBGM_StageSelect);

	return true;
}

void StageSelect::PrePostRender()
{
	m_backSprite.Draw();
}

void StageSelect::Update()
{

	//Aボタンが押されたら決定したステージの番号を設定する、または図鑑画面に遷移する
	if (Engine().GetPad(0).IsTrigger(enButtonA)) {
		if (m_stagePoint != nullptr) {
			m_gameData->SetStageNumber(EnStageNumber(m_stagePoint->GetNumber()));
			NewGO<Game>(0);
			DeleteGO(this);
			CSoundSource* se = new CSoundSource();
			se->Init(L"Assets/sound/kettei.wav");
			se->Play(false);
		}
		else if (m_isCollection) {
			NewGO<Collection>(0);
			DeleteGO(this);
			CSoundSource* se = new CSoundSource();
			se->Init(L"Assets/sound/kettei.wav");
			se->Play(false);
		}
	
	
	}

	//B押したらタイトルに戻る
	if (Engine().GetPad(0).IsTrigger(enButtonB)) {
		NewGO<Title>(0);
		DeleteGO(this);
		CSoundSource* se = new CSoundSource();
		se->Init(L"Assets/sound/kettei.wav");
		se->Play(false);
	}
	TurnPlayer();
	DistanceStagePoint();

	//プレイヤーの座標を参照してシャドウマップのカメラを設定する
	Engine().GetGraphicsEngine().SetLightCameraPosition(CVector3(m_player.GetPosition().x + 300.0f, m_player.GetPosition().y + 300.0f, m_player.GetPosition().z + 300.0f));
	Engine().GetGraphicsEngine().SetLightCameraTarget(m_player.GetPosition());
}

void StageSelect::TurnPlayer()
{
	//ちきうの回転方向と反対の方向にモデルを回転させる
	CVector3 moveSpeed = m_stageSelectGround->GetMoveSpeed();
	moveSpeed.x = -moveSpeed.x;
	moveSpeed.z = -moveSpeed.z;
	if (moveSpeed.LengthSq() <= 0.01f) {
		return;
	}
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(moveSpeed.x, moveSpeed.z));
	m_player.SetRotation(rot);
}

void StageSelect::DistanceStagePoint()
{
	const float distance = 10.0f * 10.0f;

	//プレイヤーのモデルとステージポイントや本との距離を調べる
	CVector3 diff = m_player.GetPosition() - m_collectionBook->GetPosition();
	if (diff.LengthSq() <= distance) {
		m_isCollection = true;
	}
	else {
		m_isCollection = false;
		for (auto itr : m_stagePointList) {
			CVector3 diff = m_player.GetPosition() - itr.second->GetPosition();
			if (diff.LengthSq() <= distance) {
				m_stagePoint = itr.second;
				break;
			}
			else {
				m_stagePoint = nullptr;
			}
		}
	}
}

void StageSelect::PostRender()
{
	if (m_isCollection) {
		wchar_t hoge[256];
		swprintf_s(hoge, L"コレクション");
		m_font.DrawScreenPos(hoge, { 550.0f,100.0f });
	}
	else if (m_stagePoint != nullptr) {
		wchar_t hoge[256];
		swprintf_s(hoge, L"ステージ%d",m_stagePoint->GetNumber());
		m_font.DrawScreenPos(hoge, { 550.0f,100.0f });
	}
}