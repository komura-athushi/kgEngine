#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Ground.h"
#include "GameCamera.h"
#include "Object.h"
#include "Object/Obj.h"
#include "Object/Move/IMove.h"
#include "Object/Rotation/IRot.h"
#include "Object/ObjectData.h"
#include "Moji.h"
Game::Game()
{

}

Game::~Game()
{
	DeleteGO(m_ground);
	DeleteGO(m_gamecamera);
	DeleteGO(m_player);
}

bool Game::Start()
{
	m_gamecamera = NewGO<GameCamera>(0);
	m_player = NewGO<Player>(0);
	m_ground = NewGO<Ground>(0);
	m_object = NewGO<Object>(0);
	m_object->SetScale(0.8f);
	m_object = NewGO<Object>(0);
	m_object->SetPosition({ 500.0f,100.0f,1000.0f });
	m_object->SetScale(1.2f);
	m_object = NewGO<Object>(0);
	m_object->SetPosition({ 1000.0f,200.0f,000.0f });
	m_object->SetScale(1.4f);
	m_object = NewGO<Object>(0);
	m_object->SetPosition({ 1500.0f,200.0f,500.0f });
	m_object->SetScale(1.8f);
	m_object = NewGO<Object>(0);
	m_object->SetPosition({ -1100.0f,200.0f,-500.0f });
	m_object->SetScale(1.1f);
	m_level.Init(L"Assets/level/level00.tkl", [&](LevelObjectData& objdata) {
		if (objdata.ForwardMatchName(L"obj")) {
			Obj* obj = NewGO<Obj>(0);
			MOVESTATUS ms = FindMove(objdata.name);
			obj->InitMove(ms.s_state, objdata.position, ms.s_move, ms.s_limit, objdata.rotation);
			ROTSTATUS rs = FindRot(objdata.name);
			obj->InitRot(rs.s_state, rs.s_speed);
			return true;
		}
		return true;
	});
	ObjectData::GetInstance();
	return true;
}

void Game::Update()
{
	
}