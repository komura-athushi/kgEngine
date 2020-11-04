
#include "KGstdafx.h"
#include "KgEngine.h"
#include "sound/SoundEngine.h"

CEngine::CEngine()
{
}


CEngine::~CEngine()
{
}

//�E�B���h�E�N���X�̏�����
void CEngine::InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	//�E�B���h�E�N���X�̃p�����[�^��ݒ�(�P�Ȃ�\���̂̕ϐ��̏������ł��B)
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),		//�\���̂̃T�C�Y�B
		CS_CLASSDC,				//�E�B���h�E�̃X�^�C���B
								//�����̎w��ŃX�N���[���o�[��������ł��邪�A�Q�[���ł͕s�v�Ȃ̂�CS_CLASSDC�ł悢�B
		MsgProc,				//���b�Z�[�W�v���V�[�W��(��q)
		0,						//0�ł����B
		0,						//0�ł����B
		GetModuleHandle(NULL),	//���̃N���X�̂��߂̃E�C���h�E�v���V�[�W��������C���X�^���X�n���h���B
								//�����C�ɂ��Ȃ��Ă悢�B
		NULL,					//�A�C�R���̃n���h���B�A�C�R����ς������ꍇ������ύX����B�Ƃ肠��������ł����B
		NULL,					//�}�E�X�J�[�\���̃n���h���BNULL�̏ꍇ�̓f�t�H���g�B
		NULL,					//�E�B���h�E�̔w�i�F�BNULL�̏ꍇ�̓f�t�H���g�B
		NULL,					//���j���[���BNULL�ł����B
		appName,				//�E�B���h�E�N���X�ɕt���閼�O�B
		NULL					//NULL�ł����B
	};
	//�E�B���h�E�N���X�̓o�^�B
	RegisterClassEx(&wc);

	// �E�B���h�E�̍쐬�B
	m_hWnd = CreateWindow(
		appName,		//�g�p����E�B���h�E�N���X�̖��O�B
								//��قǍ쐬�����E�B���h�E�N���X�Ɠ������O�ɂ���B
		appName,		//�E�B���h�E�̖��O�B�E�B���h�E�N���X�̖��O�ƕʖ��ł��悢�B
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C���B�Q�[���ł͊�{�I��WS_OVERLAPPEDWINDOW�ł����A
		0,						//�E�B���h�E�̏���X���W�B
		0,						//�E�B���h�E�̏���Y���W�B
		(UINT)FRAME_BUFFER_W,	//�E�B���h�E�̕��B
		(UINT)FRAME_BUFFER_H,	//�E�B���h�E�̍����B
		NULL,					//�e�E�B���h�E�B�Q�[���ł͊�{�I��NULL�ł����B
		NULL,					//���j���[�B����NULL�ł����B
		hInstance,				//�A�v���P�[�V�����̃C���X�^���X�B
		NULL
	);

	ShowWindow(m_hWnd, nCmdShow);

}

void CEngine::InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	//�E�B���h�E���������B
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, appName);
	//DirectX�̏������B
	m_graphicsEngine = std::make_unique<GraphicsEngine>();
	m_graphicsEngine->Init(m_hWnd);
	m_gameobjectmanager = &GameObjectManager();
	m_gameobjectmanager->Init(255);
	//�Q�[���p�b�h�̏������B
	//�ő�S�̃R���g���[���[��ڑ��ł���悤�ɂ��܂��傤�B
	m_pad[0].Init(0);
	m_pad[1].Init(1);
	m_pad[2].Init(2);
	m_pad[3].Init(3);

	//�����G���W���̏�����
	m_physicsEngine = std::make_unique<PhysicsWorld>();
	m_physicsEngine->Init();
}

//�E�B���h�E���b�Z�[�W���f�B�X�p�b�`�Bfalse���Ԃ��Ă�����A�Q�[���I���B
bool CEngine::DispatchWindowMessage()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message) {
		//�E�B���h�E����̃��b�Z�[�W���󂯎��B
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//�E�B���h�E���b�Z�[�W����ɂȂ����B
			break;
		}
	}
	return msg.message != WM_QUIT;
}

void CEngine::RunGameLoop()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�X�V�B
			Update();
		}
	}
}

void CEngine::Update()
{
	//�v���J�n
	m_watch.Start();
	//�`��J�n�B
	m_graphicsEngine->BegineRender();
	
	//�Q�[���p�b�h�̍X�V�B	
	for (auto& pad : m_pad) {
		pad.Update();
	}

	m_gameobjectmanager->PreUpdate();

	//�����G���W���̍X�V�B
	m_physicsEngine->Update();

	//�Q�[���I�u�W�F�N�g�̏���
	m_gameobjectmanager->Start();
	m_gameobjectmanager->Update();

	m_graphicsEngine->ShadowMapRender();

	m_graphicsEngine->ChangeMainRenderTarget();

	m_gameobjectmanager->PrePostRender();
	m_gameobjectmanager->Draw();

	m_graphicsEngine->EffektUpdate();

	m_graphicsEngine->EdgeDelectionRender();

	m_graphicsEngine->PostRender();



	m_gameobjectmanager->PostRender();

	m_gameobjectmanager->Delete();

	SoundEngine().Update();

	//�J�����̍X�V�B
	MainCamera().Update();

	//�`��I���B
	m_graphicsEngine->EndRender();

	//�v���I��
	m_watch.Stop();

	//���Ԃ��L��
	GameTime().PushFrameDeltaTime((float)m_watch.GetElapsed());
}

void CEngine::Final()
{
	//if (m_gameThread) {
	//	//�Q�[���X���b�h���E���B
	//	m_isReqDeadGameThread = true;
	//	m_isRunGameThreadCv.notify_all();
	//	m_gameThread->join();
	//}

	m_physicsEngine->Release();
	//m_soundEngine.Release();
	m_graphicsEngine->Release();

}

void ReleaseDirectX()
{

}

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//�����Ă������b�Z�[�W�ŏ����𕪊򂳂���B
	switch (msg)
	{
	case WM_DESTROY:
		ReleaseDirectX();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
