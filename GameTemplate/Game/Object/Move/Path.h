#pragma once
struct Point {
	Point(CVector3 pos, int number) {
		s_vector = pos;
		s_number = number;
	}
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
	/*!
	*@brief	���̔ԍ��̃|�C���g���擾
	*@param[in]	number		���݂̃|�C���g�̔ԍ�
	*/
	Point* GetPoint(const int& number);
private:
	std::vector<Point> m_pointList;
};
