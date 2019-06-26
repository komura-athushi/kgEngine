#pragma once
#include "character/CharacterController.h"
#include "graphics/2D/CFont.h"
class GameCamera;
//(⌒,_ゝ⌒)球体ですわぁ...
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void PostRender() override;
	//void OnDestroy() override;
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
private:
	CharacterController m_characon;
	CVector3 m_position = { 0.0f,200.0f,0.0f }, m_beforeposition = CVector3::Zero();
	CVector3 m_movespeed = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rotation = CQuaternion::Identity();
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
	CSkinModelRender m_skinModelRender;									//スキンモデル。
	GameCamera* m_gamecamera = nullptr;
	bool m_isjump = false;
	float m_radius = 45.0f;
	const float m_protradius = 45.0f;
	float m_volume = 0.0f;
	const float m_protmovespeedmultiply = 5.0f;
	float m_movespeedmultiply = 5.0f;
	bool m_isbound = false;
	const float m_PI = 3.14f;
	CFont m_font;
};

