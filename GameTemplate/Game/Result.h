#pragma once

#include "sound\SoundSource.h"

class GameData;
class Player;
class GameCamera;
class Fade;

class Result:public IGameObject
{
public:
	Result();
	~Result();
	bool Start() override;
	void Update() override;
	void PrePostRender() override;
	void PostRender() override;
	void OnDestroy() override;
private:
	//塊を動かす
	void MovePlayer();
	//あ
	void MoveGoal();
	//結果を表示
	void MoveResult();
	//画面遷移
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
};

