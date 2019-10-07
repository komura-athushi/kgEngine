/*!
 * @brief	�ÓI�I�u�W�F�N�g�B
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/SphereCollider.h"
#include "physics/BoxCollider.h"
#include "physics/RigidBody.h"


/*!
* @brief	�ÓI�����I�u�W�F�N�g
*/
class PhysicsStaticObject{
public:
	/*!
		* @brief	�R���X�g���N�^�B
		*/
	PhysicsStaticObject();
	/*!
		* @brief	�f�X�g���N�^�B
		*/
	~PhysicsStaticObject();
	/*!
		* @brief	���b�V���̐ÓI�I�u�W�F�N�g���쐬�B�|���S�������Ȃ��Ƃ��͂�����g����
		*@param[in]	skinModel	�X�L�����f���B
		*@param[in]	pos			���W�B
		*@param[in]	rot			��]�B
		*/
	void CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot);
	/*!
		* @brief	���b�V���̐ÓI�I�u�W�F�N�g���쐬(�X�L�����f�������_���[Ver)�|���S�������Ȃ��Ƃ��͂�����g����
		*@param[in]	skinModelRender		�X�L�����f�������_���[
		*@param[in]	pos			���W�B
		*@param[in]	rot			��]�B
		*/
	void CreateMeshObject(CSkinModelRender* skinModelRender, CVector3 pos, CQuaternion rot);
	/*!
		* @brief	���̂̐ÓI�I�u�W�F�N�g���쐬
		*@param[in]	radius		���a
		*@param[in]	pos			���W�B
		*@param[in]	rot			��]�B
	*/
	void CreateSphereObject(const float& radius, const CVector3& pos, const CQuaternion& rot = CQuaternion::Identity());
	//�{�b�N�X�̐ÓI�I�u�W�F�N�g���쐬
	void CreateBoxObject(CVector3 pos, CQuaternion rot, CVector3 size);
	//���̂�j��
	void Release()
	{
		Engine().GetPhysicsEngine().RemoveRigidBody(m_rigidBody);
		m_isdeleterigidbody = true;
	}
	//���̂̃T�C�Y��ݒ�
	void SetSize(const float size)
	{
		m_rigidBody.GetBody()->SetSize(size);
	}
	//���̂��擾
	RigidBody* GetRigidBody()
	{
		return &m_rigidBody;
	}
	void SetPosition(CVector3& pos);
	void SetRotation(CQuaternion& rot);
private:
	MeshCollider m_meshCollider;		//!<���b�V���R���C�_�[�B
	SphereCollider m_sphereCollider;	//�X�t�B�A�R���C�_�[
	BoxCollider m_boxCollider;			//�{�b�N�X�R���C�_�[
	RigidBody m_rigidBody;				//!<���́B
	float m_radius = 0.0f;				//���a
	bool m_isMeshCollider = false;		//���b�V���R���C�_�[���ǂ���
	bool m_isSphereCollider = false;	//�X�t�B�A�R���C�_�[���ǂ���
	bool m_isboxcollider = false;		//�{�b�N�X�R���C�_�[���ǂ���
	bool m_isdeleterigidbody = false;	//���̂��폜�������ǂ���
};
