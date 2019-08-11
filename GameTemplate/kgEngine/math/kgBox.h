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
	bool GetisLowPositionY(const CVector3& position, Enxyz xyz);
	//線分に使うための該当の面の中心の座標を取得
	CVector3 SurfaceLineSegment(Enxyz xyz);
	//該当の面の中心座標を取得
	CVector3 SurfaceVector(const int& number)
	{
		return m_vertexPosition2[number];
	}
	//配列の大きさ
	static const int m_SurfaceVectorNumber = 6;
private:
	CVector3 m_halfSize = CVector3::Zero();					//ベクトルの各要素の最大値の半分の値
	CVector3 m_vertexPosition[8];							//バウンディングボックスの各頂点座標
	CVector3 m_vertexPosition2[6];							//各面の中心座標
};

