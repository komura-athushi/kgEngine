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
	//���f����ǂݍ��ށA�X�e�[�W�̔ԍ��ɂ���ēǂݍ��ރt�@�C����ݒ肷��
	wchar_t path[256];
	swprintf_s(path, L"Resource/modelData/ground%d.cmo", stageNumber);
	m_skinModelRender.Init(path);
	m_skinModelRender.SetPosition(m_position);
	m_skinModelRender.SetShadowReceiver(true);
	m_skinModelRender.SetShadowCaster(false);
	m_skinModelRender.SetRotation(m_rotation);
	m_skinModelRender.SetOffToonShader();
	m_staticobject.CreateMeshObject(&m_skinModelRender, m_position, m_rotation);
	return true;
}

void Ground::Update()
{
	
}