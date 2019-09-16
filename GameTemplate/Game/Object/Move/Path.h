#pragma once
struct Point {
	CVector3 s_vector = CVector3::Zero();				//���W
	int s_number;										//�ԍ�
};

class Path
{
public:
	Path();
	~Path();
	//�p�X�f�[�^�����[�h
	void Load(const wchar_t* filePath);
	//�p�X�̃|�C���g�̐����擾
	int GetNumberPoint()
	{
		return m_pointList.size();
	}
	//�ŏ��̃|�C���g���擾
	Point* GetFirstPoint()
	{
		return &m_pointList[0];
	}
	//��ԋ߂��ꏊ�ɂ���|�C���g���擾
	Point* GetNearPoint(const CVector3& pos);
	/*!
	*@brief	���̔ԍ��̃|�C���g���擾
	*@param[in]	number		���݂̃|�C���g�̔ԍ�
	*/
	Point* GetPoint(const int& number);
private:
	std::vector<Point> m_pointList;
};

