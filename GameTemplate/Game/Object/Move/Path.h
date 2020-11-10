/*****************************************************************//**
 * \file   Path.h
 * \brief Pathクラス  
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
struct Point {
	CVector3 s_vector = CVector3::Zero();				//座標
	int s_number = 0;									//番号
};

/**
 * \brief MovePathで使用するパスクラス.
 */
class Path
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Path();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Path();
	/**
	 * \brief パスデータをロード.
	 * 
	 * \param filePath ファイルパス
	 */
	void Load(const wchar_t* filePath);
	/**
	 * \brief パスのポイントの数を取得.
	 * 
	 * \return パスのポイントの数
	 */
	int GetNumberPoint()
	{
		return m_pointList.size();
	}
	/**
	 * \brief 最初のポイントを取得.
	 * 
	 * \return 最初のポイント
	 */
	Point* GetFirstPoint()
	{
		return &m_pointList[0];
	}
	/**
	 * \brief 座標から一番近いポイントを取得.
	 * 
	 * \param pos 座標
	 * \return 座標から一番近いポイント
	 */
	Point* GetNearPoint(const CVector3& pos);
	/**
	 * \brief 次のポイントを取得.
	 * 
	 * \param number 現在のポイントの番号
	 * \return 次のポイント
	 */
	Point* GetPoint(const int& number);
private:
	std::vector<Point> m_pointList;						//ポイントのリスト
};

