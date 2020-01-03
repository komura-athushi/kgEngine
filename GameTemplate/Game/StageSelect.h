#pragma once
#include "level/Level.h"
#include "GameData.h"

class StageSelectGround;
class Fade;
class GameData;
class StagePoint;
class CollectionBook;
//ステージセレクト
class StageSelect:public IGameObject
{
public:
	StageSelect();
	~StageSelect();
	void OnDestroy() override;
	bool Start() override;
	void PrePostRender() override;
	void Update() override;
	void PostRender() override;
private:
	//プレイヤーの回転処理
	void TurnPlayer();
	//どのステージに遷移するか決める
	void DistanceStagePoint();
private:
	CSprite m_sprite;											//スプライト
	CSprite m_arrow;											//矢印
	CVector2 m_arrowPosition = { 200.0f,300.0f };				//矢印の座標
	GameData* m_gameData;										//ゲームデータ
	EnStageNumber m_stageNumber = enState_FirstStage;			//ステージの番号
	Fade* m_fade;												//フェード
	bool m_isWaitFadeout = false;								//フェードアウトした？
	CSkinModelRender m_player;									//プレイヤーのスキンモデルレンダー
	Level m_level;												//レベル
	StageSelectGround* m_stageSelectGround = nullptr;			//ちきう
	std::unordered_map<int, StagePoint*> m_stagePointList;		//ステージポイントのマップ
	StagePoint* m_stagePoint = nullptr;							//プレイヤーと近いステージポイント
	CFont m_font;												//文字
	CollectionBook* m_collectionBook;							//本
	bool m_isCollection = false;								//図鑑画面に遷移？
	CSprite m_backSprite;										//拝啓の画像
	
};

