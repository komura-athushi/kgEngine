#include "stdafx.h"
#include "RotSelf.h"

namespace {
	const float speed = 30.0f;
}

RotSelf::RotSelf()
{

}

RotSelf::~RotSelf()
{

}

void RotSelf::Init(const CQuaternion& rot, const float speed)
{
	m_rotation = rot;
	m_speed = speed;
	CVector3 Parallel = CVector3::AxisZ();
	m_rotation.Multiply(Parallel);
	m_degree = acosf(Parallel.Dot(CVector3::AxisZ()));
}

const CQuaternion RotSelf::Rot(const CVector3& move)
{
	m_degree += m_speed * GameTime().GetFrameDeltaTime() * speed;
	CQuaternion rot;
	rot.SetRotationDeg(CVector3::AxisY(), m_degree);
	CQuaternion finalRot;
	finalRot.Multiply(rot,m_rotation);
	return finalRot;
}