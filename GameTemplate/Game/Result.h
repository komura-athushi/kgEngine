/*****************************************************************//**
 * \file   Result.h
 * \brief  Resultクラス
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
 * \brief リザルト画面の管理.
 */
class Result:public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Result();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Result() override final;
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
	 * \brief 最初の方の描画処理.
	 * 
	 */
	void PrePostRender() override final;
	/**
	 * \brief 描画処理.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief deleteの前の処理.
	 * 
	 */
	void OnDestroy() override final;
private:
	/**
	 * \brief 塊を動かす.
	 * 
	 */
	void MovePlayer();
	/**
	 * \brief 塊を待機させて.
	 * 
	 */
	void MoveGoal();
	/**
	 * \brief 結果を表示.
	 * 
	 */
	void MoveResult();
	/**
	 * \brief ステージセレクトに戻る.
	 * 
	 */
	void TransScene();
private:
	CSprite m_sprite;											//画像
	GameData* m_gameData = nullptr;								//ゲームのデータ
	Player* m_player = nullptr;									//プレイヤー
	GameCamera* m_gameCamera = nullptr;							//カメラ
	float m_timer = 0.0f;										//プレイヤーを動かすタイマー
	CFont m_font;												//文字
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

