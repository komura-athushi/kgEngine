#pragma once
class Player;

//�ǂ����̃X�e�B�b�N����]��
enum StateEnterStick {
	enStick_EnterStickR,					//�E�X�e�B�b�N����������Ă���
	enStick_EnterStickL,					//���X�e�B�b�N����������Ă���
	enStick_EnterStickBoth,					//�ǂ���̃X�e�B�b�N�����͂���Ă���
	enStick_NoEnterStick					//�ǂ���̃X�e�B�b�N�����͂���Ă��Ȃ�
};

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start() override;
	void Update() override;
	//�p�b�h�̓��͂��l�����ăJ�����̎��_�ƒ����_���v�Z
	void Calculation();
	//�v���C���[�̑傫�����l�����ăJ�����̔��a��ς���
	void TransRadius();
	//�X�e�B�b�N�̓��͏�Ԃ��擾
	StateEnterStick GetStateStick() const
	{
		return m_state;
	}
private:
	CVector3 m_position = CVector3::Zero();				//���_
	CVector3 m_target = CVector3::Zero();				//�����_
	float m_degreey = 0.0f, m_degreexz = 20.0f;			//���ꂼ��J�����̊p�x
	Player* m_player = nullptr;							//�v���C���[�̃|�C���^
	float m_radius = 0.0f;								//�J�����̔��a
	const float m_protradius = 260.0f;					//�J�����̍ŏ��̔��a
	StateEnterStick m_state = enStick_NoEnterStick;		//�X�e�B�b�N�̓��͂ɂ���
	float m_Multiply = 1.0f;							//�J�����Ɖ�̋����̔{��
};

