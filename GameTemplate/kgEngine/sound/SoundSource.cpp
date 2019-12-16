/*!
 * @brief	�����N���X�B
 */
#include "KGstdafx.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"



CSoundSource::CSoundSource()
{
	memset(m_emitterAzimuths, 0, sizeof(m_emitterAzimuths));
	memset(m_matrixCoefficients, 0, sizeof(m_matrixCoefficients));
	g_soundEngine->AddSoundSource(this);
}
CSoundSource::~CSoundSource()
{
	Release();
}
void CSoundSource::InitCommon()
{
}
void CSoundSource::Init(wchar_t* filePath)
{
	m_isAvailable = false;
	m_waveFile = g_soundEngine->GetWaveFileBank().FindWaveFile(0, filePath);
	if (!m_waveFile) {
		m_waveFile.reset(new CWaveFile);
		bool result = m_waveFile->Open(filePath);
		if (result == false) {
			//wave�t�@�C���̓ǂݍ��݂Ɏ��s�B
			g_soundEngine->GetWaveFileBank().UnregistWaveFile(0, m_waveFile);
			m_waveFile.reset();
			return;
		}
		m_waveFile->AllocReadBuffer(m_waveFile->GetSize());	//wave�t�@�C���̃T�C�Y���̓ǂݍ��݃o�b�t�@���m�ۂ���B
		g_soundEngine->GetWaveFileBank().RegistWaveFile(0, m_waveFile);
		unsigned int dummy;
		m_waveFile->Read(m_waveFile->GetReadBuffer(), m_waveFile->GetSize(), &dummy);
		m_waveFile->ResetFile();

	}

	//�T�E���h�{�C�X�\�[�X���쐬�B
	m_sourceVoice = g_soundEngine->CreateXAudio2SourceVoice(m_waveFile.get(), false);
	
	InitCommon();
	m_isAvailable = true;
}


void CSoundSource::InitStreaming(wchar_t* filePath, unsigned int ringBufferSize, unsigned int bufferSize)
{
	m_isAvailable = false;
	//�X�g���[�~���O��CWaveFile�̎g���܂킵�͂ł��Ȃ��B
	m_waveFile.reset(new CWaveFile);
	m_waveFile->Open(filePath);

	m_isStreaming = true;
	m_streamingBufferSize = bufferSize;
	m_waveFile->AllocReadBuffer(ringBufferSize);
	m_ringBufferSize = ringBufferSize;
	m_readStartPos = 0;
	m_currentBufferingSize = 0;
	//�T�E���h�{�C�X�\�[�X���쐬�B
	m_sourceVoice = g_soundEngine->CreateXAudio2SourceVoice(m_waveFile.get(), false);
	
	InitCommon();

	m_isAvailable = true;
}
void CSoundSource::Release()
{
	if (m_isStreaming) {
		m_waveFile->Release();
	}
	if (m_sourceVoice != nullptr) {
		m_sourceVoice->DestroyVoice();
		m_sourceVoice = nullptr;
	}
	g_soundEngine->RemoveSoundSource(this);
}
void CSoundSource::Play(char* buff, unsigned int bufferSize)
{
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = (BYTE*)buff;
	buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
	buffer.AudioBytes = bufferSize;
	if (m_sourceVoice != nullptr && bufferSize > 0) {
		//�Đ��J�n�B
		if (FAILED(m_sourceVoice->SubmitSourceBuffer(&buffer))) {
			Release();
			return;
		}
	}
}

void CSoundSource::StartStreamingBuffring()
{
	char* readStartBuff = m_waveFile->GetReadBuffer();
	m_readStartPos += m_currentBufferingSize;
	if (m_readStartPos + m_streamingBufferSize >= m_ringBufferSize) {
		//�����O�o�b�t�@�̃T�C�Y�𒴂���B
		//�z����B
		m_readStartPos = 0;
	}
	m_waveFile->ReadAsync(&readStartBuff[m_readStartPos], m_streamingBufferSize, &m_currentBufferingSize);
	m_streamingState = enStreamingBuffering;
}
void CSoundSource::Play(bool isLoop)
{
	if (m_isAvailable == false) {
				
		return;
	}
	if (m_isPlaying) {
		//�Đ����̂��̂��ĊJ����B
		m_sourceVoice->Start(0);
	}
	else {
		if (m_isStreaming) {
			//�o�b�t�@�����O�J�n
			m_waveFile->ResetFile();
			StartStreamingBuffring();
			m_sourceVoice->Start(0, 0);
		}
		else {
			m_sourceVoice->FlushSourceBuffers();
			m_sourceVoice->Start(0);
			Play(m_waveFile->GetReadBuffer(), m_waveFile->GetSize());
		}
		m_isPlaying = true;
	}
	g_soundEngine->AddSoundSource(this);
	m_isLoop = isLoop;
}
void CSoundSource::UpdateStreaming()
{
	if (!m_isPlaying) {
		return;
	}
	if (m_streamingState == enStreamingBuffering) {
		//�o�b�t�@�����O���B
		if (m_waveFile->IsReadEnd()) {
			//�o�b�t�@�����O���I������B
			m_streamingState = enStreamingQueueing;
		}
	}
	if (m_streamingState == enStreamingQueueing) {
		//�L���[�C���O���B
		XAUDIO2_VOICE_STATE state;
		m_sourceVoice->GetState(&state);
		if (state.BuffersQueued <= 2) {	//�L���[�C���O����Ă���o�b�t�@���Q�ȉ��ɂȂ�����L���[�C���O�ł���B
			char* buff = m_waveFile->GetReadBuffer();
			Play(&buff[m_readStartPos], m_currentBufferingSize);
			if (m_currentBufferingSize == 0) {
				//�ǂݍ��񂾃T�C�Y���O�Ƃ������Ƃ͖��[�܂œǂݍ��݂��I������Ƃ������ƁB
				if (m_isLoop) {
					//���[�v����H
					//wave�t�@�C���̓ǂݍ��݈ʒu�����Z�b�g���ăo�b�t�@�����O�ĊJ�B
					m_waveFile->ResetFile();
					StartStreamingBuffring();
				}
				else {
					//���[�v���Ȃ��ꍇ�̓L���[����ɂȂ�����Đ��I���B
					if (state.BuffersQueued == 0) {
						//�Đ��I���B
						m_isPlaying = false;
					}
				}
			}
			else {
				//�����o�b�t�@�����O����B
				StartStreamingBuffring();
			}
		}
	}
}

void CSoundSource::UpdateOnMemory()
{
	if (!m_isPlaying) {
		return;
	}
	XAUDIO2_VOICE_STATE state;
	m_sourceVoice->GetState(&state);
	if (state.BuffersQueued <= 0) {
		m_isPlaying = false;
		if (m_isLoop) {
			//���[�v�B
			Play(m_isLoop);
		}
	}
}
void CSoundSource::Update()
{
	if (m_isAvailable == false) {
		return;
	}

	if (m_isStreaming) {
		//�X�g���[�~���O�Đ����̍X�V�B
		UpdateStreaming();
	}
	else {
		//�I���������Đ����̍X�V�����B
		UpdateOnMemory();
	}

}
