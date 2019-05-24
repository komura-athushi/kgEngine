
#pragma once

#include "gameobject/GameObjectManager.h"
//�R�s�[�֎~�ɂ��邽�߂̊�{�N���X
class NonCopyable
{
protected:
	NonCopyable() {};
	~NonCopyable() {};
private:
	//�R�s�[�R���X�g���N�^�������Z�q��private�֐��ɂ��邱�Ƃɂ��
	//�h���N���X����A�N�Z�X�ł��Ȃ�����
	NonCopyable(const NonCopyable&);
	NonCopyable& operator=(const NonCopyable&) 
	{

	}
};


//���b�Z�[�W�v���V�[�W��
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//DirectX�̏I������
void ReleaseDirectX();
class CEngine:private NonCopyable
{
public:
	CEngine();
	~CEngine();
	//�Q�[���̏�����
	void InitGame(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
	//�E�B���h�E�N���X�̏�����
	void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
	//�E�B���h�E���b�Z�[�W���f�B�X�p�b�`�Bfalse���Ԃ��Ă�����A�Q�[���I���B
	bool DispatchWindowMessage();
	//���b�Z�[�W�������~���Ă��ĂȂ�������Q�[���̏�������
	void RunGameLoop();
	//�Q�[���̏���
	void Update();
	//�Q�[���̏I������
	void Final();
	//�C���X�^���X���擾
	static CEngine& GetInstance()
	{
		static CEngine* instance = nullptr;
		if (instance == nullptr) {
			instance = new CEngine;
		}
		return *instance;
	}
	//�O���t�B�b�N�G���W�����擾
	GraphicsEngine& GetGraphicsEngine() const
	{
		return *m_graphicsEngine;
	}
private:
	HWND m_hWnd = NULL;
	GraphicsEngine* m_graphicsEngine = nullptr;
	CGameObjectManager* m_gameobjectmanager = nullptr;
};


//�G���W���̃C���X�^���X���擾
static inline CEngine& Engine()
{
	return CEngine::GetInstance();
}


