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
	CSkinModelRender* m_model;									//スキンモデル。
	//SkinModel* m_model;
};

