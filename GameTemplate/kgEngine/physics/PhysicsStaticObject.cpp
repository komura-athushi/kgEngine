/*!
 * @brief	静的オブジェクト。
 */


#include "KGstdafx.h"
#include "physics/PhysicsStaticObject.h"


PhysicsStaticObject::PhysicsStaticObject()
{
}
PhysicsStaticObject::~PhysicsStaticObject()
{
	Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
}

void PhysicsStaticObject::CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot)
{
	//メッシュコライダーを作成。
	m_meshCollider.CreateFromSkinModel(skinModel, nullptr);
	//剛体を作成、
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_meshCollider; //剛体に形状(コライダー)を設定する。
	rbInfo.mass = 0.0f;
	rbInfo.pos = pos;
	rbInfo.rot = rot;
	m_rigidBody.Create(rbInfo);
	//剛体を物理ワールドに追加する。
	Engine().GetPhysicsEngine().AddRigidBody(m_rigidBody);
	m_isMeshCollider = true;
	m_isSphereCollider = false;
}

void PhysicsStaticObject::CreateMeshObject(CSkinModelRender* skinModelRender, CVector3 pos, CQuaternion rot)
{
	//スキンモデルで↑の関数呼び出すゾ～
	skinModelRender->UpdateMatrix();
	CreateMeshObject(skinModelRender->GetSkinModel(), pos, rot);
}

void PhysicsStaticObject::CreateSphereObject(const float& radius, const CVector3& pos, const CQuaternion& rot)
{
	//スフィアコライダーを作成。
	m_sphereCollider.Create(radius);
	//剛体を作成、
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_sphereCollider; //剛体に形状(コライダー)を設定する。
	rbInfo.mass = 0.0f;
	rbInfo.pos = pos;
	rbInfo.rot = rot;
	m_rigidBody.Create(rbInfo);
	//剛体を物理ワールドに追加する。
	Engine().GetPhysicsEngine().AddRigidBody(m_rigidBody);
	m_isSphereCollider = true;
	m_isMeshCollider = false;
}