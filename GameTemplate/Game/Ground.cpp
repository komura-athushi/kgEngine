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
	m_skinModelRender = NewGO<CSkinModelRender>(0);
	m_skinModelRender->Init(L"Resource/modelData/ground.cmo");
	return true;
}

void Ground::Update()
{

}