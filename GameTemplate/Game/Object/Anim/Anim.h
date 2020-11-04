/*****************************************************************//**
 * \file   Anim.h
 * \brief Anim�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "Object/Move/IMove.h"
/**
 * \brief �I�u�W�F�N�g�̃A�j���[�V�����̐��䂷��.
 */
class Anim
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Anim();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Anim();
	/**
	 * \brief ����������.
	 * 
	 * \param filepath �A�j���[�V�����̃t�@�C���p�X
	 * \param skin �X�L�����f�������_���[
	 */
	void Init(const wchar_t* filepath,CSkinModelRender* skin);
	/**
	 * \brief �A�j���[�V�������Đ�.
	 * 
	 * \param state �ړ���Ԃ�\���X�e�[�g
	 */
	void PlayAnimation(EnMove state);
	/**
	 * \brief ���ݍĐ����̃A�j���[�V�������擾.
	 * 
	 * \return �Đ����̃A�j���[�V�����̔ԍ�
	 */
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
	EnAnimationClip m_animationState = enAnimationClip_idle;	//�Đ����̃A�j���[�V�����̔ԍ�
	bool m_isInit = false;										//�����������H
};

