#pragma once
/// <summary>
/// レンダリングターゲット
/// </summary>
class RenderTarget
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	RenderTarget();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~RenderTarget();

	/// <summary>
	/// レンダリングターゲットの解放。
	/// </summary>
	/// <remarks>
	/// この関数はデストラクタから呼ばれています。
	/// 明示的なタイミングでレンダリングターゲットを解放したい場合に使用してください。
	/// </remarks>
	void Release();

	/// <summary>
	/// レンダリングターゲットの作成。
	/// </summary>
	/// <param name="w">テクスチャの幅</param>
	/// <param name="h">テクスチャの高さ</param>
	/// <param name="texFormat">テクスチャのフォーマット</param>
	void Create(unsigned int w, unsigned int h, DXGI_FORMAT texFormat);

	/// <summary>
	/// レンダリングターゲットとデプスステンシルのクリア。
	/// </summary>
	/// <param name="clearColor">クリアカラー</param>
	void ClearRenderTarget(float* clearColor);

	/// <summary>
	/// レンダリングターゲットビューを取得。
	/// </summary>
	/// <returns>レンダリングターゲットビュー</returns>
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return m_renderTargetView;
	}

	/// <summary>
	/// デプスステンシルビューを取得。
	/// </summary>
	/// <returns>デプスステンシルビュー</returns>
	ID3D11DepthStencilView* GetDepthStensilView()
	{
		return m_depthStencilView;
	}

	/// <summary>
	/// レンダリングターゲットのSRVを取得。
	/// </summary>
	/// <returns>レンダリングターゲットのSRV</returns>
	ID3D11ShaderResourceView* GetRenderTargetSRV()
	{
		return m_renderTargetSRV;
	}
	//ビューポートを設定
	void SetViewPort(D3D11_VIEWPORT& viewPort)
	{
		m_viewport = viewPort;
	}
	/// <summary>
	/// ビューポートの取得。
	/// </summary>
	/// <returns>ビューポート</returns>
	D3D11_VIEWPORT* GetViewport()
	{
		return &m_viewport;
	}
	/// <summary>
	/// レンダリングターゲットの幅を取得。
	/// </summary>
	/// <returns></returns>
	float GetWidth() const
	{
		return m_width;
	}
	/// <summary>
	/// レンダリングターゲットの高さを取得。
	/// </summary>
	/// <returns></returns>
	float GetHeight() const
	{
		return m_height;
	}
	/// <summary>
	/// レンダリングターゲットとなっているテクスチャのフォーマットを取得。
	/// </summary>
	/// <returns></returns>
	DXGI_FORMAT GetTextureFormat() const
	{
		return m_format;
	}
private:
	float m_width = 0;										//レンダリングターゲットの幅。
	float m_height = 0;										//レンダリングターゲットの高さ。
	DXGI_FORMAT m_format = DXGI_FORMAT_UNKNOWN;				//レンダリングターゲットのフォーマット。
	ID3D11Texture2D* m_renderTargetTex = nullptr;		//!<レンダリングターゲットとなるテクスチャ。
	ID3D11RenderTargetView* m_renderTargetView = nullptr;		//!<レンダーターゲットビュー。
	ID3D11ShaderResourceView* m_renderTargetSRV = nullptr;		//!<レンダリングターゲットのSRV
	ID3D11Texture2D* m_depthStencilTex = nullptr;		//!<デプスステンシルとなるテクスチャ。
	ID3D11DepthStencilView* m_depthStencilView = nullptr;		//!<デプスステンシルビュー。
	D3D11_VIEWPORT				m_viewport;							//!<ビューポート。
};

