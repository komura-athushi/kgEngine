/*****************************************************************//**
 * \file   Ranking.h
 * \brief  Rankingクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

class GameData;

/**
 * \brief プレイヤーの大きさのランキングを作ります.
 */
class Ranking
{
private:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Ranking();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Ranking();
public:
	/**
	 * \brief インスタンスを取得.
	 * 
	 * \return インスタンス
	 */
	static Ranking& GetInstance()
	{
		static Ranking instance;
		return instance;
	}
	/**
	 * \brief ランキングデータをロードします.
	 * 
	 */
	void Load();
	/**
	 * \brief ランキングデータをセーブします.
	 * 
	 */
	void Save();
	/**
	 * \brief ランキングをソートします.
	 * 
	 */
	void Sort();
	/**
	 * \brief ランキングデータの要素数を取得.
	 * 
	 * \return ランキングデータの要素数
	 */
	const int GetRankingDataSize() const
	{
		return m_rankingList.size();
	}
	/**
	 * \brief プレイヤーがランキングで何位なのか取得.
	 * 
	 * \return ランキングに入ってなかったら0
	 */
	const int GetThisRanking() const
	{
		return m_number;
	}
	/**
	 * \brief ランキングの値を取得.
	 * 
	 * \param number 順位
	 * \return ランキングの値
	 */
	const int GetRanking(const int& number) const
	{
		return m_rankingList[number - 1];
	}
	static const int m_rankingSize = 3;
private:
	/**
	 * \brief 数値比較.
	 * 
	 * \param number 数値
	 */
	void Comparison(const int number);
private:
	std::vector<int> m_rankingList;
	GameData* m_gameData = nullptr;
	int m_number = 0;
};

