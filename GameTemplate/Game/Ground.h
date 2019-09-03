#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"
//地面とか
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
	CSkinModelRender m_skinModelRender;								//スキンモデルレンダー
	PhysicsStaticObject m_staticobject;								//静的オブジェクト
	CVector3 m_position = CVector3::Zero();							//座標
};

