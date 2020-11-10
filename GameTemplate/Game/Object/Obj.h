/*****************************************************************//**
 * \file   Obj.h
 * \brief  オブジェクト関連
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "Move/IMove.h"
#include "Rotation/IRot.h"
#include "Anim/Anim.h"
#include "ObjectData.h"
#include "physics/PhysicsStaticObject.h"
#include "LineSegment.h"
#include "math/kgBox.h"
#include "graphics/2D/kgFont.h"


class Player;
class GameData;
struct ObjModelData {
	CSkinModelRender s_skinModel;	//モデル
	int s_maxInstance = 0;	//モデルの最大数
	int s_hashKey = 0;	//名前から生成したハッシュ値
};

/**
 * \brief オブジェクトのデータを管理.
 */
class ObjModelDataFactory : public IGameObject{
private:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	ObjModelDataFactory();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~ObjModelDataFactory() {}
public:
	/**
	 * \brief インスタンスを取得.
	 * 
	 * \return インスタンス
	 */
	static ObjModelDataFactory& GetInstance()
	{
		static ObjModelDataFactory instance;
		return instance;
	}
	/**
	 * \brief 初期化処理.
	 * 
	 * \return trueで初期化完了
	 */
	bool Start() override final;
	/**
	 * \brief 更新処理の前処理.
	 * 
	 */
	void PreUpdate() override final;
	/**
	 * \brief 各スキンモデルのインスタンシング用のモデルを初期化.
	 * 
	 */
	void InitInstancingData();
	/**
	 * \brief インスタシング描画始める前の前処理.
	 * 
	 */
	void BeginUpdateInstancingData();
	/**
	 * \brief オブジェクト用のデータを削除する	.
	 * 
	 */
	void DeleteAllData();
	/**
	 * \brief ゲーム中、巻き込んだオブジェクトを右下に表示するためのモデルを取得.
	 * 
	 * \param key オブジェクトのハッシュ値
	 * \return スキンモデル
	 */
	SkinModel* GetSkinModel(int key)
	{
		return m_skinModelMap[key].get();
	}
public:
	/**
	 * \brief オブジェクト用のモデルをロード.
	 * 
	 * \param path オブジェクトの名前
	 * \return モデルデータ
	 */
	ObjModelData* Load(const wchar_t* path);
private:
	//unique_ptr  スマートポインタ、newしたメモリを指すポインタが存在しなければ自動的に
	//deleteされる
	std::unordered_map<int, std::unique_ptr<ObjModelData>> m_modelMap;			//ゲームで使用するモデルデータのマップ
	std::unordered_map<int, std::unique_ptr<SkinModel>> m_skinModelMap;							//塊魂小窓用のモデル
};

/**
 * \brief インスタンスを取得.
 * 
 * \return インスタンス
 */
static inline ObjModelDataFactory& GetObjModelDataFactory()
{
	return ObjModelDataFactory::GetInstance();
}

/**
 * \brief オブジェクト.
 */
class Obj : public IGameObject{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Obj();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Obj() override final;
	/**
	 * \brief 初期化処理.
	 * 
	 * \return trueで初期化完了
	 */
	bool Start() override final;
	/**
	 * \brief 更新処理.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief 移動に関するステータスを設定.
	 * 
	 * \param state 移動ステート
	 * \param pos 座標
	 * \param move 移動速度
	 * \param movelimit 移動範囲
	 * \param rot 回転
	 */
	void InitMove(const EnMove state ,const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot = CQuaternion::Identity());
	/**
	 * \brief 回転に関するステータスを設定.
	 * 
	 * \param state 回転ステート
	 * \param speed 回転速度
	 */
	void InitRot(const EnRotate state, const float speed);
	/**
	 * \brief プレイヤーを中心としたローカル行列を計算.
	 * 
	 * \param worldMatrix プレイヤーのワールド行列
	 */
	void ClcLocalMatrix(const CMatrix& worldMatrix);
	/**
	 * \brief 大きさを取得.
	 * 
	 * \return 大きさ
	 */
	const float GetSize() const
	{
		return m_size;
	}
	/**
	 * \brief 座標を取得.
	 * 
	 * \return 座標
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/**
	 * \brief プレイヤーに巻き込まれたというフラグをセット.
	 * 
	 */
	void SetHit()
	{
		m_moveState = enMove_MoveHit;
	}
	/**
	 * \brief オブジェクトデータを設定.
	 * 
	 * \param objdata オブジェクトデータ
	 */
	void SetObjData(StructObjectData* objdata)
	{
		m_objData = objdata;
	}
	/**
	 * \brief オブジェクトデータを取得.
	 * 
	 * \return オブジェクトデータ
	 */
	const StructObjectData& GetObjData() const
	{
		return *m_objData;
	}
	/**
	 * \brief プレイヤーに巻き込まれているかどうか判定.
	 * 
	 * \return プレイヤーに巻き込まれていたらtrue
	 */
	bool GetisHitPlayer()
	{
		return m_moveState == enMove_MoveHit;
	}
	/**
	 * \brief モデルを描画しないように設定する.
	 * 
	 */
	void SetNoDraw()
	{
		m_draw = false;
	}
	/**
	 * \brief モデルを読み込む.
	 * 
	 * \param path モデルの名前
	 */
	void SetFilePath(const wchar_t* path);
	/**
	 * \brief パスデータを読み込む.
	 * 
	 * \param number パスの名前
	 */
	void ReadMovePath(const int& number);
private:
	/**
	 * \brief ワールド行列を計算.
	 * 
	 */
	void ClcMatrix();
	/**
	 * \brief バウンディングボックスを計算.
	 * 
	 */
	void ClcVertex();
private:
	ObjModelData* m_modelData;												//スキンモデル
	CVector3 m_position = CVector3::Zero();									//座標								
	CQuaternion m_rotation = CQuaternion::Identity();						//回転
	CMatrix m_localMatrix;													//ローカル行列
	CMatrix m_worldMatrix;													//ワールド行列
	EnMove m_moveState = enMove_No;											//移動ステート
	EnRotate m_rotState = enRot_No;											//回転ステート
	IMovePtr  m_move;														//移動処理をするクラス
	IRotatePtr m_rot;														//回転処理をするクラス
	Anim m_anim;															//アニメーション再生をするクラス
	Player* m_player = nullptr;												//プレイヤー
	float m_size = 0.0f;													//オブジェクトの半径
	StructObjectData* m_objData;											//オブジェクトのデータ
	PhysicsStaticObject m_staticObject;										//スタティックオブジェクト
	bool m_isSphere = false;												//球体かどうか
	bool m_isLineSegment = false;											//線分生成するかどうか
	float m_lenght = 0.0f;													
	LineSegment m_lineSegment;												//線分クラス
	CVector3 m_lineVector;													//線分ベクトル
	bool m_isClcLineSegment = false;										//線分を計算するかどうか
	CBox m_box;																//バウンディングボックス
	CFont m_font;															//文字
	bool m_draw = true;														//ドローするかどうか
	GameData* m_gameData = nullptr;											//ゲームデータ
	float m_radius = 0.0f;													//半径(大きさ)
	bool m_isHit = false;													//巻き込まれた？
	
};

