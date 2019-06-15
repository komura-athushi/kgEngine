#include "stdafx.h"
#include "RotDirection.h"

RotDirection::RotDirection()
{

}

RotDirection::~RotDirection()
{

}

void RotDirection::Init(const CQuaternion& rot, const float& speed)
{
	m_rotation = rot;
}

CQuaternion RotDirection::Rot(const CVector3& move)
{
	if (move.LengthSq() <= 0.01f) {
		return m_rotation;
	}
	CVector3 pos = move;
	pos.y = 0.0f;
	pos.Normalize();
	m_rotation.SetRotation(CVector3::AxisY(), atan2f(pos.x, pos.z));
	return m_rotation;
}