#pragma once
#include "character/CharacterController.h"
class GameCamera;
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
	//�A�j���[�V����
	void AnimationController();
	//��]
	void Turn();
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
private:
	CVector3 m_position = CVector3::Zero();
	CVector3 m_movespeed = CVector3::Zero();
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
};

