/*****************************************************************//**
 * \file   Player.h
 * \brief  Playerクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "character/CharacterController.h"
#include "graphics/2D/kgFont.h"
class GameCamera;
class GameData;
/**
 * \brief プレイヤー(塊).
 */
class Player:public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Player();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Player() override final;
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
	 * \brief 描画処理.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief プレイヤーを大きくする.
	 * 
	 * \param volume 大きくする分
	 */
	void AddVolume(const float volume);
	/**
	 * \brief 座標を取得.
	 * 
	 * \return 座標
	 */
	CVector3 GetPosition() const
	{
		return m_position + CVector3::AxisY() * m_radius;
	}
	/**
	 * \brief キャラコンの座標を取得.
	 * 
	 * \return キャラコンの座標
	 */
	const CVector3& GetCharaconPosition() const
	{
		return m_position;
	}
	/**
	 * \brief 座標を設定.
	 * 
	 * \param pos 座標
	 */
	void SetPosition(const CVector3& pos)
	{
#ifdef _DEBUG
		if (isnan(pos.x) || isnan(pos.y) || isnan(pos.z)) {
			std::abort();
		}
#endif
		m_position = pos;
		m_charaCon.SetPosition(pos);
		m_skinModelRender.SetPosition(pos + CVector3::AxisY() * m_radius);
		if (m_isStart) {
			m_skinModelRender.UpdateWorldMatrix();
			m_skinModelRender2.UpdateWorldMatrix();
		}
	}
	/**
	 * \brief ステージの最初からどれだけ大きくなったかを取得.
	 * 
	 * \return ステージの最初からどれだけ大きくなったか
	 */
	const float GetMag() const
	{
		return m_scale.y / 1.0f;
	}
	/**
	 * \brief 塊の半径を取得.
	 * 
	 * \return 塊の半径 
	 */
	const float GetRadius() const
	{
		return m_radius;
	}
	/**
	 * \brief ステージの最初の塊の大きさを取得.
	 * 
	 * \return  ステージの最初の塊の大きさ
	 */
	const float GetProtRadius() const
	{
		return m_protRadius;
	}
	/**
	 * \brief 塊のモデルを取得.
	 * 
	 * \return 塊のモデル
	 */
	CSkinModelRender& GetCSkinModelRender()
	{
		return m_skinModelRender;
	}
	/**
	 * \brief プレイヤーのYの移動速度を0にする.
	 * 
	 */
	void SetMoveSpeedYZero()
	{
		m_moveSpeed.y = 0.0f;
	}
	/**
	 * \brief カウントを取得.
	 * 
	 * \return カウント
	 */
	const int GetCount() const
	{
		return m_count;
	}
	/**
	 * \brief 基準のプレイヤーの大きさを取得.
	 * 
	 * \return 基準のプレイヤーの大きさ 
	 */
	const float GetStandardSize() const
	{
		return m_standardSize;
	}
	/**
	 * \brief 塊の大きさを表示しない.
	 * 
	 */
	void SetisNotDisplay()
	{
		m_isDisplayFont = false;
	}
	/**
	 * \brief タイトルだよ.
	 * 
	 */
	void SetisTitle()
	{
		m_isTitle = true;
	}
	/**
	 * \brief 塊のスクリーン座標系を取得.
	 * 
	 * \return 塊のスクリーン座標系
	 */
	const CVector3& GetScreenPos() const
	{
		return m_katamariVector;
	}
	/**
	 * \brief 1Pか2Pかを設定(0で1P).
	 * 
	 * \param number 0で1P
	 */
	void SetPlayerNumber(const int number)
	{
		m_playerNumber = number;
	}
	/**
	 * \brief 1Pか2Pを取得.
	 * 
	 * \return 0で1P
	 */
	const int GetPlayerNumber() const
	{
		return m_playerNumber;
	}
	/**
	 * \brief ゲームカメラを設定.
	 * 
	 * \param gameCamera ゲームカメラ
	 */
	void SetGameCamera(GameCamera* gameCamera)
	{
		m_gameCamera = gameCamera;
	}
	/**
	 * \brief プレイヤーの最初の座標を設定.
	 * 
	 * \param position プレイヤーの最初の座標
	 */
	void SetFirstPosition(const CVector3& position)
	{
		m_firstPosition = position;
		m_position = position;
	}
	/**
	 * \brief もう片方のプレイヤーを設定.
	 * 
	 * \param player もう片方のプレイヤー
	 */
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	/**
	 * \brief 一時停止させる.
	 * 
	 */
	void SetStopTime()
	{
		m_isStopTime = true;
	}
	/**
	 * \brief 一時停止してるかどうか.
	 * 
	 * \return 一時停止してたらtrue
	 */
	const bool GetisStopTime() const
	{
		return m_isStopTime;
	}
