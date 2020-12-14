#include "stdafx.h"
#include "Collection.h"
#include < locale.h >
#include "StageSelect.h"
#include "sound/SoundSource.h"
#include "graphics\normal\NormalMap.h"
#include "graphics\normal\EdgeDetection.h"
#include "graphics\depthvalue\DepthValueMap.h"
#include "Fade.h"


namespace {
	//���f���`��̂��߂̃|�X�g�G�t�F�N�g�֘A
	const float OFF_BUFFER_W = 1280.0f;
	const float OFF_BUFFER_H = 720.0f;
	float MimX = -0.3f;
	float MaxX = 0.6f;
	float MimY = -0.7f;
	float MaxY = 0.7f;
	float degreeSpeed = 30.0f;
	//�J�[�\���֘A
	CVector2 cursorPos = CVector2(640.0f, 360.0f);
	CVector2 cursorSize = CVector2(4.266666f, 2.4f);
	//���f������J�����ɋ������Ƃ点��
	float distance = 6.0f;
	//�J�����̉�p�␳
	float angleX = 3.0f;
	float angleY = 3.9f;
	float angleZ = 3.0f;
	//���f���������\�����邽�߂̏�Z�J���[
	CVector4 multColor = CVector4(0.25f, 0.25f, 0.25f, 1.0f);
	//�t�H���g�̍��W
	CVector2 fontPos = CVector2(30.0f, 450.0f);
	CVector2 fontPos2 = CVector2(500.0f, 50.0f);
	//�t�H���g�̃T�C�Y
	CVector2 fontSize = CVector2(0.7f, 0.7f);
}

Collection::Collection()
{

}

Collection::~Collection()
{
	//�G�b�W�`����I���ɂ���B
	Engine().GetGraphicsEngine().SetisEdge(true);
}

