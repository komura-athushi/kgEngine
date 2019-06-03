#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#define _USE_MATH_DEFINES //M_PI �~�����Ăяo��
#include <math.h> 
GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>();
	//�j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��
	MainCamera().SetNear(1.0f);
	MainCamera().SetFar(50000.0f);
	return true;
}

void GameCamera::Update()
{
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}
	Calculation();
	MainCamera().SetPosition(m_position);
	MainCamera().SetTarget(m_target);
	MainCamera().Update();
}

void GameCamera::Calculation()
{
	//�p�b�h�̓��͗ʂɏ�Z
	const float PadMultiply = 100.0f;
	//�J�����̉�]�ɐ���
	const float UpperLimitDegreeXZ = 80.0f;
	const float LowerLimitDegreeXZ = -80.0f;
	//�����_���v���C���[��聪�ɂ���
	const float HeightTarget = 140.0f;
	//�J�����̔��a
	const float Radius = 260.0f;

	CVector3 stickR;
	stickR.x = GetPad(0).GetRStickXF();			
	stickR.y = -GetPad(0).GetRStickYF();
	stickR.z = 0.0f;
	stickR *= PadMultiply * GameTime().GetFrameDeltaTime();
	m_degreey += stickR.x;
	m_degreexz += stickR.y;
	if (m_degreexz >= UpperLimitDegreeXZ) {
		m_degreexz = UpperLimitDegreeXZ;
	}
	else if (m_degreexz <= LowerLimitDegreeXZ) {
		m_degreexz = LowerLimitDegreeXZ;
	}
	m_target = CVector3::Zero();
	m_target = m_player->GetPosition();
	m_target.y += HeightTarget;
	//Y������ɉ�]������
	CQuaternion rot;
	rot.SetRotationDeg(CVector3::AxisY(), m_degreey);
	CVector3 toPos = CVector3::AxisZ();
	rot.Multiply(toPos);
	//�㉺�ɉ�]������
	CVector3 rotAxis;
	rotAxis.Cross(toPos, CVector3::AxisY());
	rotAxis.Normalize();
	rot.SetRotationDeg(rotAxis, m_degreexz);
	rot.Multiply(toPos);
	toPos *= Radius;
	m_position = m_target + toPos;
}