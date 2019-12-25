#include "KGstdafx.h"
#include "EdgeDetection.h"
#include "NormalMap.h"

EdgeDetection::EdgeDetection()
{
	m_edgeMapRT.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);

	m_psShader.Load("Assets/shader/edgedelection.fx", "PSEdge", Shader::EnType::PS);
	m_vsShader.Load("Assets/shader/edgedelection.fx", "VSXEdge", Shader::EnType::VS);

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}

EdgeDetection::~EdgeDetection()
{

}

void EdgeDetection::EdgeRender(PostEffect& postEffect)
{
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

	ID3D11ShaderResourceView* srvArray[]{
		Engine().GetGraphicsEngine().GetNormalMap()->GetNormalMapSRV()
	};
	//引数がポインタのポインタ、t2なので引数を2、1にしてる
	d3dDeviceContext->PSSetShaderResources(0, 1, srvArray);
	
	//フルスクリーン描画。
	postEffect.DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vsShader, m_psShader);
}