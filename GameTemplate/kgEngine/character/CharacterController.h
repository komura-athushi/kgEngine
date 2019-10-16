/*!
* @brief	�L�����N�^�R���g���[���[�B
*/

#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/RigidBody.h"


/*!
* @brief	�L�����N�^�R���g���[���[�B
*/
class CharacterController{
public:
	CharacterController() {

	}
	~CharacterController()
	{
		RemoveRigidBoby();
	}
	/*!
		* @brief	�������B
		*@param[in]	radius		�J�v�Z���R���C�_�[�̔��a�B
		*@param[in]	position	�����ʒu�B
		*/
	void Init(float radius, const CVector3& position);
	/*!
		* @brief	���s�B
		*@param[in]	deltaTime		�o�ߎ��ԁB�P�ʂ͕b�B
		*@param[in, out]	moveSpeed		�ړ����x�B�����ŏd�͉������v�Z����A���̌��ʂ�moveSpeed�ɔ��f����܂��B
		*@return �ړ���̃L�����N�^�[�̍��W�B
		*/
	const CVector3& Execute(float deltaTime, CVector3& moveSpeed);
	/*!
		* @brief	���W���擾�B
		*/
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/*!
		* @brief	���W��ݒ�B
		*/
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}

	/*!
		* @brief	�W�����v��������
		*/
	bool IsJump() const
	{
		return m_isJump;
	}
	/*!
	* @brief	�n�ʏ�ɂ��邩����B
	*/
	bool IsOnGround() const
	{
		return m_isOnGround;
	}
	/*!
	* @brief	�R���C�_�[���擾�B
	*/
	SphereCollider* GetCollider()
	{
		return &m_collider;
	}
	/*!
	* @brief	���̂��擾�B
	*/
	RigidBody* GetRigidBody()
	{
		return &m_rigidBody;
	}
	//���a��ݒ�
	void SetRadius(const float& radius)
	{
		m_radius = radius;
		m_collider.SetRadius(m_radius);
	}
	//�n�ʂ̖@���x�N�g�����擾
	CVector3 GetGroundNormalVector() const
	{
		return m_GroundNormalVector;
	}
	//�ǂƏՓ˂������ǂ���
	bool GetisCollision() const
	{
		return m_isCollision;
	}
	//�ǂ̖@���x�N�g�����擾
	CVector3 GetWallNormalVector() const
	{
		return m_WallNormalVector;
	}
	
	/*!
	* @brief	���̂𕨗��G���W������폜�B�B
	*/
	void RemoveRigidBoby();
private:
	CVector3 			m_position = CVector3::Zero();	//���W�B
	bool 				m_isJump = false;				//�W�����v���H
	bool				m_isOnGround = true;			//�n�ʂ̏�ɂ���H
	SphereCollider		m_collider;						//�R���C�_�[�B
	float				m_radius = 0.0f;				//�X�t�B�A�R���C�_�[�̔��a
	bool				m_isSphere = false;				//���̂��ǂ���
	RigidBody			m_rigidBody;					//���́B
	CVector3			m_GroundNormalVector = CVector3::AxisY();
	bool				m_isCollision = false;			//�Փ˂������ǂ���
	CVector3			m_WallNormalVector = CVector3::Zero();
};

//�T�C�Y���ׂ�
bool static inline GetCompareSize(const float radius, const float size)
{
	if (radius >= size * 3.2f) {
		return true;
	}
	else {
		return false;
	}
}