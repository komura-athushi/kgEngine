#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "GameCamera.h"
Player::Player()
{
	
}


Player::~Player()
{
	
}

void Player::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}

bool Player::Start()
{
	//m_model = NewGO<CSkinModelRender>(0);
	m_animClip[enAnimationClip_idle].Load(L"Assets/animData/unityChan/idle.tka");
	m_animClip[enAnimationClip_jump].Load(L"Assets/animData/unityChan/jump.tka");
	//m_animClip[enAnimationClip_damage].Load(L"animData/unityChan/damage.tka");
	//m_animClip[enAnimationClip_KneelDown].Load(L"animData/unityChan/KneelDown.tka");
	//m_animClip[enAnimationClip_Clear].Load(L"animData/unityChan/Clear.tka");
	//m_animClip[enAnimationClip_Aria].Load(L"animData/unityChan/Aria.tka");
	m_animClip[enAnimationClip_walk].Load(L"Assets/animData/unityChan/walk.tka");
	m_animClip[enAnimationClip_run].Load(L"Assets/animData/unityChan/run.tka");
	for (auto& animClip : m_animClip) {
		animClip.SetLoopFlag(true);
	}
	m_animClip[enAnimationClip_jump].SetLoopFlag(false);
	//m_animClip[enAnimationClip_KneelDown].SetLoopFlag(false);
	//m_animClip[enAnimationClip_Clear].SetLoopFlag(false);
	//m_animClip[enAnimationClip_Aria].SetLoopFlag(false);

	m_skinModelRender = NewGO<CSkinModelRender>(0);
	m_skinModelRender->Init(L"Resource/modelData/unityChan.cmo", m_animClip, enAnimationClip_num, enFbxUpAxisY);
	m_skinModelRender->SetShadowCaster(true);
	m_skinModelRender->SetShadowReceiver(true);
	//cmoファイルの読み込み。
	//m_skinModelRender->Init(L"Assets/modelData/unityChan.cmo");
	m_skinModelRender->PlayAnimation(enAnimationClip_idle, 0.2f);
	m_characon.Init(
		20.0,			//半径。 
		40.0f,			//高さ。
		m_position		//初期位置。
	);
	return true;
}

void Player::Update()
{
	if (m_gamecamera == nullptr) {
		m_gamecamera = FindGO<GameCamera>();
	}
	//m_skinModelRender->PlayAnimation(enAnimationClip_walk, 0.2f);
	//m_model->SetActive(false);
	//ワールド行列の更新。
	//m_model->UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	/*QueryGOs<Game>(nullptr, [&](Game * game) {
		DeleteGO(game);		
		return true;
	});*/
	AnimationController();
	m_characon.SetPosition(m_position);
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);
}

void Player::AnimationController()
{
	//動いている速さが一定以上ならそれぞれのアニメーションを再生する
	const float RunSpeed = 9.0f * 9.0f;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               0.0f * 300.0f;
	const float WalkSpeed = 2.0f * 2.0f;
	//アニメーションの補完時間
	const float ComplementTime = 0.2f;

	switch (m_state) {
	case enState_idle:
	case enState_walk:
		//走る
		if (m_movespeed.LengthSq() >= RunSpeed) {
			m_skinModelRender->PlayAnimation(enAnimationClip_run, ComplementTime);
			m_state = enState_walk;
		}
		//歩く
		else if (m_movespeed.LengthSq() >= WalkSpeed) {
			m_skinModelRender->PlayAnimation(enAnimationClip_walk, ComplementTime);
			m_state = enState_walk;
		}
		//待機
		else {
			m_skinModelRender->PlayAnimation(enAnimationClip_idle, ComplementTime);
			m_state = enState_idle;
		}
		Move();
		Turn();
		break;
	case enState_jump:
		m_skinModelRender->PlayAnimation(enAnimationClip_jump, ComplementTime);
		Move();
		Turn();
		break;
	}
}

void Player::Move()
{
	//移動速度
	const float MoveSpeedMultiply = 550.0f;
	//重力
	const float GravityMoveSpeed = 900.0f;
	//ジャンプ速度
	const float JumpMoveSpeed = 700.0f;

	m_movespeed.x = 0.0f;
	m_movespeed.z = 0.0f;
	CVector3 stickL;
	stickL.x = GetPad(0).GetLStickXF();
	stickL.y = GetPad(0).GetLStickYF();
	stickL.z = 0.0f;
	CVector3 frontxz = MainCamera().GetFront();
	CVector3 rightxz = MainCamera().GetRight();
	frontxz *= stickL.y;
	rightxz *= stickL.x;
	m_movespeed += frontxz * MoveSpeedMultiply;
	m_movespeed += rightxz * MoveSpeedMultiply;
	m_movespeed.y -= GravityMoveSpeed * GameTime().GetFrameDeltaTime();
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_movespeed);
	if (m_characon.IsOnGround()) {
		m_movespeed.y = 0.0f;
		if (m_isjump) {
			m_isjump = false;
			m_state = enState_idle;
		}
		if (GetPad(0).IsTrigger(enButtonB)) {
			m_movespeed.y = JumpMoveSpeed;
			m_state = enState_jump;
			m_isjump = true;
		}
	}
}

void Player::Turn()
{
	CVector3 movespeedXZ = m_movespeed;
	movespeedXZ.y = 0.0f;
	if (movespeedXZ.LengthSq() <= 0.1f) {
		return;
	}
	m_rotation.SetRotation(CVector3::AxisY(), atan2f(movespeedXZ.x, movespeedXZ.z));

}