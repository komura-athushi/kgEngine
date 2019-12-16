#pragma once

#include "sound/WaveFile.h"

	

/// <summary>
/// �����N���X�B
/// </summary>
/// <remarks>
/// 
/// ���̃N���X�ɂ͔g�`�f�[�^���I���������ɏ悹�čĐ�������@�ƁA�X�g���[�~���O�Đ�������@������܂��B</br>
/// �I���������ɏ悹��ꍇ��CSoundSource::Init���g���ď��������s���Ă��������B< / br>
/// �X�g���[�~���O�Đ����s���ꍇ��CSoundSource::InitStreaming���g�p���Ă��������B< / br>
/// �I���������ł̍Đ��͔g�`�f�[�^��S�ă��C����������ɏ悹�čĐ����s�����@�ł��B< / br>
/// �X�g���[�~���O�Đ��̓��f�B�A(DVD�AHDD�Ȃǂ̊O���⏕�L�����u)���班���Âǂݍ��݂��s���āA< / br>
/// �������s���Ă������@�ł��B�X�g���[�~���O�Đ��͕K�v�ȕ������ǂݍ��݂��s��(�o�b�t�@�����O)���s���A< / br>
/// �Đ�����������Ƃ��̃o�b�t�@��j�����܂��B���̂��߁A���Ȃ��������ő傫�Ȕg�`�f�[�^���Đ����邱�Ƃ��\�ł��B< / br>
/// �I���������Đ��͓ǂݍ��݂̃I�[�o�[�w�b�h���ŏ������ɂȂ邪�A�X�g���[�~���O�Đ��͏�ɓǂݍ��݂̃I�[�o�[�w�b�h��< / br>
/// �������邽�߃p�t�H�}�[���X���ቺ���܂��B< / br>
/// ��ʓI�ɃI���������Đ��͌��ʉ��Ȃǂ̂悤�ȃT�C�Y�̏�����SE�̍Đ��ɓK���Ă��܂��B< / br>
/// �X�g���[�~���O�Đ��͑傫�ȃT�C�Y��BGM�̍Đ��ɓK���Ă��܂��B< / br>
/// 
/// ���̃N���X�̃C���X�^���X�����s���ƁA���̃C���X�^���X�̓T�E���h�G���W���N���X�ɓo�^����܂��B
/// </remarks>
class CSoundSource {
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	CSoundSource();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~CSoundSource();
	/// <summary>
	/// ������(�I���������Đ�)
	/// </summary>
	/// <param name="filePath">
	/// �t�@�C���p�X�B�Ή����Ă���t�@�C���t�H�[�}�b�g(*.wave)
	/// </param>
	void Init(wchar_t* filePath);
	/// <summary>
	/// ������(�X�g���[�~���O�Đ�)
	/// </summary>
	/// <remarks>
	/// �����O�o�b�t�@�ɃX�g���[�~���O�ǂݍ��݂��s���āA�Đ����s���Ă����܂��B</br>
	/// ��x�ɓǂݍ��܂��f�[�^�̍ő�T�C�Y��bufferingSize�ł��B< / br>
	/// �ǂݍ��܂ꂽ�f�[�^�̓����O�o�b�t�@�ɃR�s�[����Ă����܂��B< / br>
	/// </remarks>
	/// <param name="filePath">�t�@�C���p�X�B�Ή����Ă���t�@�C���t�H�[�}�b�g(*.wave)</param>
	/// <param name="ringBufferSize">�����O�o�b�t�@�̃T�C�Y�B(bufferSize�̔{���ɂȂ��Ă���Ɩ��ʂȂ����p�ł��܂��B)</param>
	/// <param name="bufferingSize">�X�g���[�~���O�̍ő�o�b�t�@�����O�T�C�Y�B</param>
	void InitStreaming(wchar_t* filePath,  unsigned int ringBufferSize = 3 * 1024 * 1024, unsigned int bufferingSize = 1024 * 512);
	/// <summary>
	/// ���
	/// </summary>
	/// <remarks>
	/// �f�X�g���N�^���玩���I�ɌĂ΂�܂��B�����I�ɊJ�����s�������ꍇ�Ɏg�p���Ă��������B
	/// </remarks>
	void Release();

