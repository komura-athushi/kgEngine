#pragma once
#include "Physics/ICollider.h"
//スフィアコライダー
class SphereCollider : public ICollider
{
public:
	//デストラクタ
	~SphereCollider();
	/*!
	* @brief	作成。
	*@param[in] radius	球体の半径。
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
	//半径を設定
	void SetRadius(const float& radius)
	{
		//設定された半径が今の半径と違う場合、新しくShapeを作り直す
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

