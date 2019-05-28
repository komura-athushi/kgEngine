#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Ground.h"
Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	m_player = NewGO<Player>(0);
	m_ground = NewGO<Ground>(0);
	return true;
}

void Game::Update()
{
	m_timer++;
	if (m_timer >= 10 && !m_is) {
		DeleteGO(m_player);
		m_is = true;
	}
}