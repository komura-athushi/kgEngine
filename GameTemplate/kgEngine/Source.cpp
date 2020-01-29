#include <tchar.h>   
#include <windows.h>
#include <dxerr9.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // 無駄な警告を出さなくする
#include <strsafe.h>
#pragma warning( default : 4996 ) 

//メモリリークを調べるために必要。
#if _DEBUG   
#include <crtdbg.h>   
#define new  ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )   
#endif

#define WINDOW_WIDTH  (640)		// ウィンドウの幅
#define WINDOW_HEIGHT (480)		// ウィンドウの高さ
#define WINDOW_X ((WINDOW_WIDTH + GetSystemMetrics( SM_CXSIZEFRAME )*2))
#define WINDOW_Y ((WINDOW_HEIGHT + GetSystemMetrics( SM_CYSIZEFRAME )*2 + GetSystemMetrics( SM_CYCAPTION )))
#define SAFE_RELEASE(p)  { if(p) { (p)->Release(); (p)=NULL; } }


HINSTANCE	g_hInstance = NULL;		// インスタンス・ハンドル
HWND		g_hWindow = NULL;		// ウインドウ・ハンドル
LPDIRECT3D9				g_pD3D = NULL;		// Direct3Dインタフェース
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;		// Direct3DDeviceインタフェース


#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE)
struct MY_VERTEX {
	float px, py, pz;
	DWORD color;
};

MY_VERTEX v1[] = { {-2.0f, 2.0f, 0.0f, 0xffff0000},
					{ 2.0f, 2.0f, 0.0f, 0xff00ff00},
					{-2.0f,-2.0f, 0.0f, 0xff0000ff},
					{ 2.0f,-2.0f, 0.0f, 0xffffffff}, };

struct MY_COLOR {
	int r, g, b;
};

MY_COLOR g_color[] = { {0,0,0},
						{20,60,20},
						{20,60,20},
						{0,0,0} };

D3DVIEWPORT9 g_port[] = { { 0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,0.0f,1.0f},
							{ WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,0.0f,1.0f},
							{ 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,0.0f,1.0f},
							{ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,0.0f,1.0f} };

struct MY_VIEW {
	D3DXVECTOR3 vEyePt;
	D3DXVECTOR3 vLookatPt;
	D3DXVECTOR3 vUpVec;
	D3DXMATRIXA16 matView;
};

MY_VIEW g_view[] = { {D3DXVECTOR3(0.0f, 4.0f,-6.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)},
					{D3DXVECTOR3(0.0f, 0.0f, -6.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)},
					{D3DXVECTOR3(0.0f, 0.0f, -5.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)},
					{D3DXVECTOR3(0.0f,-6.0f, -8.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)}
};

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: InitApp()
// Desc: Initialize Window 
//-----------------------------------------------------------------------------
HRESULT InitApp(HINSTANCE hInstance)
{
	g_hInstance = hInstance;
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, MsgProc, 0, 0, g_hInstance, NULL, NULL,
								   (HBRUSH)(COLOR_WINDOW + 1), NULL, L"D3D Tutorial", NULL };

	if (!RegisterClassEx(&wcex)) {
		return DXTRACE_ERR(L"InitApp", GetLastError());
	}

	// Create the application's window
	g_hWindow = CreateWindow(L"D3D Tutorial", L"ゲームプログラミング",
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX, 100, 100, WINDOW_X, WINDOW_Y,
		NULL, NULL, wcex.hInstance, NULL);

	if (g_hWindow == NULL)
		return DXTRACE_ERR(L"InitApp", GetLastError());

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InitDirectX()
// Desc: Initialize DirectX
//-----------------------------------------------------------------------------
HRESULT InitDirectX(void) {

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {
		DXTRACE_MSG(L"DirectXDeviceの初期化に失敗しました");
		return E_FAIL;
	}

	D3DDISPLAYMODE d3ddm;
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		DXTRACE_MSG(L"DirectX3DDeviceの初期化に失敗しました");
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT hr = g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice);

	if (FAILED(hr))
	{
		hr = g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, g_hWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice);
		if (FAILED(hr))
		{
			hr = g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, g_hWindow,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice);
			if (FAILED(hr)) {
				DXTRACE_MSG(L"DirectX3DDeviceの初期化に失敗しました");
				return E_FAIL;
			}
		}
	}
	//セットアップ　レンダーステート
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x33333333);
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	//ビュー行列作成
	for (int i = 0, count = sizeof(g_view) / sizeof(g_view[0]); i < count; i++)
	{
		D3DXMatrixLookAtLH(&g_view[i].matView, &g_view[i].vEyePt, &g_view[i].vLookatPt, &g_view[i].vUpVec);
	}
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &g_view[0].matView);

	//射影行列作成
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.0f), (float)640 / 480, 1.0f, 100.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Uninitialize()
// Desc: UnInitialize DirectX
//-----------------------------------------------------------------------------
void Uninitialize(void)
{
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG   
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif 

	HRESULT hr;
	if (FAILED(hr = InitApp(hInstance)))
	{
		return(0);
	}

	// Initialize Direct3D
	if (SUCCEEDED(hr = InitDirectX()))
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		ShowWindow(g_hWindow, SW_SHOWDEFAULT);
		UpdateWindow(g_hWindow);

		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);

		// Enter the message loop
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				g_pD3DDevice->BeginScene();

				static int n = 0;
				static int degree = 0;

				if ((n = (n + 1) % 5) == 0) {
					degree = (degree + 1) % 360;
				}
				D3DXMatrixRotationY(&mat, D3DXToRadian(degree));
				g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);

				//今回は４人対戦画面なので、４回繰り返す
				for (int i = 0, count = sizeof(g_port) / sizeof(g_port[0]); i < count; i++)
				{
					//描画領域を変更（ビューポート行列）
					g_pD3DDevice->SetViewport(&g_port[i]);
					//カメラの座標を変更（ビュー行列）
					g_pD3DDevice->SetTransform(D3DTS_VIEW, &g_view[i].matView);
					//描画領域にした所を任意の色でクリア
					g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						D3DCOLOR_XRGB(g_color[i].r, g_color[i].b, g_color[i].g), 1.0f, 0);
					//何かを描画する（今回は四角ポリゴン）
					g_pD3DDevice->SetFVF(MY_VERTEX_FVF);
					g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v1, sizeof(MY_VERTEX));

				}

				g_pD3DDevice->EndScene();
				g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
				Sleep(1);
			}
		}
	}
	Uninitialize();
	UnregisterClass(L"D3D Tutorial", g_hInstance);
	return 0;
}