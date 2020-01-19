#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"



class Player;
class GameCamera;
class Fade;
//�^�C�g��
class Title:public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override;
	void Update() override;
	void PostRender() override;
	void OnDestroy() override;
private:
	CSprite m_sprite;													//�X�v���C�g
	Fade* m_fade;														//�t�F�[�h(��)
	bool m_isWaitFadeout = false;										//�t�F�[�h�A�E�g�����H
	Player* m_player = nullptr;											//�v���C���[
	GameCamera* m_gameCamera = nullptr;									//�J����
	CSkinModelRender* m_model = nullptr;								//�O���E���h
	PhysicsStaticObject m_staticobject;									//�ÓI�I�u�W�F�N�g
	CVector2 m_titlePosition = { 1280.0f / 2, 720.0f / 2 - 370.0f };	//�^�C�g���摜�̍��W
	bool m_isStart = false;												//�X�^�[�g�ł���H
	CSprite m_pressStart;												//�v���X�X�^�[�g�̉摜
	float m_timer = 0.0f;
	
};

