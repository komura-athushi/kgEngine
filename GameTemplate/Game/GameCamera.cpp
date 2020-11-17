#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#define _USE_MATH_DEFINES //M_PI 円周率呼び出し
#include <math.h> 
#include "GameData.h"
#include <random>

namespace {
	//パッドの入力量に乗算
	const float stickMultiply = 80.0f;
	//カメラの回転に制限
	const float upperLimitDegreeXZ = 80.0f;
	const float lowerLimitDegreeXZ = -80.0f;
	//スティックが入力されているかどうか
	const float enterStick = 0.1f * 0.1f;
	//両方のスティックの入力ベクトルの角度、とりあえず90度
	const float limitStickDegree = 90.0f;
	//スティック入力の補正
	float multiPly = 1.0f;
	const float height = 60.0f;
	const float limitStickX = 0.5f;
	const float limitStickDegree2 = 50.0f;
	const float multiply = 2.0f;
}

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	m_gameData = &GetGameData();

	//ニアクリップとファークリップを設定する
	MainCamera(m_playerNumber).SetNear(1.0f);
	MainCamera(m_playerNumber).SetFar(50000.0f);

	QueryGOs<Player>(nullptr, [&](Player* player) {
		if (player->GetPlayerNumber() == m_playerNumber) {
			m_player = player;
			return false;
		}
		else {
			return true;
		}
	});
	
	if (m_player != nullptr) {
		m_playerNumber = m_player->GetPlayerNumber();
		//注視点をプレイヤーより↑にする
		const float heightTarget = 60.0f + ((60.0f * (m_gameData->GetPlayerSize() / m_player->GetStandardSize())) - 60.0f) * 0.9f;
		m_radius = m_protRadius + ((m_protRadius * (m_gameData->GetPlayerSize() / m_player->GetStandardSize())) - m_protRadius) * 0.9f;
		m_target = CVector3::Zero();
		m_target = m_player->GetPosition();
		m_target.y += heightTarget;
		//Y軸周りに回転させる
		CQuaternion rot;
		rot.SetRotationDeg(CVector3::AxisY(), m_degreeY);
		CVector3 toPos = CVector3::AxisZ();
		rot.Multiply(toPos);
		//上下に回転させる
		CVector3 rotAxis;
		rotAxis.Cross(toPos, CVector3::AxisY());
		rotAxis.Normalize();
		rot.SetRotationDeg(rotAxis, m_degreeXZ);
		rot.Multiply(toPos);
		toPos *= m_radius;
		m_position = m_target + toPos;
	}

	m_springCamera.SetPlayerNumber(m_playerNumber);
	m_springCamera.Init(10000.0f, 0.9f, m_position, m_target);


	return true;
}

void GameCamera::Update()
{
	/*if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}*/
	if (m_gameData->GetScene() == enScene_Result) {
		MainCamera(m_player->GetPlayerNumber()).SetPosition(m_position);
		MainCamera(m_player->GetPlayerNumber()).SetTarget(m_target);
		MainCamera(m_player->GetPlayerNumber()).Update();
	}
	else {
		if (!m_gameData->GetisPose() || !m_gameData->GetisStart()) {
			if (!m_player->GetisStopTime()) {
				TransView();
			}
			TransRadius();
			Calculation();
		}
		else {
			m_springCamera.SetStop();
		}
		m_springCamera.SetPosition(m_position);
		m_springCamera.SetTarget(m_target);
		m_springCamera.Update();
		m_position = m_springCamera.GetPosition();
		m_target = m_springCamera.GetTarget();
	}
}

void GameCamera::TransRadius()
{
	m_radius = m_protRadius + ((m_protRadius * (m_player->GetRadius() / m_player->GetStandardSize())) - m_protRadius) * 0.9f;
}

