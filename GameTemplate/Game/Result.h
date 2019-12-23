#pragma once


class GameData;
class Player;
class GameCamera;
class Result:public IGameObject
{
public:
	Result();
	~Result();
	bool Start() override;
	void Update() override;
	void PrePostRender() override;
	void PostRender() override;
	void OnDestroy() override;
	void MovePlayer();
	void MoveGoal();
	void MoveResult();
	void TransScene();
private:
	CSprite m_sprite;
	GameData* m_gameData = nullptr;
	Player* m_player = nullptr;
	GameCamera* m_gameCamera = nullptr;
	float m_timer = 0.0f;
	CFont m_font;
	enum EnResultScene {
		EnResultScene_MovePlayer,
		EnResultScene_MoveGoal,
		EnResultScene_MoveResult,
		EnResultScene_TransScene
	};
	EnResultScene m_resultScene = EnResultScene_MovePlayer;
	
};

