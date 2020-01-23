#pragma once


class GameData;
//ランキング作ったり
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
	//データをロードします
	void Load();
	//データをセーブします
	void Save();
	//ソートする
	void Sort();
	//ランキングデータのサイズを取得します
	int GetRankingDataSize() const
	{
		return m_rankingList.size();
	}
private:
	//数値比較
	void Comparison(const int number);
private:
	std::vector<int> m_rankingList;
	GameData* m_gameData = nullptr;
};

