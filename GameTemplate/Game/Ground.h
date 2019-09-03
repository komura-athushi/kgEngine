#pragma once
#include "..//kgEngine/physics/PhysicsStaticObject.h"
//�n�ʂƂ�
class Ground:public IGameObject
{
public:
	Ground();
	~Ground();
	bool Start() override;
	void Update() override;
	//���W��ݒ�
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
private:
	CSkinModelRender m_skinModelRender;								//�X�L�����f�������_�[
	PhysicsStaticObject m_staticobject;								//�ÓI�I�u�W�F�N�g
	CVector3 m_position = CVector3::Zero();							//���W
};