private:
	/**
	 * \brief 移動.
	 * 
	 */
	void Move();
	/**
	 * \brief スティック入力.
	 * 
	 */
	void Stick();
	/**
	 * \brief 衝突処理.
	 * 
	 */
	void Collision();
	/**
	 * \brief 回転.
	 * 
	 */
	void Turn();
	/**
	 * \brief スクリーン座標計算.
	 * 
	 */
	void ScreenPosition();
private:
	CharacterController m_charaCon;																//キャラコン
	CVector3 m_position = { 0.0f,200.0f,0.0f }, m_beforePosition = CVector3::Zero();			//座標
	CVector3 m_moveSpeed = CVector3::Zero();													//移動ベクトル
	CVector3 m_scale = CVector3::One();															//大きさ
	CQuaternion m_rotation = CQuaternion::Identity();											//回転
	//アニメーション
	enum EnAnimationClip {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_run,
		enAnimationClip_jump,
		//enAnimationClip_damage,
		//enAnimationClip_KneelDown,
		//enAnimationClip_Clear,
		//enAnimationClip_Aria,
		enAnimationClip_num,
	};
	AnimationClip m_animClip[enAnimationClip_num];
	enum EnState {
		enState_idle,
		enState_walk,
		enState_jump,
		enState_num
	};
	EnState m_state = enState_idle;
	CSkinModelRender m_skinModelRender;															//スキンモデル。
	CSkinModelRender m_skinModelRender2;														//人のモデル
	GameCamera* m_gameCamera = nullptr;															//カメラ
	bool m_isJump = false;																		//ジャンプ中かどうか
	float m_radius = 0.0f;																		//球体の半径
	float m_protRadius = 0.0f;																	//最初の球体の半径	

	float m_volume = 0.0f;																		//塊の体積																	
	const float m_protMoveSpeedMultiply = 5.0f;													//最初の移動速度
	float m_moveSpeedMultiply = 5.0f;															//移動速度
	bool m_isBound = false;																		//バウンド中かどうか
	CFont m_font;																				//文字
	float m_timer = 0.0f;																		//ダッシュの判定に使うタイマー
	int m_count = 0;																			//ダッシュの判定に使うカウント	
	bool m_isRight = false;																		//スティックの右が前方向、左が後ろ方向に倒されたか
	bool m_isDush = false;																		//ダッシュしたか
	GameData* m_gameData = nullptr;																//ゲームデータ
	/*CSprite m_sprite;
	CSprite m_sprite2;
	float m_degree = 0.0f;*/
	const float m_standardSize = 40.0f;															//基準の塊の直径
	CVector3 m_impulse = CVector3::Zero();														//今は使っていない
	int m_count2 = 0;																			//壁にぶつかった
	bool m_isDisplayFont = true;																//塊の大きさを表示する？
	bool m_isTitle = false;																		//タイトル？
	bool m_isBrake = false;																		//ブレーキ？
	float m_collisionTimer = 0.0f;																//衝突音を連続で出さないためのタイマー
	CVector3 m_katamariVector = CVector3::One();												//塊のスクリーン座標系
	int m_playerNumber = 0;
	CVector3 m_firstPosition = CVector3::Zero();
	Effekseer::Handle m_playEffectHandle = 1;
	//エフェクト
	Effekseer::Effect* m_hitEffect = nullptr;
	Effekseer::Handle m_smokeEffectHandle = 1;
	//エフェクト
	Effekseer::Effect* m_smokeEffect = nullptr;
	Player* m_player = nullptr;																	//もう一人のプレイヤー
	bool m_isStopTime = false;
	float m_stopTimer = 0.0f;
	float m_respawnTimer = 0.0f;
	CVector3 m_stick = CVector3::Zero();
	CVector3 m_frontXZ = CVector3::Zero();
	CVector3 m_rightXZ = CVector3::Zero();

};

