#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Ground.h"
#include "GameCamera.h"
#include "Object.h"
Game::Game()
{

}

Game::~Game()
{
	
}

void Game::OnDestroy()
{
	DeleteGO(m_player);
	DeleteGO(m_ground);
}

bool Game::Start()
{
	m_player = NewGO<Player>(0);
	m_ground = NewGO<Ground>(0);
	m_gamecamera = NewGO<GameCamera>(0);
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
	return true;
}

void Game::Update()
{
	
}