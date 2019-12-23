#pragma once
class StageSelectGround;
class CollectionBook : public IGameObject
{
public:
	CollectionBook();
	~CollectionBook();
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
	//大きさを設定
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
private:
	CSkinModelRender m_model;
	CVector3 m_position;
	CQuaternion m_rotation;
	CVector3 m_scale = CVector3::One();
	CMatrix m_localMatrix;
	CMatrix m_worldMatrix;
	StageSelectGround* m_stageSelectGround = nullptr;
	bool m_isFind = false;
};

