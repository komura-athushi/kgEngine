/*****************************************************************//**
 * \file   Game.h
 * \brief  Game�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "level/Level.h"

class Ground;
class Player;
class GameCamera;
class Obj;
class Time;
class Fade;
class GameData;
class OffScreen;
/**
 * \brief �Q�[���V�[�����Ǘ�����N���X.
 */
class Game:public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Game();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Game() override final;
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
	 * \brief delete�O�̏���.
	 * 
	 */
	void OnDestroy() override final;
	/**
	 * \brief �`�揈��.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief �X�e�[�W�I���t���O�����Ă�.
	 * 
	 */
	void SetOwaOwari()
	{
		m_owaOwari = true;
	}
	/**
	 * \brief �X�e�[�W�̏I���t���O�������Ă��邩�ǂ����擾.
	 * 
	 * \return �X�e�[�W�̏I���t���O�������Ă�����true
	 */
	bool GetisOwaOwari()
	{
		return m_owaOwari;
	}
private:
	/**
	 * \brief ���x���f�[�^��ǂݍ���.
	 * 
	 */
	void InitLevel();
	/**
	 * \brief ���f�B�S�[���o���̍X�V����.
	 * 
	 */
	void UpdateReadyGo();
	/**
	 * \brief �Q�[�����̍X�V����.
	 * 
	 */
	void UpdateInGame();
	/**
	 * \brief �Q�[���I�����̍X�V����.
	 * 
	 */
	void UpdateEndGame();
private:
	Ground* m_ground = nullptr;									//�O���E���h
	Player* m_player[m_splitMaximumNumber] = { nullptr ,nullptr};									//�v���C���[
	GameCamera* m_gameCamera[m_splitMaximumNumber] = { nullptr ,nullptr};							//�J����
	std::vector<Obj*> m_objList;								//�I�u�W�F�N�g�̃��X�g
	float m_timer = 0.0f;										//�^�C�}�[
	//level
	Level m_level;												//���x��
	Time* m_time;												//�^�C��
	Fade* m_fade;												//�t�F�[�h
	bool m_isWaitFadeout = false;
	GameData* m_gameData = nullptr;								//�Q�[���f�[�^
	bool m_owaOwari = false;									//�X�e�[�W���I���点�邩
	CFont m_font;												//�t�H���g
	float m_timer2 = 0.0f;										//�^�C�}�[
	bool m_isPose = false;										//�|�[�Y���H
	OffScreen* m_offScreen = nullptr;							//�����p�̃N���X
	CSprite m_pause;											//�|�[�Y
	CSprite m_end;												//�I���
	CSprite m_start;											//�X�^�[�g�����H
	bool m_isStart = false;
	bool m_isBattle = true;
	float m_timer3 = 4.9f;
	float m_firstTimer3 = m_timer3;
	int m_startTime = 4;

};

