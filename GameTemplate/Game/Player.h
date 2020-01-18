#pragma once
#include "character/CharacterController.h"
#include "graphics/2D/kgFont.h"
class GameCamera;
class GameData;
//(��,_�T��)���̂ł��킟...
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void PostRender() override;
	//�̐ς����Z����
	void AddVolume(const float volume);
	//���W���擾
	CVector3 GetPosition() const
	{
		return m_position + CVector3::AxisY() * m_radius;
	}
	//���W���擾(�L�����R��)
	CVector3 GetCharaconPosition() const
	{
		return m_position;
	}
	//���W��ݒ�
	void SetPosition(const CVector3& pos)
	{
#ifdef _DEBUG
		if (isnan(pos.x) || isnan(pos.y) || isnan(pos.z)) {
			std::abort();
		}
#endif
		m_position = pos;
		m_characon.SetPosition(pos);
		m_skinModelRender.SetPosition(pos + CVector3::AxisY() * m_radius);
		if (m_isStart) {
			m_skinModelRender.UpdateWorldMatrix();
			m_skinModelRender2.UpdateWorldMatrix();
		}
	}
	//��������ǂꂾ���傫���Ȃ��������擾
	float GetMag() const
	{
		return m_scale.y / 1.0f;
	}
	//���̂̔��a���擾
	float GetRadius() const
	{
		return m_radius;
	}
	//���̂̍ŏ��̔��a���擾
	float GetProtRadius() const
	{
		return m_protradius;
	}
	//CSkinModelRender���擾
	CSkinModelRender& GetCSkinModelRender()
	{
		return m_skinModelRender;
	}
	//MoveSpeed��y��0�ɂ���
	void SetMoveSpeedYZero()
	{
		m_movespeed.y = 0.0f;
	}
	//�J�E���g���擾
	int GetCount()
	{
		return m_count;
	}
	//��̃v���C���[�̑傫�����擾
	float GetStandardSize() const
	{
		return m_standardSize;
	}
	//�Ռ��̗͂�������
	void SetAddImpulse(const CVector3 impulse)
	{
		m_impulse += impulse;
	}
	//�傫���\�����Ȃ�
	void SetisNotDisplay()
	{
		m_isDisplayFont = false;
	}
	//�^�C�g������
	void SetisTitle()
	{
		m_isTitle = true;
	}
	//��̃X�N���[�����W�n���擾�����
	const CVector3& GetScreenPos()
	{
		return m_katamariVector;
	}
private:
	//�ړ�
	void Move();
	//��]
	void Turn();
	//����
	void Judgment();
	//�X�N���[�����W�v�Z�����
	void ScreenPosition();
private:
	CharacterController m_characon;																//�L�����R��
	CVector3 m_position = { 0.0f,200.0f,0.0f }, m_beforeposition = CVector3::Zero();			//���W
	CVector3 m_movespeed = CVector3::Zero();													//�ړ��x�N�g��
	CVector3 m_scale = CVector3::One();															//�傫��
	CQuaternion m_rotation = CQuaternion::Identity();											//��]
	//�A�j���[�V����
	enum EnAnimationClip {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_run,
		enAnimationClip_jump,
		//enAnimationClip_damage,
		//enAnimationClip_KneelDown,
		//enAnimationClip_Clear,
		//enAnimationClip_Aria,
		enAnimationClip_num,
	};
	AnimationClip m_animClip[enAnimationClip_num];
	enum EnState {
		enState_idle,
		enState_walk,
		enState_jump,
		enState_num
	};
	EnState m_state = enState_idle;
	CSkinModelRender m_skinModelRender;															//�X�L�����f���B
	CSkinModelRender m_skinModelRender2;														//�l�̃��f��
	GameCamera* m_gamecamera = nullptr;															//�J����
	bool m_isjump = false;																		//�W�����v�����ǂ���
	float m_radius = 0.0f;																		//���̂̔��a
	float m_protradius = 0.0f;																	//�ŏ��̋��̂̔��a	

	float m_volume = 0.0f;																		//��̑̐�																	
	const float m_protmovespeedmultiply = 5.0f;													//�ŏ��̈ړ����x
	float m_movespeedmultiply = 5.0f;															//�ړ����x
	bool m_isbound = false;																		//�o�E���h�����ǂ���
	CFont m_font;																				//����
	float m_timer = 0.0f;																		//�_�b�V���̔���Ɏg���^�C�}�[
	int m_count = 0;																			//�_�b�V���̔���Ɏg���J�E���g	
	bool m_isRight = false;																		//�X�e�B�b�N�̉E���O�����A�����������ɓ|���ꂽ��
	bool m_isDush = false;																		//�_�b�V��������
	GameData* m_gamedata = nullptr;																//�Q�[���f�[�^
	/*CSprite m_sprite;
	CSprite m_sprite2;
	float m_degree = 0.0f;*/
	const float m_standardSize = 40.0f;															//��̉�̒��a
	CVector3 m_impulse = CVector3::Zero();														//���͎g���Ă��Ȃ�
	int m_count2 = 0;																			//�ǂɂԂ�����
	bool m_isDisplayFont = true;																//��̑傫����\������H
	bool m_isTitle = false;																		//�^�C�g���H
	bool m_isBrake = false;																		//�u���[�L�H
	float m_collisionTimer = 0.0f;																//�Փˉ���A���ŏo���Ȃ����߂̃^�C�}�[
	CVector3 m_katamariVector = CVector3::One();												//��̃X�N���[�����W�n
};

