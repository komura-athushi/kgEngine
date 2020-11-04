#pragma once
#include "shadow/kgShadowMap.h"
#include "PostEffect.h"
#include "2D/Sprite.h"
class ShadowMap;
class NormalMap;
class DepthValueMap;
class EdgeDetection;
class CascadeShadowMap;

static const int m_splitMaximumNumber = 2;
/*!
 *@brief	グラフィックスエンジン。
 */
 /// <summary>
 /// レンダリングモード。
 /// </summary>
enum EnRenderMode {
	enRenderMode_Invalid,			//不正なレンダリングモード。
	enRenderMode_CreateShadowMap,	//シャドウマップ生成。
	enRenderMode_CreateCascadeShadowMap,	//カスケードシャドウマップの生成
	enRenderMode_Normal,			//通常レンダリング。
	enRenderMode_NormalMap,			//法線マップ生成
	enRenderMode_DepthValueMap,		//深度値マップ生成
	enRenderMode_Num,				//レンダリングモードの数。
};
class GraphicsEngine:Noncopyable
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	/*!
	 *@brief	初期化。
	 *@param[in]	hWnd		ウィンドウハンドル。
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	解放。
	 */
	void Release();
	/*!
	 *@brief	D3D11デバイスを取得。
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11デバイスコンテキストを取得。
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	/*!
	 *@brief	描画開始。
	 */
	void BegineRender();
	/*!
	 *@brief	描画終了。
	 */
	void EndRender();
	//シャドウマップを描画
	void ShadowMapRender();
	//エッジ検出
	void EdgeDelectionRender();
	void EffektUpdate();
	//ポストレンダリング
	void PostRender();
	//カスケードシャドウマップを取得
	CascadeShadowMap* GetCascadeShadowMap()
	{
		return m_cascadeShadowMap.get();
	}
	//法線マップを取得
	NormalMap* GetNormalMap()
	{
		return m_normalMap.get();
	}
	//深度値マップを主特区
	DepthValueMap* GetDepthValueMap()
	{
		return m_depthValueMao.get();
	}
	//エッジマップを取得
	EdgeDetection* GetEdgeDelection()
	{
		return m_edgeDelection.get();
	}
	//全画面描画用のポストエフェクトを取得
	PostEffect* GetPostEffect()
	{
		return m_postEffect.get();
	}
	//Sprite取得
	DirectX::SpriteBatch* GetSpriteBatch() const
	{
		return m_spriteBatch.get();
	}
	//乗算済みアルファ版
	DirectX::SpriteBatch* GetSpriteBatchPMA() const
	{
		return m_spriteBatchPMA.get();
	}
	//デフォルトフォント取得
	DirectX::SpriteFont* GetSpriteFont() const
	{
		return m_spriteFont.get();
	}
	float AddAndGetLayerDepthCnt() {
		m_layerDepthCnt += 0.001f / 2048.0f;
		return m_layerDepthCnt;
	}
	void ResetLayerDepthCnt() {
		m_layerDepthCnt = 0.0f;
	}
	/// <summary>
	/// メインレンダリングターゲットを取得。
	/// </summary>
	/// <returns></returns>
	RenderTarget* GetMainRenderTarget()
	{
		return &m_mainRenderTarget;
	}
	//レンダリングターゲットをメインに変更する
	void ChangeMainRenderTarget();
	/// <summary>
	/// レンダリングターゲットの切り替え。
	/// </summary>
	/// <param name="renderTarget">レンダリングターゲット</param>
	/// <param name="viewport">ビューポート</param>
	void ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	//シャドウマップ用のライトカメラの注視点を設定
	void SetLightCameraTarget(const CVector3& target, const int number = 0)
	{
		m_target[number] = target;
	}
	//シャドウマップ用のライトカメラの座標を設定
	void SetLightCameraPosition(const CVector3& position, const int number = 0)
	{
		m_position[number] = position;
	}
	//エッジ検出のオンオフ
	void SetisEdge(bool flag)
	{
		m_isEdge = flag;
	}
	//レンダーターゲットをフレームバッファに変更
	void ChangeRenderTargetFrameBuffer()
	{
		ChangeRenderTarget(m_frameBufferRenderTargetView, m_frameBufferDepthStencilView, &m_frameBufferViewports);
	}
	//ビューポートを設定
	void SetViewPort()
	{
		m_pd3dDeviceContext->RSSetViewports(1, &m_frameBufferViewports);
	}
	//ビューポートを画面分割用に設定
	//0で一番目
	void SetSplitViewPort(const int number)
	{
		//m_frameBufferViewports = m_splitViewPorts[number];
		m_pd3dDeviceContext->RSSetViewports(1, &m_splitViewPorts[number]);
	}
	//ビューポートを通常に設定
	void SetNormalViewPort()
	{
		//m_frameBufferViewports = m_normalViewPorts;
		//m_pd3dDeviceContext->RSSetViewports(1, &m_frameBufferViewports);
		m_pd3dDeviceContext->RSSetViewports(1, &m_normalViewPorts);
	}
	//画面分割するを設定
	void SetisSplit(bool flag)
	{
		m_isSplit = flag;
	}
	//画面分割する？
	bool GetisSplit() const
	{
		return m_isSplit;
	}
	//画面分割数を取得
	int GetSplitNumber()
	{
		if (m_isSplit) {
			return m_splitMaximumNumber;
		}
		else {
			return 1;
		}
	}
	void SetBlend()
	{
		//乗算合成用のブレンディングステートを設定する。
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pd3dDeviceContext->OMSetBlendState(m_finalBlendState, blendFactor, 0xffffffff);
	}
