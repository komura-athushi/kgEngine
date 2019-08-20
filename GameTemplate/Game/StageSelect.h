#pragma once
#include "GameData.h"
class Fade;
class GameData;
class StageSelect:public IGameObject
{
public:
	StageSelect();
	~StageSelect();
	bool Start() override;
	void Update() override;
	void PostRender() override;
private:
	CSprite m_sprite;
	CSprite m_arrow;
	CVector2 m_arrowPosition = { 200.0f,300.0f };
	GameData* m_gameData;
	EnStageNumber m_stageNumber = enState_FirstStage;
	Fade* m_fade;
	bool m_isWaitFadeout = false;
};

