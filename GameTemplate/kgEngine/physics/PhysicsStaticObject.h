/*!
 * @brief	�ÓI�I�u�W�F�N�g�B
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/SphereCollider.h"
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
private:
	MeshCollider m_meshCollider;		//!<���b�V���R���C�_�[�B
	SphereCollider m_sphereCollider;	//�X�t�B�A�R���C�_�[
	RigidBody m_rigidBody;				//!<���́B
	float m_radius = 0.0f;				//���a
	bool m_isMeshCollider = false;		//���b�V���R���C�_�[���ǂ���
	bool m_isSphereCollider = false;	//�X�t�B�A�R���C�_�[���ǂ���
};
