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
private:
	//���l��r
	void Comparison(const int number);
private:
	std::vector<int> m_rankingList;
	GameData* m_gameData = nullptr;
};

