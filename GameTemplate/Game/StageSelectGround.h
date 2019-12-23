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
	//移動速度を求める
	const CVector3& GetMoveSpeed() const
	{
		return m_currentPosition - m_beforePosition;
	}
	//モデルを取得
	CSkinModelRender& GetModel()
	{
		return  m_skinModelRender;
	}
	//モデルの回転を取得
	/*const CMatrix& GetMatrix()
	{
		return m_model.GetSkinModel().GetWorldMatrix();
	}*/
	
private:
	CSkinModelRender m_skinModelRender;
	CVector3 m_position;
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_currentPosition, m_beforePosition = CVector3::Zero();

};

