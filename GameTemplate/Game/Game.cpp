#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Ground.h"
#include "GameCamera.h"
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
	return true;
}

void Game::Update()
{
	
}