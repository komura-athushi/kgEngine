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
}

bool StageSelect::Start()
{
	//黒
	//m_sprite.Init(L"Resource/sprite/stageselect.dds", false);
	//m_arrow.Init(L"Resource/sprite/arrow.dds", false);
	//ゲームデータからステージの番号を取得
	m_gameData = &GetGameData();
	m_stageNumber = m_gameData->GetStageNumber();
	m_player.Init(L"Resource/modelData/zunko.cmo");
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
			m_stageSelectGround.SetPosition(objdata.position);
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
	//cameraTarget.z -= 15.0f;
	MainCamera().SetPosition({ cameraTarget.x,cameraTarget.y + 80.0f,cameraTarget.x });
	MainCamera().SetTarget(cameraTarget);
	MainCamera().Update();
	m_backSprite.Init(L"Resource/sprite/space.dds");

	ObjectData::GetInstance().SaveData();
	GetGameData().SaveDataStageClear();
	return true;
}

void StageSelect::PrePostRender()
{
	m_backSprite.Draw();
}

void StageSelect::Update()
{

	//Aボタンが押されたら決定したステージの番号を設定する
	if (Engine().GetPad(0).IsTrigger(enButtonA)) {
		if (m_stagePoint != nullptr) {
			m_gameData->SetStageNumber(EnStageNumber(m_stagePoint->GetNumber()));
			NewGO<Game>(0);
			DeleteGO(this);
		}
		else if (m_isCollection) {
			NewGO<Collection>(0);
			DeleteGO(this);
		}
	
	
	}

	//B押したらタイトルに戻る
	if (Engine().GetPad(0).IsTrigger(enButtonB)) {
		NewGO<Title>(0);
		DeleteGO(this);
	}
	TurnPlayer();
	DistanceStagePoint();
}

void StageSelect::TurnPlayer()
{
	CVector3 moveSpeed = m_stageSelectGround.GetMoveSpeed();
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