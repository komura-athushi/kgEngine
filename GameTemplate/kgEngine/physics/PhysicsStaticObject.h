/*!
 * @brief	静的オブジェクト。
 */

#pragma once

#include "physics/MeshCollider.h"
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
		* @brief	メッシュの静的オブジェクトを作成。
		*@param[in]	skinModel	スキンモデル。
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*/
	void CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot);
	/*!
		* @brief	メッシュの静的オブジェクトを作成(スキンモデルレンダラーVer)
		*@param[in]	skinModelRender		スキンモデルレンダラー
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*/
	void CreateMeshObject(CSkinModelRender* skinModelRender, CVector3 pos, CQuaternion rot);
private:
	MeshCollider m_meshCollider;		//!<メッシュコライダー。
	RigidBody m_rigidBody;				//!<剛体。
};
