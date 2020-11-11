/*****************************************************************//**
 * \file   SoundDirector.h
 * \brief  SoundDirector�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

#include "sound/SoundSource.h"


enum EnBGM {
	enBGM_Title,
	enBGM_StageSelect,
	enBGM_Stage1,
	enBGM_Stage2,
	enBGM_Stage3,
	enBGM_Result,
	enBGM_Battle,
	enBGM_Non
};

/**
 * \brief �Q�[������BGM���Ǘ�����N���X.
 */
class SoundDirector
{
private:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	SoundDirector();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~SoundDirector();
public:
	/**
	 * \brief �C���X�^���X���擾.
	 * 
	 * \return �C���X�^���X
	 */
	static SoundDirector& GetInstance()
	{
		static SoundDirector instance;
		return instance;
	}
	/**
	 * \brief �Đ�����BGM�̃X�e�[�g��ݒ�.
	 * 
	 * \param state �Đ�����BGM�̃X�e�[�g
	 */
	void SetBGM(EnBGM state);
	/**
	 * \brief �Đ�����BGM���~.
	 * 
	 */
	void SetStopBGM()
	{
		if (m_bgm != nullptr) {
			m_bgm->Pause();
		}
	}
	/**
	 * \brief ��~����BGM���Đ�.
	 * 
	 */
	void SetPlayBGM()
	{
		if (m_bgm != nullptr) {
			m_bgm->Play(true);
		}
	}
private:
	CSoundSource* m_bgm = nullptr;						//���ݍĐ�����BGM
	EnBGM m_state = enBGM_Non;							//�Ȃ��BGM���Đ����Ă��邩
};

/**
 * \brief �C���X�^���X���擾.
 * 
 * \return �C���X�^���X
 */
static inline SoundDirector& SoundData()
{
	return SoundDirector::GetInstance();
}



