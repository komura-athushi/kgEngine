#include "stdafx.h"
#include "Collection.h"
#include < locale.h >
#include "StageSelect.h"
#include "sound/SoundSource.h"
Collection::Collection()
{

}

Collection::~Collection()
{
	Engine().GetGraphicsEngine().SetisEdge(true);
}

bool Collection::Start()
{
	const float OFF_BUFFER_W = 500.0f;
	const float OFF_BUFFER_H = 500.0f;

	m_offRenderTarget.Create(OFF_BUFFER_W, OFF_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
	//m_skinModel.Init(L"Resource/modelData/zunko.cmo");
	m_vs.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/bloom.fx", "PSMain", Shader::EnType::PS);
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	float MimX = -0.3f;
	float MaxX = 0.6f;
	float MimY = -0.7f;
	float MaxY = 0.7f;
	float DifferenceX = MaxX - MimX;
	float DifferenceY = MaxY - MimY;
	//ラスタライザとビューポートを初期化。
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateRasterizerState(&desc, &m_rasterizerState);
	InitSamplerState();
	for (int i = 0; i < H_NUMBER; i++) {
		for (int j = 0; j < W_NUMBER; j++) {
			m_postEffect[i * W_NUMBER + j].InitScreenQuadPrimitive(
				CVector2(MimX + ((DifferenceX / W_NUMBER) * j), MaxY - (DifferenceY / H_NUMBER) - (DifferenceY / H_NUMBER) * i),
			    CVector2(MimX + (DifferenceX / W_NUMBER) + (( DifferenceX / W_NUMBER) * j), MaxY - (DifferenceY / H_NUMBER) - (DifferenceY / H_NUMBER) * i),
				CVector2(MimX + (DifferenceX / W_NUMBER) * j, MaxY - (DifferenceY / H_NUMBER) * i),
				CVector2(MimX + (DifferenceX / W_NUMBER) + ((DifferenceX / W_NUMBER) * j), MaxY - (DifferenceY / H_NUMBER) * i));
		}
	}

	m_mainPostEffect.InitScreenQuadPrimitive(
		CVector2(-1.0f,-0.1f),
		CVector2(-0.4f,-0.1f),
		CVector2(-1.0f,1.0f),
		CVector2(-0.4f,1.0f)
	);

	m_modelList = ObjectData::GetInstance().GetList();
	m_listSize = m_modelList.size();

	m_sprite.Init(L"Resource/sprite/tekitou.dds");
	m_haikei.Init(L"Resource/sprite/collection.dds");
	m_collectionCursor.Init(L"Resource/sprite/collectioncursor.dds");
	m_model.Init(L"Resource/modelData/a.cmo");

	m_maximumPage = (m_listSize - 1) / (W_NUMBER * H_NUMBER) + 1;
	m_finalPageNumber = m_listSize % m_maximumPage;
	for (auto itr : m_modelList) {
		if (itr.second->s_isHit) {
			m_hitNumber++;
		}
	}

	Engine().GetGraphicsEngine().SetisEdge(false);
	return true;
}

void Collection::InitSamplerState()
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

void Collection::Draw()
{
	m_haikei.Draw();

	if (Engine().GetPad(0).IsTrigger(enButtonLeft)) {
		if (m_cursorNumber % W_NUMBER == 1) {
			if (m_page != m_mimmumPage) {
				m_page--;
				m_cursorNumber -= W_NUMBER * H_NUMBER - W_NUMBER + 1;
			}
		}
		else {
			m_cursorNumber--;
		}
	}
	else if (Engine().GetPad(0).IsTrigger(enButtonRight)) {
		if (m_cursorNumber % W_NUMBER == 0) {
			if (m_page != m_maximumPage) {
				m_page++;
				m_cursorNumber += W_NUMBER * H_NUMBER - W_NUMBER + 1;
				if (m_cursorNumber > m_listSize) {
					m_cursorNumber = m_listSize;
				}
			}
		}
		else {
			m_cursorNumber++;
		}
	}
	else if (Engine().GetPad(0).IsTrigger(enButtonUp)) {
		if ((m_cursorNumber - 1 - ((m_page - m_mimmumPage) * W_NUMBER * H_NUMBER)) / W_NUMBER != 0) {
			m_cursorNumber -= W_NUMBER;
		}
	}
	else if (Engine().GetPad(0).IsTrigger(enButtonDown)) {
		if ((m_cursorNumber - 1 - ((m_page - m_mimmumPage) * W_NUMBER * H_NUMBER)) / W_NUMBER != H_NUMBER - 1) {
			m_cursorNumber += W_NUMBER;
		}
	}
	m_degree += GameTime().GetFrameDeltaTime() * 30.0f;
	m_rot.SetRotationDeg(CVector3::AxisY(), m_degree);

	OffScreenRender();
	if (Engine().GetPad(0).IsTrigger(enButtonB)) {
		NewGO<StageSelect>(0);
		DeleteGO(this);
		CSoundSource* se = new CSoundSource();
		se->Init(L"Assets/sound/kettei.wav");
		se->Play(false);
	}
}

void Collection::OffScreenRender()
{
	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	int i = 0;
	for (auto itr : m_modelList) {
		i++;
		if (i == W_NUMBER * H_NUMBER * m_page + 1) {
			break;
		}
		if (i < W_NUMBER * H_NUMBER * (m_page - 1) + 1)
			continue;
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

		if (m_cursorNumber == i) {
			m_collectionCursor.Draw();
		}
		else {
			m_sprite.Draw();
		}

		//if (itr.second->s_isHit) {
			float x = itr.second->s_x * 6.0f;
			float z = itr.second->s_z * 6.0f;
			float y = itr.second->s_y * 6.0f;
			if (x >= z && x >= y) {
				m_offScreenCamera.SetPosition(CVector3(0.0f, x / 2.0f, x));
			}
			else if (z >= y && z >= x) {
				m_offScreenCamera.SetPosition(CVector3(0.0f, z / 2.0f, z));
			}
			else {
				m_offScreenCamera.SetPosition(CVector3(0.0f, y / 2.0f, y));
			}

			//float angle = atanf((itr.second->s_y * 1.1f) / (m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z) * 1.0f);
			float angle = atan2f(itr.second->s_y * 3.4f, m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z);
			float angle2;
			if (itr.second->s_x >= itr.second->s_z) {
				angle2 = atan2f(itr.second->s_x * 2.8f, m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z); // (FRAME_BUFFER_W / FRAME_BUFFER_H);
			}
			else {
				angle2 = atan2f(itr.second->s_z * 2.8f, m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z); // (FRAME_BUFFER_W / FRAME_BUFFER_H);
			}
			if (angle >= angle2) {
				m_offScreenCamera.SetViewAngle(angle);
			}
			else {
				m_offScreenCamera.SetViewAngle(angle2);
			}
			m_offScreenCamera.Update();
			if (!itr.second->s_isHit) {
				itr.second->s_skinModel.SetColor(CVector4(0.0f, 0.0f, 0.0f, 1.0f));
			}
			else {
				itr.second->s_skinModel.SetColor(CVector4::White());
			}
			itr.second->s_skinModel.UpdateWorldMatrix(m_position, m_rot, m_scale);
			itr.second->s_skinModel.Draw(m_offScreenCamera.GetViewMatrix(), m_offScreenCamera.GetProjectionMatrix());
		//}
		/*else {
			m_model.Draw(m_offScreenCamera.GetViewMatrix(), m_offScreenCamera.GetProjectionMatrix());
		}*/



		Engine().GetGraphicsEngine().ChangeRenderTarget(Engine().GetGraphicsEngine().GetMainRenderTarget(), Engine().GetGraphicsEngine().GetMainRenderTarget()->GetViewport());
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
		//シーンをテクスチャとする。///;5
		auto srv = m_offRenderTarget.GetRenderTargetSRV();
		d3dDeviceContext->PSSetShaderResources(0, 1, &srv);
		m_postEffect[(i - 1) % (H_NUMBER * W_NUMBER)].DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vs, m_ps);

		if (m_cursorNumber == i) {
			m_mainPostEffect.DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vs, m_ps);
			ID3D11ShaderResourceView* s[] = { NULL };
			d3dDeviceContext->PSSetShaderResources(0, 1, s);

			wchar_t output[256];
			if (itr.second->s_isHit) {
				size_t wLen = 0;
				errno_t err = 0;
				setlocale(LC_ALL, "japanese");
				err = mbstowcs_s(&wLen, output, 20, itr.second->s_jName, _TRUNCATE);
			}
			else {
				swprintf_s(output, L"？？？");
			}
			m_font.DrawScreenPos(output, CVector2(30.0f, 450.0f), CVector4::White(), { 1.0f,1.0 });
		}
	}
	ID3D11ShaderResourceView* s[] = { NULL };
	d3dDeviceContext->PSSetShaderResources(0, 1, s);
	wchar_t output2[256];
	swprintf_s(output2, L"モノの種類  %dコ\n", m_hitNumber);
	m_font.DrawScreenPos(output2, CVector2(500.0f, 50.0f), CVector4::White(), { 0.7f,0.7f });

}
