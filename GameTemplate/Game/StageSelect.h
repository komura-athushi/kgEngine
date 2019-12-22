#pragma once
#include "level/Level.h"
#include "GameData.h"
#include "StageSelectGround.h"
#include "sound/SoundSource.h"
class Fade;
class GameData;
class StagePoint;
class CollectionBook;
//ステージセレクト
class StageSelect:public IGameObject
{
public:
	StageSelect();
	~StageSelect();
	void OnDestroy() override;
	bool Start() override;
	void PrePostRender() override;
	void Update() override;
	void PostRender() override;
	void TurnPlayer();
	void DistanceStagePoint();
private:
	CSprite m_sprite;										//スプライト
	CSprite m_arrow;										//矢印
	CVector2 m_arrowPosition = { 200.0f,300.0f };			//矢印の座標
	GameData* m_gameData;									//ゲームデータ
	EnStageNumber m_stageNumber = enState_FirstStage;		//ステージの番号
	Fade* m_fade;	
	bool m_isWaitFadeout = false;
	CSkinModelRender m_player;
	Level m_level;
	StageSelectGround m_stageSelectGround;
	std::unordered_map<int, StagePoint*> m_stagePointList;
	StagePoint* m_stagePoint = nullptr;
	CFont m_font;
	CollectionBook* m_collectionBook;
	bool m_isCollection = false;
	CSprite m_backSprite;
	CSoundSource m_bgm;
};