bool Collection::Start()
{
	//�����_�[�^�[�Q�b�g�̏�����
	m_offRenderTarget.Create(OFF_BUFFER_W, OFF_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_vs.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/bloom.fx", "PSMain", Shader::EnType::PS);
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	float differenceX = MaxX - MimX;
	float differenceY = MaxY - MimY;
	//���X�^���C�U�ƃr���[�|�[�g���������B
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateRasterizerState(&desc, &m_rasterizerState);
	InitSamplerState();
	//�|�X�g�G�t�F�N�g�̏�����
	for (int i = 0; i < H_NUMBER; i++) {
		for (int j = 0; j < W_NUMBER; j++) {
			m_postEffect[i * W_NUMBER + j].InitScreenQuadPrimitive(
				CVector2(MimX + ((differenceX / W_NUMBER) * j), MaxY - (differenceY / H_NUMBER) - (differenceY / H_NUMBER) * i),
			    CVector2(MimX + (differenceX / W_NUMBER) + (( differenceX / W_NUMBER) * j), MaxY - (differenceY / H_NUMBER) - (differenceY / H_NUMBER) * i),
				CVector2(MimX + (differenceX / W_NUMBER) * j, MaxY - (differenceY / H_NUMBER) * i),
				CVector2(MimX + (differenceX / W_NUMBER) + ((differenceX / W_NUMBER) * j), MaxY - (differenceY / H_NUMBER) * i));
		}
	}

	//���[�p�̃|�X�g�G�t�F�N�g�̏�����
	m_mainPostEffect.InitScreenQuadPrimitive(
		CVector2(-1.0f,-0.1f),
		CVector2(-0.4f,-0.1f),
		CVector2(-1.0f,1.0f),
		CVector2(-0.4f,1.0f)
	);

	m_modelList = &ObjectData::GetInstance().GetList();
	m_listSize = m_modelList->size();

	//�e��摜�����[�h����
	m_sprite.Init(L"Resource/sprite/tekitou.dds");
	m_haikei.Init(L"Resource/sprite/collection.dds");
	m_collectionCursor.Init(L"Resource/sprite/collectioncursor.dds");
	m_model.Init(L"Resource/modelData/a.cmo");

	m_maximumPage = (m_listSize - 1) / (W_NUMBER * H_NUMBER) + 1;
	m_finalPageNumber = m_listSize % m_maximumPage;
	//�������񂾂��̂̐�������
	for (const auto& itr : *m_modelList) {
		if (itr.second->s_isHit) {
			m_hitNumber++;
		}
	}

	Engine().GetGraphicsEngine().SetisEdge(false);

	m_fade = &Fade::GetInstance();
	m_fade->StartFadeIn();
	return true;
}

void Collection::InitSamplerState()
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

void Collection::UpdateCursor()
{
	//�\���{�^����������J�[�\�����ړ�
	//��
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
	//�E	
	else if (Engine().GetPad(0).IsTrigger(enButtonRight)) {
		if (m_cursorNumber % W_NUMBER == 0) {
			if (m_page != m_maximumPage) {
				m_page++;
				m_cursorNumber += W_NUMBER * H_NUMBER - W_NUMBER + 1;
				if (m_cursorNumber > m_listSize) {
					if (m_listSize % 4 > 0) {
						m_cursorNumber = m_listSize - m_listSize % 4 + 1;
					}
					else {
						m_cursorNumber = m_listSize - W_NUMBER + 1;
					}
					if (m_cursorNumber > m_listSize) {
						m_cursorNumber = m_listSize;
					}
				}
			}
		}
		else {
			m_cursorNumber++;
		}
	}
	//��
	else if (Engine().GetPad(0).IsTrigger(enButtonUp)) {
		if ((m_cursorNumber - 1 - ((m_page - m_mimmumPage) * W_NUMBER * H_NUMBER)) / W_NUMBER != 0) {
			m_cursorNumber -= W_NUMBER;
		}
	}
	//��
	else if (Engine().GetPad(0).IsTrigger(enButtonDown)) {
		if ((m_cursorNumber - 1 - ((m_page - m_mimmumPage) * W_NUMBER * H_NUMBER)) / W_NUMBER != H_NUMBER - 1) {
			m_cursorNumber += W_NUMBER;
		}
	}
	//�J�[�\�������X�g�̃T�C�Y�������烊�X�g�̍ő�l�ɐݒ肷��B
	if (m_cursorNumber > m_listSize) {
		m_cursorNumber = m_listSize;
	}
	else if (m_cursorNumber == 0) {
		m_cursorNumber = 1;
	}
}

void Collection::Update()
{
	UpdateCursor();
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			NewGO<StageSelect>(0);
			DeleteGO(this);
		}
	}
	else {
		//B��������X�e�[�W�Z���N�g�ɑJ��
		if (Engine().GetPad(0).IsTrigger(enButtonB)) {
			CSoundSource* se = new CSoundSource();
			se->Init(L"Assets/sound/kettei.wav");
			se->Play(false);
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}
}

void Collection::Draw()
{
	m_haikei.Draw();

	//���f���̉�]
	m_degree += GameTime().GetFrameDeltaTime() * degreeSpeed;
	m_rot.SetRotationDeg(CVector3::AxisY(), m_degree);

	//�e�탂�f����`��
	OffScreenRender();
}

void Collection::UpdateCamera(SkinModel_ObjData* data)
{
	//���f���̑傫���ɂ���ăJ�����̍��W��ς���
	float x = data->s_x;
	float z = data->s_z;
	float y = data->s_y;
	if (x >= z && x >= y) {
		m_offScreenCamera.SetPosition(CVector3(0.0f, x * distance / 2.0f, x * distance));
	}
	else if (z >= y && z >= x) {
		m_offScreenCamera.SetPosition(CVector3(0.0f, z / 2.0f * distance, z * distance));
	}
	else {
		m_offScreenCamera.SetPosition(CVector3(0.0f, y / 2.0f * distance, y * distance));
	}

	//https://qiita.com/akurobit/items/a6dd03baef6c05d7eae8
	//���Q��
	float angle = atan2f(data->s_y * angleY, m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z);
	float angle2;
	//���f���̑傫���ɂ���ăJ�����̉�p��ς���
	if (data->s_x >= data->s_z) {
		angle2 = atan2f(data->s_x * angleX, m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z); // (FRAME_BUFFER_W / FRAME_BUFFER_H);
	}
	else {
		angle2 = atan2f(data->s_z * angleZ, m_offScreenCamera.GetPosition().z - m_offScreenCamera.GetTarget().z); // (FRAME_BUFFER_W / FRAME_BUFFER_H);
	}
	if (angle >= angle2) {
		m_offScreenCamera.SetViewAngle(angle);
	}
	else {
		m_offScreenCamera.SetViewAngle(angle2);
	}
	m_offScreenCamera.Update();
}

