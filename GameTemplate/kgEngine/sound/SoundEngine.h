

#pragma once
#include "sound/WaveFileBank.h"

#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//最大出力チャンネル数。
class CWaveFile;
class CSoundSource;

/// <summary>
/// サウンドエンジン。
/// </summary>
class CSoundEngine{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	CSoundEngine();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~CSoundEngine();

	/// <summary>
	/// 初期化。
	/// </summary>
	void Init();
	/// <summary>
	/// 解放
	/// </summary>
	/// <remarks>
	/// デストラクタから呼ばれています。
	/// 明示的に開放したい場合に使用してください。
	/// </remarks>
	void Release();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// サウンドソースの追加。
	/// </summary>
	/// <param name="s">サウンドソース</param>
	void AddSoundSource(CSoundSource* s) 
	{
		auto it = std::find(m_soundSources.begin(), m_soundSources.end(), s);
		if (it == m_soundSources.end()) {
			//未登録なら登録する。
			m_soundSources.push_back(s);
		}
	}
	/// <summary>
	/// サウンドソースを削除。
	/// </summary>
	/// <param name="s"></param>
	/// <returns>削除した要素の次を指すイテレーター</returns>
	auto RemoveSoundSource(CSoundSource* s)
	{
		auto it = std::find(m_soundSources.begin(), m_soundSources.end(), s);
		if (it != m_soundSources.end()) {
			it = m_soundSources.erase(it);
		}
		return it;
	
	}
	/// <summary>
	/// XAudio2のソースボイスの作成。
	/// </summary>
	/// <param name="waveFile"></param>
	/// <param name="is3DSound"></param>
	/// <returns></returns>
	IXAudio2SourceVoice* CreateXAudio2SourceVoice(CWaveFile* waveFile, bool is3DSound);
	IXAudio2MasteringVoice* GetMasteringVoice()
	{
		return m_masteringVoice;
	}
	IXAudio2SubmixVoice* GetSubmixVoice()
	{
		return m_submixVoice;
	}

	/// <summary>
	/// 出力チャンネルの数を取得。
	/// </summary>
	/// <returns></returns>
	int GetNumChannel() const
	{
		return m_nChannels;
	}

	/// <summary>
	/// 波形データバンクを取得。
	/// </summary>
	CWaveFileBank& GetWaveFileBank()
	{
		return m_waveFileBank;
	}

private:
	IXAudio2* m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;
	IUnknown* m_reverbEffect = nullptr;				//!<リバープエフェクト。
	IXAudio2SubmixVoice* m_submixVoice = nullptr;	//!<サブミックスボイス。
	DWORD m_channelMask = 0;						//!<オーディオチャンネルマスク。
	DWORD m_nChannels = 0;							//!<チャンネル数。
	bool m_isInited = false;
	std::list<CSoundSource*>	m_soundSources;		//!<サウンドソースのリスト。
	CWaveFileBank m_waveFileBank;					//!<波形データのバンク。
};

//サウンドエンジンのインスタンス。
extern CSoundEngine* g_soundEngine ;