#pragma once
#include "character/CharacterController.h"
class GameCamera;
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	//移動
	void Move();
	//アニメーション
	void AnimationController();
	//回転
	void Turn();
	//座標を取得
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//座標を設定
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
private:
	CVector3 m_position = CVector3::Zero();
	CVector3 m_movespeed = CVector3::Zero();
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
	CSkinModelRender* m_skinModelRender = nullptr;							//スキンモデル。
	GameCamera* m_gamecamera = nullptr;
};

