#include "stdafx.h"
#include "GameData.h"
#include <iostream>
#include <string>
#include <fstream>

void GameData::LoadDataStageClear()
{
	std::map<int, int> stageClearList;

	FILE* fp = NULL;
	const char* fname = "Assets/binarydata/savadata_stageclear.txt";
	int number = 0;
	if (fopen_s(&fp, fname, "rb") == 0) {
		int i = 1;
		while (i != EOF) {
			//fscanfでファイルを読み込む、戻り値がEOFだった場合処理を終了する
			i = fscanf(fp, "%d", &number);
			if (i == EOF) {
				break;
			}
			stageClearList.emplace(number, 0);
		}
		fclose(fp);
	}

	for (auto itr : stageClearList) {
		m_stageClearList[itr.first - 1] = true;
	}
}

void GameData::SaveDataStageClear()
{
	std::string fileName = "Assets/binarydata/savadata_stageclear.txt";

	std::ofstream file;
	file.open(fileName, std::ios::trunc);

	for (int i = 1; i <= enState_Stage3; i++) {
		if (m_stageClearList[i - 1]) {
			file << i << std::endl;
		}
	}
	file.close();
}