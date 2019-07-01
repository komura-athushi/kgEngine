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

void Obj::SetFilePath(const wchar_t* path)
{
	m_filepath = path;
	wchar_t filepath[256];
	swprintf_s(filepath, L"Resource/modelData/%ls.cmo", path);
	m_skin.Init(filepath);
}

bool Obj::Start()
{
	m_skin.SetPosition(m_position);
	m_skin.SetRotation(m_rotation);
	m_size = (m_objdata->s_x + m_objdata->s_y + m_objdata->s_z) / 3;
	if (m_objdata->s_issphere == 1) {
		//球体である
		m_issphere = true;
		m_size = m_objdata->s_x;
		m_lenght = m_size * 2;
	}
	else {
		//m_staticobject.CreateMeshObject(&m_skin, m_position, m_rotation);
		m_staticobject.CreateBoxObject(m_position, m_rotation, {m_objdata->s_x * 2,m_objdata->s_y * 2,m_objdata->s_z * 2});
		//m_staticobject.CreateBoxObject(m_position, m_rotation, { m_objdata->s_x,m_objdata->s_y,m_objdata->s_z});
		m_lenght = (m_objdata->s_x + m_objdata->s_y + m_objdata->s_z) * 2;
	}
	if (m_objdata->s_islinesegment == 1) {
		m_islinesegment = true;
		m_linevector = m_objdata->s_linevector;
	}
	m_box.Init(CVector3(m_objdata->s_x,m_objdata->s_y,m_objdata->s_z));
	ClcVertex();
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

void Obj::ClcVertex()
{
	if (m_movestate != enMove_MoveHit) {
		m_box.Update(m_skin.GetSkinModel().GetWorldMatrix());
	}
	else {
		m_box.Update(m_worldMatrix);
		if (m_box.GetisLowPositionY(m_player->GetPosition(),m_objdata->s_state)) {
			m_isclclinesegment = false;
		}
		else {
			m_linevector = m_box.SurfaceLineSegment(m_objdata->s_state) - m_player->GetPosition();
			m_isclclinesegment = true;
		}
	}
}

void Obj::ClcLocalMatrix(const CMatrix& worldMatrix)
{
	//プレイヤーの逆行列を求める
	CMatrix ReverseMatrix;
	ReverseMatrix.Inverse(worldMatrix);
	//オブジェクトのワールド行列とプレイヤーの逆行列を乗算して、
	//プレイヤーを基準としたオブジェクトのローカル行列を求める
	m_localMatrix.Mul(m_skin.GetSkinModel().GetWorldMatrix(),ReverseMatrix);
	m_player = FindGO<Player>();
	m_movestate = enMove_MoveHit;
	m_staticobject.Release();
	if (m_islinesegment) {
		m_linesegment.Init(m_position);
	}
	m_box.Init(CVector3(m_objdata->s_x, m_objdata->s_y, m_objdata->s_z));
}

void Obj::ClcMatrix()
{
	m_worldMatrix.Mul(m_localMatrix, m_player->GetCSkinModelRender().GetSkinModel().GetWorldMatrix());
}

void Obj::Update()
{
	if (m_movestate == enMove_MoveHit) {
		ClcMatrix();
		m_skin.SetWorldMatrix(m_worldMatrix);
		if (m_islinesegment) {
			ClcVertex();
			if (m_isclclinesegment) {
				m_linesegment.Execute(m_player->GetPosition(), m_linevector);
			}
		}
		//m_skin.SetRotation(m_rotation);
	}
	else {
		if (m_movestate != enMove_No) {
			m_position = m_move->Move();
			m_staticobject.SetPosition(m_position);
		}
		if (m_rotstate != enRot_No) {
			m_rotation = m_rot->Rot(m_move->GetMoveVector());
			m_staticobject.SetRotation(m_rotation);
		}
		if (m_movestate != enMove_No || m_rotstate != enRot_No) {
			ClcVertex();
		}
		m_skin.SetPosition(m_position);
		m_skin.SetRotation(m_rotation);
	}	
}

void Obj::PostRender()
{
	if (m_movestate != enMove_MoveHit) {
		return;
	}
	/*wchar_t output[256];
	swprintf_s(output, L"%f", m_bufferList[2].y - m_bufferList[5].y);
	m_font.DrawScreenPos(output,CVector2::Zero());*/
}