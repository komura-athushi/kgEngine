#pragma once
#include "GameData.h"
class Fade;
class GameData;
//�X�e�[�W�Z���N�g
class StageSelect:public IGameObject
{
public:
	StageSelect();
	~StageSelect();
	bool Start() override;
	void Update() override;
	void PostRender() override;
private:
	CSprite m_sprite;										//�X�v���C�g
	CSprite m_arrow;										//���
	CVector2 m_arrowPosition = { 200.0f,300.0f };			//���̍��W
	GameData* m_gameData;									//�Q�[���f�[�^
	EnStageNumber m_stageNumber = enState_FirstStage;		//�X�e�[�W�̔ԍ�
	Fade* m_fade;	
	bool m_isWaitFadeout = false;
};

