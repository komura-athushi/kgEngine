#pragma once
#include "character/CharacterController.h"
class GameCamera;
//(��,_�T��)���̂ł��킟...
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	//�ړ�
	void Move();
	//��]
	void Turn();
	//����
	void Judgment();
	//���W���擾
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//���W��ݒ�
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	//��������ǂꂾ���傫���Ȃ��������擾
	float GetMag() const
	{
		return m_scale.y / 1.0f;;
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
private:
	CharacterController m_characon;
	CVector3 m_position = { 0.0f,200.0f,0.0f }, m_beforeposition = CVector3::Zero();
	CVector3 m_movespeed = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rotation = CQuaternion::Identity();
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
	CSkinModelRender* m_skinModelRender = nullptr;							//�X�L�����f���B
	GameCamera* m_gamecamera = nullptr;
	bool m_isjump = false;
	float m_radius = 45.0f;
	const float m_protradius = 45.0f;
};

