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
	//À•W‚ğİ’è
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	//À•W‚ğæ“¾
	const CVector3& GetPosition()
	{
		return m_position;
	}
	//‰ñ“]‚ğİ’è
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	//”Ô†‚ğİ’è
	void SetNumber(int number)
	{
		m_number = number;
	}
	//”Ô†‚ğæ“¾
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

