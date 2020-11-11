/*****************************************************************//**
 * \file   Result.h
 * \brief  Result�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

#include "sound\SoundSource.h"

class GameData;
class Player;
class GameCamera;
class Fade;
class Ranking;
/**
 * \brief ���U���g��ʂ̊Ǘ�.
 */
class Result:public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Result();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Result() override final;
	/**
	 * \brief ����������.
	 * 
	 * \return true�ŏ���������
	 */
	bool Start() override final;
	/**
	 * \brief �X�V����.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief �ŏ��̕��̕`�揈��.
	 * 
	 */
	void PrePostRender() override final;
	/**
	 * \brief �`�揈��.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief delete�̑O�̏���.
	 * 
	 */
	void OnDestroy() override final;
private:
	/**
	 * \brief ��𓮂���.
	 * 
	 */
	void MovePlayer();
	/**
	 * \brief ���ҋ@������.
	 * 
	 */
	void MoveGoal();
	/**
	 * \brief ���ʂ�\��.
	 * 
	 */
	void MoveResult();
	/**
	 * \brief �X�e�[�W�Z���N�g�ɖ߂�.
	 * 
	 */
	void TransScene();
private:
	CSprite m_sprite;											//�摜
	GameData* m_gameData = nullptr;								//�Q�[���̃f�[�^
	Player* m_player = nullptr;									//�v���C���[
	GameCamera* m_gameCamera = nullptr;							//�J����
	float m_timer = 0.0f;										//�v���C���[�𓮂����^�C�}�[
	CFont m_font;												//����
	enum EnResultScene {
		EnResultScene_FadeIn,
		EnResultScene_MovePlayer,
		EnResultScene_MoveGoal,
		EnResultScene_MoveResult,
		EnResultScene_TransScene
	};
	EnResultScene m_resultScene = EnResultScene_FadeIn;
	CSprite m_stageClear;
	CSprite m_gameOver;
	Fade* m_fade = nullptr;
	bool m_isWaitFadeout = false;
	float m_timer2 = 0.0f;
	CSoundSource m_se;
	Ranking* m_ranking = nullptr;
	bool m_isBattle = false;
	CSprite m_winner1, m_winner2, m_draw;
};

