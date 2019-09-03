#include "stdafx.h"
#include "Ground.h"

Ground::Ground()
{

}

Ground::~Ground()
{
	
}

bool Ground::Start()
{
	//ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	m_skinModelRender.Init(L"Resource/modelData/ground.cmo");
	m_skinModelRender.SetPosition(m_position);
	m_skinModelRender.SetShadowReceiver(true);
	m_staticobject.CreateMeshObject(&m_skinModelRender, m_position,CQuaternion::Identity());

	return true;
}

void Ground::Update()
{
	
}