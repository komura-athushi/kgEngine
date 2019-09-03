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
	//��
	m_sprite.Init(L"Resource/sprite/stageselect.dds", false);
	m_arrow.Init(L"Resource/sprite/arrow.dds", false);
	//�Q�[���f�[�^����X�e�[�W�̔ԍ����擾
	m_gameData = &GetGameData();
	m_stageNumber = m_gameData->GetStageNumber();
	return true;
}

void StageSelect::Update()
{
	//���̈ړ�����x���W
	const float PlusPosition = 250.0f;
	//�E�{�^���������ꂽ������E�Ɉړ�������
	if (Engine().GetPad(0).IsTrigger(enButtonRight) && m_stageNumber != enState_LastStage) {
		m_stageNumber = EnStageNumber(m_stageNumber + 1);
		m_arrowPosition.x += PlusPosition;
	}
	//���{�^���������ꂽ��������Ɉړ�������
	else if (Engine().GetPad(0).IsTrigger(enButtonLeft) && m_stageNumber != enState_FirstStage) {
		m_stageNumber = EnStageNumber(m_stageNumber - 1);
		m_arrowPosition.x -= PlusPosition;
	}
	//A�{�^���������ꂽ�猈�肵���X�e�[�W�̔ԍ���ݒ肷��
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