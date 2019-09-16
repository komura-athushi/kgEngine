#pragma once
struct Point {
	CVector3 s_vector = CVector3::Zero();				//座標
	int s_number;										//番号
};

class Path
{
public:
	Path();
	~Path();
	//パスデータをロード
	void Load(const wchar_t* filePath);
	//パスのポイントの数を取得
	int GetNumberPoint()
	{
		return m_pointList.size();
	}
	//最初のポイントを取得
	Point* GetFirstPoint()
	{
		return &m_pointList[0];
	}
	//一番近い場所にあるポイントを取得
	Point* GetNearPoint(const CVector3& pos);
	/*!
	*@brief	次の番号のポイントを取得
	*@param[in]	number		現在のポイントの番号
	*/
	Point* GetPoint(const int& number);
private:
	std::vector<Point> m_pointList;
};

