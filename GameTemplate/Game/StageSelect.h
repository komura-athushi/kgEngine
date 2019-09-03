#pragma once
#include "GameData.h"
class Fade;
class GameData;
//ステージセレクト
class StageSelect:public IGameObject
{
public:
	StageSelect();
	~StageSelect();
	bool Start() override;
	void Update() override;
	void PostRender() override;
private:
	CSprite m_sprite;										//スプライト
	CSprite m_arrow;										//矢印
	CVector2 m_arrowPosition = { 200.0f,300.0f };			//矢印の座標
	GameData* m_gameData;									//ゲームデータ
	EnStageNumber m_stageNumber = enState_FirstStage;		//ステージの番号
	Fade* m_fade;	
	bool m_isWaitFadeout = false;
};

