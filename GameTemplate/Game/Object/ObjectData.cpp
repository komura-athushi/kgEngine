#include "stdafx.h"
#include "ObjectData.h"

ObjectData::ObjectData()
{
	FILE* fp = NULL;
	const char* fname = "Assets/binarydata/test.cag";
	float x = 0, y = 0, z = 0, volume = 0;
	int issphere = 0;
	//fopen_s�Ńt�@�C���̃I�[�v���ɐ���������0���Ԃ�炵��
	if(fopen_s(&fp, fname, "r") == 0) {
		//fscanf�̖߂�l��EOF(-1)�ɂȂ�܂Ń��[�v������
		//�ǂ݂��񂾃f�[�^���\���̂Ƃ��ĕۑ�����
		int i = 1;
		while (i != EOF) {
			wchar_t* name = new wchar_t[20];
			//fscanf�Ńt�@�C����ǂݍ��ށA�߂�l��EOF�������ꍇ�������I������
			i = fscanf(fp, "%ls %f %f %f %f %d ", name, &x, &y, &z, &volume, &issphere);
			if (i == EOF) {
				break;
			}
			m_objectdataList.push_back({ name, x, y, z, volume, issphere });
		}
		fclose(fp);
	}
	int i = wcslen(m_objectdataList[0].s_name);
	int p = wcslen(m_objectdataList[1].s_name);
	int k = m_objectdataList[3].s_z;
	int l = m_objectdataList[4].s_y;
	wchar_t c[20] = L"aiueo";
	const wchar_t* d = L"aiueo";
	if (wcscmp(c, d) == 0) {
		int i = 0;
	}
	return;
}

 