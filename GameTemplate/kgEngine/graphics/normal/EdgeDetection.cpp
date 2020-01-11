#include "KGstdafx.h"
#include "EdgeDetection.h"
#include "NormalMap.h"
#include "../depthvalue/DepthValueMap.h"

EdgeDetection::EdgeDetection()
{
	m_edgeMapRT.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);

	m_psShader.Load("Assets/shader/edgedelection.fx", "PSEdge", Shader::EnType::PS);
	m_vsShader.Load("Assets/shader/edgedelection.fx", "VSXEdge", Shader::EnType::VS);

	m_vs.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_psFinal.Load("Assets/shader/bloom.fx", "PSFinal", Shader::EnType::PS);

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);


	CD3D11_DEFAULT defaultSettings;
	//デフォルトセッティングで初期化する。
	CD3D11_BLEND_DESC blendDesc(defaultSettings);
	auto device = Engine().GetGraphicsEngine().GetD3DDevice();

	device->CreateBlendState(&blendDesc, &m_disableBlendState);

	//最終合成用のブレンドステートを作成する。
	//最終合成は加算合成。
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
	device->CreateBlendState(&blendDesc, &m_finalBlendState);

	
}

void EdgeDetection::InitGaussian(NormalMap* normalMap)
{
	//輝度テクスチャをぼかすためのガウシアンブラーを初期化する。
	ID3D11ShaderResourceView* srcBlurTexture = normalMap->GetNormalMapSRV();
	
	m_gaussianBlur.Init(srcBlurTexture, 0.0001f);

	m_gaussianBlur.SetResolution(FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT);
}

EdgeDetection::~EdgeDetection()
{

}

void EdgeDetection::EdgeRender(PostEffect& postEffect)
{
	//m_gaussianBlur.Execute(postEffect,false);

	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//レンダリングターゲットを切り替える。
	ID3D11RenderTargetView* rts[] = {
		m_edgeMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_edgeMapRT.GetDepthStensilView());
	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_edgeMapRT.GetViewport());

	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);

	//シャドウマップをクリア。
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_edgeMapRT.ClearRenderTarget(clearColor);

	/*ID3D11ShaderResourceView* srvArray[]{
		m_gaussianBlur.GetResultTextureSRV()
	};*/
	ID3D11ShaderResourceView* srvArray[]{
		Engine().GetGraphicsEngine().GetNormalMap()->GetNormalMapSRV()
	};
	//引数がポインタのポインタ、t2なので引数を2、1にしてる
	d3dDeviceContext->VSSetShaderResources(0, 1, srvArray);
	d3dDeviceContext->PSSetShaderResources(0, 1, srvArray);
	
	//深度値のテクスチャをt1に設定する
	auto srv2 = Engine().GetGraphicsEngine().GetDepthValueMap()->GetDepthValueMapSRV();
	d3dDeviceContext->PSSetShaderResources(1, 1, &srv2);

	//フルスクリーン描画。
	postEffect.DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vsShader, m_psShader);

	
}

void EdgeDetection::Draw(PostEffect& postEffect ,RenderTarget* renderTarget)
{
	if (renderTarget != nullptr) {
		Engine().GetGraphicsEngine().ChangeRenderTarget(renderTarget, renderTarget->GetViewport());
	}
	else {
		auto mainRT = Engine().GetGraphicsEngine().GetMainRenderTarget();
		Engine().GetGraphicsEngine().ChangeRenderTarget(mainRT, mainRT->GetViewport());
	}
	auto deviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();

	//合成したボケテクスチャのアドレスをt0レジスタに設定する。
	auto srv = m_edgeMapRT.GetRenderTargetSRV();
	deviceContext->PSSetShaderResources(0, 1, &srv);

	


	//加算合成用のブレンディングステートを設定する。
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(m_finalBlendState, blendFactor, 0xffffffff);
	//フルスクリーン描画。
	postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vs, m_psFinal);

	//ブレンディングステートを戻す。
	deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
}