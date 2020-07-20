#include "KGstdafx.h"
#include "GraphicsEngine.h"
#include "shadow/kgShadowMap.h"
#include "normal\NormalMap.h"
#include "normal\EdgeDetection.h"
#include "depthvalue\DepthValueMap.h"
#include "shadow\CascadeShadowMap.h"
#include "CEffektEngine.h"

GraphicsEngine::GraphicsEngine()
{
}


GraphicsEngine::~GraphicsEngine()
{
	Release();
}

void GraphicsEngine::BegineRender()
{
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
													  //描き込み先をバックバッファにする。
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
	//バックバッファを灰色で塗りつぶす。
	m_pd3dDeviceContext->ClearRenderTargetView(m_backBuffer, ClearColor);
	m_pd3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	auto d3dDeviceContext = m_pd3dDeviceContext;
	//現在のレンダリングターゲットをバックアップしておく。
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	if (!m_isSplit) {
		m_frameBufferViewports = m_normalViewPorts;
		//m_mainRenderTarget.SetViewPort(m_normalViewPorts);
	}
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);
}

void GraphicsEngine::EndRender()
{
	//バックバッファとフロントバッファを入れ替える。
	m_pSwapChain->Present(2, 0);
}
void GraphicsEngine::Release()
{
	if (m_rasterizerState != NULL) {
		m_rasterizerState->Release();
		m_rasterizerState = NULL;
	}
	if (m_depthStencil != NULL) {
		m_depthStencil->Release();
		m_depthStencil = NULL;
	}
	if (m_depthStencilView != NULL) {
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}
	if (m_backBuffer != NULL) {
		m_backBuffer->Release();
		m_backBuffer = NULL;
	}
	if (m_pSwapChain != NULL) {
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}
	if (m_pd3dDeviceContext != NULL) {
		m_pd3dDeviceContext->Release();
		m_pd3dDeviceContext = NULL;
	}
	if (m_pd3dDevice != NULL) {
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
}
void GraphicsEngine::Init(HWND hWnd)
{
	//スワップチェインを作成するための情報を設定する。
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;									//スワップチェインのバッファ数。通常は１。
	sd.BufferDesc.Width = (UINT)FRAME_BUFFER_W;			//フレームバッファの幅。
	sd.BufferDesc.Height = (UINT)FRAME_BUFFER_H;		//フレームバッファの高さ。
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//フレームバッファのフォーマット。R8G8B8A8の32bit。
	sd.BufferDesc.RefreshRate.Numerator = 60;			//モニタのリフレッシュレート。(バックバッファとフロントバッファを入れ替えるタイミングとなる。)
	sd.BufferDesc.RefreshRate.Denominator = 1;			//２にしたら30fpsになる。1でいい。
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//サーフェスまたはリソースを出力レンダー ターゲットとして使用します。
	sd.OutputWindow = hWnd;								//出力先のウィンドウハンドル。
	sd.SampleDesc.Count = 1;							//1でいい。
	sd.SampleDesc.Quality = 0;							//MSAAなし。0でいい。
	sd.Windowed = TRUE;									//ウィンドウモード。TRUEでよい。

														//利用するDirectXの機能セット。DirectX10以上に対応しているGPUを利用可能とする。
														//この配列はD3D11CreateDeviceAndSwapChainの引数として使う。
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//D3Dデバイスとスワップチェインを作成する。
	D3D11CreateDeviceAndSwapChain(
		NULL,											//NULLでいい。
		D3D_DRIVER_TYPE_HARDWARE,						//D3Dデバイスがアクセスするドライバーの種類。
														//基本的にD3D_DRIVER_TYPE_HARDWAREを指定すればよい。
		NULL,											//NULLでいい。
		createDeviceFlags,												//０でいい。
		featureLevels,									//D3Dデバイスのターゲットとなる機能セットを指定する。
														//今回のサンプルはDirectX10以上をサポートするので、
														//それらを含むD3D_FEATURE_LEVELの配列を渡す。
		sizeof(featureLevels) / sizeof(featureLevels[0]),	//機能セットの数。
		D3D11_SDK_VERSION,								//使用するDirectXのバージョン。
														//D3D11_SDK_VERSIONを指定すればよい。
		&sd,											//スワップチェインを作成するための情報。
		&m_pSwapChain,									//作成したスワップチェインのアドレスの格納先。
		&m_pd3dDevice,									//作成したD3Dデバイスのアドレスの格納先。
		&m_featureLevel,								//使用される機能セットの格納先。
		&m_pd3dDeviceContext							//作成したD3Dデバイスコンテキストのアドレスの格納先。
	);

	//書き込み先になるレンダリングターゲットを作成。
	ID3D11Texture2D* pBackBuffer = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
	pBackBuffer->Release();
	//深度ステンシルビューの作成。
	{
		//深度テクスチャの作成。
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = (UINT)FRAME_BUFFER_W;
		texDesc.Height = (UINT)FRAME_BUFFER_H;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_D32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &m_depthStencil);
		//深度ステンシルビューを作成。
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = texDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		m_pd3dDevice->CreateDepthStencilView(m_depthStencil, &descDSV, &m_depthStencilView);
	}
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	//ラスタライザとビューポートを初期化。
	m_pd3dDevice->CreateRasterizerState(&desc, &m_rasterizerState);

	m_normalViewPorts.Width = FRAME_BUFFER_W;
	m_normalViewPorts.Height = FRAME_BUFFER_H;
	m_normalViewPorts.TopLeftX = 0;
	m_normalViewPorts.TopLeftY = 0;
	m_normalViewPorts.MinDepth = 0.0f;
	m_normalViewPorts.MaxDepth = 1.0f;

	m_splitViewPorts[0].Width = FRAME_BUFFER_W;
	m_splitViewPorts[0].Height = FRAME_BUFFER_H / 2;
	m_splitViewPorts[0].TopLeftX = 0;
	m_splitViewPorts[0].TopLeftY = 0;
	m_splitViewPorts[0].MinDepth = 0.0f;
	m_splitViewPorts[0].MaxDepth = 1.0f;

	m_splitViewPorts[1].Width = FRAME_BUFFER_W;
	m_splitViewPorts[1].Height = FRAME_BUFFER_H / 2;
	m_splitViewPorts[1].TopLeftX = 0;
	m_splitViewPorts[1].TopLeftY = FRAME_BUFFER_H / 2;
	m_splitViewPorts[1].MinDepth = 0.0f;
	m_splitViewPorts[1].MaxDepth = 1.0f;

	m_pd3dDeviceContext->RSSetViewports(1, &m_normalViewPorts);
	m_pd3dDeviceContext->RSSetState(m_rasterizerState);
	m_shadowMap = new ShadowMap;
	m_cascadeShadowMap = new CascadeShadowMap;
	m_normalMap = new NormalMap;
	m_depthValueMao = new DepthValueMap;
	m_edgeDelection = new EdgeDetection;
	m_edgeDelection->InitGaussian(m_normalMap);
	//Sprite初期化
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_pd3dDevice, L"Assets/Font/myfile.spritefont");
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pd3dDeviceContext);
	m_spriteBatchPMA = std::make_unique<DirectX::SpriteBatch>(m_pd3dDeviceContext);
	
	//メインとなるレンダリングターゲットを作成する。
	m_mainRenderTarget.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
	m_copyMainRtToFrameBufferSprite.Init(m_mainRenderTarget.GetRenderTargetSRV());

	m_postEffect = new PostEffect();
	m_postEffect->InitFullScreenQuadPrimitive();

	CEffektEngine::GetInstance().InitEffekseer();
}

