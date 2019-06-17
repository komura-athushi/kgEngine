#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"
class Ground:public IGameObject
{
public:
	Ground();
	~Ground();
	bool Start() override;
	void Update() override;
	//void OnDestroy() override;
private:
	CSkinModelRender* m_skinModelRender;
	PhysicsStaticObject m_staticobject1;
	PhysicsStaticObject m_staticobject2;
	PhysicsStaticObject m_staticobject3;
	CVector3 m_position = CVector3::Zero();
};

