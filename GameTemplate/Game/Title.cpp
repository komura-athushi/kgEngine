#include "stdafx.h"
#include "Title.h"
#include "StageSelect.h"
#include "Fade.h"
#include "Collection.h"
#include "Player.h"
#include "GameCamera.h"

Title::Title()
{

}

Title::~Title()
{

}

void Title::OnDestroy()
{
	DeleteGO(m_player);
	DeleteGO(m_gameCamera);
	DeleteGO(m_model);
}

bool Title::Start()
{
	//画像を読み込む
	m_sprite.Init(L"Resource/sprite/title.dds", false);
	m_player = NewGO<Player>(0);
	m_player->SetPosition({0.0f,20.0f,0.0f});
	m_player->SetisTitle();
	m_gameCamera = NewGO<GameCamera>(0);
	m_model = NewGO<CSkinModelRender>(0);
	m_model->Init(L"Resource/modelData/ground1.cmo");
	m_model->SetPosition({0.0f,0.0f,0.0f});
	m_model->SetShadowCaster(false);
	m_staticobject.CreateMeshObject(m_model,CVector3::Zero(),CQuaternion::Identity());
	return true;
}

void Title::Update()
{
	//m_player->SetPosition({ 0.0f,0.0f,0.0f });
	//スタートボタンが押されたら画面を切り替える
	if (Engine().GetPad(0).IsTrigger(enButtonStart)) {
		m_isWaitFadeout = true;
		NewGO<StageSelect>(0);
		DeleteGO(this);
	}
	if (Engine().GetPad(0).IsTrigger(enButtonSelect)) {
		m_isWaitFadeout = true;
		NewGO<Collection>(0);
		DeleteGO(this);
	}
	//m_player->SetPosition({ 0.0f,500.0f,0.0f });
}

void Title::PostRender()
{
	m_sprite.DrawScreenPos();
}