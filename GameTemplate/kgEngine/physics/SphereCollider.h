#pragma once
#include "Physics/ICollider.h"
//�X�t�B�A�R���C�_�[
class SphereCollider : public ICollider
{
public:
	//�f�X�g���N�^
	~SphereCollider();
	/*!
	* @brief	�쐬�B
	*@param[in] radius	���̂̔��a�B
	*/
	void Create(float radius)
	{
		m_shape = new btSphereShape(radius);
		this->m_radius = radius;
	}
	btCollisionShape* GetBody() const override
	{
		return m_shape;
	}
	float GetRadius() const
	{
		return m_radius;
	}
	//���a��ݒ�
	void SetRadius(const float& radius)
	{
		//�ݒ肳�ꂽ���a�����̔��a�ƈႤ�ꍇ�A�V����Shape����蒼��
		/*if (m_radius != radius) {
			delete m_shape;
			m_shape = new btSphereShape(radius);
			this->m_radius = radius;
		}*/
		m_shape->setUnscaledRadius(radius);
		m_radius = radius;
	}
private:
	btSphereShape* m_shape = nullptr;
	float m_radius = 0.0f;
};

