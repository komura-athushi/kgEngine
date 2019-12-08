#include "stdafx.h"
#include "StageSelect.h"
#include "Fade.h"
#include "Game.h"
#include "Title.h"
#include "StageSelectGround.h"
#include "StagePoint.h"

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
			StagePoint* stagePoint = NewGO<StagePoint>(0);
			stagePoint->SetPosition(objdata.position);
			stagePoint->SetRotation(objdata.rotation);
		
			stagePoint->SetNumber(number);
			SetPriorityGO(stagePoint, 2);
			m_stagePointList.emplace(number, stagePoint);
			return true;
		}
		else if (objdata.EqualObjectName(L"earth")) {
			m_stageSelectGround.SetPosition(objdata.position);
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
	return true;
}

void StageSelect::Update()
{
	//Aボタンが押されたら決定したステージの番号を設定する
	if (Engine().GetPad(0).IsTrigger(enButtonA) && m_stagePoint != nullptr) {
		m_gameData->SetStageNumber(EnStageNumber(m_stagePoint->GetNumber()));
		NewGO<Game>(0);
		DeleteGO(this);
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

void StageSelect::PostRender()
{
	m_sprite.DrawScreenPos();
	m_arrow.DrawScreenPos(m_arrowPosition);
	if (m_stagePoint != nullptr) {
		wchar_t hoge[256];
		swprintf_s(hoge, L"ステージ%d",m_stagePoint->GetNumber());
		m_font.DrawScreenPos(hoge, { 550.0f,100.0f });
	}
}