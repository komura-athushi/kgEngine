/*!
 *@brief	波形データバンク。
 */

#include "KGstdafx.h"
#include "sound/WaveFile.h"
#include "sound/WaveFileBank.h"
#include "util/Util.h"

CWaveFileBank::CWaveFileBank()
{
}
CWaveFileBank::~CWaveFileBank()
{
	ReleaseAll();
}
void CWaveFileBank::Release(int groupID)
{
	for (auto waveFile : m_waveFileMap[groupID]) {
		waveFile.second->Release();
	}
	m_waveFileMap[groupID].clear();
}
void CWaveFileBank::UnregistWaveFile(int groupID, CWaveFilePtr waveFile)
{
	m_waveFileMap->erase(waveFile->GetFilePathHash());
}
void CWaveFileBank::RegistWaveFile(int groupID, CWaveFilePtr waveFile)
{
	m_waveFileMap[groupID].insert({ waveFile->GetFilePathHash(), waveFile });
}
CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const wchar_t* filePath)
{
	auto value = m_waveFileMap[groupID].find(Util::MakeHash(filePath));
	if (value != m_waveFileMap[groupID].end()) {
		return value->second;
	}
	return CWaveFilePtr();
}

