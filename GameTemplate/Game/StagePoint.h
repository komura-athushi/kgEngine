#pragma once
class StageSelectGround;
class StagePoint : public IGameObject
{
public:
	StagePoint();
	~StagePoint();
	bool Start() override;
	void Update() override;
	void ClcLocalMatrix();
	//���W��ݒ�
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	//���W���擾
	const CVector3& GetPosition()
	{
		return m_position;
	}
	//��]��ݒ�
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	//�ԍ���ݒ�
	void SetNumber(int number)
	{
		m_number = number;
	}
	//�ԍ����擾
	int GetNumber() const
	{
		return m_number;
	}
private:
	CSkinModelRender m_model;
	CVector3 m_position;
	CQuaternion m_rotation;
	CMatrix m_localMatrix;
	CMatrix m_worldMatrix;
	int m_number = 0;
	StageSelectGround* m_stageSelectGround = nullptr;
};

