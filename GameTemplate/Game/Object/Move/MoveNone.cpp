#include "stdafx.h"
#include "MoveNone.h"

MoveNone::MoveNone()
{

}

MoveNone::~MoveNone()
{

}

void MoveNone::Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot)
{
	m_position = pos;
	SetMoveState();
}

