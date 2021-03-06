/*!
* @brief	キャラクタコントローラー。
*/

#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/RigidBody.h"


/*!
* @brief	キャラクタコントローラー。
*/
class CharacterController{
public:
	CharacterController() {

	}
	~CharacterController()
	{
		RemoveRigidBoby();
	}
	/*!
		* @brief	初期化。
		*@param[in]	radius		カプセルコライダーの半径。
		*@param[in]	position	初期位置。
		*/
	void Init(float radius, const CVector3& position);
	/*!
		* @brief	実行。
		*@param[in]	deltaTime		経過時間。単位は秒。
		*@param[in, out]	moveSpeed		移動速度。内部で重力加速が計算され、その結果がmoveSpeedに反映されます。
		*@return 移動後のキャラクターの座標。
		*/
	const CVector3& Execute(float deltaTime, CVector3& moveSpeed);
	/*!
		* @brief	座標を取得。
		*/
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/*!
		* @brief	座標を設定。
		*/
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}

	/*!
		* @brief	ジャンプ中か判定
		*/
	bool IsJump() const
	{
		return m_isJump;
	}
	/*!
	* @brief	地面上にいるか判定。
	*/
	bool IsOnGround() const
	{
		return m_isOnGround;
	}
	/*!
	* @brief	コライダーを取得。
	*/
	SphereCollider* GetCollider()
	{
		return &m_collider;
	}
	/*!
	* @brief	剛体を取得。
	*/
	RigidBody* GetRigidBody()
	{
		return &m_rigidBody;
	}
	//半径を設定
	void SetRadius(const float& radius)
	{
		m_radius = radius;
		m_collider.SetRadius(m_radius);
	}
	//地面の法線ベクトルを取得
	CVector3 GetGroundNormalVector() const
	{
		return m_GroundNormalVector;
	}
	//壁と衝突したかどうか
	bool GetisCollision() const
	{
		return m_isCollision;
	}
	//壁の法線ベクトルを取得
	CVector3 GetWallNormalVector() const
	{
		return m_WallNormalVector;
	}
	
	/*!
	* @brief	剛体を物理エンジンから削除。。
	*/
	void RemoveRigidBoby();
	//何番目ですか？
	//0で1P
	void SetPlayerNumber(const int number)
	{
		m_playerNumber = number;
	}
	/// <summary>
	/// 衝突した場所を取得(壁)
	/// </summary>
	/// <returns></returns>
	CVector3& GetHitPos()
	{
		return m_hitPosition;
	}
	/// <summary>
	/// キャラクタ属性のコリジョンと衝突したか
	/// </summary>
	/// <returns></returns>
	bool GetisHitCharacter()
	{
		return m_isHitCharacter;
	}
private:
	CVector3 			m_position = CVector3::Zero();	//座標。
	bool 				m_isJump = false;				//ジャンプ中？
	bool				m_isOnGround = true;			//地面の上にいる？
	SphereCollider		m_collider;						//コライダー。
	float				m_radius = 0.0f;				//スフィアコライダーの半径
	bool				m_isSphere = false;				//球体かどうか
	RigidBody			m_rigidBody;					//剛体。
	CVector3			m_GroundNormalVector = CVector3::AxisY();
	bool				m_isCollision = false;			//衝突したかどうか
	CVector3			m_WallNormalVector = CVector3::Zero();
	int					m_playerNumber = 0;
	CVector3			m_hitPosition = CVector3::Zero();
	bool				m_isHitCharacter = false;		//キャラクタ属性のコリジョンと衝突したか？
	CVector3			m_moveVector = CVector3::Zero();
};

//サイズを比べる
bool static inline GetCompareSize(const float radius, const float size)
{
	if (radius >= size * 3.2f) {
		return true;
	}
	else {
		return false;
	}
}