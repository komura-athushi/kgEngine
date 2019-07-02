#pragma once
#include "Move/IMove.h"
#include "Rotation/IRot.h"
#include "ObjectData.h"
#include "physics/PhysicsStaticObject.h"
#include "LineSegment.h"
#include "math/Box.h"
#include "graphics/2D/CFont.h"
class Player;

class Obj : public IGameObject{
public:
	Obj();
	~Obj();
	bool Start() override;
	void Update() override;
	void PostRender() override;
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
	bool m_issphere = false;
	bool m_islinesegment = false;
	float m_lenght = 0.0f;
	LineSegment m_linesegment;
	CVector3 m_linevector;
	bool m_isclclinesegment = false;
	CBox m_box;										//バウンディングボックス
	CFont m_font;
	
};

