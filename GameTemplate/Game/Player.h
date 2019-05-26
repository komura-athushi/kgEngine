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
	CSkinModelRender* m_model;									//�X�L�����f���B
	//SkinModel* m_model;
};

