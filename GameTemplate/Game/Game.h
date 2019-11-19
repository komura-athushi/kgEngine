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
private:
	Ground* m_ground = nullptr;									//グラウンド
	Player* m_player = nullptr;									//プレイヤー
	GameCamera* m_gamecamera = nullptr;							//カメラ
	std::vector<Obj*> m_objList;								//オブジェクトのリスト
	float m_timer = 0.0f;										//タイマー
	//level
	Level m_level;												//レベル
	Time* m_time;												//タイム
	Fade* m_fade;
	GameData* m_gameData = nullptr;								//ゲームデータ
	bool m_owaOwari = false;									//ステージをオワらせるか
	CFont m_font;												//フォント
	float m_timer2 = 0.0f;
	bool m_isPose = false;
	OffScreen* m_offScreen = nullptr;
};

