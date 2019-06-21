#pragma once

//バウンディングボックス
class CBox {
public:
	CBox();
	~CBox();
	//初期化
	void Init(const CVector3& halfSize);
	//更新
	void Update(const CMatrix& worldMatrix);

private:
	CVector3 m_halfSize = CVector3::Zero();					//ベクトルの各要素の最大値の半分の値
	CVector3 m_vertexPosition[8];							//バウンディングボックスの各頂点座標
};
