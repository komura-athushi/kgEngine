#pragma once
#include "character/CharacterController.h"
#include "graphics/2D/kgFont.h"
class GameCamera;
class GameData;
//(⌒,_ゝ⌒)球体ですわぁ...
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void PostRender() override;
	//移動
	void Move();
	//回転
	void Turn();
	//判定
	void Judgment();
	//座標を取得
	CVector3 GetPosition() const
	{
		return m_position + CVector3::AxisY() * m_radius;
	}
	//座標を取得(キャラコン)
	CVector3 GetCharaconPosition() const
	{
		return m_position;
	}
	//座標を設定
	void SetPosition(const CVector3& pos)
	{
#ifdef _DEBUG
		if (isnan(pos.x) || isnan(pos.y) || isnan(pos.z)) {
			std::abort();
		}
#endif
		m_position = pos;
		m_characon.SetPosition(pos);
		m_skinModelRender.SetPosition(pos + CVector3::AxisY() * m_radius);
	}
	//初期からどれだけ大きくなったかを取得
	float GetMag() const
	{
		return m_scale.y / 1.0f;
	}
	//球体の半径を取得
	float GetRadius() const
	{
		return m_radius;
	}
	//球体の最初の半径を取得
	float GetProtRadius() const
	{
		return m_protradius;
	}
	//CSkinModelRenderを取得
	CSkinModelRender& GetCSkinModelRender()
	{
		return m_skinModelRender;
	}
	//MoveSpeedのyを0にする
	void SetMoveSpeedYZero()
	{
		m_movespeed.y = 0.0f;
	}
	//カウントを取得
	int GetCount()
	{
		return m_count;
	}
private:
	CharacterController m_characon;																//キャラコン
	CVector3 m_position = { 0.0f,200.0f,0.0f }, m_beforeposition = CVector3::Zero();			//座標
	CVector3 m_movespeed = CVector3::Zero();													//移動ベクトル
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
	GameCamera* m_gamecamera = nullptr;															//カメラ
	bool m_isjump = false;																		//ジャンプ中かどうか
	float m_radius = 0.0f;																		//球体の半径
	float m_protradius = 0.0f;															//最初の球体の半径	
	float m_volume = 0.0f;																		
	const float m_protmovespeedmultiply = 5.0f;													//最初の移動速度
	float m_movespeedmultiply = 5.0f;															//移動速度
	bool m_isbound = false;																		//バウンド中かどうか
	CFont m_font;																				//文字
	float m_timer = 0.0f;																		//ダッシュの判定に使うタイマー
	int m_count = 0;																			//ダッシュの判定に使うカウント	
	bool m_isRight = false;																		//スティックの右が前方向、左が後ろ方向に倒されたか
	bool m_isDush = false;																		//ダッシュしたか
	GameData* m_gamedata = nullptr;																//ゲームデータ
	/*CSprite m_sprite;
	CSprite m_sprite2;
	float m_degree = 0.0f;*/
};

