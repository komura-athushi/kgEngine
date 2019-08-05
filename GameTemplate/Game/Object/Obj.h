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

class ObjModelDataFactory {
private:
	ObjModelDataFactory() {}
	~ObjModelDataFactory() {}
public:
	ObjModelDataFactory& GetInstance()
	{
		static ObjModelDataFactory instance;
		return instance;
	}
public:
	CSkinModelRender* Load(const wchar_t* filepath);
private:
	//unique_ptr  スマートポインタ、newしたメモリを指すポインタが存在しなければ自動的に
	//deleteされる
	std::unordered_map<int, std::unique_ptr<CSkinModelRender>> m_modelmap;
};

class Obj : public IGameObject{
public:
	Obj();
	~Obj();
	bool Start() override;
	void Update() override;
	void PostRender() override;
	/// <summary>
	/// 移動に関するステータスを設定
	/// </summary>
	/// <param name="state">ステート</param>
	/// <param name="pos">座標</param>
	/// <param name="move">移動速度</param>
	/// <param name="movelimit">移動範囲</param>
	/// <param name="rot">回転</param>
	void InitMove(EnMove state ,const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity());
	/// <summary>
	/// 回転に関するステータスを設定
	/// </summary>
	/// <param name="state">ステート</param>
	/// <param name="speed">回転速度</param>
	void InitRot(EnRotate state, const float& speed);
	//プレイヤーを基準としたローカル行列を計算
	void ClcLocalMatrix(const CMatrix& worldMatrix);
	//プレイヤーに巻き込まれたときの移動や回転の処理
	void ClcMatrix();
	//当たり判定用の頂点データを生成
	void ClcVertex();
	//大きさを取得
	float GetSize() const
	{
		return m_size;
	}
	//座標を取得
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//プレイヤーに巻き込まれた！
	void SetHit()
	{
		m_movestate = enMove_MoveHit;
	}
	//プレイヤーにくっついているかどうかを取得
	bool GetisStickPlayer()
	{
		return m_movestate == enMove_MoveHit;
	}
	//オブジェクトデータを設定
	void SetObjData(StructObjectData* objdata)
	{
		m_objdata = objdata;
	}
	//オブジェクトデータを取得
	StructObjectData& GetObjData()
	{
		return *m_objdata;
	}
	//当たり判定用のオブジェクトの長さを取得
	int GetLenght() const
	{
		return m_lenght;
	}
	//球体かどうか
	bool GetisSphere() const
	{
		return m_issphere;
	}
	//バウンディングボックスを取得
	CBox* GetBox()
	{
		return &m_box;
	}
	//ファイルパスを設定、cmoファイルを読み込む
	void SetFilePath(const wchar_t* path);
	//パス移動用のファイルパスを読みこむ
	void ReadMovePath(const int& number);
private:
	CSkinModelRender m_skin;												//スキンモデル
	CVector3 m_position = CVector3::Zero();									//座標								
	CQuaternion m_rotation = CQuaternion::Identity();						//回転
	CMatrix m_localMatrix;													//ローカル行列
	CMatrix m_worldMatrix;													//ワールド行列
	EnMove m_movestate = enMove_No;											//移動ステート
	EnRotate m_rotstate = enRot_No;											//回転ステート
	IMove*  m_move = nullptr;												//移動処理をするクラス
	IRotate* m_rot = nullptr;												//回転処理をするクラス
	Anim m_anim;															//アニメーション再生をするクラス
	Player* m_player = nullptr;												//プレイヤー
	float m_size = 0.0f;							//オブジェクトの半径
	StructObjectData* m_objdata;											//オブジェクトのデータ
	PhysicsStaticObject m_staticobject;										//スタティックオブジェクト
	bool m_issphere = false;												//球体かどうか
	bool m_islinesegment = false;											//線分生成するかどうか
	float m_lenght = 0.0f;													
	LineSegment m_linesegment;												//線分クラス
	CVector3 m_linevector;													//線分ベクトル
	bool m_isclclinesegment = false;										//線分を計算するかどうか
	CBox m_box;																//バウンディングボックス
	CFont m_font;															//文字
	
};

