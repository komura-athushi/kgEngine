#pragma once

#include "sound/SoundSource.h"


enum EnBGM {
	enBGM_Title,
	enBGM_StageSelect,
	enBGM_Stage1,
	enBGM_Stage2,
	enBGM_Stage3,
	enBGM_Non
};

//BGM‚ğŠÇ—‚·‚éƒNƒ‰ƒX‚Å‚·
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
	void SetBGM(EnBGM state);
private:
	CSoundSource* m_bgm = nullptr;
	EnBGM m_state = enBGM_Non;
};

static inline SoundDirector& SoundData()
{
	return SoundDirector::GetInstance();
}



