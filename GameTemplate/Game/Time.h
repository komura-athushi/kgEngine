/*****************************************************************//**
 * \file   Time.h
 * \brief  Timeクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
class Game;
class GameData;
#include "graphics/2D/kgFont.h"
//ステージの制限時間を管理する
/**
 * \brief ステージ中の制限時間を管理する.
 */
class Time:public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Time();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Time() override final;
	/**
	 * \brief 初期化処理.
	 * 
	 * \return trueで初期化完了
	 */
	bool Start() override final;
	/**
	 * \brief 描画処理.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief 制限時間を設定する.
	 * 
	 * \param time 制限時間
	 */
	void SetTime(const float time) 
	{
		m_time = time;
		m_timer = time;
	}
private:
	CSprite m_sprite1,m_sprite2;								//スプライト
	CFont m_font;												//フォント
	Game* m_game = nullptr;										//ゲーム
	GameData* m_gamedata = nullptr;								//ゲームデータ
	float m_time = 0.0f;										//制限時間
	float m_timer = 0.0f;										//タイマー
	float m_degree = 0.0f;										//円形ゲージの角度
	int m_beforeSeconds = 60;									//前フレームの秒数
};

