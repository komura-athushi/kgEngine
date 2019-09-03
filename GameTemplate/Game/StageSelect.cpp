#include "stdafx.h"
#include "StageSelect.h"
#include "Fade.h"
#include "Game.h"
StageSelect::StageSelect()
{

}

StageSelect::~StageSelect()
{

}

bool StageSelect::Start()
{
	//黒
	m_sprite.Init(L"Resource/sprite/stageselect.dds", false);
	m_arrow.Init(L"Resource/sprite/arrow.dds", false);
	//ゲームデータからステージの番号を取得
	m_gameData = &GetGameData();
	m_stageNumber = m_gameData->GetStageNumber();
	return true;
}

void StageSelect::Update()
{
	//矢印の移動するx座標
	const float PlusPosition = 250.0f;
	//右ボタンが押されたら矢印を右に移動させる
	if (Engine().GetPad(0).IsTrigger(enButtonRight) && m_stageNumber != enState_LastStage) {
		m_stageNumber = EnStageNumber(m_stageNumber + 1);
		m_arrowPosition.x += PlusPosition;
	}
	//左ボタンが押されたら矢印を左に移動させる
	else if (Engine().GetPad(0).IsTrigger(enButtonLeft) && m_stageNumber != enState_FirstStage) {
		m_stageNumber = EnStageNumber(m_stageNumber - 1);
		m_arrowPosition.x -= PlusPosition;
	}
	//Aボタンが押されたら決定したステージの番号を設定する
	if (Engine().GetPad(0).IsTrigger(enButtonA)) {
		m_gameData->SetStageNumber(m_stageNumber);
		NewGO<Game>(0);
		DeleteGO(this);
	}

}

void StageSelect::PostRender()
{
	m_sprite.DrawScreenPos();
	m_arrow.DrawScreenPos(m_arrowPosition);
}