/*!
 * @brief	静的オブジェクト。
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/SphereCollider.h"
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
private:
	MeshCollider m_meshCollider;		//!<メッシュコライダー。
	SphereCollider m_sphereCollider;	//スフィアコライダー
	RigidBody m_rigidBody;				//!<剛体。
	float m_radius = 0.0f;				//半径
	bool m_isMeshCollider = false;		//メッシュコライダーかどうか
	bool m_isSphereCollider = false;	//スフィアコライダーかどうか
};
