#pragma once
#include "level/Level.h"
#include "GameData.h"
#include "StageSelectGround.h"
#include "sound/SoundSource.h"
class Fade;
class GameData;
class StagePoint;
class CollectionBook;
//�X�e�[�W�Z���N�g
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
	CSprite m_sprite;										//�X�v���C�g
	CSprite m_arrow;										//���
	CVector2 m_arrowPosition = { 200.0f,300.0f };			//���̍��W
	GameData* m_gameData;									//�Q�[���f�[�^
	EnStageNumber m_stageNumber = enState_FirstStage;		//�X�e�[�W�̔ԍ�
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

