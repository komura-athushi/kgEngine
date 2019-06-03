#pragma once
class Player;
class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start() override;
	void Update() override;
	//�p�b�h�̓��͂��l�����ăJ�����̎��_�ƒ����_���v�Z
	void Calculation();
private:
	CVector3 m_position = CVector3::Zero();				//���_
	CVector3 m_target = CVector3::Zero();				//�����_
	float m_degreey = 0.0f, m_degreexz = 0.0f;			//���ꂼ��J�����̊p�x
	Player* m_player = nullptr;							//�v���C���[�̃|�C���^
};

