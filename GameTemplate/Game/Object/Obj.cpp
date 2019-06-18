#include "stdafx.h"
#include "Obj.h"
#include "Move/MoveLR.h"
#include "Move/MoveUp.h"
#include "Rotation/IRot.h"
#include "Rotation/RotSelf.h"
#include "Rotation/RotDirection.h"
#include "Player.h"
Obj::Obj()
{

}

Obj::~Obj()
{
	if (m_move != nullptr) {
		delete m_move;
	}
	if (m_rot != nullptr) {
		delete m_rot;
	}
}

bool Obj::Start()
{
	m_skin = NewGO<CSkinModelRender>(0);
	m_skin->Init(L"Resource/modelData/obj.cmo");
	m_skin->SetPosition(m_position);
	m_skin->SetRotation(m_rotation);
	return true;
}

void Obj::InitMove(EnMove state, const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot)
{
	switch (state)
	{
	case enMove_Lr:
		m_move = new MoveLR();
		break;
	case enMove_Up:
		m_move = new MoveUp();
	default:
		break;
	}
	if (state != enMove_No) {
		m_move->Init(pos, move, movelimit, rot);
		m_move->SetMoveState();
	}
	m_movestate = state;
	m_position = pos;
	m_rotation = rot;
}

void Obj::InitRot(EnRotate state, const float& speed)
{
	switch (state)
	{
	case enRot_Rot:
		m_rot = new RotSelf();
		break;
	case enRot_DirectionRot:
		m_rot = new RotDirection();
		break;
	default:
		break;
	}
	if (state != enRot_No) {
		m_rot->Init(m_rotation, speed);
	}
	m_rotstate = state;
}

void Obj::ClcLocalMatrix(const CMatrix& worldMatrix)
{
	//プレイヤーの逆行列を求める
	CMatrix ReverseMatrix;
	ReverseMatrix.Inverse(worldMatrix);
	//オブジェクトのワールド行列とプレイヤーの逆行列を乗算して、
	//プレイヤーを基準としたオブジェクトのローカル行列を求める
	m_localMatrix.Mul(m_skin->GetSkinModel().GetWorldMatrix(),ReverseMatrix);
	m_player = FindGO<Player>();
	m_movestate = enMove_MoveHit;
}

void Obj::ClcMatrix()
{
	m_worldMatrix.Mul(m_localMatrix, m_player->GetCSkinModelRender().GetSkinModel().GetWorldMatrix());
}

void Obj::Update()
{
	if (m_movestate == enMove_MoveHit) {
		ClcMatrix();
		m_skin->SetWorldMatrix(m_worldMatrix);
	}
	else {
		if (m_movestate != enMove_No) {
			m_position = m_move->Move();
		}
		if (m_rotstate != enRot_No) {
			m_rotation = m_rot->Rot(m_move->GetMoveVector());
		}
		m_skin->SetPosition(m_position);
		m_skin->SetRotation(m_rotation);
	}	
}