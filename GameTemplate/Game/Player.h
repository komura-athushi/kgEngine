#pragma once
#include "character/CharacterController.h"

class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Draw();
private:
	//アニメーション
	enum EnAnimationClip {
		//enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_run,
		//enAnimationClip_jump,
		//enAnimationClip_damage,
		//enAnimationClip_KneelDown,
		//enAnimationClip_Clear,
		//enAnimationClip_Aria,
		enAnimationClip_num,
	};
	AnimationClip m_animClip[enAnimationClip_num];
	CSkinModelRender* m_skinModelRender;									//スキンモデル。
};

