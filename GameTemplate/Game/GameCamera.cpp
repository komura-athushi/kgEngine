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
	//パッドの入力量に乗算
	const float PadMultiply = 100.0f;
	const float ButtonMultiply = 70.0f;
	//カメラの回転に制限
	const float UpperLimitDegreeXZ = 80.0f;
	const float LowerLimitDegreeXZ = -80.0f;
	//注視点をプレイヤーより↑にする
	const float HeightTarget = 140.0f;
	//スティックが入力されているかどうか
	const float EnterStick = 0.1f * 0.1f;

	//スティックの入力量をはかる
	CVector3 stickR;
	stickR.x = GetPad(0).GetRStickXF();			
	stickR.y = GetPad(0).GetRStickYF();
	stickR.z = 0.0f;
	CVector3 stickL;
	stickL.x = GetPad(0).GetLStickXF();
	stickL.y = GetPad(0).GetLStickYF();
	stickL.z = 0.0f;
	CVector3 Degree = CVector3::Zero();
	//右スティックの入力がありかつ
	if (stickR.LengthSq() >= EnterStick) {
		//左スティックの入力が無いなら
		if (stickL.LengthSq() <= EnterStick) {
			//右スティックの入力で回転する
			Degree = stickR * PadMultiply * GameTime().GetFrameDeltaTime();
			m_state = enStick_EnterStickR;
		}
		else {
			//どちらのスティックも入力されている
			m_state = enStick_EnterStickBoth;
		}
	}
	//左スティックの入力がありかつ
	else if (stickL.LengthSq() >= EnterStick) {
		//右スティックの入力が無いなら
		if (stickR.LengthSq() <= EnterStick) {
			//左スティックの入力で回転する
			Degree = stickL * PadMultiply * GameTime().GetFrameDeltaTime();
			m_state = enStick_EnterStickL;
		}
		else {
			//どちらのスティックも入力されている
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
	////十字キーの左右の入力
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
	toPos *= m_radius;
	m_position = m_target + toPos;
}