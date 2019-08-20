﻿
#ifndef	__EFFEKSEERRENDERER_DX9_RENDERER_IMPLEMENTED_H__
#define	__EFFEKSEERRENDERER_DX9_RENDERER_IMPLEMENTED_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerRendererDX9.Base.h"
#include "EffekseerRendererDX9.Renderer.h"
#include "../../EffekseerRendererCommon/EffekseerRenderer.RenderStateBase.h"
#include "../../EffekseerRendererCommon/EffekseerRenderer.StandardRenderer.h"

#include <array>

#ifdef _MSC_VER
#include <xmmintrin.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerRendererDX9
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct Vertex
{
	::Effekseer::Vector3D	Pos;
	D3DCOLOR	Col;
	float		UV[2];

	void SetColor( const ::Effekseer::Color& color )
	{
		Col = D3DCOLOR_ARGB(
			color.A,
			color.R,
			color.G,
			color.B);
	}
};

struct VertexDistortion
{
	::Effekseer::Vector3D	Pos;
	D3DCOLOR	Col;
	float		UV[2];
	::Effekseer::Vector3D	Tangent;
	::Effekseer::Vector3D	Binormal;

	void SetColor(const ::Effekseer::Color& color)
	{
		Col = D3DCOLOR_ARGB(
			color.A,
			color.R,
			color.G,
			color.B);
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	描画クラス
	@note
	ツール向けの描画機能。
*/
class RendererImplemented
	: public Renderer
	, public ::Effekseer::ReferenceObject
{
friend class DeviceObject;

private:
	LPDIRECT3DDEVICE9	m_d3d_device;

	VertexBuffer*		m_vertexBuffer;
	IndexBuffer*		m_indexBuffer = nullptr;
	IndexBuffer*		m_indexBufferForWireframe = nullptr;
	int32_t				m_squareMaxCount;

	int32_t				drawcallCount = 0;
	int32_t				drawvertexCount = 0;

	Shader*							m_shader;
	Shader*							m_shader_no_texture;

	Shader*							m_shader_distortion;
	Shader*							m_shader_no_texture_distortion;

	Shader*		currentShader = nullptr;

	EffekseerRenderer::StandardRenderer<RendererImplemented, Shader, Vertex, VertexDistortion>*	m_standardRenderer;


	::Effekseer::Vector3D	m_lightDirection;
	::Effekseer::Color		m_lightColor;
	::Effekseer::Color		m_lightAmbient;

	::Effekseer::Matrix44	m_proj;
	::Effekseer::Matrix44	m_camera;
	::Effekseer::Matrix44	m_cameraProj;

	::Effekseer::Vector3D	m_cameraPosition;
	::Effekseer::Vector3D	m_cameraFrontDirection;

	// 座標系
	::Effekseer::CoordinateSystem		m_coordinateSystem;

	::EffekseerRenderer::RenderStateBase*			m_renderState;

	::Effekseer::TextureData					m_background;

	std::set<DeviceObject*>	m_deviceObjects;

	// ステート
	DWORD	m_state_FVF;

	DWORD	m_state_D3DRS_ALPHABLENDENABLE;
	DWORD	m_state_D3DRS_BLENDOP;
	DWORD	m_state_D3DRS_DESTBLEND;
	DWORD	m_state_D3DRS_SRCBLEND;
	DWORD	m_state_D3DRS_ALPHAREF;

	DWORD	m_state_D3DRS_DESTBLENDALPHA;
	DWORD	m_state_D3DRS_SRCBLENDALPHA;
	DWORD	m_state_D3DRS_SEPARATEALPHABLENDENABLE;
	DWORD	m_state_D3DRS_BLENDOPALPHA;

	DWORD	m_state_D3DRS_ZENABLE;
	DWORD	m_state_D3DRS_ZWRITEENABLE;
	DWORD	m_state_D3DRS_ALPHATESTENABLE;
	DWORD	m_state_D3DRS_CULLMODE;

	DWORD	m_state_D3DRS_COLORVERTEX;
	DWORD	m_state_D3DRS_LIGHTING;
	DWORD	m_state_D3DRS_SHADEMODE;

	std::array<DWORD, 4>	m_state_D3DSAMP_MAGFILTER;
	std::array<DWORD, 4>	m_state_D3DSAMP_MINFILTER;
	std::array<DWORD, 4>	m_state_D3DSAMP_MIPFILTER;
	std::array<DWORD, 4>	m_state_D3DSAMP_ADDRESSU;
	std::array<DWORD, 4>	m_state_D3DSAMP_ADDRESSV;

	IDirect3DVertexShader9*			m_state_vertexShader;
	IDirect3DPixelShader9*			m_state_pixelShader;
	IDirect3DVertexDeclaration9*	m_state_vertexDeclaration;

	IDirect3DVertexBuffer9* m_state_streamData;
	UINT m_state_OffsetInBytes;
	UINT m_state_pStride;

	IDirect3DIndexBuffer9*	m_state_IndexData;

	std::vector<float>	m_state_VertexShaderConstantF;
	std::vector<float>	m_state_PixelShaderConstantF;

	std::array<IDirect3DBaseTexture9*, 2>	m_state_pTexture;

	bool	m_isChangedDevice;

	bool	m_restorationOfStates;

	EffekseerRenderer::DistortingCallback* m_distortingCallback;

	Effekseer::RenderMode m_renderMode = Effekseer::RenderMode::Normal;

public:
	/**
		@brief	コンストラクタ
	*/
	RendererImplemented( int32_t squareMaxCount );

	/**
		@brief	デストラクタ
	*/
	~RendererImplemented();

	void OnLostDevice();
	void OnResetDevice();

	/**
		@brief	初期化
	*/
	bool Initialize( LPDIRECT3DDEVICE9 device );

	void Destroy();

	void SetRestorationOfStatesFlag(bool flag);

	/**
		@brief	描画開始
	*/
	bool BeginRendering();

	/**
		@brief	描画終了
	*/
	bool EndRendering();

	/**
		@brief	デバイス取得
	*/
	LPDIRECT3DDEVICE9 GetDevice();

	/**
		@brief	頂点バッファ取得
	*/
	VertexBuffer* GetVertexBuffer();

	/**
		@brief	インデックスバッファ取得
	*/
	IndexBuffer* GetIndexBuffer();

	/**
		@brief	インデックスバッファ取得
	*/
	IndexBuffer* GetIndexBufferForWireframe();

	/**
		@brief	最大描画スプライト数
	*/
	int32_t GetSquareMaxCount() const;

	::EffekseerRenderer::RenderStateBase* GetRenderState();

	/**
		@brief	ライトの方向を取得する。
	*/
	const ::Effekseer::Vector3D& GetLightDirection() const;

	/**
		@brief	ライトの方向を設定する。
	*/
	void SetLightDirection( const ::Effekseer::Vector3D& direction );

	/**
		@brief	ライトの色を取得する。
	*/
	const ::Effekseer::Color& GetLightColor() const;

	/**
		@brief	ライトの色を設定する。
	*/
	void SetLightColor( const ::Effekseer::Color& color );

	/**
		@brief	ライトの環境光の色を取得する。
	*/
	const ::Effekseer::Color& GetLightAmbientColor() const;

	/**
		@brief	ライトの環境光の色を設定する。
	*/
	void SetLightAmbientColor( const ::Effekseer::Color& color );

	/**
		@brief	投影行列を取得する。
	*/
	const ::Effekseer::Matrix44& GetProjectionMatrix() const;

	/**
		@brief	投影行列を設定する。
	*/
	void SetProjectionMatrix( const ::Effekseer::Matrix44& mat );

	/**
		@brief	カメラ行列を取得する。
	*/
	const ::Effekseer::Matrix44& GetCameraMatrix() const;

	/**
		@brief	カメラ行列を設定する。
	*/
	void SetCameraMatrix( const ::Effekseer::Matrix44& mat );

	/**
		@brief	カメラプロジェクション行列を取得する。
	*/
	::Effekseer::Matrix44& GetCameraProjectionMatrix();

	::Effekseer::Vector3D GetCameraFrontDirection() const override;

	::Effekseer::Vector3D GetCameraPosition() const  override;

	void SetCameraParameter(const ::Effekseer::Vector3D& front, const ::Effekseer::Vector3D& position)  override;

	/**
		@brief	スプライトレンダラーを生成する。
	*/
	::Effekseer::SpriteRenderer* CreateSpriteRenderer();

	/**
		@brief	リボンレンダラーを生成する。
	*/
	::Effekseer::RibbonRenderer* CreateRibbonRenderer();
	
	/**
		@brief	リングレンダラーを生成する。
	*/
	::Effekseer::RingRenderer* CreateRingRenderer();

	/**
		@brief	モデルレンダラーを生成する。
	*/
	::Effekseer::ModelRenderer* CreateModelRenderer();

	/**
		@brief	軌跡レンダラーを生成する。
	*/
	::Effekseer::TrackRenderer* CreateTrackRenderer();

	/**
		@brief	テクスチャ読込クラスを生成する。
	*/
	::Effekseer::TextureLoader* CreateTextureLoader( ::Effekseer::FileInterface* fileInterface = NULL );

	/**
		@brief	モデル読込クラスを生成する。
	*/
	::Effekseer::ModelLoader* CreateModelLoader( ::Effekseer::FileInterface* fileInterface = NULL );

	/**
	@brief	背景を取得する。
	*/
	Effekseer::TextureData* GetBackground() override 
	{
		if (m_background.UserPtr == nullptr) return nullptr;
		return &m_background;
	}

	/**
	@brief	背景を設定する。
	*/
	void SetBackground(IDirect3DTexture9* background) override;

	EffekseerRenderer::DistortingCallback* GetDistortingCallback() override;

	void SetDistortingCallback(EffekseerRenderer::DistortingCallback* callback) override;

	/**
	@brief	描画モードを設定する。
	*/
	void SetRenderMode( Effekseer::RenderMode renderMode ) override
	{
		m_renderMode = renderMode;
	}
	Effekseer::RenderMode GetRenderMode() override
	{
		return m_renderMode;
	}

	EffekseerRenderer::StandardRenderer<RendererImplemented, Shader, Vertex, VertexDistortion>* GetStandardRenderer() { return m_standardRenderer; }

	void SetVertexBuffer( VertexBuffer* vertexBuffer, int32_t size );
	void SetVertexBuffer(IDirect3DVertexBuffer9* vertexBuffer, int32_t size);
	void SetIndexBuffer( IndexBuffer* indexBuffer );
	void SetIndexBuffer(IDirect3DIndexBuffer9* indexBuffer);

	void SetLayout( Shader* shader );
	void DrawSprites( int32_t spriteCount, int32_t vertexOffset );
	void DrawPolygon( int32_t vertexCount, int32_t indexCount);

	Shader* GetShader(bool useTexture, bool useDistortion) const;
	void BeginShader(Shader* shader);
	void EndShader(Shader* shader);

	void SetVertexBufferToShader(const void* data, int32_t size);

	void SetPixelBufferToShader(const void* data, int32_t size);

	void SetTextures(Shader* shader, Effekseer::TextureData** textures, int32_t count);

	void ChangeDevice( LPDIRECT3DDEVICE9 device );

	void ResetRenderState();

	int32_t GetDrawCallCount() const override;

	int32_t GetDrawVertexCount() const override;

	void ResetDrawCallCount() override;

	void ResetDrawVertexCount() override;

	virtual int GetRef() { return ::Effekseer::ReferenceObject::GetRef(); }
	virtual int AddRef() { return ::Effekseer::ReferenceObject::AddRef(); }
	virtual int Release() { return ::Effekseer::ReferenceObject::Release(); }

private:
	void GenerateIndexData();
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_DX9_RENDERER_IMPLEMENTED_H__