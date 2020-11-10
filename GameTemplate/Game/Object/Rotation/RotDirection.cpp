#include "stdafx.h"
#include "RotDirection.h"

RotDirection::RotDirection()
{

}

RotDirection::~RotDirection()
{

}

void RotDirection::Init(const CQuaternion& rot, const float speed)
{
	m_rotation = rot;
}

const CQuaternion RotDirection::Rot(const CVector3& move)
{
	CVector3 pos = move;
	pos.y = 0.0f;
	pos.Normalize();
	m_rotation.SetRotation(CVector3::AxisY(), atan2f(pos.x, pos.z));
	return m_rotation;
}