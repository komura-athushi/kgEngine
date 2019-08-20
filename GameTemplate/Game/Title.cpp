#include "stdafx.h"
#include "Title.h"
#include "StageSelect.h"
#include "Fade.h"
Title::Title()
{

}

Title::~Title()
{

}

bool Title::Start()
{
	//çï
	m_sprite.Init(L"Resource/sprite/title.dds", false);
	return true;
}

void Title::Update()
{
	if (Engine().GetPad(0).IsTrigger(enButtonStart)) {
		m_isWaitFadeout = true;
		NewGO<StageSelect>(0);
		DeleteGO(this);
	}
}

void Title::PostRender()
{
	m_sprite.DrawScreenPos();
}