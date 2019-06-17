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
	//���W���擾
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//���W��ݒ�
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	//�I�u�W�F�N�g�̑傫���̔{����ݒ�
	void SetScale(const float& scale)
	{
		m_scale = scale;
	}
	//�I�u�W�F�N�g�̑傫�����擾�A���͋��̂̔��a
	float GetSize() const
	{
		return m_size;
	}
private:
	CSkinModelRender* m_skinModelRender = nullptr;
	PhysicsStaticObject m_staticobject;
	CVector3 m_position = {00.0f,100.0f,00.0f};
	float m_size = 45.0f;							//�I�u�W�F�N�g�̔��a
	float m_scale = 1.0f;
};

