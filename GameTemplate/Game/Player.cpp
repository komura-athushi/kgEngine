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
	//cmo�t�@�C���̓ǂݍ��݁B
	m_skinModelRender->Init(L"Resource/modelData/sphere.cmo");
	m_skinModelRender->SetShadowCaster(true);
	m_skinModelRender->SetShadowReceiver(true);
	m_skinModelRender->SetScale(m_scale);
	//�L�����R���A�X�t�B�A�R���C�_�[���g��
	m_characon.Init(
		m_radius,			//���a�B 
		m_position			//�����ʒu�B
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
	//�ړ����x
	const float MoveSpeedMultiply = 5.0f;
	//�ړ����x����
	const float MoveSpeedAtten = 0.98f;
	//�d��
	const float GravityMoveSpeed = 900.0f;
	//�W�����v���x
	const float JumpMoveSpeed = 700.0f;

	CVector3 Stick = CVector3::Zero();
	//�����̃X�e�B�b�N�����͂���Ă�����
	if (m_gamecamera->GetStateStick() == enStick_EnterStickBoth) {
		CVector3 stickL;
		stickL.x = GetPad(0).GetLStickXF();
		stickL.y = GetPad(0).GetLStickYF();
		stickL.z = 0.0f;
		CVector3 stickR;
		stickR.x = GetPad(0).GetRStickXF();
		stickR.y = GetPad(0).GetRStickYF();
		stickR.z = 0.0f;
		Stick = stickL + stickR;
	}
	CVector3 frontxz = MainCamera().GetFront();
	CVector3 rightxz = MainCamera().GetRight();
	frontxz *= Stick.y;
	rightxz *= Stick.x;
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
	m_movespeed *= MoveSpeedAtten;
}

void Player::Turn()
{
	const float RotationSpeed = 1.0f;

	CVector3 movespeedXZ = m_position - m_beforeposition;
	movespeedXZ.y = 0.0f;
	if (movespeedXZ.LengthSq() <= 0.001f) {
		return;
	}
	float Lengh = movespeedXZ.Length();
	//�m�[�}���C�Y���Ĉړ������̕����x�N�g�������߂�
	movespeedXZ /= Lengh;
	CVector3 pos;
	//Y���ƕ����x�N�g���̊O�σx�N�g�������߂�
	pos.Cross(CVector3::AxisY(), movespeedXZ);
	//�O�σx�N�g�������ɉ�]������N�H�[�^�j�I�������߂�
	CQuaternion rot;
	rot.SetRotationDeg(pos, Lengh * RotationSpeed);
	//���߂��N�H�[�^�j�I������Z����
	m_rotation.Multiply(rot,m_rotation);
}