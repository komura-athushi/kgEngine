#include "stdafx.h"
#include "ObjectData.h"

ObjectData::ObjectData()
{
	FILE* fp = NULL;
	const char* fname = "Assets/binarydata/test.cag";
	float x = 0, y = 0, z = 0, volume = 0;
	int issphere = 0;
	//fopen_sでファイルのオープンに成功したら0が返るらしい
	if(fopen_s(&fp, fname, "r") == 0) {
		//fscanfの戻り値がEOFになるまでループさせる
		//読みこんだデータをリストに保存する
		int i = 1;
		while (i != EOF) {
			wchar_t* name = new wchar_t[20];
			i = fscanf(fp, "%ls %f %f %f %f %d ", name, &x, &y, &z, &volume, &issphere);
			if (i == EOF) {
				break;
			}
			m_objectdataList.push_back({ name, x, y, z, volume, issphere });
		}
		fclose(fp);
	}
	wchar_t* a = m_objectdataList[0].s_name;
	wchar_t* a1 = m_objectdataList[1].s_name;
 	wchar_t* a2 = m_objectdataList[2].s_name;
 	wchar_t* a3 = m_objectdataList[3].s_name;
	const wchar_t* b = L"aiueo";
	float y1 = m_objectdataList[0].s_issphere;
	float y2 = m_objectdataList[1].s_issphere; 
	float y3 = m_objectdataList[2].s_issphere;
	float y4 = m_objectdataList[3].s_issphere;
	wchar_t c[20] = L"aiueo";
	const wchar_t* d = L"aiueo";
	if (wcscmp(c, d) == 0) {
		int i = 0;
	}
	return;
}