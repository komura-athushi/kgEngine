#pragma once
#include "Object/Move/IMove.h"
class Anim
{
public:
	Anim();
	~Anim();
	void Init(const wchar_t* filepath,CSkinModelRender* skin);
	void PlayAnimation(EnMove state);
	int GetPlayAnimationType()
	{
		return m_animationState;
	}
private:
	CSkinModelRender* m_skinModelRender = nullptr;
	//アニメーション
	enum EnAnimationClip {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_num
	};
	AnimationClip m_animClip[enAnimationClip_num];
	EnAnimationClip m_animationState = enAnimationClip_idle;
	bool m_isInit = false;
};

