#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"
class Player;
class GameCamera;
class Fade;
//タイトル
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
	CSprite m_sprite;										//スプライト
	Fade* m_fade;											//フェード(仮)
	bool m_isWaitFadeout = false;
	Player* m_player = nullptr;
	GameCamera* m_gameCamera = nullptr;
	CSkinModelRender* m_model = nullptr;
	PhysicsStaticObject m_staticobject;								//静的オブジェクト
};