	/*!
	* @brief	�Đ��B
	*@param[in]	isLoop		���[�v�Đ��t���O�B
	* ���̃t���O��false�̏ꍇ�̓����V���b�g�Đ��ƂȂ�܂��B
	* �����V���b�g�Đ��̏ꍇ�́A�Đ�����������Ǝ����I�ɃC���X�^���X���j������܂��B
	* �����V���b�g�Đ���CSoundSource�̃C���X�^���X�������o�ϐ��Ȃǂŕێ����Ă���ƁA�j�����ꂽ
	* �C���X�^���X�ɃA�N�Z�X���Ă��܂��\��������̂ŁA�ێ����Ȃ����Ƃ������߂��܂��B
	* �Đ��̊��������o�������ꍇ�Ȃǂ̓C�x���g���X�i�[���g�p���Ă��������B
	*/
	/// <summary>
	/// �Đ��B
	/// </summary>
	/// <param name="isLoop">
	/// ���[�v�Đ��t���O�B
	/// ���̃t���O��false�̏ꍇ�̓����V���b�g�Đ��ƂȂ�܂��B
	/// </param>
	void Play(bool isLoop);

	/// <summary>
	/// ��~
	/// </summary>
	void Stop()
	{
		m_sourceVoice->Stop();
		m_isPlaying = false;
	}
	/// <summary>
	/// �ꎞ��~�B
	/// </summary>
	void Pause()
	{
		m_sourceVoice->Stop(/*XAUDIO2_PLAY_TAILS*/);
	}
	/// <summary>
	/// �Đ���������
	/// </summary>
	/// <returns>true���Ԃ��Ă�����Đ���</returns>
	bool IsPlaying() const
	{
		return m_isPlaying;
	}
	/// <summary>
	/// �{�����[����ݒ�B
	/// </summary>
	/// <param name="vol">�{�����[��</param>
	void SetVolume(float vol)
	{
		m_sourceVoice->SetVolume(vol);
	}

	/// <summary>
	/// �{�����[�����擾�B
	/// </summary>
	/// <returns>�{�����[��</returns>
	float GetVolume() const
	{
		float vol;
		m_sourceVoice->GetVolume(&vol);
		return vol;
	}
	/// <summary>
	/// ���[�v�t���O���擾�B
	/// </summary>
	/// <returns></returns>
	bool GetLoopFlag() const
	{
		return m_isLoop;
	}
	/// <summary>
	/// �{�C�X�̎��g��������
	/// </summary>
	/// <remarks>
	/// �ڍׂ�IXAudio2SourceVoice��SetFrequencyRatio���Q�Ƃ��Ă��������B
	/// </remarks>
	/// <param name="ratio">���g����</param>
	void SetFrequencyRatio(float ratio)
	{
		if (m_sourceVoice != nullptr) {
			m_sourceVoice->SetFrequencyRatio(ratio);
		}
	}
	IXAudio2SourceVoice* GetXAudio2SourceVoice()
	{
		return m_sourceVoice;
	}
	int GetNumInputChannel()const
	{
		return m_waveFile->GetFormat()->nChannels;
	}
	FLOAT32* GetEmitterAzimuths()
	{
		return m_emitterAzimuths;
	}
	FLOAT32* GetMatrixCoefficients()
	{
		return m_matrixCoefficients;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	/// <remarks>
	/// �T�E���h�G���W������Ă΂��̂ŁA���[�U�[�͎g�p���Ȃ��ł��������B
	/// </remarks>
	void Update();
private:
	void InitCommon();
	//�X�g���[�~���O�Đ����̍X�V�����B
	void UpdateStreaming();
	//�I���������Đ����̍X�V�����B
	void UpdateOnMemory();
	void Play(char* buff, unsigned int bufferSize);
	void StartStreamingBuffring();
	
private:
	enum EnStreamingStatus {
		enStreamingBuffering,	//�o�b�t�@�����O���B
		enStreamingQueueing,	//�L���[�C���O���B
	};
	std::shared_ptr<CWaveFile>	m_waveFile;					//!<�g�`�f�[�^�B
	IXAudio2SourceVoice*		m_sourceVoice = nullptr;	//!<�\�[�X�{�C�X�B
	bool						m_isLoop = false;			//!<���[�v�t���O�B
	bool						m_isPlaying = false;		//!<�Đ����t���O�B
	bool					m_isStreaming = false;		//!<�X�g���[�~���O�Đ��H
	unsigned int			m_streamingBufferSize = 0;	//!<�X�g���[�~���O�p�̃o�b�t�@�����O�T�C�Y�B
	unsigned int			m_currentBufferingSize = 0;	//!<���݂̃o�b�t�@�����O�̃T�C�Y�B
	unsigned int			m_readStartPos = 0;			//!<�ǂݍ��݊J�n�ʒu�B
	unsigned int			m_ringBufferSize = 0;		//!<�����O�o�b�t�@�̃T�C�Y�B
	EnStreamingStatus		m_streamingState = enStreamingBuffering;	//!<�X�g���[�~���O�X�e�[�^�X�B
	FLOAT32 m_emitterAzimuths[INPUTCHANNELS];
	FLOAT32 m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	bool m_isAvailable = false;			//!<�C���X�^���X�����p�\�H
};

