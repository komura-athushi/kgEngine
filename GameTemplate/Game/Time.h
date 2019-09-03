#pragma once
class Game;
class GameData;
#include "graphics/2D/kgFont.h"
//�X�e�[�W�̐������Ԃ��Ǘ�����
class Time:public IGameObject
{
public:
	Time();
	~Time();
	bool Start() override;
	void PostRender() override;
	//�������Ԃ�ݒ肷��
	void SetTime(const float& time) 
	{
		m_time = time;
		m_timer = time;
	}
private:
	CSprite m_sprite1,m_sprite2;								//�X�v���C�g
	CFont m_font;												//�t�H���g
	Game* m_game = nullptr;										//�Q�[��
	GameData* m_gamedata = nullptr;								//�Q�[���f�[�^
	float m_time = 0.0f;										//��������
	float m_timer = 0.0f;										//�^�C�}�[
	float m_degree = 0.0f;										//�~�`�Q�[�W�̊p�x
};

