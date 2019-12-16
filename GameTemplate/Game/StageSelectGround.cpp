#include "stdafx.h"
#include "StageSelectGround.h"

StageSelectGround::StageSelectGround()
{

}

StageSelectGround::~StageSelectGround()
{

}

bool StageSelectGround::Start()
{
	m_model.Init(L"Resource/modelData/earth.cmo");
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(CVector3::One());
	m_model.UpdateWorldMatrix();
	m_model.SetOffToonShader();
	return true;

}

void StageSelectGround::Update()
{
	Move();
	Turn();
	m_model.UpdateWorldMatrix();
}

void StageSelectGround::Move()
{
	const float moveSpeedMultiply = 2.0f;

	m_beforePosition = m_currentPosition;
	CVector3 stickL;
	stickL.x = GetPad(0).GetLStickXF();
	stickL.y = GetPad(0).GetLStickYF();
	stickL.z = 0.0f;
	CVector3 moveSpeed = stickL;
	moveSpeed.z = moveSpeed.y;
	moveSpeed.y = 0.0f;
	m_currentPosition += moveSpeed * moveSpeedMultiply;
}

void StageSelectGround::Turn()
{
	const float RotationSpeed = 0.8f;

	CVector3 movespeedXZ = m_currentPosition - m_beforePosition;
	movespeedXZ.y = 0.0f;
	if (movespeedXZ.LengthSq() <= 0.01f) {
		return;
	}
	float Lengh = movespeedXZ.Length();
	//�m�[�}���C�Y���Ĉړ������̕����x�N�g�������߂�
	movespeedXZ /= Lengh;
	//Y���ƕ����x�N�g���̊O�σx�N�g�������߂�
	CVector3 pos;
	pos.Cross(CVector3::AxisY(), movespeedXZ);
	//�O�σx�N�g�������ɉ�]������N�H�[�^�j�I�������߂�
	CQuaternion rot;
	rot.SetRotationDeg(pos, Lengh * RotationSpeed);
	//���߂��N�H�[�^�j�I������Z����
	m_rotation.Multiply(rot, m_rotation);
	m_model.SetRotation(m_rotation);
}