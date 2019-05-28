#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	
}


Player::~Player()
{
}

bool Player::Start()
{
	//m_model = NewGO<CSkinModelRender>(0);
	//m_animClip[enAnimationClip_idle].Load(L"animData/unityChan/idle.tka");
	
	//m_animClip[enAnimationClip_jump].Load(L"animData/unityChan/jump.tka");
	//m_animClip[enAnimationClip_damage].Load(L"animData/unityChan/damage.tka");
	//m_animClip[enAnimationClip_KneelDown].Load(L"animData/unityChan/KneelDown.tka");
	//m_animClip[enAnimationClip_Clear].Load(L"animData/unityChan/Clear.tka");
	//m_animClip[enAnimationClip_Aria].Load(L"animData/unityChan/Aria.tka");
	m_animClip[enAnimationClip_walk].Load(L"Assets/animData/unityChan/walk.tka");
	m_animClip[enAnimationClip_run].Load(L"Assets/animData/unityChan/run.tka");
	for (auto& animClip : m_animClip) {
		animClip.SetLoopFlag(true);
	}
	//m_animClip[enAnimationClip_jump].SetLoopFlag(false);
	//m_animClip[enAnimationClip_KneelDown].SetLoopFlag(false);
	//m_animClip[enAnimationClip_Clear].SetLoopFlag(false);
	//m_animClip[enAnimationClip_Aria].SetLoopFlag(false);

	m_skinModelRender = NewGO<CSkinModelRender>(0);
	m_skinModelRender->Init(L"Resource/modelData/unityChan.cmo", m_animClip, enAnimationClip_num, enFbxUpAxisY);
	//cmoファイルの読み込み。
	//m_skinModelRender->Init(L"Assets/modelData/unityChan.cmo");
	m_skinModelRender->PlayAnimation(enAnimationClip_run, 0.2f);
	return true;
}

void Player::Update()
{
	//m_skinModelRender->PlayAnimation(enAnimationClip_walk, 0.2f);
	//m_model->SetActive(false);
	//ワールド行列の更新。
	//m_model->UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
}
void Player::Draw()
{
	//m_model->Draw();
}