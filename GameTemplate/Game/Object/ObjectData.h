/*****************************************************************//**
 * \file   ObjectData.h
 * \brief  ObjectDataクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "math/kgBox.h"

//オブジェクトのデータ
struct StructObjectData {
	wchar_t* s_name;						//オブジェクトの名前
	char* s_jName;							//日本語の名前
	float s_x;								//幅
	float s_y;								//高さ
	float s_z;								//奥行
	float s_volume;							//体積
	int s_isSphere;							//球体かどうか
	int s_isLineSegment;					//線分を生成するかどうか
	int s_isAnimation;						//アニメーションがあるかどうか
	int s_isMeshCollider;					//メッシュコライダーにするかどうか
	int s_isSpec = 0;						//鏡面反射するかどうか
	wchar_t* s_specName;					//巻き込まれたときのSEはデフォルトか
	int s_isDefalutSe = 0;						//巻き込まれた時にデフォルトのSE使うか？
	wchar_t* s_seFileName;						//SEファイルの名前
	CVector3 s_lineVector = CVector3::Zero();	//線分のベクトル
	Enxyz s_state = enState_No;					//線分がどの方向か

};

struct SkinModel_ObjData {
	SkinModel s_skinModel;
	bool s_isHit = false;
	int s_hashKey = 0;
	float s_x, s_y, s_z = 0.0f;
	char* s_jName;
	int s_number = 0;
};
using SkinModel_ObjDataPtr = std::unique_ptr< SkinModel_ObjData>;

/**
 * \brief オブジェクトのデータを外部ファイルから読み込む.
 */
class ObjectData
{
private:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	ObjectData();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~ObjectData() {}
public:
	/**
	 * \brief インスタンスを取得.
	 * 
	 * \return インスタンス
	 */
	static ObjectData& GetInstance()
	{
		static ObjectData instance;
		return instance;
	}
public:
	/**
	 * \brief オブジェクト数を取得.
	 * 
	 * \return オブジェクトの数
	 */
	int GetListSize() const
	{
		return m_objectDataList.size();
	}
	/**
	 * \brief 該当のオブジェクトのデータを取得.
	 * 
	 * \param number リストの番号
	 * \return オブジェクトデータ
	 */
	StructObjectData* GetObjectData(const int& number)
	{
		return &m_objectDataList[number];
	}
	/**
	 * 図鑑で表示させるフラグを設定.
	 * 
	 * \param volume オブジェクトの大きさ
	 */
	void SetisHit(float volume)
	{
		m_modelObjDataList[volume]->s_isHit = true;
	}
	/**
	 * \brief 配列取得.
	 * 
	 * \return 配列 
	 */
	const std::map<float, SkinModel_ObjDataPtr>& GetList()
	{
		return m_modelObjDataList;
	}
	/**
	 * \brief セーブする.
	 * 
	 */
	void SaveData();
private:
	/**
	 * \brief セーブデータをロードする.
	 * 
	 */
	void LoadSaveData();
private:
	std::vector<StructObjectData> m_objectDataList;						//オブジェクトのデータ
	std::map<float, SkinModel_ObjDataPtr> m_modelObjDataList;			//図鑑で表示するためのオブジェクトのデータ
};

/**
 * \brief インスタンスを取得.
 * 
 * \return インスタンス
 */
static inline ObjectData& GetObjectData()
{
	return ObjectData::GetInstance();
}


