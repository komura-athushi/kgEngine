#pragma once
class SkinModelDataManager;
class CSkinModelRender:public IGameObject
{
public:
	CSkinModelRender();
	~CSkinModelRender();
	/*!
	* @brief	�������B
	*@param[in]	filePath			���[�h������cmo�t�@�C���̃t�@�C���p�X�AL"Resource/"����K�v�ł�
	*@param[in]	animationClips		�A�j���[�V�����N���b�v�̔z��̐擪�A�h���X
	*@param[in]	numAnimClip			�A�j���[�V�����N���b�v�̐�
	*@param[in]	enUpdateAxis		fbx�̏�����B
	*/
	void Init(const wchar_t* filePath,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ
		);
	//���̃N���X�͂���ȏ�p�������Ȃ�����I�[�o�[���C�h�͂����ŏI���
	bool Start() override final;
	void Update() override final;
	void PreUpdate() override final;
private:
	//�A�j���[�V�����̏�����
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips);
public:
	//�X�L�����f�����擾
	SkinModel& GetSkinModel()
	{
		return m_skinModel;
	}

	//���W��ݒ�
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
		m_update = true;
	}
	//���W���擾
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//�傫����ݒ�
	void SetScale(const CVector3 scale)
	{
		m_scale = scale;
		m_update = true;
	}
	//�傫�����擾
	CVector3 GetScale() const
	{
		return m_scale;
	}
	//��]��ݒ�
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
		m_update = true;
	}
	//��]���擾
	CQuaternion GetRotation() const
	{
		return m_rotation;
	}
	//�A�N�e�B�u�t���O��ݒ�
	void SetisActive(const bool& flag)
	{
		m_isActive = flag;
	}
	//�A�N�e�B�u���ǂ������擾
	bool isActive() const
	{
		return m_isActive;
	}
	/*!
	* @brief	�A�j���[�V�������Đ��B
	*@param[in]	clipNo	�A�j���[�V�����N���b�v�̔ԍ��B�R���X�g���N�^�ɓn����animClipList�̕��тƂȂ�B
	*@param[in]	interpolateTime		�⊮����(�P�ʁF�b)
	*/
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_animation.Play(animNo, interpolateTime);
	}
	//�A�j���[�V�������Đ������ǂ������擾
	bool IsPlayingAnimation() const
	{
		return m_animation.IsPlaying();
	}
	//�V���h�E�L���X�^�[��ݒ�
	void SetShadowCaster(bool caster)
	{
		m_skinModel.SetShadowCaster(caster);
	}
	//�V���h�E���V�[�o�[��ݒ�
	void SetShadowReceiver(bool receiver)
	{
		m_skinModel.SetShadowReceiver(receiver);
	}
private:
	SkinModel m_skinModel;										//�X�L�����f��
	AnimationClip* m_animationClip;								//�A�j���[�V�����N���b�v�̐�
	int	m_numAnimationClips = 0;								//!<�A�j���[�V�����N���b�v�̐��B
	Animation m_animation;										//�A�j���[�V����
	EnFbxUpAxis	m_enFbxUpAxis = enFbxUpAxisZ;					//!<FBX�̏�����B
	CVector3 m_position = CVector3::Zero();						//���W
	CVector3 m_scale = CVector3::One();							//�傫��
	CQuaternion m_rotation = CQuaternion::Identity();			//��]
	bool m_isActive = false;									//�A�N�e�B�u���ǂ���
	bool m_update = false;										//���W�Ƃ��傫�����X�V���ꂽ���ǂ����A����͕K��true�ɂ��ď���������
};

