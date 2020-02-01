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
//ゲームクラス
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	void PostRender() override;
	//ステージがオワオワリと設定する
	void SetOwaOwari()
	{
		m_owaOwari = true;
	}
	//オワオワリ？
	bool GetisOwaOwari()
	{
		return m_owaOwari;
	}
private:
	Ground* m_ground = nullptr;									//グラウンド
	Player* m_player[m_splitMaximumNumber] = { nullptr ,nullptr};									//プレイヤー
	GameCamera* m_gamecamera[m_splitMaximumNumber] = { nullptr ,nullptr};							//カメラ
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

