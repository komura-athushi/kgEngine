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
	//¡Ä¶’†‚ÌBGM‚Æˆá‚Á‚Ä‚½‚çV‚µ‚­BGM‚ðÄ¶
	if (m_state != state) {
		if (m_bgm != nullptr) {
			delete m_bgm;
		}
		m_bgm = new CSoundSource();
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
		default:
			break;
		}
		m_state = state;
		m_bgm->Play(true);
	}
}