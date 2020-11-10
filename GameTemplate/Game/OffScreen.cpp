#include "stdafx.h"
#include "OffScreen.h"
#include "graphics\normal\NormalMap.h"
#include "graphics\normal\EdgeDetection.h"
#include "graphics\depthvalue\DepthValueMap.h"
#include < locale.h >

namespace {
	const float speed = 30.0f;
	const float distance = 5.0f;		//大きくすればモデルとカメラの距離が大きくなる
	const float divide = 1.65f;			//大きくすればカメラのy座標が小さくなる
	const float angleMagY = 3.3f;		//画角の倍率
	const float angleMagXZ = 2.3f;		//画角の倍率
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
	//小窓用のレンダーターゲットを初期化
	m_offRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
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
	
	m_sprite.Init(L"Resource/sprite/komado.dds");
	
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

void OffScreen::PostRender()
{
	//バトルモードは小窓表示しない
	if (Engine().GetGraphicsEngine().GetSplitNumber() != 1) {
		return;
	}
	m_degree += GameTime().GetFrameDeltaTime() * speed;
	m_rot.SetRotationDeg(CVector3::AxisY(), m_degree);	
	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//レンダリングターゲットを切り替える。
	ID3D11RenderTargetView* rts[] = {
		m_offRenderTarget.GetRenderTargetView(),
		Engine().GetGraphicsEngine().GetNormalMap()->GetRenderTarget()->GetRenderTargetView(),
		Engine().GetGraphicsEngine().GetDepthValueMap()->GetRenderTarget()->GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(3, rts, m_offRenderTarget.GetDepthStensilView());
	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_offRenderTarget.GetViewport());
	d3dDeviceContext->RSSetState(m_rasterizerState);
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
	float clearColor2[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_offRenderTarget.ClearRenderTarget(clearColor);
	Engine().GetGraphicsEngine().GetNormalMap()->GetRenderTarget()->ClearRenderTarget(clearColor2);
	Engine().GetGraphicsEngine().GetDepthValueMap()->GetRenderTarget()->ClearRenderTarget(clearColor2);

	m_sprite.Draw();

	//モデルが設定されていたら
	if (m_skinModel != nullptr) {
		//モデルにあわせてカメラの座標を設定
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
		//を参照
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
	//シーンをテクスチャとする。
	auto srv = m_offRenderTarget.GetRenderTargetSRV();
	d3dDeviceContext->PSSetShaderResources(0, 1, &srv);
	m_postEffect.DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vs, m_ps);
	ID3D11ShaderResourceView* s[] = { NULL };
	d3dDeviceContext->PSSetShaderResources(0, 1, s);
	if (m_skinModel != nullptr) {
		//オブジェクトの名前を表示する
		wchar_t output[256];
		size_t wLen = 0;
		errno_t err = 0;
		setlocale(LC_ALL, "japanese");
		err = mbstowcs_s(&wLen, output, 20, m_objData->s_jName, _TRUNCATE);
		m_font.DrawScreenPos(output, fontPosition, CVector4::Yellow(), fontSize);
	}
}

