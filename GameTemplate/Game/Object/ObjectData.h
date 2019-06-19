#pragma once
//�I�u�W�F�N�g�̃f�[�^
struct StructObjectData {
	wchar_t* s_name;						//�I�u�W�F�N�g�̖��O
	float s_x;								//��
	float s_y;								//����
	float s_z;								//���s
	float s_volume;							//�̐�
	int s_issphere;							//���̂��ǂ���
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
private:
	std::vector<StructObjectData> m_objectdataList;
};



