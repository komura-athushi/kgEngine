#pragma once
#include "Object/Move/IMove.h"
class Anim
{
public:
	Anim();
	~Anim();
	//������
	void Init(const wchar_t* filepath,CSkinModelRender* skin);
	//�A�j���[�V�������Đ�
	void PlayAnimation(EnMove state);
	//���ݍĐ����̃A�j���[�V�������擾
	int GetPlayAnimationType()
	{
		return m_animationState;
	}
private:
	CSkinModelRender* m_skinModelRender = nullptr;				//���f��
	//�A�j���[�V����
	enum EnAnimationClip {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_num
	};
	AnimationClip m_animClip[enAnimationClip_num];				//�A�j���[�V�����N���b�v
	EnAnimationClip m_animationState = enAnimationClip_idle;
	bool m_isInit = false;										//�����������H
};

