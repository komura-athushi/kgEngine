#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"
class Ground:public IGameObject
{
public:
	Ground();
	~Ground();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	CSkinModelRender* m_skinModelRender = nullptr;
	PhysicsStaticObject m_staticobject;
	CVector3 m_position = CVector3::Zero();
};

