#pragma once
class StageSelectGround : public IGameObject
{
public:
	StageSelectGround();
	~StageSelectGround();
	bool Start() override;
	void Update() override;
	void Turn();
	void Move();
	void SetPosition(const CVector3 position)
	{
		m_position = position;
	}
	//ˆÚ“®‘¬“x‚ğ‹‚ß‚é
	const CVector3& GetMoveSpeed() const
	{
		return m_currentPosition - m_beforePosition;
	}
	//ƒ‚ƒfƒ‹‚Ì‰ñ“]‚ğæ“¾
	const CMatrix& GetMatrix()
	{
		return m_model.GetSkinModel().GetWorldMatrix();
	}
private:
	CSkinModelRender m_model;
	CVector3 m_position;
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_currentPosition, m_beforePosition = CVector3::Zero();
};

