#pragma once

enum Enxyz {
	enState_X,
	enState_Y,
	enState_Z,
	enState_No
};

//バウンディングボックス
class CBox {
public:
	CBox();
	~CBox();
	//初期化
	void Init(const CVector3& halfSize);
	//更新
	void Update(const CMatrix& worldMatrix);
	void Update(const CVector3& pos,const CQuaternion& rot,const CVector3& scale = CVector3::One());
	//該当の面の中心のy座標がプレイヤーの座標より低いかどうか
	bool GetisLowPositionY(const CVector3& position, Enxyz xyz) const;
	//線分に使うための該当の面の中心の座標を取得
	CVector3 SurfaceLineSegment(Enxyz xyz) const;
	//該当の面の中心座標を取得
	CVector3 GetSurfaceVector(const int& number) const
	{
		return m_vertexPosition2[number];
	}
	//頂点の座標を取得
	CVector3 GetVertexVector(const int number) const
	{
		return m_vertexPosition[number];
	}
	//配列の大きさ
	static const int m_SurfaceVectorNumber = 6;
	static const int m_vertexNumber = 8;
private:
	CVector3 m_halfSize = CVector3::Zero();					//ベクトルの各要素の最大値の半分の値
	CVector3 m_vertexPosition[8];							//バウンディングボックスの各頂点座標
	CVector3 m_vertexPosition2[6];							//各面の中心座標
};

