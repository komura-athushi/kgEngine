/*****************************************************************//**
 * \file   StageSelectGround.h
 * \brief  StageSelectGroundクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
/**
 * \brief ステージセレクト画面の地面.
 */
class StageSelectGround : public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	StageSelectGround();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~StageSelectGround() override final;
	/**
	 * \brief 初期化処理.
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
	 * \param position 座標
	 */
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	/**
	 * \brief 地面の移動速度を取得.
	 * 
	 * \return 移動速度
	 */
	const CVector3& GetMoveSpeed() const
	{
		return m_currentPosition - m_beforePosition;
	}
	/**
	 * \brief スキンモデルレンダーを取得.
	 * 
	 * \return スキンモデルレンダラー
	 */
	CSkinModelRender& GetModel()
	{
		return  m_skinModelRender;
	}
private:
	/**
	 * \brief 回転.
	 * 
	 */
	void Turn();
	/**
	 * \brief 移動.
	 * 
	 */
	void Move();
private:
	CSkinModelRender m_skinModelRender;									//ちきうのモデル
	CVector3 m_position;												//座標
	CQuaternion m_rotation = CQuaternion::Identity();					//回転
	CVector3 m_currentPosition, m_beforePosition = CVector3::Zero();	//移動する前と後の座標

};

