#include "stdafx.h"
#include "Obj.h"
#include "MoveLR.h"
Obj::Obj()
{

}

Obj::~Obj()
{

}

bool Obj::Start()
{
	m_skin = NewGO<CSkinModelRender>(0);
	m_skin->Init(L"Resource/modelData/obj.cmo");
	m_skin->SetPosition(m_position);
	m_skin->SetRotation(m_rotation);
	return true;
}

void Obj::Init(EnMove state, const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot)
{
	switch (state)
	{
	case enMove_Lr:
		m_move = new MoveLR();
		m_move->Init(pos, move, movelimit, rot);
		m_move->SetMoveState();
		m_state = state;
		m_position = pos;
		break;
	default:
		break;
	}
}

void Obj::Update()
{
	m_position = m_move->Move();
	m_skin->SetPosition(m_position);
	m_skin->SetRotation(m_rotation);
}