#pragma once
class Fade;
//タイトル
class Title:public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override;
	void Update() override;
	void PostRender() override;
private:
	CSprite m_sprite;										//スプライト
	Fade* m_fade;
	bool m_isWaitFadeout = false;
};

