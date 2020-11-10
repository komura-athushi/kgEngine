/*****************************************************************//**
 * \file   IRot.h
 * \brief  IRotクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma  once
enum EnRotate{
	enRot_Rot,									//自転
	enRot_DirectionRot,							//移動方向に回転
	enRot_No,									//回転しない
};

/**
 * \brief オブジェクトの回転を制御する基本クラス.
 */
class IRotate
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	IRotate() {}
	/**
	 * \brief デストラクタ　基本クラスなのでvirtual.
	 * 
	 */
	virtual ~IRotate() {}
	/**
	 * \brief 回転を計算.
	 * 
	 * \param move 移動ベクトル
	 * \return 計算後の回転
	 */
	virtual const CQuaternion Rot(const CVector3& move) { return CQuaternion::Identity(); }
	/**
	 * \brief 移動ステートを設定.
	 * 
	 */
	virtual void SetRotState() {}
	/**
	 * \brief 初期化処理.
	 * 
	 * \param rot 初期回転
	 * \param speed 回転速度(1.0fで360度を一秒で回る、-にすると反対方向に回る)
	 */
	virtual void Init(const CQuaternion& rot, const float speed = 0.0f) {}
	/**
	 * \brief 移動ステートを取得.
	 * 
	 * \return 
	 */
	EnRotate GetRotState() const
	{
		return m_state;
	}
	/**
	 * \brief 移動ステートを設定.
	 * 
	 * \param state 移動ステート
	 */
	void SetRotStateBasic(EnRotate state)
	{
		m_state = state;
	}
protected:
	CQuaternion m_rotation = CQuaternion::Identity();					//回転
private:
	EnRotate m_state;
};
//IRotateのunique_ptrの別名定義。
using IRotatePtr = std::unique_ptr<IRotate>;
