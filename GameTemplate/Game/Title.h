/*****************************************************************//**
 * \file   Title.h
 * \brief  Titleクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"

class Player;
class GameCamera;
class Fade;

/**
 * \brief タイトル画面を管理するクラス.
 */
class Title:public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Title();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Title() override final;
	/**
	 * \brief 初期化処理.
	 * 
	 * \return trueで初期化完了
	 */
	bool Start() override final;
	/**
	 * \brief 更新処理.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief 描画処理.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief delete前の処理.
	 * 
	 */
	void OnDestroy() override final;
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

