#include "stdafx.h"
#include "system/system.h"
#include "Player.h"
#include "level/Level.h"
#include "KgEngine.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"
#include "StageSelect.h"
#include "Object/ObjectData.h"
#include "GameData.h"
#include "Result.h"
#include "Ranking.h"

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	Engine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	//�J�������������B
    MainCamera().SetPosition({ 0.0f, 100.0f, 300.0f });
	MainCamera().SetTarget({ 0.0f, 100.0f, 0.0f });
	MainCamera().SetFar(10000.0f);
	
	ObjectData::GetInstance();
	GetGameData().LoadDataStageClear();

	////�v���C���[
	//Player player;
	//Player* player = NewGO<Player>(0);
	//Game* game = NewGO<Game>(0);
	//Fade::GetInstance();
	//NewGO<Title>(0);
	Ranking::GetInstance();
	NewGO<Game>(0);
	//NewGO<Result>(0);

	//NewGO<StageSelect>(0);
	//�Q�[�����[�v�B
	Engine().RunGameLoop();
	
		////�`��J�n�B
		//g_graphicsEngine->BegineRender();
		////�Q�[���p�b�h�̍X�V�B	
		//for (auto& pad : g_pad) {
		//	pad.Update();
		//}
		////�����G���W���̍X�V�B
		//g_physics.Update();
		//////�v���C���[�̍X�V�B
		//player.Update();
		//////�v���C���[�̕`��B
		//player.Draw();
		////�J�����̍X�V�B
		//g_camera3D.Update();
		////�`��I���B
		//g_graphicsEngine->EndRender();
	Engine().Final();
	return 0;
}