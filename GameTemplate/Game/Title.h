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
	CSprite m_sprite;													//スプライト
	Fade* m_fade;														//フェード(仮)
	bool m_isWaitFadeout = false;										//フェードアウトした？
	Player* m_player = nullptr;											//プレイヤー
	GameCamera* m_gameCamera = nullptr;									//カメラ
	CSkinModelRender* m_model = nullptr;								//グラウンド
	PhysicsStaticObject m_staticobject;									//静的オブジェクト
	CVector2 m_titlePosition = { 1280.0f / 2, 720.0f / 2 - 370.0f };	//タイトル画像の座標
	bool m_isStart = false;												//スタートできる？
	CSprite m_pressStart;												//プレススタートの画像
	float m_timer = 0.0f;
	
};

