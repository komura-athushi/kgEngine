#include "KGstdafx.h"
#include "Bloom.h"
#include "PostEffect.h"

Bloom::Bloom()
{
	//各種レンダリングターゲットの初期化。
	InitRenderTarget();
	//シェーダーを初期化。
	InitShader();
	//αブレンドステートを初期化。
	InitAlphaBlendState();
	//サンプラステートの初期化。
	InitSamplerState();

	//輝度テクスチャをぼかすためのガウシアンブラーを初期化する。
	ID3D11ShaderResourceView* srcBlurTexture = m_luminanceRT.GetRenderTargetSRV();
	for (auto& gaussianBlur : m_gausianBlur) {
		gaussianBlur.Init(srcBlurTexture, 25.0f);
		//次のガウスブラーで使用するソーステクスチャを設定する。
		srcBlurTexture = gaussianBlur.GetResultTextureSRV();
	}
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	//ラスタライザとビューポートを初期化。
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateRasterizerState(&desc, &m_rasterizerState);
}

Bloom::~Bloom()
{
	if (m_disableBlendState != nullptr) {
		m_disableBlendState->Release();
	}

	if (m_samplerState != nullptr) {
		m_samplerState->Release();
	}
	if (m_finalBlendState != nullptr) {
		m_finalBlendState->Release();
	}
}

void Bloom::InitSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}

void Bloom::InitShader()
{
	m_vs.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_psLuminance.Load("Assets/shader/bloom.fx", "PSSamplingLuminance", Shader::EnType::PS);
	m_psFinal.Load("Assets/shader/bloom.fx", "PSFinal", Shader::EnType::PS);
	///////////////////////////////////////////////////////////////////////////
	//最適化のポイント②
	//ボケ画像合成用のピクセルシェーダーをロードする。
	m_psCombine.Load("Assets/shader/bloom.fx", "PSCombine", Shader::EnType::PS);
}

void Bloom::InitRenderTarget()
{
	//輝度抽出用のレンダリングターゲットを作成する。
	m_luminanceRT.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
	///////////////////////////////////////////////////////////////////////////
	//最適化のポイント① 
	//フレームバッファの1/2の解像度のボケ画像合成用のレンダリングターゲットを
	//作成する。
	//ぼかした画像を最終合成するためのレンダリングターゲットを作成する。
	//Q. なぜ1/2の解像度？
	//A. ガウシアンブラーで作成したボケ画像で、最も高い解像度がフレームバッファの
	//   1/2になっているから。
	///////////////////////////////////////////////////////////////////////////
	m_blurCombineRT.Create(
		FRAME_BUFFER_W / 2,
		FRAME_BUFFER_H / 2,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
}

void Bloom::InitAlphaBlendState()
{
	CD3D11_DEFAULT defaultSettings;
	//デフォルトセッティングで初期化する。
	CD3D11_BLEND_DESC blendDesc(defaultSettings);
	auto device = Engine().GetGraphicsEngine().GetD3DDevice();

	device->CreateBlendState(&blendDesc, &m_disableBlendState);

	//最終合成用のブレンドステートを作成する。
	//最終合成は加算合成。
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	device->CreateBlendState(&blendDesc, &m_finalBlendState);
}

void Bloom::Draw(PostEffect& postEffect)
{
	auto deviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//まずは輝度を抽出する。
	{
		//αブレンドを無効にする。
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
		deviceContext->RSSetState(m_rasterizerState);
		//輝度抽出用のレンダリングターゲットに変更する。
		Engine().GetGraphicsEngine().ChangeRenderTarget(&m_luminanceRT, m_luminanceRT.GetViewport());
		//レンダリングターゲットのクリア。
		float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_luminanceRT.ClearRenderTarget(clearColor);

		//シーンをテクスチャとする。
		auto mainRTTexSRV = Engine().GetGraphicsEngine().GetMainRenderTarget()->GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(0, 1, &mainRTTexSRV);

		//フルスクリーン描画。
		postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vs, m_psLuminance);
	}
	//続いて、輝度テクスチャをガウシアンブラーでぼかす。
	{
		for (auto& gaussianBlur : m_gausianBlur) {
			gaussianBlur.Execute(postEffect);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	//最適化のポイント③
	//ガウスブラーでぼかした画像を1/2の解像度のレンダリングターゲットを使用して、
	//合成する。
	///////////////////////////////////////////////////////////////////////////
	{
		//レンダリングターゲットをぼかし画像合成用のモノにする。
		Engine().GetGraphicsEngine().ChangeRenderTarget(&m_blurCombineRT, m_blurCombineRT.GetViewport());
		//ガウシアンブラーをかけたテクスチャをt0～t3レジスタに設定する。
		for (int registerNo = 0; registerNo < NUM_DOWN_SAMPLE; registerNo++) {
			auto srv = m_gausianBlur[registerNo].GetResultTextureSRV();
			deviceContext->PSSetShaderResources(registerNo, 1, &srv);
		}

		//フルスクリーン描画。
		postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vs, m_psCombine);
	}
	//最後にぼかした絵を加算合成でメインレンダリングターゲットに合成して終わり。
	{
		auto mainRT = Engine().GetGraphicsEngine().GetMainRenderTarget();
		Engine().GetGraphicsEngine().ChangeRenderTarget(mainRT, mainRT->GetViewport());

		//合成したボケテクスチャのアドレスをt0レジスタに設定する。
		auto srv = m_blurCombineRT.GetRenderTargetSRV();
		deviceContext->PSSetShaderResources(0, 1, &srv);

		//加算合成用のブレンディングステートを設定する。
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->OMSetBlendState(m_finalBlendState, blendFactor, 0xffffffff);
		//フルスクリーン描画。
		postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vs, m_psFinal);

		//ブレンディングステートを戻す。
		deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);

	}

}