private:
	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D デバイスのターゲットとなる機能セット。
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11デバイス。
	IDXGISwapChain*			m_pSwapChain = NULL;		//スワップチェイン。
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11デバイスコンテキスト。
	ID3D11RenderTargetView* m_backBuffer = NULL;		//バックバッファ。
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//ラスタライザステート。
	ID3D11Texture2D*		m_depthStencil = NULL;		//デプスステンシル。
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//デプスステンシルビュー。
	std::unique_ptr<CascadeShadowMap> m_cascadeShadowMap = nullptr;		//カスケードシャドウマップ
	std::unique_ptr<NormalMap>				m_normalMap = nullptr;		//法線マップ
	std::unique_ptr<DepthValueMap>			m_depthValueMao = nullptr;  //深度値マップ
	std::unique_ptr<EdgeDetection>			m_edgeDelection = nullptr;  //エッジ検出
	RenderTarget			m_mainRenderTarget;			//!<メインレンダリングターゲット。
	CSprite m_copyMainRtToFrameBufferSprite;
	//Sprite
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatchPMA;
	float m_layerDepthCnt = 0.0f;

	std::unique_ptr<PostEffect> m_postEffect = nullptr;
	D3D11_VIEWPORT m_frameBufferViewports;			//フレームバッファのビューポート。
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//フレームバッファのレンダリングターゲットビュー。
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//フレームバッファのデプスステンシルビュー。

	CVector3 m_target[m_splitMaximumNumber] = { CVector3::Zero() , CVector3::Zero()};
	CVector3 m_position[m_splitMaximumNumber] = { { 0.0f,1000.0f,1000.0f }, { 0.0f,1000.0f,1000.0f } };

	bool m_isEdge = true;
	//ビューポートは描画する範囲を指定する、シャドウマップの時は変えるんじゃぞ
	D3D11_VIEWPORT m_normalViewPorts;				//通常ビューポート
	D3D11_VIEWPORT m_splitViewPorts[2];				//分割ビューポート
	ID3D11BlendState* m_finalBlendState = nullptr;		//最終合成用のブレンディングステート。
	bool m_isSplit = false;
};

//extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン