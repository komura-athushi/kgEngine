#include "stdafx.h"
#include "ObjectData.h"

ObjectData::ObjectData()
{
	FILE* fp = NULL;
	const char* fname = "Assets/binarydata/test.cag";
	float x = 0, y = 0, z = 0, volume = 0;
	int issphere = 0,islinesegment = 0,isanimation = 0, isMeshCollider = 0;
	//fopen_sでファイルのオープンに成功したら0が返るらしい
	if(fopen_s(&fp, fname, "rb") == 0) {
		//fscanfの戻り値がEOF(-1)になるまでループさせる
		//読みこんだデータを構造体として保存する
		int i = 1;
		while (i != EOF) {
			wchar_t* name = new wchar_t[20];
			//fscanfでファイルを読み込む、戻り値がEOFだった場合処理を終了する
			i = fscanf(fp, "%ls %f %f %f %f %d %d %d %d", name, &x, &y, &z, &volume, &issphere, &islinesegment, &isanimation, &isMeshCollider);
			if (i == EOF) {
				break;
			}
			/*x *= 0.8f;
			y *= 0.8f;
			z *= 0.8f;*/
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
				m_objectdataList.push_back({ name, x, y, z, volume, issphere,islinesegment,isMeshCollider,isanimation,linevector,state });
			}
			else {
				m_objectdataList.push_back({ name, x, y, z, volume, issphere,islinesegment,isanimation,isMeshCollider });
			}
		}
		fclose(fp);
	}
	return;
}

 