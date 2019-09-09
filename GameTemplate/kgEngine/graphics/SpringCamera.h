#pragma once
//バネカメラ
class SpringCamera
{
public:
	SpringCamera() {}
	~SpringCamera() {}
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="maxMoveSpeed">最大速度</param>
	/// <param name="attenuationRate">減衰率</param>
	void Init(
		float maxMoveSpeed,
		float attenuationRate,
		const CVector3& position,
		const CVector3& target
	);
	/// <summary>
	/// 座標を計算
	/// </summary>
	/// <param name="positionNow">今の座標</param>
	/// <param name="positionTarget">目標の座標</param>
	/// <param name="moveSpeed">移動速度</param>
	/// <returns></returns>
	CVector3 ClcSpringVector(
		const CVector3& positionNow,
		const CVector3& positionTarget,
		CVector3& moveSpeed
		);
	//更新
	void Update();
	//座標を設定
	void SetPosition(const CVector3 position)
	{
		m_position = position;
	}
	//座標を取得
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//注視点を設定
	void SetTarget(const CVector3 target)
	{
		m_target = target;
	}
	//注視点を取得
	CVector3 GetTarget() const
	{
		return m_target;
	}
	//リフレッシュを設定する
	void SetRefresh(bool isRefresh)
	{
		m_isRefresh = isRefresh;
	}
	//カメラをストップする
	void SetStop()
	{
		m_isStop = true;
	}
private:
	CVector3 m_position = CVector3::Zero();					//座標
	CVector3 m_target = CVector3::Zero();					//注視点
	float m_attenuationRate = 0.0f;							//減衰率
	CVector3 m_positionMoveSpeed = CVector3::Zero();		//座標の移動速度
	CVector3 m_targetMoveSpeed = CVector3::Zero();			//注視点の移動速度
	float m_maxMoveSpeed = 0.0f;							//最高移動速度
	bool m_isRefresh = false;								//カメラの座標や注視点を一気に切り替える
	bool m_isStop = false;
};

