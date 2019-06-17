#pragma once
//オブジェクトのデータ
struct StructObjectData {
	wchar_t* s_name;						//オブジェクトの名前
	float s_x;								//幅
	float s_y;								//高さ
	float s_z;								//奥行
	float s_volume;							//体積
	int s_issphere;							//球体かどうか
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
		ObjectData instance;
		return instance;
	}
private:
	std::vector<StructObjectData> m_objectdataList;
};



