#pragma once

#include "math/kgBox.h"

//�I�u�W�F�N�g�̃f�[�^
struct StructObjectData {
	wchar_t* s_name;						//�I�u�W�F�N�g�̖��O
	char* s_jName;							//���{��̖��O
	float s_x;								//��
	float s_y;								//����
	float s_z;								//���s
	float s_volume;							//�̐�
	int s_issphere;							//���̂��ǂ���
	int s_islinesegment;					//�����𐶐����邩�ǂ���
	int s_isanimation;						//�A�j���[�V���������邩�ǂ���
	int s_isMeshCollider;					//���b�V���R���C�_�[�ɂ��邩�ǂ���
	CVector3 s_linevector = CVector3::Zero();	//�����̃x�N�g��
	Enxyz s_state = enState_No;					//�������ǂ̕�����
};

struct SkinModel_ObjData {
	SkinModel s_skinModel;
	bool s_isHit = false;
	int s_hashKey = 0;
	float s_x, s_y, s_z = 0.0f;
	char* s_jName;
	int s_number = 0;
};

//�I�u�W�F�N�g�̃f�[�^���o�C�i���t�@�C������ǂݍ���
class ObjectData
{
private:
	ObjectData();
	~ObjectData() {}
public:
	static ObjectData& GetInstance()
	{
		static ObjectData instance;
		return instance;
	}
public:
	//�z��̃T�C�Y���擾
	int GetListSize() const
	{
		int i = m_objectdataList.size();
		return i;
	}
	//�\���̂��擾
	StructObjectData* GetObjectData(const int& number)
	{
		return &m_objectdataList[number];
	}
	//�������܂ꂽ�I
	void SetisHit(float volume)
	{
		m_modelObjDataList[volume]->s_isHit = true;
	}
	//�z��擾
	std::map<float, SkinModel_ObjData*> GetList()
	{
		return m_modelObjDataList;
	}
	//�Z�[�u����
	void SaveData();
private:
	//�Z�[�u�f�[�^���[�h�������
	void LoadSaveData();
private:
	std::vector<StructObjectData> m_objectdataList;						//�I�u�W�F�N�g�̃f�[�^
	std::map<float, SkinModel_ObjData*> m_modelObjDataList;				//�}�ӂŕ\�����邽�߂̃I�u�W�F�N�g�̃f�[�^
};




