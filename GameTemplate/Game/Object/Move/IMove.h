#pragma once
enum EnMove {
	enMove_Lr,									//左右に移動
	enMove_Up,									//上下に移動
	enMove_Rot,									//回転
	enMove_No,									//移動しない
	enMove_MoveHit,								//プレイヤー(球体に巻き込まれた)
};

//Moveクラスの基本クラス
class IMove
{
public:
	IMove() {}
	virtual ~IMove() {}
	//移動後の座標を返す
	virtual CVector3 Move() { return CVector3::Zero(); }
	//ステートを設定、派生クラスで設定する用
	virtual void SetMoveState() {}
	/*!
	* @brief	初期化。
	*@param[in] pos				座標
	*@param[in]	move			移動速度	
	*@param[in]	movelimit		移動範囲
	*@param[in]	rot				回転
	*/
	virtual void Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity()) {}
	//移動ベクトルを取得
	virtual CVector3 GetMoveVector() const { return CVector3::Zero(); }
	//ステートを取得
	EnMove GetMoveState()
	{
		return m_state;
	}
	//ステートを設定
	void SetMoveStateBasic(EnMove state)
	{
		m_state = state;
	}
private:
	EnMove m_state;
};

