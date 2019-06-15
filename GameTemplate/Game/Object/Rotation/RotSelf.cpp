#include "stdafx.h"
#include "RotSelf.h"

RotSelf::RotSelf()
{

}

RotSelf::~RotSelf()
{

}

void RotSelf::Init(const CQuaternion& rot, const float& speed)
{
	m_rotation = rot;
	m_speed = speed;
	CVector3 Parallel = CVector3::AxisZ();
	m_rotation.Multiply(Parallel);
	m_degree = acosf(Parallel.Dot(CVector3::AxisZ()));
}

CQuaternion RotSelf::Rot(const CVector3& move)
{
	m_degree += m_speed * 360.0f * GameTime().GetFrameDeltaTime();
	m_rotation.SetRotationDeg(CVector3::AxisY(), m_degree);
	return m_rotation;
}