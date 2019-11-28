#include "stdafx.h"
#include "OffScreen.h"

OffScreen::OffScreen()
{

}

OffScreen::~OffScreen()
{

}

bool OffScreen::Start()
{
	m_offRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
	//m_skinModel.Init(L"Resource/modelData/zunko.cmo");
	m_vs.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/bloom.fx", "PSMain", Shader::EnType::PS);
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	//ラスタライザとビューポートを初期化。
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateRasterizerState(&desc, &m_rasterizerState);
	InitSamplerState();
	m_postEffect.InitScreenQuadPrimitive(CVector2(0.5f, -1.0f), CVector2(1.0f, -1.0f), CVector2(0.5f, -0.5f), CVector2(1.0f, -0.5f));
	return true;
}

void OffScreen::InitSamplerState()
{
	//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}

void OffScreen::Draw()
{
	m_degree += GameTime().GetFrameDeltaTime() * 30.0f;
	m_rot.SetRotationDeg(CVector3::AxisY(), m_degree);

	m_offScreenCamera.Update();
	
	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//レンダリングターゲットを切り替える。
	ID3D11RenderTargetView* rts[] = {
		m_offRenderTarget.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_offRenderTarget.GetDepthStensilView());
	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_offRenderTarget.GetViewport());
	d3dDeviceContext->RSSetState(m_rasterizerState);
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 0.7f, 0.7f, 0.7f, 1.0f }; //red,green,blue,alpha
	m_offRenderTarget.ClearRenderTarget(clearColor);

	if (m_skinModel != nullptr) {
		m_skinModel->UpdateWorldMatrix(m_position, m_rot, m_scale);
		m_skinModel->Draw(m_offScreenCamera.GetViewMatrix(), m_offScreenCamera.GetProjectionMatrix());
	}


	Engine().GetGraphicsEngine().ChangeRenderTarget(Engine().GetGraphicsEngine().GetMainRenderTarget(),Engine().GetGraphicsEngine().GetMainRenderTarget()->GetViewport());
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//シーンをテクスチャとする。
	auto srv = m_offRenderTarget.GetRenderTargetSRV();
	d3dDeviceContext->PSSetShaderResources(0, 1, &srv);
	m_postEffect.DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vs, m_ps);
	ID3D11ShaderResourceView* s[] = { NULL };
	d3dDeviceContext->PSSetShaderResources(0, 1, s);

}

