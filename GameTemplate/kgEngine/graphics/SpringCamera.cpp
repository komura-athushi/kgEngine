#include "KGstdafx.h"
#include "SpringCamera.h"

void SpringCamera::Init(
	float maxMoveSpeed,
	float attenuationRate,
	const CVector3& position,
	const CVector3& target
)
{
	m_maxMoveSpeed = maxMoveSpeed;
	m_attenuationRate = attenuationRate;
	MainCamera().SetPosition(position);
	MainCamera().SetTarget(target);
	m_positionMoveSpeed = CVector3::Zero();
	m_targetMoveSpeed = CVector3::Zero();
}

CVector3 SpringCamera::ClcSpringVector(
	const CVector3& positionNow,
	const CVector3& positionTarget,
	CVector3& moveSpeed
)
{
	const float dampingK = 35.0f;

	CVector3 distance = positionTarget - positionNow;
	CVector3 distanceNor = distance;
	distanceNor;
	distanceNor.Normalize();
	CVector3 springAccel;
	springAccel = distance;
	float t = dampingK / (2.0f * m_attenuationRate);
	springAccel *= t * t;
	CVector3 vt = moveSpeed;
	vt *= dampingK;
	springAccel -= vt;
	springAccel *= GameTime().GetFrameDeltaTime();
	moveSpeed += springAccel;
	if (moveSpeed.LengthSq() > m_maxMoveSpeed * m_maxMoveSpeed) {
		moveSpeed.Normalize();
		moveSpeed *= m_maxMoveSpeed;
	}
	CVector3 newPos = positionNow;
	if (moveSpeed.LengthSq() < 1.0f) {
		newPos = positionTarget;
		moveSpeed = CVector3::Zero();
	}
	else {
		newPos += moveSpeed * GameTime().GetFrameDeltaTime();
		CVector3 vt = positionTarget - newPos;
		vt.Normalize();
		if (vt.Dot(distanceNor) < 0.0f) {
			newPos = positionTarget;
			moveSpeed = CVector3::Zero();
		}
	}
	return newPos;
}

void SpringCamera::Update()
{
	if (m_isRefresh || m_isStop) {
		MainCamera().SetPosition(m_position);
		MainCamera().SetTarget(m_target);
		if (m_isRefresh) {
			m_positionMoveSpeed = CVector3::Zero();
			m_targetMoveSpeed = CVector3::Zero();
		}
		m_isStop = false;
	}
	else {
		m_position = ClcSpringVector(MainCamera().GetPosition(), m_position, m_positionMoveSpeed);
		m_target = ClcSpringVector(MainCamera().GetTarget(), m_target, m_targetMoveSpeed);
		MainCamera().SetPosition(m_position);
		MainCamera().SetTarget(m_target);
	}
	MainCamera().Update();
}