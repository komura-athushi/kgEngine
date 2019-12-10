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
	CSprite m_sprite;										//�X�v���C�g
	CVector3 m_position;
	Fade* m_fade;											//�t�F�[�h(��)
	bool m_isWaitFadeout = false;
	Player* m_player = nullptr;
	GameCamera* m_gameCamera = nullptr;
	CSkinModelRender* m_model = nullptr;
	PhysicsStaticObject m_staticobject;								//�ÓI�I�u�W�F�N�g
	CVector2 m_titlePosition = { 1280.0f / 2, 720.0f / 2 - 370.0f };
	bool m_isStart = false;
	CSprite m_pressStart;
};

