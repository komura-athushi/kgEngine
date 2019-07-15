#pragma once
#include "character/CharacterController.h"
#include "graphics/2D/kgFont.h"
class GameCamera;
//(��,_�T��)���̂ł��킟...
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void PostRender() override;
	//�ړ�
	void Move();
	//��]
	void Turn();
	//����
	void Judgment();
	//���W���擾
	CVector3 GetPosition() const
	{
		return m_position + CVector3::AxisY() * m_radius;
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
	float m_radius = 45.0f;																		//���̂̔��a
	const float m_protradius = 45.0f;															//�ŏ��̋��̂̔��a	
	float m_volume = 0.0f;																		
	const float m_protmovespeedmultiply = 5.0f;													//�ŏ��̈ړ����x
	float m_movespeedmultiply = 5.0f;															//�ړ����x
	bool m_isbound = false;																		//�o�E���h�����ǂ���
	CFont m_font;																				//����
	/*CSprite m_sprite;
	CSprite m_sprite2;
	float m_degree = 0.0f;*/
};

