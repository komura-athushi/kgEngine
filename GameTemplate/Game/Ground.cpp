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
	m_skinModelRender->SetScale({ 1.0f, 1.0f, 1.0f });
	return true;
}

void Ground::Update()
{

}