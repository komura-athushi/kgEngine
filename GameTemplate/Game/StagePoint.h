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
	//座標を設定
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	//座標を取得
	const CVector3& GetPosition()
	{
		return m_position;
	}
	//回転を設定
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	//番号を設定
	void SetNumber(int number)
	{
		m_number = number;
	}
	//番号を取得
	int GetNumber() const
	{
		return m_number;
	}
	//大きさを設定
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
private:
	CSkinModelRender m_model;
	CVector3 m_position;
	CQuaternion m_rotation;
	CVector3 m_scale;
	CMatrix m_localMatrix;
	CMatrix m_worldMatrix;
	int m_number = 0;
	StageSelectGround* m_stageSelectGround = nullptr;
	bool m_isFind = false;
};

