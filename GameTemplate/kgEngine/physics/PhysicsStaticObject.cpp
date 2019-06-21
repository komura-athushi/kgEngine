/*!
 * @brief	�ÓI�I�u�W�F�N�g�B
 */


#include "KGstdafx.h"
#include "physics/PhysicsStaticObject.h"


PhysicsStaticObject::PhysicsStaticObject()
{
}
PhysicsStaticObject::~PhysicsStaticObject()
{
	if (!m_isdeleterigidbody) {
		Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
	}
}

void PhysicsStaticObject::CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot)
{
	//���b�V���R���C�_�[���쐬�B
	m_meshCollider.CreateFromSkinModel(skinModel, nullptr);
	//���̂��쐬�A
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_meshCollider; //���̂Ɍ`��(�R���C�_�[)��ݒ肷��B
	rbInfo.mass = 0.0f;
	rbInfo.pos = pos;
	rbInfo.rot = rot;
	m_rigidBody.Create(rbInfo);
	//���̂𕨗����[���h�ɒǉ�����B
	Engine().GetPhysicsEngine().AddRigidBody(m_rigidBody);
	m_isMeshCollider = true;
	m_isSphereCollider = false;
}

void PhysicsStaticObject::CreateMeshObject(CSkinModelRender* skinModelRender, CVector3 pos, CQuaternion rot)
{
	//�X�L�����f���Ł��̊֐��Ăяo���]�`
	skinModelRender->UpdateMatrix();
	CreateMeshObject(skinModelRender->GetSkinModel(), pos, rot);
}

void PhysicsStaticObject::CreateSphereObject(const float& radius, const CVector3& pos, const CQuaternion& rot)
{
	//�X�t�B�A�R���C�_�[���쐬�B
	m_sphereCollider.Create(radius);
	//���̂��쐬�A
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_sphereCollider; //���̂Ɍ`��(�R���C�_�[)��ݒ肷��B
	rbInfo.mass = 0.0f;
	rbInfo.pos = pos;
	rbInfo.rot = rot;
	m_rigidBody.Create(rbInfo);
	//���̂𕨗����[���h�ɒǉ�����B
	Engine().GetPhysicsEngine().AddRigidBody(m_rigidBody);
	m_isSphereCollider = true;
	m_isMeshCollider = false;
}

void PhysicsStaticObject::CreateBoxObject(CVector3 pos, CQuaternion rot, CVector3 size)
{
	//Box�R���C�_�[���쐬�B
	m_boxCollider.Create(size);

	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_boxCollider;
	rbInfo.mass = 0.0f;
	rbInfo.pos = pos;
	rbInfo.rot = rot;
	m_rigidBody.Create(rbInfo);
	//���̂𕨗����[���h�ɒǉ�����B
	Engine().GetPhysicsEngine().AddRigidBody(m_rigidBody);
}

void PhysicsStaticObject::SetPosition(CVector3& pos)
{
	btTransform& worldTrans = m_rigidBody.GetBody()->getWorldTransform();
	auto& origin = worldTrans.getOrigin();
	origin.setX(pos.x);
	origin.setY(pos.y);
	origin.setZ(pos.z);
}

void PhysicsStaticObject::SetRotation(CQuaternion& rot)
{
	btQuaternion btq(rot.x, rot.y, rot.z, rot.w);
	btTransform& worldTrans = m_rigidBody.GetBody()->getWorldTransform();
	auto rotation = worldTrans.getRotation();
	rotation = btq;
}