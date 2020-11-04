/*****************************************************************//**
 * \file   IMove.h
 * \brief  IMoveクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
enum EnMove {
	enMove_Lr,									//左右に移動
	enMove_Fb,									//前後に移動
	enMove_Up,									//上下に移動
	enMove_Rot,									//回転
	enMove_Path,								//パス移動
	enMove_No,									//移動しない
	enMove_MoveHit,								//プレイヤー(球体に巻き込まれた)
};

/**
 * \brief  オブジェクトの移動の制御をする基本クラス.
 */
class IMove
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	IMove() {}
	/**
	 * \brief デストラクタ、基本クラスなのでvirtual.
	 * 
	 */
	virtual ~IMove() {}
	/**
	 * \brief 移動処理.
	 * 
	 * \return 移動した後の座標
	 */
	virtual const CVector3& Move() { return CVector3::Zero(); } 
	/**
	 * \brief 移動ステートを設定.
	 * 
	 */
	virtual void SetMoveState() {}
	/**
	 * \brief 初期化処理.
	 * 
	 * \param pos 座標
	 * \param move 移動速度
	 * \param movelimit 移動範囲
	 * \param rot 回転
	 */
	virtual void Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot = CQuaternion::Identity()) {}
	/**
	 * 移動ベクトルを取得.
	 * 
	 * \return 移動ベクトル
	 */
	virtual const CVector3& GetMoveVector() const { return CVector3::Zero(); }
	/**
	 * \brief 移動ステートを取得.
	 * 
	 * \return 移動ステート
	 */
	const EnMove GetMoveState() const
	{
		return m_state;
	}
	/**
	 * 移動ステートを設定.
	 * 
	 * \param state 移動ステート
	 */
	void SetMoveStateBasic(EnMove state)
	{
		m_state = state;
	}
protected:
	CVector3 m_position = CVector3::Zero();					//座標
	CVector3 m_moveVector = CVector3::Zero();				//移動ベクトル
	float m_moveSpeed = 0.0f;								//移動速度
	bool m_isCulcMoveVector = false;						//移動ベクトルを計算したかどうか
private:
	EnMove m_state;							//ステート
};
//IMoveのunique_ptrの別名定義。
using IMovePtr = std::unique_ptr<IMove>;

