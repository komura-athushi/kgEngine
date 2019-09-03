#pragma once
class GameData;
class Result:public IGameObject
{
public:
	Result();
	~Result();
	bool Start() override;
	void Update() override;
	void PrePostRender() override;
private:
	CSprite m_sprite;
	GameData* m_gamedata = nullptr;
};

