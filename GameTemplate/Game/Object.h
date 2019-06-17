#pragma once
#include "../kgEngine/physics/PhysicsStaticObject.h"
class Object:public IGameObject
{
public:
	Object();
	~Object();
	bool Start() override;
	void Update() override;
	//void OnDestroy() override;
	//座標を取得
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//座標を設定
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	//オブジェクトの大きさの倍率を設定
	void SetScale(const float& scale)
	{
		m_scale = scale;
	}
	//オブジェクトの大きさを取得、今は球体の半径
	float GetSize() const
	{
		return m_size;
	}
private:
	CSkinModelRender* m_skinModelRender = nullptr;
	PhysicsStaticObject m_staticobject;
	CVector3 m_position = {00.0f,100.0f,00.0f};
	float m_size = 45.0f;							//オブジェクトの半径
	float m_scale = 1.0f;
};

