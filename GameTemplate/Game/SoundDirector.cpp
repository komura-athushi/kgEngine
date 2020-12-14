#include "stdafx.h"
#include "SoundDirector.h"

SoundDirector::SoundDirector()
{

}

SoundDirector::~SoundDirector()
{

}

void SoundDirector::SetBGM(EnBGM state)
{
	//return;
	if (state == enBGM_Result) {
		m_bgm->Stop();
	}
	//今再生中のBGMと違ってたら新しくBGMを再生
	else if (m_state != state) {
		if (m_bgm != nullptr) {
			delete m_bgm;
		}
		m_bgm = new CSoundSource();
		//設定されたステートによって再生するBGMを決定
		switch (state)
		{
		case enBGM_Title:
			m_bgm->InitStreaming(L"Assets/sound/op.wav");
			break;
		case enBGM_StageSelect:
			m_bgm->InitStreaming(L"Assets/sound/stageselect.wav");
			break;
		case enBGM_Stage1:
			m_bgm->InitStreaming(L"Assets/sound/stage1.wav");
			m_bgm->SetVolume(0.25f);
			break;
		case enBGM_Stage2:
			m_bgm->InitStreaming(L"Assets/sound/stage2.wav");
			m_bgm->SetVolume(0.35f);
			break;
		case enBGM_Battle:
			m_bgm->InitStreaming(L"Assets/sound/battle.wav");
			m_bgm->SetVolume(0.7f);
			break;
		default:
			break;
		}
		m_state = state;
		m_bgm->Play(true);
	}
}