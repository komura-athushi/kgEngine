#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"
class Ground:public IGameObject
{
public:
	Ground();
	~Ground();
	bool Start() override;
	void Update() override;
	//座標を設定
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
private:
	CSkinModelRender m_skinModelRender;
	PhysicsStaticObject m_staticobject;
	CVector3 m_position = CVector3::Zero();
};

