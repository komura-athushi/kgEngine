#pragma once
#include "level/Level.h"
#include "GameData.h"

class StageSelectGround;
class Fade;
class GameData;
class StagePoint;
class CollectionBook;
class Battle;
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
private:
	//�v���C���[�̉�]����
	void TurnPlayer();
	//�ǂ̃X�e�[�W�ɑJ�ڂ��邩���߂�
	void DistanceStagePoint();
private:
	CSprite m_sprite;											//�X�v���C�g
	CSprite m_arrow;											//���
	CVector2 m_arrowPosition = { 200.0f,300.0f };				//���̍��W
	GameData* m_gameData;										//�Q�[���f�[�^
	EnStageNumber m_stageNumber = enState_FirstStage;			//�X�e�[�W�̔ԍ�
	Fade* m_fade;												//�t�F�[�h
	bool m_isWaitFadeout = false;								//�t�F�[�h�A�E�g�����H
	CSkinModelRender* m_player = nullptr;						//�v���C���[�̃X�L�����f�������_�[
	Level m_level;												//���x��
	StageSelectGround* m_stageSelectGround = nullptr;			//������
	std::unordered_map<int, StagePoint*> m_stagePointList;		//�X�e�[�W�|�C���g�̃}�b�v
	StagePoint* m_stagePoint = nullptr;							//�v���C���[�Ƌ߂��X�e�[�W�|�C���g
	CFont m_font;												//����
	CollectionBook* m_collectionBook;							//�{
	Battle* m_battle;											//�o�g������H
	bool m_isCollection = false;								//�}�Ӊ�ʂɑJ�ځH
	CSprite m_backSprite;										//�q�[�̉摜
	bool m_isTransTitle = false;								//�^�C�g���ɑJ�ڂ���H
	bool m_isBattle = false;									//�o�g������H
};

