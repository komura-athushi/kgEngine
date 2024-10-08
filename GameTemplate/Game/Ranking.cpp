#include "stdafx.h"
#include "Ranking.h"
#include "GameData.h"
#include <iostream>
#include <string>
#include <fstream>
#include < locale.h >

Ranking::Ranking()
{
	m_gameData = &GameData::GetInstance();
}

Ranking::~Ranking()
{

}

void Ranking::Load()
{
	m_rankingList.clear();
	FILE* fp = NULL;
	char fileName[256];
	wchar_t filePath[256];
	swprintf_s(filePath, L"Assets/binarydata/savedata_ranking%d.txt", m_gameData->GetStageNumber());
	size_t wLen = 0;
	errno_t err = 0;
	err = wcstombs_s(&wLen, fileName, 50, filePath, _TRUNCATE);
	int number = 0;
	if (fopen_s(&fp, fileName, "rb") == 0) {
		int i = 1;
		while (i != EOF) {
			//fscanfでファイルを読み込む、戻り値がEOFだった場合処理を終了する
			i = fscanf(fp, "%d", &number);
			if (i == EOF) {
				break;
			}
			m_rankingList.push_back(number);
		}
		fclose(fp);
	}
}

void Ranking::Sort()
{
	for (int i = 0; i < m_rankingList.size(); i++) {
		if (m_rankingList[i] <= m_gameData->GetResultPlayerSize()) {
			Comparison(i);
			m_rankingList[i] = m_gameData->GetResultPlayerSize();
			m_number = i + 1;
			return;
		}
	}
	m_number = 0;
}

void Ranking::Comparison(const int number)
{
	int memory = m_rankingList[number];
	for (int i = number + 1; i < m_rankingList.size(); i++) {
		int tmp = m_rankingList[i];
		m_rankingList[i] = memory;
		memory = tmp;
	}
}

void Ranking::Save()
{
	FILE* fp = NULL;
	char tmp2[MAX_PATH];
	wchar_t tmp[MAX_PATH];
	swprintf_s(tmp, L"Assets/binarydata/savedata_ranking%d.txt", m_gameData->GetStageNumber());
	size_t wLen = 0;
	errno_t err = 0;
	err = wcstombs_s(&wLen, tmp2, MAX_PATH-1, tmp, _TRUNCATE);
	std::string fileName = tmp2;

	std::ofstream file;
	file.open(fileName, std::ios::trunc);

	for (auto itr : m_rankingList) {
		file << itr << std::endl;
	}
	file.close();
};