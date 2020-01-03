#pragma once
#include "graphics/SpringCamera.h"
class Player;
class GameData;
//�ǂ����̃X�e�B�b�N����]��
enum StateEnterStick {
	enStick_EnterStickR,					//�E�X�e�B�b�N����������Ă���
	enStick_EnterStickL,					//���X�e�B�b�N����������Ă���
	enStick_EnterStickBoth,					//�ǂ���̃X�e�B�b�N�����͂���Ă���
	enStick_NoEnterStick,					//�ǂ���̃X�e�B�b�N�����͂���Ă��Ȃ�
	enStick_EnterStickBothOppositeLeft,		//���E�̃X�e�B�b�N�����ꂼ��t�����ɓ��͂���Ă���A�����O����
	enStick_EnterStickBothOppositeRight		//���E�̃X�e�B�b�N�����ꂼ��t�����ɓ��͂���Ă���A�E���O����
};

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start() override;
	void Update() override;
	//�X�e�B�b�N�̓��͏�Ԃ��擾
	StateEnterStick GetStateStick() const
	{
		return m_state;
	}
	//���W��ݒ�
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	//�����_��ݒ�
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	//�J�����̍��W���璍���_�܂ł̋������擾
	float GetRadius() const
	{
		return m_radius;
	}
private:
	//�p�b�h�̓��͂��l�����ăJ�����̎��_�ƒ����_���v�Z
	void Calculation();
	//�v���C���[�̑傫�����l�����ăJ�����̔��a��ς���
	void TransRadius();
	//L3��R3�𓯎������������Ɏ��_��ύX����
	void TransView();
private:
	CVector3 m_position = CVector3::Zero();				//���_
	CVector3 m_target = CVector3::One();				//�����_
	float m_degreey = 0.0f, m_degreexz = 20.0f;			//���ꂼ��J�����̊p�x
	Player* m_player = nullptr;							//�v���C���[�̃|�C���^
	float m_radius = 0.0f;								//�J�����̔��a
	const float m_protradius = 170.0f;					//�J�����̍ŏ��̔��a
	StateEnterStick m_state = enStick_NoEnterStick;		//�X�e�B�b�N�̓��͂ɂ���
	float m_Multiply = 1.0f;							//�J�����Ɖ�̋����̔{��
	float m_timer = 0.0f;								//L3��R3�������ɉ����ꂽ���Ɏ��Ԃ��v��
	bool m_transView = false;							//L3��R3�������ɉ����ꂽ���̃J������J�ڂ��Ă��邩
	GameData* m_gamedata = nullptr;						//�Q�[���f�[�^
	SpringCamera m_springCamera;						//�΂˃J����
};

