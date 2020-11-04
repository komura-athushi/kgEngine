
#include "KGstdafx.h"
#include "KgEngine.h"
#include "sound/SoundEngine.h"

CEngine::CEngine()
{
}


CEngine::~CEngine()
{
}

//ウィンドウクラスの初期化
void CEngine::InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	//ウィンドウクラスのパラメータを設定(単なる構造体の変数の初期化です。)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		//構造体のサイズ。
		CS_CLASSDC,				//ウィンドウのスタイル。
								//ここの指定でスクロールバーをつけたりできるが、ゲームでは不要なのでCS_CLASSDCでよい。
		MsgProc,				//メッセージプロシージャ(後述)
		0,						//0でいい。
		0,						//0でいい。
		GetModuleHandle(NULL),	//このクラスのためのウインドウプロシージャがあるインスタンスハンドル。
								//何も気にしなくてよい。
		NULL,					//アイコンのハンドル。アイコンを変えたい場合ここを変更する。とりあえずこれでいい。
		NULL,					//マウスカーソルのハンドル。NULLの場合はデフォルト。
		NULL,					//ウィンドウの背景色。NULLの場合はデフォルト。
		NULL,					//メニュー名。NULLでいい。
		appName,				//ウィンドウクラスに付ける名前。
		NULL					//NULLでいい。
	};
	//ウィンドウクラスの登録。
	RegisterClassEx(&wc);

	// ウィンドウの作成。
	m_hWnd = CreateWindow(
		appName,		//使用するウィンドウクラスの名前。
								//先ほど作成したウィンドウクラスと同じ名前にする。
		appName,		//ウィンドウの名前。ウィンドウクラスの名前と別名でもよい。
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル。ゲームでは基本的にWS_OVERLAPPEDWINDOWでいい、
		0,						//ウィンドウの初期X座標。
		0,						//ウィンドウの初期Y座標。
		(UINT)FRAME_BUFFER_W,	//ウィンドウの幅。
		(UINT)FRAME_BUFFER_H,	//ウィンドウの高さ。
		NULL,					//親ウィンドウ。ゲームでは基本的にNULLでいい。
		NULL,					//メニュー。今はNULLでいい。
		hInstance,				//アプリケーションのインスタンス。
		NULL
	);

	ShowWindow(m_hWnd, nCmdShow);

}

void CEngine::InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	//ウィンドウを初期化。
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, appName);
	//DirectXの初期化。
	m_graphicsEngine = std::make_unique<GraphicsEngine>();
	m_graphicsEngine->Init(m_hWnd);
	m_gameobjectmanager = &GameObjectManager();
	m_gameobjectmanager->Init(255);
	//ゲームパッドの初期化。
	//最大４つのコントローラーを接続できるようにしましょう。
	m_pad[0].Init(0);
	m_pad[1].Init(1);
	m_pad[2].Init(2);
	m_pad[3].Init(3);

	//物理エンジンの初期化
	m_physicsEngine = std::make_unique<PhysicsWorld>();
	m_physicsEngine->Init();
}

//ウィンドウメッセージをディスパッチ。falseが返ってきたら、ゲーム終了。
bool CEngine::DispatchWindowMessage()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message) {
		//ウィンドウからのメッセージを受け取る。
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ウィンドウメッセージが空になった。
			break;
		}
	}
	return msg.message != WM_QUIT;
}

void CEngine::RunGameLoop()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//更新。
			Update();
		}
	}
}

void CEngine::Update()
{
	//計測開始
	m_watch.Start();
	//描画開始。
	m_graphicsEngine->BegineRender();
	
	//ゲームパッドの更新。	
	for (auto& pad : m_pad) {
		pad.Update();
	}

	m_gameobjectmanager->PreUpdate();

	//物理エンジンの更新。
	m_physicsEngine->Update();

	//ゲームオブジェクトの処理
	m_gameobjectmanager->Start();
	m_gameobjectmanager->Update();

	m_graphicsEngine->ShadowMapRender();

	m_graphicsEngine->ChangeMainRenderTarget();

	m_gameobjectmanager->PrePostRender();
	m_gameobjectmanager->Draw();

	m_graphicsEngine->EffektUpdate();

	m_graphicsEngine->EdgeDelectionRender();

	m_graphicsEngine->PostRender();



	m_gameobjectmanager->PostRender();

	m_gameobjectmanager->Delete();

	SoundEngine().Update();

	//カメラの更新。
	MainCamera().Update();

	//描画終了。
	m_graphicsEngine->EndRender();

	//計測終了
	m_watch.Stop();

	//時間を記憶
	GameTime().PushFrameDeltaTime((float)m_watch.GetElapsed());
}

void CEngine::Final()
{
	//if (m_gameThread) {
	//	//ゲームスレッドを殺す。
	//	m_isReqDeadGameThread = true;
	//	m_isRunGameThreadCv.notify_all();
	//	m_gameThread->join();
	//}

	m_physicsEngine->Release();
	//m_soundEngine.Release();
	m_graphicsEngine->Release();

}

void ReleaseDirectX()
{

}

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//送られてきたメッセージで処理を分岐させる。
	switch (msg)
	{
	case WM_DESTROY:
		ReleaseDirectX();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
