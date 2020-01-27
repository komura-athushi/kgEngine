#pragma once


class GameData;
//�����L���O�������
class Ranking
{
private:
	Ranking();
	~Ranking();
public:
	static Ranking& GetInstance()
	{
		static Ranking instance;
		return instance;
	}
	//�f�[�^�����[�h���܂�
	void Load();
	//�f�[�^���Z�[�u���܂�
	void Save();
	//�\�[�g����
	void Sort();
	//�����L���O�f�[�^�̃T�C�Y���擾���܂�
	int GetRankingDataSize() const
	{
		return m_rankingList.size();
	}
	//����̃v���C���[�������L���O�łǂ̏��ʂȂ̂����擾���܂�
	//�����L���O�̏��ʂłȂ����0��Ԃ��܂�
	int GetThisRanking() const
	{
		return m_number;
	}
	//�����L���O�̒l���擾���܂�
	int GetRanking(const int& number)
	{
		return m_rankingList[number - 1];
	}
	static const int m_rankingSize = 3;
private:
	//���l��r
	void Comparison(const int number);
private:
	std::vector<int> m_rankingList;
	GameData* m_gameData = nullptr;
	int m_number = 0;
};

