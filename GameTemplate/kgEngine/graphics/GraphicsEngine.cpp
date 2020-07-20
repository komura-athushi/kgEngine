#include "KGstdafx.h"
#include "GraphicsEngine.h"
#include "shadow/kgShadowMap.h"
#include "normal\NormalMap.h"
#include "normal\EdgeDetection.h"
#include "depthvalue\DepthValueMap.h"
#include "shadow\CascadeShadowMap.h"
#include "CEffektEngine.h"

GraphicsEngine::GraphicsEngine()
{
}


GraphicsEngine::~GraphicsEngine()
{
	Release();
}

void GraphicsEngine::BegineRender()
{
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
													  //�`�����ݐ���o�b�N�o�b�t�@�ɂ���B
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);
	//�o�b�N�o�b�t�@���D�F�œh��Ԃ��B
	m_pd3dDeviceContext->ClearRenderTargetView(m_backBuffer, ClearColor);
	m_pd3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	auto d3dDeviceContext = m_pd3dDeviceContext;
	//���݂̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//�r���[�|�[�g���o�b�N�A�b�v������Ă����B
	unsigned int numViewport = 1;
	if (!m_isSplit) {
		m_frameBufferViewports = m_normalViewPorts;
		//m_mainRenderTarget.SetViewPort(m_normalViewPorts);
	}
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);
}

