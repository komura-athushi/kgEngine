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
	//今回のプレイヤーがランキングでどの順位なのかを取得します
	//ランキングの順位でなければ0を返します
	int GetThisRanking() const
	{
		return m_number;
	}
	//ランキングの値を取得します
	int GetRanking(const int& number)
	{
		return m_rankingList[number - 1];
	}
	static const int m_rankingSize = 3;
private:
	//数値比較
	void Comparison(const int number);
private:
	std::vector<int> m_rankingList;
	GameData* m_gameData = nullptr;
	int m_number = 0;
};

