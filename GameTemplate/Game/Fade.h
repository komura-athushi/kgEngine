/*****************************************************************//**
 * \file   Fade.h
 * \brief  Fade�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

/**
 * \brief �t�F�[�h���Ǘ�.
 */
class Fade:public IGameObject
{
private:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Fade();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Fade();
public:
	/**
	 * \brief �C���X�^���X���擾.
	 * 
	 * \return 
	 */
	static Fade& GetInstance()
	{
		//private�ȃR���X�g���N�^���Ăяo��
		static Fade inst;
		return inst;
	}
	/**
	 * \brief ������.
	 * 
	 * \return true�ŏ���������
	 */
	bool Start() override final;
	/**
	 * \brief �X�V����.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief �`�揈��.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief �X�^�[�g�t�F�[�h�C��.
	 * 
	 */
	void StartFadeIn()
	{
		m_state = enState_FadeOutLoading;
	}
	/**
	 * \brief �X�^�[�g�t�F�[�h�A�E�g.
	 * 
	 */
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	/**
	 * \brief �X�^�[�g�X���[�t�F�[�h�C��.
	 * 
	 */
	void StartFadeSlowIn()
	{
		m_state = enState_FadeSlowIn;
	}
	/**
	 * \brief �X�^�[�g�X���[�t�F�[�h�A�E�g.
	 * 
	 */
	void StartFadeSlowOut()
	{
		m_state = enState_FadeSlowOut;
	}
	/**
	 * \brief �t�F�[�h���H.
	 * 
	 * \return �t�F�[�h���Ă���true
	 */
	bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	/**
	 * \brief �A���t�@�l���擾.
	 * 
	 * \return �A���t�@�l
	 */
	float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}
	/**
	 * \brief �t�F�[�h�C�����H.
	 * 
	 * \return �t�F�[�h�C�����Ȃ�true
	 */
	bool IsFadeIn() const
	{
		return m_state == enState_FadeIn;
	}
	/**
	 * \brief �ҋ@���H.
	 * 
	 * \return �ҋ@���Ȃ�true
	 */
	bool IsIdle() const
	{
		return m_state == enState_Idle;
	}
private:
	enum EnState {
		enState_FadeIn,			//!<�t�F�[�h�C�����B
		enState_FadeSlowIn,     //�t�F�[�h�C����(�X���E)
		enState_FadeOut,		//!<�t�F�[�h�A�E�g���B
		enState_FadeSlowOut,    //�t�F�[�h�A�E�g��(�X���E)
		enState_FadeInLoading,  //�������t�F�[�h�C��
		enState_FadeOutLoading, //�������t�F�[�h�A�E�g
		enState_Idle,		//!<�A�C�h�����B

	};
	CSprite m_sprite;						//�X�v���C�g
	CSprite m_loading;
	EnState m_state = enState_Idle;	//!<��ԁB
	float m_currentAlpha = 0.0f;		//!<���݂̃��l�B
	CVector2 m_position = CVector2(FRAME_BUFFER_W / 2, FRAME_BUFFER_H / 2);
};

