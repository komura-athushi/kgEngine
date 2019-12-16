/*!
 * @brief	音源クラス。
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
			//waveファイルの読み込みに失敗。
			g_soundEngine->GetWaveFileBank().UnregistWaveFile(0, m_waveFile);
			m_waveFile.reset();
			return;
		}
		m_waveFile->AllocReadBuffer(m_waveFile->GetSize());	//waveファイルのサイズ分の読み込みバッファを確保する。
		g_soundEngine->GetWaveFileBank().RegistWaveFile(0, m_waveFile);
		unsigned int dummy;
		m_waveFile->Read(m_waveFile->GetReadBuffer(), m_waveFile->GetSize(), &dummy);
		m_waveFile->ResetFile();

	}

	//サウンドボイスソースを作成。
	m_sourceVoice = g_soundEngine->CreateXAudio2SourceVoice(m_waveFile.get(), false);
	
	InitCommon();
	m_isAvailable = true;
}


void CSoundSource::InitStreaming(wchar_t* filePath, unsigned int ringBufferSize, unsigned int bufferSize)
{
	m_isAvailable = false;
	//ストリーミングはCWaveFileの使いまわしはできない。
	m_waveFile.reset(new CWaveFile);
	m_waveFile->Open(filePath);

	m_isStreaming = true;
	m_streamingBufferSize = bufferSize;
	m_waveFile->AllocReadBuffer(ringBufferSize);
	m_ringBufferSize = ringBufferSize;
	m_readStartPos = 0;
	m_currentBufferingSize = 0;
	//サウンドボイスソースを作成。
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
		//再生開始。
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
		//リングバッファのサイズを超える。
		//循環する。
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
		//再生中のものを再開する。
		m_sourceVoice->Start(0);
	}
	else {
		if (m_isStreaming) {
			//バッファリング開始
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
		//バッファリング中。
		if (m_waveFile->IsReadEnd()) {
			//バッファリングが終わった。
			m_streamingState = enStreamingQueueing;
		}
	}
	if (m_streamingState == enStreamingQueueing) {
		//キューイング中。
		XAUDIO2_VOICE_STATE state;
		m_sourceVoice->GetState(&state);
		if (state.BuffersQueued <= 2) {	//キューイングされているバッファが２以下になったらキューイングできる。
			char* buff = m_waveFile->GetReadBuffer();
			Play(&buff[m_readStartPos], m_currentBufferingSize);
			if (m_currentBufferingSize == 0) {
				//読み込んだサイズが０ということは末端まで読み込みが終わったということ。
				if (m_isLoop) {
					//ループする？
					//waveファイルの読み込み位置をリセットしてバッファリング再開。
					m_waveFile->ResetFile();
					StartStreamingBuffring();
				}
				else {
					//ループしない場合はキューが空になったら再生終了。
					if (state.BuffersQueued == 0) {
						//再生終了。
						m_isPlaying = false;
					}
				}
			}
			else {
				//次をバッファリングする。
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
			//ループ。
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
		//ストリーミング再生中の更新。
		UpdateStreaming();
	}
	else {
		//オンメモリ再生中の更新処理。
		UpdateOnMemory();
	}

}
