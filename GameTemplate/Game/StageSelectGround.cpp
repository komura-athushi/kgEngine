#include "stdafx.h"
#include "StageSelectGround.h"

namespace {
	const float moveSpeedMultiply = 2.0f;		//移動速度
	const float rotationSpeed = 0.8f;			//回転速度

}

StageSelectGround::StageSelectGround()
{

}

StageSelectGround::~StageSelectGround()
{

}

bool StageSelectGround::Start()
{
	//モデル初期化
	m_skinModelRender.Init(L"Resource/modelData/earth.cmo");
	m_skinModelRender.SetPosition(m_position);
	m_skinModelRender.SetRotation(m_rotation);
	m_skinModelRender.SetScale(CVector3::One());
	m_skinModelRender.UpdateWorldMatrix();
	m_skinModelRender.SetOffToonShader();
	m_skinModelRender.SetShadowCaster(false);
	m_currentPosition = m_position;
	return true;

}

void StageSelectGround::Update()
{
	Move();
	Turn();
	m_skinModelRender.UpdateWorldMatrix();
}

void StageSelectGround::Move()
{
	
	m_beforePosition = m_currentPosition;
	CVector3 stickL;
	stickL.x = GetPad(0).GetLStickXF();
	stickL.y = GetPad(0).GetLStickYF();
	stickL.z = 0.0f;
	CVector3 moveSpeed = stickL;
	moveSpeed.z = moveSpeed.y;
	moveSpeed.y = 0.0f;
	m_currentPosition += moveSpeed * moveSpeedMultiply;
}

void StageSelectGround::Turn()
{
	CVector3 movespeedXZ = m_currentPosition - m_beforePosition;
	movespeedXZ.y = 0.0f;
	if (movespeedXZ.LengthSq() <= 0.01f) {
		return;
	}
	float Lengh = movespeedXZ.Length();
	//ノーマライズして移動方向の方向ベクトルを求める
	movespeedXZ /= Lengh;
	//Y軸と方向ベクトルの外積ベクトルを求める
	CVector3 pos;
	pos.Cross(CVector3::AxisY(), movespeedXZ);
	//外積ベクトルを元に回転させるクォータニオンを求める
	CQuaternion rot;
	rot.SetRotationDeg(pos, Lengh * rotationSpeed);
	//求めたクォータニオンを乗算する
	m_rotation.Multiply(rot, m_rotation);
	m_skinModelRender.SetRotation(m_rotation);
}