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
	TransRadius();
	Calculation();
	MainCamera().SetPosition(m_position);
	MainCamera().SetTarget(m_target);
	MainCamera().Update();
}

void GameCamera::TransRadius()
{
	m_radius = m_protradius + ((m_protradius * (m_player->GetRadius() / m_player->GetProtRadius())) - m_protradius) * 0.5f;
}

void GameCamera::Calculation()
{
	//�p�b�h�̓��͗ʂɏ�Z
	const float PadMultiply = 100.0f;
	const float ButtonMultiply = 70.0f;
	//�J�����̉�]�ɐ���
	const float UpperLimitDegreeXZ = 80.0f;
	const float LowerLimitDegreeXZ = -80.0f;
	//�����_���v���C���[��聪�ɂ���
	const float HeightTarget = 140.0f;
	//�X�e�B�b�N�����͂���Ă��邩�ǂ���
	const float EnterStick = 0.1f * 0.1f;

	//�X�e�B�b�N�̓��͗ʂ��͂���
	CVector3 stickR;
	stickR.x = GetPad(0).GetRStickXF();			
	stickR.y = GetPad(0).GetRStickYF();
	stickR.z = 0.0f;
	CVector3 stickL;
	stickL.x = GetPad(0).GetLStickXF();
	stickL.y = GetPad(0).GetLStickYF();
	stickL.z = 0.0f;
	CVector3 Degree = CVector3::Zero();
	//�E�X�e�B�b�N�̓��͂����肩��
	if (stickR.LengthSq() >= EnterStick) {
		//���X�e�B�b�N�̓��͂������Ȃ�
		if (stickL.LengthSq() <= EnterStick) {
			//�E�X�e�B�b�N�̓��͂ŉ�]����
			Degree = stickR * PadMultiply * GameTime().GetFrameDeltaTime();
			m_state = enStick_EnterStickR;
		}
		else {
			//�ǂ���̃X�e�B�b�N�����͂���Ă���
			m_state = enStick_EnterStickBoth;
		}
	}
	//���X�e�B�b�N�̓��͂����肩��
	else if (stickL.LengthSq() >= EnterStick) {
		//�E�X�e�B�b�N�̓��͂������Ȃ�
		if (stickR.LengthSq() <= EnterStick) {
			//���X�e�B�b�N�̓��͂ŉ�]����
			Degree = stickL * PadMultiply * GameTime().GetFrameDeltaTime();
			m_state = enStick_EnterStickL;
		}
		else {
			//�ǂ���̃X�e�B�b�N�����͂���Ă���
			m_state = enStick_EnterStickBoth;
		}
	}
	else {
		m_state = enStick_NoEnterStick;
	}
	if (m_state == enStick_EnterStickBoth) {
		Degree = stickR + stickL;
		Degree /= 2;
	}
	m_degreexz += Degree.LengthSq();
	////�\���L�[�̍��E�̓���
	//if (GetPad(0).IsPress(enButtonRight)) {
	//	m_degreey += ButtonMultiply * GameTime().GetFrameDeltaTime();
	//}
	//else if (GetPad(0).IsPress(enButtonLeft)) {
	//	m_degreey -= ButtonMultiply * GameTime().GetFrameDeltaTime();
	//}
	//if (m_degreexz >= UpperLimitDegreeXZ) {
	//	m_degreexz = UpperLimitDegreeXZ;
	//}
	//else if (m_degreexz <= LowerLimitDegreeXZ) {
	//	m_degreexz = LowerLimitDegreeXZ;
	//}
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
	toPos *= m_radius;
	m_position = m_target + toPos;
}