#include "stdafx.h"
#include "Anim.h"

Anim::Anim()
{

}

Anim::~Anim()
{

}

void Anim::Init(const wchar_t* filepath, CSkinModelRender* skin)
{
	m_skinModelRender = skin;
	//アニメーション読み込む
	wchar_t path[256];
	swprintf_s(path, L"Assets/animData/%ls/walk.tka",filepath);
	m_animClip[enAnimationClip_walk].Load(path);
	wchar_t path2[256];
	swprintf_s(path2, L"Assets/animData/%ls/idle.tka",filepath);
	m_animClip[enAnimationClip_idle].Load(path2);
	wchar_t path3[256];
	swprintf_s(path3, L"Resource/modelData/%ls.cmo", filepath);
	m_animClip[enAnimationClip_walk].SetLoopFlag(true);
	m_animClip[enAnimationClip_idle].SetLoopFlag(true);
	m_skinModelRender->InitAnimation(m_animClip, enAnimationClip_num, path3);
	m_isInit = true;
}

void Anim::PlayAnimation(EnMove state)
{
	if (m_isInit) {
		//ヒットしたらヒット用のアニメーションを再生
		if(state == enMove_MoveHit) {
			//m_skinModelRender->PlayAnimation(enAnimationClip_idle, 1.0f);
			m_animationState = enAnimationClip_idle;
		}
		//していなかったらそれ以外のアニメーション再生
		else {
			//m_skinModelRender->PlayAnimation(enAnimationClip_walk, 1.0f);
			m_animationState = enAnimationClip_walk;
		}
	}
}