void Collection::OffScreenRender()
{
	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	int i = 0;
	//���f���}�b�v����
	for (const auto& itr : *m_modelList) {
		i++;
		//�y�[�W�̍Ō�܂ŉ񂵂���u���C�N
		if (i == W_NUMBER * H_NUMBER * m_page + 1) {
			break;
		}
		//�Y���̃y�[�W�܂ŉ񂷂܂ŉ��̏��������Ȃ�
		if (i < W_NUMBER * H_NUMBER * (m_page - 1) + 1)
			continue;

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
		//�����_�����O�^�[�Q�b�g��h��Ԃ�
		float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
		float clearColor2[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_offRenderTarget.ClearRenderTarget(clearColor);
		Engine().GetGraphicsEngine().GetNormalMap()->GetRenderTarget()->ClearRenderTarget(clearColor2);
		Engine().GetGraphicsEngine().GetDepthValueMap()->GetRenderTarget()->ClearRenderTarget(clearColor2);

		//�J�[�\���̏ꏊ��������\��������摜��ς���
		if (m_cursorNumber == i) {
			m_collectionCursor.DrawScreenPos(cursorPos, cursorSize);
		}
		else {
			m_sprite.DrawScreenPos(cursorPos, cursorSize);
		}

		//�J�����X�V
		UpdateCamera(itr.second.get());

		//��������łȂ����͍̂��F�ŕ\���������
		if (!itr.second->s_isHit) {
			itr.second->s_skinModel.SetColor(multColor);
		}
		else {
			itr.second->s_skinModel.SetColor(CVector4::White());
		}
		itr.second->s_skinModel.UpdateWorldMatrix(m_position, m_rot, m_scale);
		itr.second->s_skinModel.Draw(m_offScreenCamera.GetCamera(),enRenderMode_Normal, false);

		Engine().GetGraphicsEngine().GetEdgeDelection()->EdgeRender(*Engine().GetGraphicsEngine().GetPostEffect());
		Engine().GetGraphicsEngine().GetEdgeDelection()->Draw(*Engine().GetGraphicsEngine().GetPostEffect(), &m_offRenderTarget);


		Engine().GetGraphicsEngine().ChangeRenderTarget(Engine().GetGraphicsEngine().GetMainRenderTarget(), Engine().GetGraphicsEngine().GetMainRenderTarget()->GetViewport());
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
		//�V�[�����e�N�X�`���Ƃ���B
		auto srv = m_offRenderTarget.GetRenderTargetSRV();
		d3dDeviceContext->PSSetShaderResources(0, 1, &srv);
		m_postEffect[(i - 1) % (H_NUMBER * W_NUMBER)].DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vs, m_ps);

		//�J�[�\���Ɠ����ԍ��������獶��ɂ��\��������
		if (m_cursorNumber == i) {
			m_mainPostEffect.DrawFullScreenQuadPrimitive(d3dDeviceContext, m_vs, m_ps);
			ID3D11ShaderResourceView* s[] = { NULL };
			d3dDeviceContext->PSSetShaderResources(0, 1, s);

			wchar_t output[256];
			//��������ł郂�m�������疼�O�\��������
			if (itr.second->s_isHit) {
				//char�^��wchar_t�^�ɕϊ�
				size_t wLen = 0;
				errno_t err = 0;
				setlocale(LC_ALL, "japanese");
				err = mbstowcs_s(&wLen, output, 20, itr.second->s_jName, _TRUNCATE);
			}
			else {
				swprintf_s(output, L"�H�H�H");
			}
			m_font.DrawScreenPos(output, fontPos, CVector4::White(), { 1.0f,1.0 });
		}
	}
	ID3D11ShaderResourceView* s[] = { NULL };
	d3dDeviceContext->PSSetShaderResources(0, 1, s);
	wchar_t output2[256];
	swprintf_s(output2, L"�A�c���^���m  %d�R\n", m_hitNumber);
	m_font.DrawScreenPos(output2, fontPos2, CVector4::White(), fontSize);

}
