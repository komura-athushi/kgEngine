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
	const float Multiply = 1.1f;
	if (!m_issphere && VertexFactory::GetInstance().m_vertexList.count(m_objdata->s_name) == 0) {
		Vertex vertex;
		CVector3 pos = CVector3::Zero();
		vertex.s_list[0] = pos += CVector3::AxisY() * m_objdata->s_y;					//+y
		/*vertex.s_list[1] = pos += CVector3::AxisZ() * m_objdata->s_z + CVector3::AxisX() * m_objdata->s_x;
		vertex.s_list[2] = pos -= CVector3::AxisZ() * m_objdata->s_z * 2;
		vertex.s_list[3] = pos -= CVector3::AxisX() * m_objdata->s_x * 2;
		vertex.s_list[4] = pos += CVector3::AxisZ() * m_objdata->s_z * 2;*/
		CVector3 pos2 = CVector3::Zero();
		vertex.s_list[1] = pos2 -= CVector3::AxisY() * m_objdata->s_y;					//-y
		/*vertex.s_list[6] = pos2 += CVector3::AxisZ() * m_objdata->s_z + CVector3::AxisX() * m_objdata->s_x;
		vertex.s_list[7] = pos2 -= CVector3::AxisZ() * m_objdata->s_z * 2;
		vertex.s_list[8] = pos2 -= CVector3::AxisX() * m_objdata->s_x * 2;
		vertex.s_list[9] = pos2 += CVector3::AxisZ() * m_objdata->s_z * 2;*/
		CVector3 pos3 = CVector3::Zero();
		vertex.s_list[2] = pos3 += CVector3::AxisZ() * m_objdata->s_z;					//+z
		vertex.s_list[3] = pos3 -= CVector3::AxisZ() * m_objdata->s_z + CVector3::AxisX() * m_objdata->s_x;			//-x
		vertex.s_list[4] = pos3 += CVector3::AxisX() * m_objdata->s_x * 2;											//+x
		vertex.s_list[5] = pos3 -= CVector3::AxisZ() * m_objdata->s_z + CVector3::AxisX() * m_objdata->s_x;			//-z
		CVector3 pos4 = CVector3::Zero() + CVector3::AxisY() * m_objdata->s_y;
		vertex.s_list[6] = pos4 + CVector3::AxisX() * m_objdata->s_x;
		vertex.s_list[7] = pos4 - CVector3::AxisX() * m_objdata->s_x;
		vertex.s_list[8] = pos4 + CVector3::AxisZ() * m_objdata->s_z;
		vertex.s_list[9] = pos4 - CVector3::AxisZ() * m_objdata->s_z;
		CVector3 pos5 = CVector3::Zero() - CVector3::AxisY() * m_objdata->s_y;
		vertex.s_list[10] = pos5 + CVector3::AxisX() * m_objdata->s_x;
		vertex.s_list[11] = pos5 - CVector3::AxisX() * m_objdata->s_x;
		vertex.s_list[12] = pos5 + CVector3::AxisZ() * m_objdata->s_z;
		vertex.s_list[13] = pos5 - CVector3::AxisZ() * m_objdata->s_z;
		for (int i = 0; i < sizeof(m_bufferList) / sizeof(m_bufferList[0]); i++) {
			vertex.s_list[i] = vertex.s_list[i] * Multiply;
		}
		VertexFactory::GetInstance().m_vertexList[m_objdata->s_name] = vertex;
	}
	Vertex vertex = VertexFactory::GetInstance().m_vertexList[m_objdata->s_name];
	if (m_movestate != enMove_MoveHit) {
		for (int i = 0; i < sizeof(m_bufferList) / sizeof(m_bufferList[0]); i++) {
			m_bufferList[i] = m_position + m_rotation.ReturnMultiply(vertex.s_list[i]);
		}
	}
	else {
		CVector3 pos;
		pos.x = m_worldMatrix.m[3][0];
		pos.y = m_worldMatrix.m[3][1];
		pos.z = m_worldMatrix.m[3][2];
		CQuaternion rot;
		rot.SetRotation(m_worldMatrix);
		m_rotation = rot;
		m_box.Update(m_worldMatrix);
		if (m_objdata->s_state == enState_X) {
			m_bufferList[4] = pos + rot.ReturnMultiply(vertex.s_list[4]);
			m_bufferList[3] = pos + rot.ReturnMultiply(vertex.s_list[3]);
			CVector3 diff = m_bufferList[4] - m_player->GetPosition();
			CVector3 diff2 = m_bufferList[3] - m_player->GetPosition();
			if (diff.LengthSq() <= diff2.LengthSq()) {
				m_position = m_bufferList[4];
				m_linevector = m_bufferList[3] - m_bufferList[4];
			}
			else {
				m_position = m_bufferList[3];
				m_linevector = m_bufferList[4] - m_bufferList[3];
			}
		}
		else if(m_objdata->s_state == enState_Y){
			m_bufferList[0] = pos + rot.ReturnMultiply(vertex.s_list[0]);
			m_bufferList[1] = pos + rot.ReturnMultiply(vertex.s_list[1]);
			CVector3 diff = m_bufferList[0] - m_player->GetPosition();
			CVector3 diff2 = m_bufferList[1] - m_player->GetPosition();
			if (diff.LengthSq() <= diff2.LengthSq()) {
				m_position = m_bufferList[0];
				m_linevector = m_bufferList[1] - m_bufferList[0];
			}
			else {
				m_position = m_bufferList[1];
				m_linevector = m_bufferList[0] - m_bufferList[1];
			}
		}
		else if (m_objdata->s_state == enState_Z) {
			CVector3 po = m_worldMatrix.ReturnApply(vertex.s_list[2]);
			CVector3 po2 = rot.ReturnMultiply(vertex.s_list[2]);
			CVector3 po3 = m_worldMatrix.ReturnApply(vertex.s_list[5]);
			CVector3 po4 = rot.ReturnMultiply(vertex.s_list[5]);
			//m_bufferList[2] = pos + rot.ReturnMultiply(vertex.s_list[2]);
			//m_bufferList[5] = pos + rot.ReturnMultiply(vertex.s_list[5]);
			m_bufferList[2] = po;
			m_bufferList[5] = po3;
			CVector3 diff = m_bufferList[2] - m_player->GetPosition();
			CVector3 diff2 = m_bufferList[5] - m_player->GetPosition();
			CVector3 line = m_box.SurfaceLineSegment(enXYZ_Z);

			if (m_bufferList[2].y >= m_player->GetPosition().y && m_bufferList[5].y >= m_player->GetPosition().y) {
				m_isclclinesegment = false;
			}
			else {
				/*if (m_bufferList[2].y >= m_bufferList[5].y) {
					m_position = m_bufferList[2];
					m_linevector = m_bufferList[5] - m_bufferList[2];
				}
				else {
					m_position = m_bufferList[5];
					m_linevector = m_bufferList[2] - m_bufferList[5];
				}*/
				m_linevector = m_box.SurfaceLineSegment(enXYZ_Z) - m_player->GetPosition();
				m_isclclinesegment = true;
			}
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