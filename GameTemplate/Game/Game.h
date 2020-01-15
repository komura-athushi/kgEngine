#pragma once
#include "level/Level.h"

class Ground;
class Player;
class GameCamera;
class Obj;
class Time;
class Fade;
class GameData;
class OffScreen;
//�Q�[���N���X
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	void PostRender() override;
	//�X�e�[�W���I���I�����Ɛݒ肷��
	void SetOwaOwari()
	{
		m_owaOwari = true;
	}
	//�I���I�����H
	bool GetisOwaOwari()
	{
		return m_owaOwari;
	}
private:
	Ground* m_ground = nullptr;									//�O���E���h
	Player* m_player = nullptr;									//�v���C���[
	GameCamera* m_gamecamera = nullptr;							//�J����
	std::vector<Obj*> m_objList;								//�I�u�W�F�N�g�̃��X�g
	float m_timer = 0.0f;										//�^�C�}�[
	//level
	Level m_level;												//���x��
	Time* m_time;												//�^�C��
	Fade* m_fade;												//�t�F�[�h
	GameData* m_gameData = nullptr;								//�Q�[���f�[�^
	bool m_owaOwari = false;									//�X�e�[�W���I���点�邩
	CFont m_font;												//�t�H���g
	float m_timer2 = 0.0f;										//�^�C�}�[
	bool m_isPose = false;										//�|�[�Y���H
	OffScreen* m_offScreen = nullptr;							//�����p�̃N���X
	CSprite m_pause;											//�|�[�Y
	CSprite m_end;												//�I���
};

