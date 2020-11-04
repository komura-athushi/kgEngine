#pragma  once
enum EnRotate{
	enRot_Rot,									//自転
	enRot_DirectionRot,							//移動方向に回転
	enRot_No,									//回転しない
};

//Rotクラスの基本クラス
class IRotate
{
public:
	IRotate() {}
	virtual ~IRotate() {}
	//計算後の回転を返す
	virtual CQuaternion Rot(const CVector3& move) { return CQuaternion::Identity(); }
	//ステートを設定、派生クラスで設定する用
	virtual void SetRotState() {}
	/*!
	* @brief	初期化。
	*@param[in] rot				初期回転
	*@param[in]	speed			回転速度(1.0fで360度を一秒で回る、-にすると反対方向に回る)
	*/
	virtual void Init(const CQuaternion& rot, const float& speed = 0.0f) {}
	//ステートを取得
	EnRotate GetRotState()
	{
		return m_state;
	}
	//ステートを設定
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
