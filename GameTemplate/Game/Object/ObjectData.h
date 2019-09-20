#pragma once

#include "math/kgBox.h"

//オブジェクトのデータ
struct StructObjectData {
	wchar_t* s_name;						//オブジェクトの名前
	float s_x;								//幅
	float s_y;								//高さ
	float s_z;								//奥行
	float s_volume;							//体積
	int s_issphere;							//球体かどうか
	int s_islinesegment;					//線分を生成するかどうか
	int s_isanimation;						//アニメーションがあるかどうか
	int s_isMeshCollider;					//メッシュコライダーにするかどうか
	CVector3 s_linevector = CVector3::Zero();	//線分のベクトル
	Enxyz s_state = enState_No;					//線分がどの方向か
};

//オブジェクトのデータをバイナリファイルから読み込む
class ObjectData
{
private:
	ObjectData();
	~ObjectData() {}
public:
	static ObjectData& GetInstance()
	{
		static ObjectData instance;
		return instance;
	}
public:
	//配列のサイズを取得
	int GetListSize() const
	{
		int i = m_objectdataList.size();
		return i;
	}
	//構造体を取得
	StructObjectData* GetObjectData(const int& number)
	{
		return &m_objectdataList[number];
	}
private:
	std::vector<StructObjectData> m_objectdataList;
};



