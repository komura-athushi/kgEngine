#include "stdafx.h"
#include "Result.h"
#include "GameData.h"

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	m_sprite.Init(L"Resource/sprite/result.dds", false);
	m_gamedata = &GetGameData();
	m_gamedata->SetScene(enScene_Result);
	return true;
}

void Result::Update()
{

}

void Result::PrePostRender()
{
	m_sprite.DrawScreenPos();
}