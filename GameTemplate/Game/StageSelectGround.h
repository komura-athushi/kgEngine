#pragma once
class StageSelectGround : public IGameObject
{
public:
	StageSelectGround();
	~StageSelectGround();
	bool Start() override;
	void Update() override;
	//座標を設定
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
private:
	//回転
	void Turn();
	//移動
	void Move();
private:
	CSkinModelRender m_skinModelRender;									//ちきうのモデル
	CVector3 m_position;												//座標
	CQuaternion m_rotation = CQuaternion::Identity();					//回転
	CVector3 m_currentPosition, m_beforePosition = CVector3::Zero();	//移動する前と後の座標

};

