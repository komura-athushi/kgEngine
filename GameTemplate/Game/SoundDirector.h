/*****************************************************************//**
 * \file   SoundDirector.h
 * \brief  SoundDirectorクラス
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
 * \brief ゲーム中のBGMを管理するクラス.
 */
class SoundDirector
{
private:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	SoundDirector();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~SoundDirector();
public:
	/**
	 * \brief インスタンスを取得.
	 * 
	 * \return インスタンス
	 */
	static SoundDirector& GetInstance()
	{
		static SoundDirector instance;
		return instance;
	}
	/**
	 * \brief 再生するBGMのステートを設定.
	 * 
	 * \param state 再生するBGMのステート
	 */
	void SetBGM(EnBGM state);
	/**
	 * \brief 再生中のBGMを停止.
	 * 
	 */
	void SetStopBGM()
	{
		if (m_bgm != nullptr) {
			m_bgm->Pause();
		}
	}
	/**
	 * \brief 停止中のBGMを再生.
	 * 
	 */
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

/**
 * \brief インスタンスを取得.
 * 
 * \return インスタンス
 */
static inline SoundDirector& SoundData()
{
	return SoundDirector::GetInstance();
}



