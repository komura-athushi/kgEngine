#pragma once

#include "sound/SoundSource.h"


enum EnBGM {
	enBGM_Title,
	enBGM_StageSelect,
	enBGM_Stage1,
	enBGM_Stage2,
	enBGM_Stage3,
	enBGM_Result,
	enBGM_Non
};

//BGMを管理するクラスです
class SoundDirector
{
private:
	SoundDirector();
	~SoundDirector();
public:
	static SoundDirector& GetInstance()
	{
		static SoundDirector instance;
		return instance;
	}
	//再生するBGMのステートを設定
	void SetBGM(EnBGM state);
	//再生中のBGMをストップ
	void SetStopBGM()
	{
		if (m_bgm != nullptr) {
			m_bgm->Pause();
		}
	}
	//ストップ中のBGMを再生
	void SetPlayBGM()
	{
		if (m_bgm != nullptr) {
			m_bgm->Play(true);
		}
	}
private:
	CSoundSource* m_bgm = nullptr;						//現在再生中のBGM
	EnBGM m_state = enBGM_Non;							//なんのBGMを再生しているか
};

static inline SoundDirector& SoundData()
{
	return SoundDirector::GetInstance();
}



