#pragma once
class Fade;
//�^�C�g��
class Title:public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override;
	void Update() override;
	void PostRender() override;
private:
	CSprite m_sprite;										//�X�v���C�g
	Fade* m_fade;											//�t�F�[�h(��)
	bool m_isWaitFadeout = false;							
};

