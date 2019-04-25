#pragma once

extern HWND			g_hWnd ;				//ウィンドウハンドル。
extern GraphicsEngine* g_graphicsEngine ;	//グラフィックスエンジン。

//ゲームの初期化。
void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
//ウィンドウメッセージをディスパッチ。falseが返ってきたら、ゲーム終了。
bool DispatchWindowMessage();
