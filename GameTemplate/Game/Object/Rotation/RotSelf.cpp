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
	m_degree += m_speed * GameTime().GetFrameDeltaTime() * 30.0f;
	CQuaternion rot;
	rot.SetRotationDeg(CVector3::AxisY(), m_degree);
	CQuaternion finalRot;
	finalRot.Multiply(rot,m_rotation);
	return finalRot;
}