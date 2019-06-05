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
	DeleteGO(m_skinModelRender);
}

bool Ground::Start()
{
	m_skinModelRender = NewGO<CSkinModelRender>(0);
	m_skinModelRender->Init(L"Resource/modelData/ground.cmo");
	m_skinModelRender->SetPosition(m_position);
	//m_skinModelRender->SetScale(CVector3::One() * 0.3f);
	m_skinModelRender->SetShadowReceiver(true);
	//m_skinModelRender->SetShadowCaster(true);
	m_staticobject.CreateMeshObject(m_skinModelRender, m_position, CQuaternion::Identity());
	return true;
}

void Ground::Update()
{

}