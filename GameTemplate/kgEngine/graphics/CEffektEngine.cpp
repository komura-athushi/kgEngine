#include "KGstdafx.h"
#include "CEffektEngine.h"



CEffektEngine::CEffektEngine()
{
	//m_attackEffect = Effekseer::Effect::Create(m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/test.efk");
}


CEffektEngine::~CEffektEngine()
{
}

//エフェクトの初期化。
void CEffektEngine::InitEffekseer()
{
	m_effekseerManager = Effekseer::Manager::Create(10000);
	m_effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
		Engine().GetGraphicsEngine().GetD3DDevice(),			//D3Dデバイス。
		Engine().GetGraphicsEngine().GetD3DDeviceContext(),	//D3Dデバイスコンテキスト。
		20000										//板ポリの最大数。
	);
	// 描画用インスタンスから描画機能を設定
	m_effekseerManager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
	m_effekseerManager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
	m_effekseerManager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
	m_effekseerManager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
	m_effekseerManager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	m_effekseerManager->SetTextureLoader(m_effekseerRenderer->CreateTextureLoader());
	m_effekseerManager->SetModelLoader(m_effekseerRenderer->CreateModelLoader());

	m_attackEffect = Effekseer::Effect::Create(m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/test.efk");
}
//カメラ行列
void CEffektEngine::CameraEffekseer(const int cameraNumber)
{
	Effekseer::Matrix44 efCameraMat;
	MainCamera(cameraNumber).GetViewMatrix().CopyTo(efCameraMat);
	Effekseer::Matrix44 efProjMat;
	MainCamera(cameraNumber).GetProjectionMatrix().CopyTo(efProjMat);
	//カメラ行列とプロジェクション行列を設定。
	m_effekseerRenderer->SetCameraMatrix(efCameraMat);
	m_effekseerRenderer->SetProjectionMatrix(efProjMat);
}

//エフェクトのドロー
void CEffektEngine::Draw(const int cameraNumber)
{ 
	CameraEffekseer(cameraNumber);
	m_effekseerRenderer->BeginRendering();
	m_effekseerManager->Draw();
	m_effekseerRenderer->EndRendering();
}
//アップデート
void CEffektEngine::Update()
{
	m_effekseerManager->Update();
}

Effekseer::Effect * CEffektEngine::CreateEffekseerEffect(const wchar_t * filePath)
{
	return Effekseer::Effect::Create(m_effekseerManager, (const EFK_CHAR*)filePath);
}

Effekseer::Handle CEffektEngine::Play(Effekseer::Effect * effect)
{
	return m_effekseerManager->Play(effect, 0, 0, 0);
}


