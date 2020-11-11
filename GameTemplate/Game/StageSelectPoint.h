/*****************************************************************//**
 * \file   StageSelectPoint.h
 * \brief  StageSelectPointクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
class StageSelectGround;

enum enPoint {
	enPoint_nun,
	enPoint_Stage1 = 1,
	enPoint_Stage2 = 2,
	enPoint_Collection,
	enPoint_Battle
};

/**
 * \brief ステージセレクトのポイント.
 */
class StageSelectPoint:public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	StageSelectPoint();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~StageSelectPoint() override final;
	/**
	 * \brief 初期化.
	 * 
	 * \return trueで初期化完了
	 */
	bool Start() override final;
	/**
	 * \brief 更新処理.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief 座標を設定.
	 * 
	 * \param pos 座標
	 */
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	/**
	 * ^\brief 座標を取得.
	 * 
	 * \return 座標
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/**
	 * \brief 回転を設定.
	 * 
	 * \param rot 回転
	 */
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	/**
	 * \brief 大きさを設定.
	 * 
	 * \param scale 大きさ
	 */
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
	/**
	 * \brief ポイントの属性を設定.
	 * 
	 * \param point ポイントの属性
	 */
	void SetPoint(enPoint point)
	{
		m_enPoint = point;
	}
	/**
	 * \brief ポイントの属性を取得.
	 * 
	 * \return ポイントの属性
	 */
	 const enPoint GetPoint() const
	{
		return m_enPoint;
	}
private:
	/**
	 * \brief ローカル行列を計算.
	 * 
	 */
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

