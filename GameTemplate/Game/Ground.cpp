#include "stdafx.h"
#include "Ground.h"

Ground::Ground()
{

}

Ground::~Ground()
{

}

void Ground::OnDestroy()
{
	
}

bool Ground::Start()
{
	m_skinModelRender = NewGO<CSkinModelRender>(0);
	m_skinModelRender->Init(L"Resource/modelData/ground.cmo");
	m_skinModelRender->SetPosition(m_position);
	//m_skinModelRender->SetScale(CVector3::One() * 0.3f);
	m_skinModelRender->SetShadowReceiver(true);
	//m_skinModelRender->SetShadowCaster(true);
	m_staticobject1.CreateMeshObject(m_skinModelRender, m_position, CQuaternion::Identity());
	m_skinModelRender = NewGO<CSkinModelRender>(0);
	m_skinModelRender->Init(L"Resource/modelData/sphere.cmo");
	m_skinModelRender->SetPosition(m_position + CVector3::AxisY() * 50.0f + CVector3::AxisX() * 200.0f);
	//m_skinModelRender->SetScale(CVector3::One() * 0.3f);
	m_skinModelRender->SetShadowReceiver(true);
	//m_skinModelRender->SetShadowCaster(true);
	m_staticobject2.CreateSphereObject(45.0f, m_position + CVector3::AxisY() * 50.0f + CVector3::AxisX() * 200.0f, CQuaternion::Identity());
	m_skinModelRender = NewGO<CSkinModelRender>(0);
	m_skinModelRender->Init(L"Resource/modelData/sphere.cmo");
	m_skinModelRender->SetPosition(m_position + CVector3::AxisY() * 50.0f + CVector3::AxisY() * 100.0f + CVector3::AxisZ() * 200.0f);
	//m_skinModelRender->SetScale(CVector3::One() * 0.3f);
	m_skinModelRender->SetShadowReceiver(true);
	//m_skinModelRender->SetShadowCaster(true);
	m_staticobject3.CreateSphereObject(45.0f, m_position + CVector3::AxisY() * 50.0f + CVector3::AxisY() * 100.0f + CVector3::AxisZ() * 200.0f, CQuaternion::Identity());
	return true;
}

void Ground::Update()
{
	
}