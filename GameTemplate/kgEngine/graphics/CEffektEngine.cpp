#include "KGstdafx.h"
#include "CEffektEngine.h"



CEffektEngine::CEffektEngine()
{
	//m_attackEffect = Effekseer::Effect::Create(m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/test.efk");
}


CEffektEngine::~CEffektEngine()
{
}

//�G�t�F�N�g�̏������B
void CEffektEngine::InitEffekseer()
{
	m_effekseerManager = Effekseer::Manager::Create(10000);
	m_effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
		Engine().GetGraphicsEngine().GetD3DDevice(),			//D3D�f�o�C�X�B
		Engine().GetGraphicsEngine().GetD3DDeviceContext(),	//D3D�f�o�C�X�R���e�L�X�g�B
		20000										//�|���̍ő吔�B
	);
	// �`��p�C���X�^���X����`��@�\��ݒ�
	m_effekseerManager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
	m_effekseerManager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
	m_effekseerManager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
	m_effekseerManager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
	m_effekseerManager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	m_effekseerManager->SetTextureLoader(m_effekseerRenderer->CreateTextureLoader());
	m_effekseerManager->SetModelLoader(m_effekseerRenderer->CreateModelLoader());

	m_attackEffect = Effekseer::Effect::Create(m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/test.efk");
}
//�J�����s��
void CEffektEngine::CameraEffekseer(const int cameraNumber)
{
	Effekseer::Matrix44 efCameraMat;
	MainCamera(cameraNumber).GetViewMatrix().CopyTo(efCameraMat);
	Effekseer::Matrix44 efProjMat;
	MainCamera(cameraNumber).GetProjectionMatrix().CopyTo(efProjMat);
	//�J�����s��ƃv���W�F�N�V�����s���ݒ�B
	m_effekseerRenderer->SetCameraMatrix(efCameraMat);
	m_effekseerRenderer->SetProjectionMatrix(efProjMat);
}

//�G�t�F�N�g�̃h���[
void CEffektEngine::Draw(const int cameraNumber)
{ 
	CameraEffekseer(cameraNumber);
	m_effekseerRenderer->BeginRendering();
	m_effekseerManager->Draw();
	m_effekseerRenderer->EndRendering();
}
//�A�b�v�f�[�g
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


