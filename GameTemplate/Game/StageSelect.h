/*****************************************************************//**
 * \file   StageSelect.h
 * \brief  StageSelectクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "level/Level.h"
#include "GameData.h"

class StageSelectGround;
class Fade;
class GameData;
class StagePoint;
class Battle;
class StageSelectPoint;

/**
 * \brief ステージセレクトシーンを管理するクラス.
 */
class StageSelect:public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	StageSelect();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~StageSelect();
	/**
	 * \brief delete前の処理.
	 * 
	 */
	void OnDestroy() override final;
	/**
	 * \brief 初期化処理.
	 * 
	 * \return trueで初期化完了
	 */
	bool Start() override final;
	/**
	 * \brief 描画前の前処理.
	 * 
	 */
	void PrePostRender() override final;
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
private:
	/**
	 * \briefb プレイヤーの回転処理.
	 * 
	 */
	void TurnPlayer();
	/**
	 * \brief どのステージに遷移するか決める.
	 * 
	 */
	void DistanceStagePoint();
private:
	CSprite m_sprite;											//スプライト
	CSprite m_arrow;											//矢印
	CVector2 m_arrowPosition = { 200.0f,300.0f };				//矢印の座標
	GameData* m_gameData = nullptr;								//ゲームデータ
	EnStageNumber m_stageNumber = enState_FirstStage;			//ステージの番号
	Fade* m_fade = nullptr;										//フェード
	bool m_isWaitFadeout = false;								//フェードアウトした？
	CSkinModelRender* m_player = nullptr;						//プレイヤーのスキンモデルレンダー
	Level m_level;												//レベル
	StageSelectGround* m_stageSelectGround = nullptr;			//ちきう
	std::unordered_map<int, StagePoint*> m_stagePointList;		//ステージポイントのマップ
	CFont m_font;												//文字
	Battle* m_battle = nullptr;									//バトルする？
	bool m_isCollection = false;								//図鑑画面に遷移？
	CSprite m_backSprite;										//拝啓の画像
	bool m_isTransTitle = false;								//タイトルに遷移する？
	bool m_isBattle = false;									//バトルする？
	std::vector<StageSelectPoint*> m_stageSelectPointList;		//ステージセレクトのポイントのリスト
	StageSelectPoint* m_stageSelectPoint = nullptr;				//ポイント
};

