#include "stdafx.h"
#include "Ground.h"
#include "GameData.h"
Ground::Ground()
{

}

Ground::~Ground()
{
	
}

bool Ground::Start()
{
	int stageNumber = GetGameData().GetStageNumber();
	//ƒ‚ƒfƒ‹‚ğ“Ç‚İ‚Ş
	wchar_t path[256];
	swprintf_s(path, L"Resource/modelData/ground%d.cmo", stageNumber);
	m_skinModelRender.Init(path);
	m_skinModelRender.SetPosition(m_position);
	m_skinModelRender.SetShadowReceiver(true);
	m_skinModelRender.SetRotation(m_rotation);
	m_staticobject.CreateMeshObject(&m_skinModelRender, m_position, m_rotation);

	return true;
}

void Ground::Update()
{
	
}