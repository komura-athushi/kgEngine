#pragma once
#include "character/CharacterController.h"

class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Draw();
private:
	SkinModel m_model;									//スキンモデル。
};

