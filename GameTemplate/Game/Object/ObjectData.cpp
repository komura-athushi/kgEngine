#include "stdafx.h"
#include "ObjectData.h"
#include <iostream>
#include <string>
#include <fstream>

ObjectData::ObjectData()
{
	FILE* fp = NULL;
	const char* fname = "Assets/binarydata/test.cag";
	float x = 0, y = 0, z = 0, volume = 0;
	int issphere = 0, islinesegment = 0, isanimation = 0, isMeshCollider = 0, isDefalutSe = 0;
	//fopen_sでファイルのオープンに成功したら0が返るらしい
	if(fopen_s(&fp, fname, "rb") == 0) {
		//fscanfの戻り値がEOF(-1)になるまでループさせる
		//読みこんだデータを構造体として保存する
		int i = 1;
		while (i != EOF) {
			wchar_t* name = new wchar_t[50];
			char* jName = new char[50];
			wchar_t* seFileName = new wchar_t[50];
			//fscanfでファイルを読み込む、戻り値がEOFだった場合処理を終了する
			i = fscanf(fp, "%ls %s %f %f %f %f %d %d %d %d %d %ls", name, jName, &x, &y, &z, &volume, &issphere, &islinesegment, &isanimation, &isMeshCollider,&isDefalutSe,seFileName);
			if (i == EOF) {
				break;
			}
			/*x *= 0.8f;
			y *= 0.8f;
			z *= 0.9f;*/
			//volume = (x + x) * (y + y) * (z + z);
			if (islinesegment == 1) {
				CVector3 linevector;
				Enxyz state;
				if (x >= y && x >= z) {
					linevector = CVector3::AxisX() * x;
					state = enState_X;
				}
				else if (y >= x && y >= z) {
					linevector = CVector3::AxisY() * y;
					state = enState_Y;
 				}
				else if (z >= x && z >= y) {
					linevector = CVector3::AxisZ() * z;
					state = enState_Z;
				}
				m_objectdataList.push_back({ name, jName, x, y, z, volume, issphere,islinesegment,isanimation,isMeshCollider,isDefalutSe,seFileName,linevector,state });
			}
			else {
				m_objectdataList.push_back({ name, jName, x, y, z, volume, issphere,islinesegment,isanimation,isMeshCollider,isDefalutSe,seFileName });
			}

			if (m_modelObjDataList.count(volume) == 0) {
				wchar_t filepath[256];
				swprintf_s(filepath, L"Resource/modelData/%ls.cmo", name);
				SkinModel_ObjData* data = new SkinModel_ObjData();
				data->s_skinModel.Init(filepath);
				int key = Util::MakeHash(name);
				data->s_hashKey = key;
				data->s_x = x;
				data->s_y = y;
				data->s_z = z;
				data->s_jName = jName;
				m_modelObjDataList.emplace(volume, data);
			}
		}
		fclose(fp);
	}
	LoadSaveData();
	return;
}

void ObjectData::LoadSaveData()
{
	std::map<int, int> hitNumberList;

	FILE* fp = NULL;
	const char* fname = "Assets/binarydata/savedata_obj.txt";
	int number = 0;
	if (fopen_s(&fp, fname, "rb") == 0) {
		int i = 1;
		while (i != EOF) {
			//fscanfでファイルを読み込む、戻り値がEOFだった場合処理を終了する
			i = fscanf(fp, "%d", &number);
			if (i == EOF) {
				break;
			}
			hitNumberList.emplace(number, 0);
		}
		fclose(fp);
	}
	int j = 0;
	for (auto itr : m_modelObjDataList) {
		itr.second->s_number = j;
		if (hitNumberList.count(j) != 0) {
			itr.second->s_isHit = true;
		}
		j++;
	}
	int i = 0;
}

void ObjectData::SaveData()
{
	std::string fileName = "Assets/binarydata/savedata_obj.txt";

	std::ofstream file;
	file.open(fileName, std::ios::trunc);

	for (auto itr : m_modelObjDataList) {
		if (itr.second->s_isHit) {
			file << itr.second->s_number << std::endl;
		}
	}
	file.close();
}