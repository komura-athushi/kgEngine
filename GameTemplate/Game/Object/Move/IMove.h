#pragma once
enum EnMove {
	enMove_Lr,									//左右に移動
	enMove_Fb,
	enMove_Up,									//上下に移動
	enMove_Rot,									//回転
	enMove_Path,
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
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="move">移動速度</param>
	/// <param name="movelimit">移動範囲</param>
	/// <param name="rot">回転</param>
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
protected:
	CVector3 m_position = CVector3::Zero();					//座標
	CVector3 m_movevector = CVector3::Zero();				//移動ベクトル
private:
	EnMove m_state;							//ステート
};
//IMoveのunique_ptrの別名定義。
using IMovePtr = std::unique_ptr<IMove>;

