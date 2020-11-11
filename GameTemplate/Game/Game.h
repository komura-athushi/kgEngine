/*****************************************************************//**
 * \file   Game.h
 * \brief  Gameクラス
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
 * \brief ゲームシーンを管理するクラス.
 */
class Game:public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Game();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Game() override final;
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
	 * \brief delete前の処理.
	 * 
	 */
	void OnDestroy() override final;
	/**
	 * \brief 描画処理.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief ステージ終了フラグを建てる.
	 * 
	 */
	void SetOwaOwari()
	{
		m_owaOwari = true;
	}
	/**
	 * \brief ステージの終了フラグが建っているかどうか取得.
	 * 
	 * \return ステージの終了フラグが建っていたらtrue
	 */
	bool GetisOwaOwari()
	{
		return m_owaOwari;
	}
private:
	/**
	 * \brief レベルデータを読み込む.
	 * 
	 */
	void InitLevel();
	/**
	 * \brief レディゴー演出中の更新処理.
	 * 
	 */
	void UpdateReadyGo();
	/**
	 * \brief ゲーム中の更新処理.
	 * 
	 */
	void UpdateInGame();
	/**
	 * \brief ゲーム終了時の更新処理.
	 * 
	 */
	void UpdateEndGame();
private:
	Ground* m_ground = nullptr;									//グラウンド
	Player* m_player[m_splitMaximumNumber] = { nullptr ,nullptr};									//プレイヤー
	GameCamera* m_gameCamera[m_splitMaximumNumber] = { nullptr ,nullptr};							//カメラ
	std::vector<Obj*> m_objList;								//オブジェクトのリスト
	float m_timer = 0.0f;										//タイマー
	//level
	Level m_level;												//レベル
	Time* m_time;												//タイム
	Fade* m_fade;												//フェード
	bool m_isWaitFadeout = false;
	GameData* m_gameData = nullptr;								//ゲームデータ
	bool m_owaOwari = false;									//ステージをオワらせるか
	CFont m_font;												//フォント
	float m_timer2 = 0.0f;										//タイマー
	bool m_isPose = false;										//ポーズ中？
	OffScreen* m_offScreen = nullptr;							//小窓用のクラス
	CSprite m_pause;											//ポーズ
	CSprite m_end;												//終わり
	CSprite m_start;											//スタートした？
	bool m_isStart = false;
	bool m_isBattle = true;
	float m_timer3 = 4.9f;
	float m_firstTimer3 = m_timer3;
	int m_startTime = 4;

};