void GameCamera::Stick()
{
	//スティックの入力量をはかる	
	m_stickR.x = GetPad(m_player->GetPlayerNumber()).GetRStickXF();
	m_stickR.y = GetPad(m_player->GetPlayerNumber()).GetRStickYF();
	m_stickR.z = 0.0f;
	m_stickL.x = GetPad(m_player->GetPlayerNumber()).GetLStickXF();
	m_stickL.y = GetPad(m_player->GetPlayerNumber()).GetLStickYF();
	m_stickL.z = 0.0f;
	m_degree = CVector3::Zero();

	if (!m_gameData->GetisStart()) {
		m_stickR = CVector3::Zero();
		m_stickL = CVector3::Zero();
	}

	//右スティックの入力がありかつ
	if (m_stickR.LengthSq() >= enterStick) {
		//左スティックの入力が無いなら
		if (m_stickL.LengthSq() <= enterStick) {
			//右スティックの入力で回転する
			m_degree = m_stickR;
			m_state = enStick_EnterStickR;
		}
		else {
			//どちらのスティックも入力されている
			m_state = enStick_EnterStickBoth;
		}
	}
	//左スティックの入力がありかつ
	else if (m_stickL.LengthSq() >= enterStick) {
		//右スティックの入力が無いなら
		if (m_stickR.LengthSq() <= enterStick) {
			//左スティックの入力で回転する
			m_degree = m_stickL;
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
}

void GameCamera::Calculation()
{
	//注視点をプレイヤーより↑にする
	const float heightTarget = height + ((height * (m_player->GetRadius() / m_player->GetStandardSize())) - 60.0f) * 0.9f;
	const float angle = m_degreeY;

	Stick();

	//両方のスティックに入力があったら
	if (m_state == enStick_EnterStickBoth) {
		if (m_player->GetisStopTime()) {
			m_stickR = CVector3::Zero();
			m_stickL = CVector3::Zero();
		}
		CVector3 r = m_stickR;
		CVector3 l = m_stickL;
		CVector3 rX(m_stickR.x,0.0f,0.0f);
		CVector3 lX(m_stickL.x,0.0f,0.0f);
		r.Normalize();
		l.Normalize();
	
		float angle = acosf(r.Dot(l));
		angle *= 180.0f / M_PI;
		if (fabsf(angle) >= limitStickDegree && rX.LengthSq() <= limitStickX && lX.LengthSq() <= limitStickX) {
			if (m_stickR.y >= m_stickL.y) {
				m_degreeY -= m_stickR.LengthSq() * stickMultiply * GameTime().GetFrameDeltaTime() * multiply;
				m_state = enStick_EnterStickBothOppositeRight;
			}
			else {
				m_degreeY += m_stickL.LengthSq() * stickMultiply * GameTime().GetFrameDeltaTime() * multiply;
				m_state = enStick_EnterStickBothOppositeLeft;
			}
			m_degree = m_stickR + m_stickL; 
			m_degree /= 2;
		}
	}
	if (m_state != enStick_NoEnterStick && m_state != enStick_EnterStickBoth) {
		CVector3 front = MainCamera(m_player->GetPlayerNumber()).GetFront();
		CVector3 back = front * -1;
		CVector3 stick = m_degree;
		stick.Normalize();
		float angle = fabsf(acosf(stick.Dot(CVector3::AxisY())));
		angle *= 180.0f / M_PI;
		if (angle <= limitStickDegree2) {
			multiPly = (limitStickDegree2 - angle) / limitStickDegree2;
			if (m_state == enStick_EnterStickL) {
				m_degree *= multiPly;
				m_degreeY += m_degree.LengthSq() * stickMultiply * GameTime().GetFrameDeltaTime();
			}
			else if (m_state == enStick_EnterStickR) {
				m_degree *= multiPly;
				m_degreeY -= m_degree.LengthSq() * stickMultiply * GameTime().GetFrameDeltaTime();
			} 
		}
		else {
			float angle2 = fabsf(acosf(stick.Dot(CVector3::AxisY() * -1)));
			angle2 *= 180.0f / M_PI;
			multiPly = (limitStickDegree2 - angle2) / limitStickDegree2;
			if (m_state == enStick_EnterStickL) {
				m_degree *= multiPly;
				m_degreeY -= m_degree.LengthSq() * stickMultiply * GameTime().GetFrameDeltaTime();
			}
			else if (m_state == enStick_EnterStickR) {
				m_degree *= multiPly;
				m_degreeY += m_degree.LengthSq() * stickMultiply * GameTime().GetFrameDeltaTime();
			}
		}
	}
	if (m_player->GetCount() > 1) {
		m_degreeY = angle;
	}
	m_target = CVector3::Zero();
	m_target = m_player->GetPosition();
	m_target.y += heightTarget;
	//Y軸周りに回転させる
	CQuaternion rot;
	rot.SetRotationDeg(CVector3::AxisY(), m_degreeY);
	CVector3 toPos = CVector3::AxisZ();
	rot.Multiply(toPos);
	//上下に回転させる
	CVector3 rotAxis;
	rotAxis.Cross(toPos, CVector3::AxisY());
	rotAxis.Normalize();
	rot.SetRotationDeg(rotAxis, m_degreeXZ);
	rot.Multiply(toPos);
	toPos *= m_radius;
	m_position = m_target + toPos;
	if (m_player->GetisStopTime()) {
		//縦にカメラが揺れる。ガクガクしている。
		m_position.y += rand() % 5000 * GameTime().GetFrameDeltaTime() * 0.1f;
		if (rand() % 2 == 0) {
			m_position.x += rand() % 1200 * GameTime().GetFrameDeltaTime() * 0.1f;
		}
		else {
			m_position.x -= rand() % 1200 * GameTime().GetFrameDeltaTime() * 0.1f;
		}
	}
}

void GameCamera::TransView()
{
	if (GetPad(m_player->GetPlayerNumber()).IsTrigger(enButtonLB3) && GetPad(m_player->GetPlayerNumber()).IsTrigger(enButtonRB3) && !m_transView) {
		m_transView = true;
	}
	if (m_transView) {
		m_timer += GameTime().GetFrameDeltaTime();
		m_degreeY += 180.0f * GameTime().GetFrameDeltaTime();
		if (m_timer >= 1.0f) {
			m_transView = false;
			m_timer = 0.0f;
		}
	}
}