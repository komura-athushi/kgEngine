#pragma once
#include "Object/Move/IMove.h"
class Anim
{
public:
	Anim();
	~Anim();
	//初期化
	void Init(const wchar_t* filepath,CSkinModelRender* skin);
	//アニメーションを再生
	void PlayAnimation(EnMove state);
	//現在再生中のアニメーションを取得
	int GetPlayAnimationType()
	{
		return m_animationState;
	}
private:
	CSkinModelRender* m_skinModelRender = nullptr;				//モデル
	//アニメーション
	enum EnAnimationClip {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_num
	};
	AnimationClip m_animClip[enAnimationClip_num];				//アニメーションクリップ
	EnAnimationClip m_animationState = enAnimationClip_idle;
	bool m_isInit = false;										//初期化した？
};

