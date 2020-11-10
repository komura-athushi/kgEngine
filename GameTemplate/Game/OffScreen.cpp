#include "stdafx.h"
#include "OffScreen.h"
#include "graphics\normal\NormalMap.h"
#include "graphics\normal\EdgeDetection.h"
#include "graphics\depthvalue\DepthValueMap.h"
#include < locale.h >

namespace {
	const float speed = 30.0f;
	const float distance = 5.0f;		//�傫������΃��f���ƃJ�����̋������傫���Ȃ�
	const float divide = 1.65f;			//�傫������΃J������y���W���������Ȃ�
	const float angleMagY = 3.3f;		//��p�̔{��
	const float angleMagXZ = 2.3f;		//��p�̔{��
	const CVector2 fontPosition = CVector2(1050.0f, 685);
	const CVector2 fontSize = CVector2(0.7f, 0.7f);
}

OffScreen::OffScreen()
{

}

OffScreen::~OffScreen()
{
	
}

bool OffScreen::Start()
{
	//�����p�̃����_�[�^�[�Q�b�g��������
	m_offRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_vs.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/bloom.fx", "PSMain", Shader::EnType::PS);
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	//���X�^���C�U�ƃr���[�|�[�g���������B
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateRasterizerState(&desc, &m_rasterizerState);
	InitSamplerState();
	m_postEffect.InitScreenQuadPrimitive(CVector2(0.5f, -1.0f), CVector2(1.0f, -1.0f), CVector2(0.5f, -0.5f), CVector2(1.0f, -0.5f));
	
	m_sprite.Init(L"Resource/sprite/komado.dds");
	
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

void OffScreen::PostRender()
{
	//�o�g�����[�h�͏����\�����Ȃ�
	if (Engine().GetGraphicsEngine().GetSplitNumber() != 1) {
		return;
	}
	m_degree += GameTime().GetFrameDeltaTime() * speed;
	m_rot.SetRotationDeg(CVector3::AxisY(), m_degree);	
	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//�����_�����O�^�[�Q�b�g��؂�ւ���B
	ID3D11RenderTargetView* rts[] = {
		m_offRenderTarget.GetRenderTargetView(),
		Engine().GetGraphicsEngine().GetNormalMap()->GetRenderTarget()->GetRenderTargetView(),
		Engine().GetGraphicsEngine().GetDepthValueMap()->GetRenderTarget()->GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(3, rts, m_offRenderTarget.GetDepthStensilView());
	//�r���[�|�[�g��ݒ�B
	d3dDeviceContext->RSSetViewports(1, m_offRenderTarget.GetViewport());
	d3dDeviceContext->RSSetState(m_rasterizerState);
	//��ԉ���Z��1.0�Ȃ̂ŁA1.0�œh��Ԃ��B
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
	float clearColor2[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_offRenderTarget.ClearRenderTarget(clearColor);
	Engine().GetGraphicsEngine().GetNormalMap()->GetRenderTarget()->ClearRenderTarget(clearColor2);
	Engine().GetGraphicsEngine().GetDepthValueMap()->GetRenderTarget()->ClearRenderTarget(clearColor2);

	m_sprite.Draw();

	//���f�����ݒ肳��Ă�����
	if (m_skinModel != nullptr) {
		//���f���ɂ��킹�ăJ�����̍��W��ݒ�
		float x = m_objData->s_x * distance;
		float z = m_objData->s_z * distance;
		float y = m_objData->s_y * distance;
		if (x >= z && x >= y) {
			m_offScreenCamera.SetPosition(CVector3(0.0f, x / divide, x));
		}
		else if (z >= y && z >= x) {
			m_offScreenCamera.SetPosition(CVector3(0.0f, z / divide, z));
		}
		else {
			m_offScreenCamera.SetPosition(CVector3(0.0f, y / divide, y));
		}
		//https://qiita.com/akurobit/items/a6dd03baef6c05d7eae8
		//���Q��
		float angle = atan2f(m_objData->s_y * angleMagY, m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z);
		float angle2;
		if (m_objData->s_x >= m_objData->s_z) {
			angle2 = atan2f(m_objData->s_x * angleMagXZ, m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z) / (FRAME_BUFFER_H / FRAME_BUFFER_W);
		}
		else {
			angle2 = atan2f(m_objData->s_z * angleMagXZ, m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z) / (FRAME_BUFFER_H / FRAME_BUFFER_W);
		}
		if (angle >= angle2) {
			m_offScreenCamera.SetViewAngle(angle);
		}
		else {
			m_offScreenCamera.SetViewAngle(angle2);
		}
		m_offScreenCamera.Update();
		m_skinModel->UpdateWorldMatrix(m_position, m_rot, m_scale);
		m_skinModel->Draw(m_offScreenCamera.GetCamera(), enRenderMode_Normal, false);

		Engine().GetGraphicsEngine().GetEdgeDelection()->EdgeRender(*Engine().GetGraphicsEngine().GetPostEffect());
		Engine().GetGraphicsEngine().GetEdgeDelection()->Draw(*Engine().GetGraphicsEngine().GetPostEffect(), &m_offRenderTarget);
	}

	Engine().GetGraphicsEngine().ChangeRenderTargetFrameBuffer();
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//�V�[�����e�N�X�`���Ƃ���B
	auto srv = m_offRenderTarget.GetRenderTargetSRV();
	d3dDeviceContext->PSSetShaderResources(0, 1, &srv);
	m_postEffect.DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vs, m_ps);
	ID3D11ShaderResourceView* s[] = { NULL };
	d3dDeviceContext->PSSetShaderResources(0, 1, s);
	if (m_skinModel != nullptr) {
		//�I�u�W�F�N�g�̖��O��\������
		wchar_t output[256];
		size_t wLen = 0;
		errno_t err = 0;
		setlocale(LC_ALL, "japanese");
		err = mbstowcs_s(&wLen, output, 20, m_objData->s_jName, _TRUNCATE);
		m_font.DrawScreenPos(output, fontPosition, CVector4::Yellow(), fontSize);
	}
}

