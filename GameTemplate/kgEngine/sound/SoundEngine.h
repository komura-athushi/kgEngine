

#pragma once
#include "sound/WaveFileBank.h"

#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//�ő�o�̓`�����l�����B
class CWaveFile;
class CSoundSource;

/// <summary>
/// �T�E���h�G���W���B
/// </summary>
class CSoundEngine{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	CSoundEngine();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~CSoundEngine();

	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// ���
	/// </summary>
	/// <remarks>
	/// �f�X�g���N�^����Ă΂�Ă��܂��B
	/// �����I�ɊJ���������ꍇ�Ɏg�p���Ă��������B
	/// </remarks>
	void Release();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �T�E���h�\�[�X�̒ǉ��B
	/// </summary>
	/// <param name="s">�T�E���h�\�[�X</param>
	void AddSoundSource(CSoundSource* s) 
	{
		auto it = std::find(m_soundSources.begin(), m_soundSources.end(), s);
		if (it == m_soundSources.end()) {
			//���o�^�Ȃ�o�^����B
			m_soundSources.push_back(s);
		}
	}
	/// <summary>
	/// �T�E���h�\�[�X���폜�B
	/// </summary>
	/// <param name="s"></param>
	/// <returns>�폜�����v�f�̎����w���C�e���[�^�[</returns>
	auto RemoveSoundSource(CSoundSource* s)
	{
		auto it = std::find(m_soundSources.begin(), m_soundSources.end(), s);
		if (it != m_soundSources.end()) {
			it = m_soundSources.erase(it);
		}
		return it;
	
	}
	/// <summary>
	/// XAudio2�̃\�[�X�{�C�X�̍쐬�B
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
	/// �o�̓`�����l���̐����擾�B
	/// </summary>
	/// <returns></returns>
	int GetNumChannel() const
	{
		return m_nChannels;
	}

	/// <summary>
	/// �g�`�f�[�^�o���N���擾�B
	/// </summary>
	CWaveFileBank& GetWaveFileBank()
	{
		return m_waveFileBank;
	}

private:
	IXAudio2* m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;
	IUnknown* m_reverbEffect = nullptr;				//!<���o�[�v�G�t�F�N�g�B
	IXAudio2SubmixVoice* m_submixVoice = nullptr;	//!<�T�u�~�b�N�X�{�C�X�B
	DWORD m_channelMask = 0;						//!<�I�[�f�B�I�`�����l���}�X�N�B
	DWORD m_nChannels = 0;							//!<�`�����l�����B
	bool m_isInited = false;
	std::list<CSoundSource*>	m_soundSources;		//!<�T�E���h�\�[�X�̃��X�g�B
	CWaveFileBank m_waveFileBank;					//!<�g�`�f�[�^�̃o���N�B
};

//�T�E���h�G���W���̃C���X�^���X�B
extern CSoundEngine* g_soundEngine ;