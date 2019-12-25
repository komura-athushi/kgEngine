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
	//�f�t�H���g�Z�b�e�B���O�ŏ���������B
	CD3D11_BLEND_DESC blendDesc(defaultSettings);
	auto device = Engine().GetGraphicsEngine().GetD3DDevice();

	device->CreateBlendState(&blendDesc, &m_disableBlendState);

	//�ŏI�����p�̃u�����h�X�e�[�g���쐬����B
	//�ŏI�����͉��Z�����B
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
	device->CreateBlendState(&blendDesc, &m_finalBlendState);

	
}

void EdgeDetection::InitGaussian(NormalMap* normalMap)
{
	//�P�x�e�N�X�`�����ڂ������߂̃K�E�V�A���u���[������������B
	ID3D11ShaderResourceView* srcBlurTexture = normalMap->GetNormalMapSRV();
	
	m_gaussianBlur.Init(srcBlurTexture, 0.5f);

	m_gaussianBlur.SetResolution(FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT);
}

EdgeDetection::~EdgeDetection()
{

}

void EdgeDetection::EdgeRender(PostEffect& postEffect)
{
	m_gaussianBlur.Execute(postEffect);

	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//�����_�����O�^�[�Q�b�g��؂�ւ���B
	ID3D11RenderTargetView* rts[] = {
		m_edgeMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_edgeMapRT.GetDepthStensilView());
	//�r���[�|�[�g��ݒ�B
	d3dDeviceContext->RSSetViewports(1, m_edgeMapRT.GetViewport());

	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);

	//�V���h�E�}�b�v���N���A�B
	//��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_edgeMapRT.ClearRenderTarget(clearColor);

	ID3D11ShaderResourceView* srvArray[]{
		m_gaussianBlur.GetResultTextureSRV()
	};
	//�������|�C���^�̃|�C���^�At2�Ȃ̂ň�����2�A1�ɂ��Ă�
	d3dDeviceContext->PSSetShaderResources(0, 1, srvArray);
	
	//�t���X�N���[���`��B
	postEffect.DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vsShader, m_psShader);

	
}

void EdgeDetection::Draw(PostEffect& postEffect)
{
	auto mainRT = Engine().GetGraphicsEngine().GetMainRenderTarget();
	Engine().GetGraphicsEngine().ChangeRenderTarget(mainRT, mainRT->GetViewport());

	auto deviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();

	//���������{�P�e�N�X�`���̃A�h���X��t0���W�X�^�ɐݒ肷��B
	auto srv = m_edgeMapRT.GetRenderTargetSRV();
	deviceContext->PSSetShaderResources(0, 1, &srv);

	//���Z�����p�̃u�����f�B���O�X�e�[�g��ݒ肷��B
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(m_finalBlendState, blendFactor, 0xffffffff);
	//�t���X�N���[���`��B
	postEffect.DrawFullScreenQuadPrimitive(deviceContext, m_vs, m_psFinal);

	//�u�����f�B���O�X�e�[�g��߂��B
	deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
}