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

//BGM���Ǘ�����N���X�ł�
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
	//�Đ�����BGM�̃X�e�[�g��ݒ�
	void SetBGM(EnBGM state);
private:
	CSoundSource* m_bgm = nullptr;						//���ݍĐ�����BGM
	EnBGM m_state = enBGM_Non;							//�Ȃ��BGM���Đ����Ă��邩
};

static inline SoundDirector& SoundData()
{
	return SoundDirector::GetInstance();
}



