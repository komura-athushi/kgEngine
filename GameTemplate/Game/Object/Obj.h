#pragma once
#include "Move/IMove.h"
#include "Rotation/IRot.h"
#include "ObjectData.h"
#include "physics/PhysicsStaticObject.h"
class Player;
struct  Vertex
{
	CVector3 s_list[22];
};

class Obj : public IGameObject{
public:
	Obj();
	~Obj();
	bool Start() override;
	void Update() override;
	//移動ステータスを設定
	void InitMove(EnMove state ,const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity());
	//回転ステータスを設定
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
	//当たり判定用の頂点配列の大きさを取得
	int GetVertexSize() const
	{
		int i = sizeof(m_bufferList) / sizeof(m_bufferList[0]);
		return i;
	}
	//該当の番号の当たり判定用の頂点データを取得
	CVector3 GetBuffer(const int& number) 
	{
		return m_bufferList[number];
	}
	//球体かどうか
	bool GetisSphere() const
	{
		return m_issphere;
	}
	//ファイルパスを設定、cmoファイルを読み込む
	void SetFilePath(const wchar_t* path);
private:
	CSkinModelRender m_skin;
	CVector3 m_position = CVector3::Zero();
	CQuaternion m_rotation = CQuaternion::Identity();
	CMatrix m_localMatrix;
	CMatrix m_worldMatrix;
	EnMove m_movestate = enMove_No;
	EnRotate m_rotstate = enRot_No;
	IMove*  m_move = nullptr;
	IRotate* m_rot = nullptr;
	Player* m_player = nullptr;
	float m_size = 0.0f;							//オブジェクトの半径
	const wchar_t* m_filepath = nullptr;
	StructObjectData* m_objdata;
	PhysicsStaticObject m_staticobject;
	CVector3 m_bufferList[22];					//当たり判定用の頂点
	//CVector3 m_vertexList[14];					//当たり判定用のベクトル(座標からのベクトル)
	bool m_issphere = false;
};

class VertexFactory
{
private:
	VertexFactory() {};
	~VertexFactory() {};
public:
	static VertexFactory& GetInstance()
	{
		static VertexFactory instance;
		return instance;
	}
	friend Obj;
private:
	std::unordered_map<const wchar_t*, Vertex> m_vertexList;
};