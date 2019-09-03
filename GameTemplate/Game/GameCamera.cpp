#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "GameData.h"
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
	m_gamedata = &GetGameData();
	return true;
}

void GameCamera::Update()
{
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}
	if (!m_gamedata->GetisPose()) {
		TransView();
		TransRadius();
		Calculation();
	}
	MainCamera().SetPosition(m_position);
	MainCamera().SetTarget(m_target);
	MainCamera().Update();
}

void GameCamera::TransRadius()
{
	m_radius = m_protradius + ((m_protradius * (m_player->GetRadius() / m_player->GetProtRadius())) - m_protradius) * 0.9f;
}

void GameCamera::Calculation()
{
	//パッドの入力量に乗算
	const float StickMultiply = 80.0f;
	//カメラの回転に制限
	const float UpperLimitDegreeXZ = 80.0f;
	const float LowerLimitDegreeXZ = -80.0f;
	//注視点をプレイヤーより↑にする
	const float HeightTarget = 80.0f + ((80.0f * (m_player->GetRadius() / m_player->GetProtRadius())) - 80.0f) * 0.9f;
	//スティックが入力されているかどうか
	const float EnterStick = 0.1f * 0.1f;
	//両方のスティックの入力ベクトルの角度、とりあえず90度
	const float LimitStickDegree = 90.0f;
	//スティック入力の補正
	float MultiPly = 1.0f;
	const float Angle = m_degreey;
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
			Degree = stickR;
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
			Degree = stickL;
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
	if (m_transView) {
		m_state = enStick_NoEnterStick;
	}
	//両方のスティックに入力があったら
	if (m_state == enStick_EnterStickBoth) {
		CVector3 R = stickR;
		CVector3 L = stickL;
		CVector3 Rx(stickR.x,0.0f,0.0f);
		CVector3 Lx(stickL.x,0.0f,0.0f);
		R.Normalize();
		L.Normalize();
		float LimitStickX = 0.3f;
		float Angle = acosf(R.Dot(L));
		Angle *= 180.0f / M_PI;
		const float LimitStickDegree = 140.0f;
		const float Multiply = 2.0f;
		if (fabsf(Angle) >= LimitStickDegree && Rx.LengthSq() <= LimitStickX && Lx.LengthSq() <= LimitStickX) {
			if (stickR.y >= stickL.y) {
				m_degreey -= stickR.LengthSq() * StickMultiply * GameTime().GetFrameDeltaTime() * Multiply;
				m_state = enStick_EnterStickBothOppositeRight;
			}
			else {
				m_degreey += stickL.LengthSq() * StickMultiply * GameTime().GetFrameDeltaTime() * Multiply;
				m_state = enStick_EnterStickBothOppositeLeft;
			}
			Degree = stickR + stickL; 
			Degree /= 2;
		}
	}
	if (m_state != enStick_NoEnterStick && m_state != enStick_EnterStickBoth) {
		CVector3 Front = MainCamera().GetFront();
		CVector3 Back = Front * -1;
		CVector3 Stick = Degree;
		Stick.Normalize();
		float Angle = fabsf(acosf(Stick.Dot(CVector3::AxisY())));
		Angle *= 180.0f / M_PI;
		if (Angle <= LimitStickDegree) {
			MultiPly = (LimitStickDegree - Angle) / LimitStickDegree;
			if (m_state == enStick_EnterStickL) {
				Degree *= MultiPly;
				m_degreey += Degree.LengthSq() * StickMultiply * GameTime().GetFrameDeltaTime();
			}
			else if (m_state == enStick_EnterStickR) {
				Degree *= MultiPly;
				m_degreey -= Degree.LengthSq() * StickMultiply * GameTime().GetFrameDeltaTime();
			} 
		}
		else {
			float Angle2 = fabsf(acosf(Stick.Dot(CVector3::AxisY() * -1)));
			Angle2 *= 180.0f / M_PI;
			MultiPly = (LimitStickDegree - Angle2) / LimitStickDegree;
			if (m_state == enStick_EnterStickL) {
				Degree *= MultiPly;
				m_degreey -= Degree.LengthSq() * StickMultiply * GameTime().GetFrameDeltaTime();
			}
			else if (m_state == enStick_EnterStickR) {
				Degree *= MultiPly;
				m_degreey += Degree.LengthSq() * StickMultiply * GameTime().GetFrameDeltaTime();
			}
		}
	}
	if (m_player->GetCount() > 1) {
		m_degreey = Angle;
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
	toPos *= m_radius;
	m_position = m_target + toPos;
}

void GameCamera::TransView()
{
	if (GetPad(0).IsTrigger(enButtonLB3) && GetPad(0).IsTrigger(enButtonRB3) && !m_transView) {
		m_transView = true;
	}
	if (m_transView) {
		m_timer += GameTime().GetFrameDeltaTime();
		m_degreey += 180.0f * GameTime().GetFrameDeltaTime();
		if (m_timer >= 1.0f) {
			m_transView = false;
			m_timer = 0.0f;
		}
	}
}