void GraphicsEngine::ShadowMapRender()
{
	for (int i = 0; i < GetSplitNumber(); i++) {
		//シャドウマップを更新。
		m_shadowMap->UpdateFromLightTarget(
			m_position[i],
			m_target[i],
			i
		);
	}
	m_cascadeShadowMap->Update();
	auto d3dDeviceContext = m_pd3dDeviceContext;
	//現在のレンダリングターゲットをバックアップしておく。
	/*ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	D3D11_VIEWPORT oldViewports;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);*/

	//シャドウマップにレンダリング
	//m_shadowMap->RenderToShadowMap();
	m_cascadeShadowMap->RenderToShadowMap();

	//元に戻す。
	/*d3dDeviceContext->OMSetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView
	);*/
	//d3dDeviceContext->RSSetViewports(numViewport, &m_frameBufferViewports);
	//レンダリングターゲットとデプスステンシルの参照カウンタを下す。
	//oldRenderTargetView->Release();
	//oldDepthStencilView->Release();


}

void GraphicsEngine::EdgeDelectionRender()
{
	m_edgeDelection->EdgeRender(*m_postEffect);
	ChangeRenderTarget(&m_mainRenderTarget, m_mainRenderTarget.GetViewport());
}

void GraphicsEngine::ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport)
{
	ChangeRenderTarget(
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}
void GraphicsEngine::ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport)
{
	ID3D11RenderTargetView* rtTbl[] = {
		renderTarget
	};
	//レンダリングターゲットの切り替え。
	m_pd3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//ビューポートが指定されていたら、ビューポートも変更する。
		m_pd3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void GraphicsEngine::ChangeMainRenderTarget()
{
	//ChangeRenderTarget(&m_mainRenderTarget, &m_frameBufferViewports);

	ID3D11RenderTargetView* rt[] = {
		m_mainRenderTarget.GetRenderTargetView(),
		m_normalMap->GetRenderTarget()->GetRenderTargetView(),
		m_depthValueMao->GetRenderTarget()->GetRenderTargetView()
	};
	m_pd3dDeviceContext->OMSetRenderTargets(3, rt, m_mainRenderTarget.GetDepthStensilView());
	//m_pd3dDeviceContext->RSSetViewports(1, m_mainRenderTarget.GetViewport());
	m_pd3dDeviceContext->RSSetViewports(1, &m_frameBufferViewports);
	//メインレンダリングターゲットをクリアする。
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float clearColor2[] = { 1.0f,1.0f,1.0f,1.0f };
	m_mainRenderTarget.ClearRenderTarget(clearColor);
	m_normalMap->GetRenderTarget()->ClearRenderTarget(clearColor2);
	m_depthValueMao->GetRenderTarget()->ClearRenderTarget(clearColor2);
}

void GraphicsEngine::PostRender()
{
	//ポストエフェクトの描画。
	m_postEffect->Draw();

	if (m_isEdge) {
		m_edgeDelection->Draw(*m_postEffect);
	}

	//レンダリングターゲットをフレームバッファに戻す。
	ChangeRenderTarget(
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);

	SetNormalViewPort();

	//ドロドロ
	m_copyMainRtToFrameBufferSprite.Draw();

	//SetSplitViewPort(0);

	m_frameBufferRenderTargetView->Release();
	m_frameBufferDepthStencilView->Release();
}

void GraphicsEngine::EffektUpdate()
{
	auto effectEngine = CEffektEngine::GetInstance();

	effectEngine.Update();

	for (int i = 0; i < GetSplitNumber(); i++) {
		SetSplitViewPort(i);
		effectEngine.CameraEffekseer(i);
		effectEngine.Draw(i);

	}
}
