/*****************************************************************//**
 * \file   StageSelect.h
 * \brief  StageSelect�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "level/Level.h"
#include "GameData.h"

class StageSelectGround;
class Fade;
class GameData;
class StagePoint;
class Battle;
class StageSelectPoint;

/**
 * \brief �X�e�[�W�Z���N�g�V�[�����Ǘ�����N���X.
 */
class StageSelect:public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	StageSelect();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~StageSelect();
	/**
	 * \brief delete�O�̏���.
	 * 
	 */
	void OnDestroy() override final;
	/**
	 * \brief ����������.
	 * 
	 * \return true�ŏ���������
	 */
	bool Start() override final;
	/**
	 * \brief �`��O�̑O����.
	 * 
	 */
	void PrePostRender() override final;
	/**
	 * \brief �X�V����.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief �`�揈��.
	 * 
	 */
	void PostRender() override final;
private:
	/**
	 * \briefb �v���C���[�̉�]����.
	 * 
	 */
	void TurnPlayer();
	/**
	 * \brief �ǂ̃X�e�[�W�ɑJ�ڂ��邩���߂�.
	 * 
	 */
	void DistanceStagePoint();
private:
	CSprite m_sprite;											//�X�v���C�g
	CSprite m_arrow;											//���
	CVector2 m_arrowPosition = { 200.0f,300.0f };				//���̍��W
	GameData* m_gameData = nullptr;								//�Q�[���f�[�^
	EnStageNumber m_stageNumber = enState_FirstStage;			//�X�e�[�W�̔ԍ�
	Fade* m_fade = nullptr;										//�t�F�[�h
	bool m_isWaitFadeout = false;								//�t�F�[�h�A�E�g�����H
	CSkinModelRender* m_player = nullptr;						//�v���C���[�̃X�L�����f�������_�[
	Level m_level;												//���x��
	StageSelectGround* m_stageSelectGround = nullptr;			//������
	std::unordered_map<int, StagePoint*> m_stagePointList;		//�X�e�[�W�|�C���g�̃}�b�v
	CFont m_font;												//����
	Battle* m_battle = nullptr;									//�o�g������H
	bool m_isCollection = false;								//�}�Ӊ�ʂɑJ�ځH
	CSprite m_backSprite;										//�q�[�̉摜
	bool m_isTransTitle = false;								//�^�C�g���ɑJ�ڂ���H
	bool m_isBattle = false;									//�o�g������H
	std::vector<StageSelectPoint*> m_stageSelectPointList;		//�X�e�[�W�Z���N�g�̃|�C���g�̃��X�g
	StageSelectPoint* m_stageSelectPoint = nullptr;				//�|�C���g
};

