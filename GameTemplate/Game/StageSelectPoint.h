#pragma once

class StageSelectGround;

enum enPoint {
	enPoint_nun,
	enPoint_Stage1 = 1,
	enPoint_Stage2 = 2,
	enPoint_Collection,
	enPoint_Battle
};

class StageSelectPoint:public IGameObject
{
public:
	StageSelectPoint();
	~StageSelectPoint();
	bool Start() override;
	void Update() override;
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
	//属性を設定
	void SetPoint(enPoint point)
	{
		m_enPoint = point;
	}
	//属性を取得
	enPoint GetPoint()
	{
		return m_enPoint;
	}
private:
	//ローカル行列を計算
	void ClcLocalMatrix();
private:
	CSkinModelRender m_model;								//スキンモデルレンダー
	CVector3 m_position;									//座標		
	CQuaternion m_rotation;									//回転
	CVector3 m_scale;										//大きさ
	CMatrix m_localMatrix;									//ローカル行列
	CMatrix m_worldMatrix;									//ワールド行列
	StageSelectGround* m_stageSelectGround = nullptr;		//ちきう
	bool m_isFind = false;									//ちきう見つかった？
	enPoint m_enPoint = enPoint_nun;
};

