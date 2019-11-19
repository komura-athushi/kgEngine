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
	m_skinModel.Init(L"Resource/modelData/zunko.cmo");
	m_vs.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/bloom.fx", "PSMain", Shader::EnType::PS);
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	//���X�^���C�U�ƃr���[�|�[�g���������B
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateRasterizerState(&desc, &m_rasterizerState);
	return true;
}

void OffScreen::InitSamplerState()
{
	//�e�N�X�`���̃T���v�����O���@���w�肷�邽�߂̃T���v���X�e�[�g���쐬�B
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
	m_offScreenCamera.Update();
	m_skinModel.UpdateWorldMatrix(m_position, m_rot, m_scale);

	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//�����_�����O�^�[�Q�b�g��؂�ւ���B
	ID3D11RenderTargetView* rts[] = {
		m_offRenderTarget.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_offRenderTarget.GetDepthStensilView());
	//�r���[�|�[�g��ݒ�B
	d3dDeviceContext->RSSetViewports(1, m_offRenderTarget.GetViewport());
	d3dDeviceContext->RSSetState(m_rasterizerState);
	//��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha
	m_offRenderTarget.ClearRenderTarget(clearColor);
	m_skinModel.Draw(m_offScreenCamera.GetViewMatrix(),m_offScreenCamera.GetProjectionMatrix());

	Engine().GetGraphicsEngine().ChangeRenderTarget(Engine().GetGraphicsEngine().GetMainRenderTarget(),Engine().GetGraphicsEngine().GetMainRenderTarget()->GetViewport());
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//�V�[�����e�N�X�`���Ƃ���B
	auto srv = m_offRenderTarget.GetRenderTargetSRV();
	d3dDeviceContext->PSSetShaderResources(0, 1, &srv);
	m_postEffect.DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vs, m_ps);

}

