#include "stdafx.h"
#include "system/system.h"
#include "Player.h"
#include "level/Level.h"
#include "KgEngine.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"
#include "StageSelect.h"
#include "Object/ObjectData.h"
#include "GameData.h"
#include "Result.h"
#include "Ranking.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	Engine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	//カメラを初期化。
    MainCamera().SetPosition({ 0.0f, 100.0f, 300.0f });
	MainCamera().SetTarget({ 0.0f, 100.0f, 0.0f });
	MainCamera().SetFar(10000.0f);
	
	ObjectData::GetInstance();
	GetGameData().LoadDataStageClear();

	////プレイヤー
	//Player player;
	//Player* player = NewGO<Player>(0);
	//Game* game = NewGO<Game>(0);
	//Fade::GetInstance();
	//NewGO<Title>(0);
	Ranking::GetInstance();
	NewGO<Game>(0);
	//NewGO<Result>(0);

	//NewGO<StageSelect>(0);
	//ゲームループ。
	Engine().RunGameLoop();
	
		////描画開始。
		//g_graphicsEngine->BegineRender();
		////ゲームパッドの更新。	
		//for (auto& pad : g_pad) {
		//	pad.Update();
		//}
		////物理エンジンの更新。
		//g_physics.Update();
		//////プレイヤーの更新。
		//player.Update();
		//////プレイヤーの描画。
		//player.Draw();
		////カメラの更新。
		//g_camera3D.Update();
		////描画終了。
		//g_graphicsEngine->EndRender();
	Engine().Final();
	return 0;
}