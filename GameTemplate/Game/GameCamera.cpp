#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>();
	//ニアクリップとファークリップを設定する
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
	//パッドの入力量に乗算
	const float PadMultiply = 100.0f;
	//カメラの回転に制限
	const float UpperLimitDegreeXZ = 80.0f;
	const float LowerLimitDegreeXZ = -80.0f;
	//注視点をプレイヤーより↑にする
	const float HeightTarget = 140.0f;
	//カメラの半径
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
	//Y軸周りに回転させる
	CQuaternion rot;
	rot.SetRotationDeg(CVector3::AxisY(), m_degreey);
	CVector3 toPos = CVector3::AxisZ();
	rot.Multiply(toPos);
	//上下に回転させる
	CVector3 rotAxis;
	rotAxis.Cross(toPos, CVector3::AxisY());
	rotAxis.Normalize();
	rot.SetRotationDeg(rotAxis, m_degreexz);
	rot.Multiply(toPos);
	toPos *= Radius;
	m_position = m_target + toPos;
}