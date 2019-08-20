#pragma once

class Fade:public IGameObject
{
	//�V���O���g���A���̃N���X�̃C���X�^���X����������݂����Ȃ����Ƃ������f�U�C���p�^�[���̈�ł�
private:
	//�R���X�g���N�^�ƃf�X�g���N�^��private�ɐ錾���邱�ƂŊO�����琶���o���Ȃ����܂�
	Fade();
	~Fade();
public:
	//���̊֐����g����GameData�N���X�̃C���X�^���X�𐶐����܂�
	//���̊֐����̃��[�J���ȐÓI�ϐ��͍ŏ��Ɋ֐����Ă΂��Ƃ��ɂ͏���������܂����A�ȍ~�Ă΂�鎞�͏���������܂���
	static Fade& GetInstance()
	{
		//private�ȃR���X�g���N�^���Ăяo��
		static Fade inst;
		return inst;
	}
	bool Start() override;
	void Update() override;
	void PostRender() override;
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	void StartFadeSlowIn()
	{
		m_state = enState_FadeSlowIn;
	}
	void StartFadeSlowOut()
	{
		m_state = enState_FadeSlowOut;
	}
	bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}

private:
	enum EnState {
		enState_FadeIn,			//!<�t�F�[�h�C�����B
		enState_FadeSlowIn,     //�t�F�[�h�C����(�X���E)
		enState_FadeOut,		//!<�t�F�[�h�A�E�g���B
		enState_FadeSlowOut,    //�t�F�[�h�A�E�g��(�X���E)
		enState_Idle,		//!<�A�C�h�����B
	};
	CSprite m_sprite;
	EnState m_state = enState_Idle;	//!<��ԁB
	float m_currentAlpha = 1.0f;		//!<���݂̃��l�B
};

