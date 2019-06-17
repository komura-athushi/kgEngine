#include "stdafx.h"
#include "Object.h"

Object::Object()
{

}

Object::~Object()
{
	DeleteGO(m_skinModelRender);
}

bool Object::Start()
{
	m_skinModelRender = NewGO<CSkinModelRender>(0);
	m_skinModelRender->Init(L"Resource/modelData/sphere2.cmo");
	m_skinModelRender->SetPosition(m_position);
	//m_skinModelRender->SetScale(CVector3::One() * 0.3f);
	m_skinModelRender->SetShadowReceiver(true);
	m_skinModelRender->SetShadowCaster(true);
	m_skinModelRender->SetScale(CVector3::One() + CVector3::One() * (m_scale - 1));
	m_size *= m_scale;
	m_staticobject.CreateSphereObject(m_size, m_position, CQuaternion::Identity());
	return true;
}

void Object::Update()
{
	
}