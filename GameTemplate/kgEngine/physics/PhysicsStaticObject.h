/*!
 * @brief	静的オブジェクト。
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/SphereCollider.h"
#include "physics/BoxCollider.h"
#include "physics/RigidBody.h"


/*!
* @brief	静的物理オブジェクト
*/
class PhysicsStaticObject{
public:
	/*!
		* @brief	コンストラクタ。
		*/
	PhysicsStaticObject();
	/*!
		* @brief	デストラクタ。
		*/
	~PhysicsStaticObject();
	/*!
		* @brief	メッシュの静的オブジェクトを作成。ポリゴンが少ないときはこれを使おう
		*@param[in]	skinModel	スキンモデル。
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*/
	void CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot);
	/*!
		* @brief	メッシュの静的オブジェクトを作成(スキンモデルレンダラーVer)ポリゴンが少ないときはこれを使おう
		*@param[in]	skinModelRender		スキンモデルレンダラー
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*/
	void CreateMeshObject(CSkinModelRender* skinModelRender, CVector3 pos, CQuaternion rot);
	/*!
		* @brief	球体の静的オブジェクトを作成
		*@param[in]	radius		半径
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
	*/
	void CreateSphereObject(const float& radius, const CVector3& pos, const CQuaternion& rot = CQuaternion::Identity());
	//ボックスの静的オブジェクトを作成
	void CreateBoxObject(CVector3 pos, CQuaternion rot, CVector3 size);
	//剛体を破棄
	void Release()
	{
		Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
		m_isdeleterigidbody = true;
	}
	//剛体のサイズを設定
	void SetSize(const float size)
	{
		m_rigidBody.GetBody()->SetSize(size);
	}
	//剛体を取得
	RigidBody* GetRigidBody()
	{
		return &m_rigidBody;
	}
	void SetPosition(CVector3& pos);
	void SetRotation(CQuaternion& rot);
private:
	MeshCollider m_meshCollider;		//!<メッシュコライダー。
	SphereCollider m_sphereCollider;	//スフィアコライダー
	BoxCollider m_boxCollider;			//ボックスコライダー
	RigidBody m_rigidBody;				//!<剛体。
	float m_radius = 0.0f;				//半径
	bool m_isMeshCollider = false;		//メッシュコライダーかどうか
	bool m_isSphereCollider = false;	//スフィアコライダーかどうか
	bool m_isboxcollider = false;		//ボックスコライダーかどうか
	bool m_isdeleterigidbody = false;	//剛体を削除したかどうか
};
