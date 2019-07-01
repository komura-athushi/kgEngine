#include "stdafx.h"
#include "MovePath.h"

MovePath::MovePath()
{

}

MovePath::~MovePath()
{

}

void MovePath::Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot)
{
	m_position = pos;
	m_movespeed = move;
	SetMoveState();
}

void MovePath::ReadPath(const wchar_t* filePath)
{
	m_path.Load(filePath);
}

CVector3 MovePath::Move()
{
	const float Molecule = 10.0f;

	m_movevector = m_point->s_vector - m_position;
	if (m_movevector.LengthSq() >= m_movespeed / Molecule) {
		m_point = m_path.GetPoint(m_point->s_number);
		m_movevector = m_point->s_vector - m_position;
	}
	m_movevector.Normalize();
	m_movevector *= m_movespeed * GameTime().GetFrameDeltaTime();
	m_position += m_movevector;
	return m_position;
}