void GraphicsEngine::EndRender()
{
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
	m_pSwapChain->Present(2, 0);
}
void GraphicsEngine::Release()
{
	if (m_rasterizerState != NULL) {
		m_rasterizerState->Release();
		m_rasterizerState = NULL;
	}
	if (m_depthStencil != NULL) {
		m_depthStencil->Release();
		m_depthStencil = NULL;
	}
	if (m_depthStencilView != NULL) {
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}
	if (m_backBuffer != NULL) {
		m_backBuffer->Release();
		m_backBuffer = NULL;
	}
	if (m_pSwapChain != NULL) {
		m_pSwapChain->Release();
		m_pSwapChain = NULL;
	}
	if (m_pd3dDeviceContext != NULL) {
		m_pd3dDeviceContext->Release();
		m_pd3dDeviceContext = NULL;
	}
	if (m_pd3dDevice != NULL) {
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
}
void GraphicsEngine::Init(HWND hWnd)
{
	//�X���b�v�`�F�C�����쐬���邽�߂̏���ݒ肷��B
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;									//�X���b�v�`�F�C���̃o�b�t�@���B�ʏ�͂P�B
	sd.BufferDesc.Width = (UINT)FRAME_BUFFER_W;			//�t���[���o�b�t�@�̕��B
	sd.BufferDesc.Height = (UINT)FRAME_BUFFER_H;		//�t���[���o�b�t�@�̍����B
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�t���[���o�b�t�@�̃t�H�[�}�b�g�BR8G8B8A8��32bit�B
	sd.BufferDesc.RefreshRate.Numerator = 60;			//���j�^�̃��t���b�V�����[�g�B(�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���^�C�~���O�ƂȂ�B)
	sd.BufferDesc.RefreshRate.Denominator = 1;			//�Q�ɂ�����30fps�ɂȂ�B1�ł����B
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�T�[�t�F�X�܂��̓��\�[�X���o�̓����_�[ �^�[�Q�b�g�Ƃ��Ďg�p���܂��B
	sd.OutputWindow = hWnd;								//�o�͐�̃E�B���h�E�n���h���B
	sd.SampleDesc.Count = 1;							//1�ł����B
	sd.SampleDesc.Quality = 0;							//MSAA�Ȃ��B0�ł����B
	sd.Windowed = TRUE;									//�E�B���h�E���[�h�BTRUE�ł悢�B

														//���p����DirectX�̋@�\�Z�b�g�BDirectX10�ȏ�ɑΉ����Ă���GPU�𗘗p�\�Ƃ���B
														//���̔z���D3D11CreateDeviceAndSwapChain�̈����Ƃ��Ďg���B
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//D3D�f�o�C�X�ƃX���b�v�`�F�C�����쐬����B
	D3D11CreateDeviceAndSwapChain(
		NULL,											//NULL�ł����B
		D3D_DRIVER_TYPE_HARDWARE,						//D3D�f�o�C�X���A�N�Z�X����h���C�o�[�̎�ށB
														//��{�I��D3D_DRIVER_TYPE_HARDWARE���w�肷��΂悢�B
		NULL,											//NULL�ł����B
		createDeviceFlags,												//�O�ł����B
		featureLevels,									//D3D�f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g���w�肷��B
														//����̃T���v����DirectX10�ȏ���T�|�[�g����̂ŁA
														//�������܂�D3D_FEATURE_LEVEL�̔z���n���B
		sizeof(featureLevels) / sizeof(featureLevels[0]),	//�@�\�Z�b�g�̐��B
		D3D11_SDK_VERSION,								//�g�p����DirectX�̃o�[�W�����B
														//D3D11_SDK_VERSION���w�肷��΂悢�B
		&sd,											//�X���b�v�`�F�C�����쐬���邽�߂̏��B
		&m_pSwapChain,									//�쐬�����X���b�v�`�F�C���̃A�h���X�̊i�[��B
		&m_pd3dDevice,									//�쐬����D3D�f�o�C�X�̃A�h���X�̊i�[��B
		&m_featureLevel,								//�g�p�����@�\�Z�b�g�̊i�[��B
		&m_pd3dDeviceContext							//�쐬����D3D�f�o�C�X�R���e�L�X�g�̃A�h���X�̊i�[��B
	);

	//�������ݐ�ɂȂ郌���_�����O�^�[�Q�b�g���쐬�B
	ID3D11Texture2D* pBackBuffer = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
	pBackBuffer->Release();
	//�[�x�X�e���V���r���[�̍쐬�B
	{
		//�[�x�e�N�X�`���̍쐬�B
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = (UINT)FRAME_BUFFER_W;
		texDesc.Height = (UINT)FRAME_BUFFER_H;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_D32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &m_depthStencil);
		//�[�x�X�e���V���r���[���쐬�B
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = texDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		m_pd3dDevice->CreateDepthStencilView(m_depthStencil, &descDSV, &m_depthStencilView);
	}
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;

	//���X�^���C�U�ƃr���[�|�[�g���������B
	m_pd3dDevice->CreateRasterizerState(&desc, &m_rasterizerState);

	m_normalViewPorts.Width = FRAME_BUFFER_W;
	m_normalViewPorts.Height = FRAME_BUFFER_H;
	m_normalViewPorts.TopLeftX = 0;
	m_normalViewPorts.TopLeftY = 0;
	m_normalViewPorts.MinDepth = 0.0f;
	m_normalViewPorts.MaxDepth = 1.0f;

	m_splitViewPorts[0].Width = FRAME_BUFFER_W;
	m_splitViewPorts[0].Height = FRAME_BUFFER_H / 2;
	m_splitViewPorts[0].TopLeftX = 0;
	m_splitViewPorts[0].TopLeftY = 0;
	m_splitViewPorts[0].MinDepth = 0.0f;
	m_splitViewPorts[0].MaxDepth = 1.0f;

	m_splitViewPorts[1].Width = FRAME_BUFFER_W;
	m_splitViewPorts[1].Height = FRAME_BUFFER_H / 2;
	m_splitViewPorts[1].TopLeftX = 0;
	m_splitViewPorts[1].TopLeftY = FRAME_BUFFER_H / 2;
	m_splitViewPorts[1].MinDepth = 0.0f;
	m_splitViewPorts[1].MaxDepth = 1.0f;

	m_pd3dDeviceContext->RSSetViewports(1, &m_normalViewPorts);
	m_pd3dDeviceContext->RSSetState(m_rasterizerState);
	m_shadowMap = new ShadowMap;
	m_cascadeShadowMap = new CascadeShadowMap;
	m_normalMap = new NormalMap;
	m_depthValueMao = new DepthValueMap;
	m_edgeDelection = new EdgeDetection;
	m_edgeDelection->InitGaussian(m_normalMap);
	//Sprite������
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_pd3dDevice, L"Assets/Font/myfile.spritefont");
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pd3dDeviceContext);
	m_spriteBatchPMA = std::make_unique<DirectX::SpriteBatch>(m_pd3dDeviceContext);
	
	//���C���ƂȂ郌���_�����O�^�[�Q�b�g���쐬����B
	m_mainRenderTarget.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
	m_copyMainRtToFrameBufferSprite.Init(m_mainRenderTarget.GetRenderTargetSRV());

	m_postEffect = new PostEffect();
	m_postEffect->InitFullScreenQuadPrimitive();

	CEffektEngine::GetInstance().InitEffekseer();
}

void GraphicsEngine::ShadowMapRender()
{
	for (int i = 0; i < GetSplitNumber(); i++) {
		//�V���h�E�}�b�v���X�V�B
		m_shadowMap->UpdateFromLightTarget(
			m_position[i],
			m_target[i],
			i
		);
	}
	m_cascadeShadowMap->Update();
	auto d3dDeviceContext = m_pd3dDeviceContext;
	//���݂̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	/*ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//�r���[�|�[�g���o�b�N�A�b�v������Ă����B
	unsigned int numViewport = 1;
	D3D11_VIEWPORT oldViewports;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);*/

	//�V���h�E�}�b�v�Ƀ����_�����O
	//m_shadowMap->RenderToShadowMap();
	m_cascadeShadowMap->RenderToShadowMap();

	//���ɖ߂��B
	/*d3dDeviceContext->OMSetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView
	);*/
	//d3dDeviceContext->RSSetViewports(numViewport, &m_frameBufferViewports);
	//�����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���̎Q�ƃJ�E���^�������B
	//oldRenderTargetView->Release();
	//oldDepthStencilView->Release();


}

void GraphicsEngine::EdgeDelectionRender()
{
	m_edgeDelection->EdgeRender(*m_postEffect);
	ChangeRenderTarget(&m_mainRenderTarget, m_mainRenderTarget.GetViewport());
}

void GraphicsEngine::ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport)
{
	ChangeRenderTarget(
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}
void GraphicsEngine::ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport)
{
	ID3D11RenderTargetView* rtTbl[] = {
		renderTarget
	};
	//�����_�����O�^�[�Q�b�g�̐؂�ւ��B
	m_pd3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//�r���[�|�[�g���w�肳��Ă�����A�r���[�|�[�g���ύX����B
		m_pd3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void GraphicsEngine::ChangeMainRenderTarget()
{
	//ChangeRenderTarget(&m_mainRenderTarget, &m_frameBufferViewports);

	ID3D11RenderTargetView* rt[] = {
		m_mainRenderTarget.GetRenderTargetView(),
		m_normalMap->GetRenderTarget()->GetRenderTargetView(),
		m_depthValueMao->GetRenderTarget()->GetRenderTargetView()
	};
	m_pd3dDeviceContext->OMSetRenderTargets(3, rt, m_mainRenderTarget.GetDepthStensilView());
	//m_pd3dDeviceContext->RSSetViewports(1, m_mainRenderTarget.GetViewport());
	m_pd3dDeviceContext->RSSetViewports(1, &m_frameBufferViewports);
	//���C�������_�����O�^�[�Q�b�g���N���A����B
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float clearColor2[] = { 1.0f,1.0f,1.0f,1.0f };
	m_mainRenderTarget.ClearRenderTarget(clearColor);
	m_normalMap->GetRenderTarget()->ClearRenderTarget(clearColor2);
	m_depthValueMao->GetRenderTarget()->ClearRenderTarget(clearColor2);
}

void GraphicsEngine::PostRender()
{
	//�|�X�g�G�t�F�N�g�̕`��B
	m_postEffect->Draw();

	if (m_isEdge) {
		m_edgeDelection->Draw(*m_postEffect);
	}

	//�����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɖ߂��B
	ChangeRenderTarget(
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);

	SetNormalViewPort();

	//�h���h��
	m_copyMainRtToFrameBufferSprite.Draw();

	//SetSplitViewPort(0);

	m_frameBufferRenderTargetView->Release();
	m_frameBufferDepthStencilView->Release();
}

void GraphicsEngine::EffektUpdate()
{
	auto effectEngine = CEffektEngine::GetInstance();

	effectEngine.Update();

	for (int i = 0; i < GetSplitNumber(); i++) {
		SetSplitViewPort(i);
		effectEngine.CameraEffekseer(i);
		effectEngine.Draw(i);

	}
}
