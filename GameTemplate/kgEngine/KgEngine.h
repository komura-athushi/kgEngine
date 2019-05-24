
#pragma once

#include "gameobject/GameObjectManager.h"
//コピー禁止にするための基本クラス
class NonCopyable
{
protected:
	NonCopyable() {};
	~NonCopyable() {};
private:
	//コピーコンストラクタや代入演算子をprivate関数にすることにより
	//派生クラスからアクセスできなくする
	NonCopyable(const NonCopyable&);
	NonCopyable& operator=(const NonCopyable&) 
	{

	}
};


//メッセージプロシージャ
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//DirectXの終了処理
void ReleaseDirectX();
class CEngine:private NonCopyable
{
public:
	CEngine();
	~CEngine();
	//ゲームの初期化
	void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
	//ウィンドウクラスの初期化
	void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
	//ウィンドウメッセージをディスパッチ。falseが返ってきたら、ゲーム終了。
	bool DispatchWindowMessage();
	//メッセージ来たら停止してきてなかったらゲームの処理する
	void RunGameLoop();
	//ゲームの処理
	void Update();
	//ゲームの終了処理
	void Final();
	//インスタンスを取得
	static CEngine& GetInstance()
	{
		static CEngine* instance = nullptr;
		if (instance == nullptr) {
			instance = new CEngine;
		}
		return *instance;
	}
	//グラフィックエンジンを取得
	GraphicsEngine& GetGraphicsEngine() const
	{
		return *m_graphicsEngine;
	}
private:
	HWND m_hWnd = NULL;
	GraphicsEngine* m_graphicsEngine = nullptr;
	CGameObjectManager* m_gameobjectmanager = nullptr;
};


//エンジンのインスタンスを取得
static inline CEngine& Engine()
{
	return CEngine::GetInstance();
}


