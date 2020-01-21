#pragma once

#include "sound\SoundSource.h"

class GameData;
class Player;
class GameCamera;
class Fade;

class Result:public IGameObject
{
public:
	Result();
	~Result();
	bool Start() override;
	void Update() override;
	void PrePostRender() override;
	void PostRender() override;
	void OnDestroy() override;
private:
	//��𓮂���
	void MovePlayer();
	//��
	void MoveGoal();
	//���ʂ�\��
	void MoveResult();
	//��ʑJ��
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
};

