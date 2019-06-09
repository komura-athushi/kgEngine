#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "GameCamera.h"
#include "Object.h"
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
	m_skinModelRender = NewGO<CSkinModelRender>(0);
	//cmoファイルの読み込み。
	m_skinModelRender->Init(L"Resource/modelData/sphere.cmo");
	m_skinModelRender->SetShadowCaster(true);
	m_skinModelRender->SetShadowReceiver(true);
	m_skinModelRender->SetScale(m_scale);
	//キャラコン、スフィアコライダーを使う
	m_characon.Init(
		m_radius,			//半径。 
		m_position			//初期位置。
	);
	m_beforeposition = m_position;
	return true;
}

void Player::Update()
{
	if (m_gamecamera == nullptr) {
		m_gamecamera = FindGO<GameCamera>();
	}
	Judgment();
	Move();
	Turn();
	m_characon.SetPosition(m_position);
	m_beforeposition = m_position;
	m_skinModelRender->SetPosition(m_position + CVector3::AxisY() * m_radius);
	m_skinModelRender->SetRotation(m_rotation);
}

void Player::Judgment()
{
	const float SizeMultiply = 0.2f;

	QueryGOs<Object>(nullptr, [&](Object* object) {
		CVector3 pos = m_position + CVector3::AxisY() * m_radius - object->GetPosition();
		if (pos.LengthSq() <= (pow(m_radius + object->GetSize(), 2.0f) * 1.3f) && m_radius >= object->GetSize()) {
			float Radius = m_radius + (object->GetSize() / m_radius) * SizeMultiply * m_radius;
			float Quotient = Radius / m_radius;
			m_radius = Radius;
			m_characon.SetRadius(m_radius);
			m_scale = CVector3::One() + CVector3::One() * (m_radius / m_protradius - 1);
			m_skinModelRender->SetScale(m_scale);
			DeleteGO(object);
		}
		return true;
		});
}

void Player::Move()
{
	//移動速度
	const float MoveSpeedMultiply = 550.0f;
	//移動速度減衰
	const float MoveSpeedAtten = 0.98f;
	//重力
	const float GravityMoveSpeed = 900.0f;
	//ジャンプ速度
	const float JumpMoveSpeed = 700.0f;

	//両方のスティックが入力されていたら
	if (m_gamecamera->GetStateStick() == enStick_EnterStickBoth) {

	}
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
		if (GetPad(0).IsTrigger(enButtonB)) {
			m_movespeed.y = JumpMoveSpeed;
		}
	}
}

void Player::Turn()
{
	const float RotationSpeed = 1.0f;

	CVector3 movespeedXZ = m_position - m_beforeposition;
	movespeedXZ.y = 0.0f;
	if (movespeedXZ.LengthSq() <= 0.1f) {
		return;
	}
	float Lengh = movespeedXZ.Length();
	//ノーマライズして移動方向の方向ベクトルを求める
	movespeedXZ /= Lengh;
	CVector3 pos;
	//Y軸と方向ベクトルの外積ベクトルを求める
	pos.Cross(CVector3::AxisY(), movespeedXZ);
	//外積ベクトルを元に回転させるクォータニオンを求める
	CQuaternion rot;
	rot.SetRotationDeg(pos, Lengh * RotationSpeed);
	//求めたクォータニオンを乗算する
	m_rotation.Multiply(rot,m_rotation);
}