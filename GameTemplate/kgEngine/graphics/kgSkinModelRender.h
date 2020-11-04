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
	*@param[in]	isInstancing		�C���X�^���V���O���邩�ǂ���
	*/
	void Init(const wchar_t* filePath,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ,
		bool isInstancing = false
		);
	//�C���X�^���V���O�`��̃C���X�^���X�̐���������
	void SetInstanceNumber(const int& maxInstance);
	//�C���X�^���V���O�`��̃f�[�^��������
	void InitInstancing()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.InitInstancingData();
		}
	}
	//���̃N���X�͂���ȏ�p�������Ȃ�����I�[�o�[���C�h�͂����ŏI���
	bool Start() override final;
	void Update() override final;
	void Draw() override final;
	void PreUpdate() override final;
	//���[���h�s����X�V
	void UpdateWorldMatrix();
	//�A�j���[�V�����̏�����
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips, const wchar_t* filePath = nullptr);
	void InitTexture(TextureData* textureData)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.InitTexture(textureData);
		}
	}
public:
	//�X�L�����f�����擾
	SkinModel& GetSkinModel()
	{
		return m_skinModelList[0]->s_skinModel;
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
	*@brief	�A�j���[�V�������Đ��B
	*@param[in]	clipNo	�A�j���[�V�����N���b�v�̔ԍ��B�R���X�g���N�^�ɓn����animClipList�̕��тƂȂ�B
	*@param[in]	interpolateTime		�⊮����(�P�ʁF�b)
	*/
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_skinModelList[0]->s_animation.Play(animNo, interpolateTime);
	}
	//�A�j���[�V�������Đ������ǂ������擾
	/*bool IsPlayingAnimation() const
	{
		return m_skinModelList[0]->s_animation.IsPlaying();
	}*/
	//�V���h�E�L���X�^�[��ݒ�
	void SetShadowCaster(bool caster)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetShadowCaster(caster);
		}
	}
	//�V���h�E���V�[�o�[��ݒ�
	void SetShadowReceiver(bool receiver)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetShadowReceiver(receiver);
		}
	}
	//�X�L�����f���̐F�X���X�V
	void UpdateMatrix()
	{
		if (m_update) {
			m_skinModelList[0]->s_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
			m_update = false;
		}
	}
	void BeginUpdateInstancingData()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.BeginUpdateInstancingData();
		}
	}
	/// <summary>
	/// i�C���X�^���V���O�`��̃f�[�^���X�V���܂�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g��</param>
	void UpdateInstancingData(const CVector3& pos, const CQuaternion& rot, const CVector3& scale = CVector3::One(),int animNo = 0, float interpolateTime = 0.0f)
	{
		if (m_animationClip == nullptr) {
			m_skinModelList[0]->s_skinModel.UpdateInstancingData(pos, rot, scale);
		}
		else {
			m_skinModelList[animNo + 1]->s_skinModel.UpdateInstancingData(pos, rot, scale);
		}
	}
	//�C���X�^���V���O�p�̃f�[�^���X�V�A���[���h�s��𒼐ڐݒ肷��p
	void UpdateInstancingData(const CMatrix& worldMatrix, int animNo = 0, float interpolateTime = 0.0f)
	{
		if (m_animationClip == nullptr) {
			m_skinModelList[0]->s_skinModel.UpdateInstancingData(worldMatrix);
		}
		else {
			m_skinModelList[animNo + 1]->s_skinModel.UpdateInstancingData(worldMatrix);
		}
	}
	void SetWorldMatrix(const CMatrix& worldmatrix);
	void SetOffToonShader()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetOffToonShader();
		}
	}
	/// <summary>
	/// ��Z�J���[��ݒ�
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const CVector4& color)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetColor(color);
		}
	}
private:
	//SkinModel m_skinModel;										//�X�L�����f��
	AnimationClip* m_animationClip;								//�A�j���[�V�����N���b�v�̐�
	int	m_numAnimationClips = 0;								//!<�A�j���[�V�����N���b�v�̐��B
	//Animation m_animation;										//�A�j���[�V����
	EnFbxUpAxis	m_enFbxUpAxis = enFbxUpAxisZ;					//!<FBX�̏�����B
	CVector3 m_position = CVector3::Zero();						//���W
	CVector3 m_scale = CVector3::One();							//�傫��
	CQuaternion m_rotation = CQuaternion::Identity();			//��]
	CMatrix m_worldMatrix;										//���[���h�s��
	bool m_isActive = false;									//�A�N�e�B�u���ǂ���
	bool m_update = false;										//���W�Ƃ��傫�����X�V���ꂽ���ǂ����A����͕K��true�ɂ��ď���������
	bool m_isInitAnimation = false;								//�A�j���[�V�������������������ǂ���
	struct  AnimModel {
		SkinModel s_skinModel;
		Animation s_animation;
	};
	std::unordered_map<int, std::unique_ptr<AnimModel>> m_skinModelList;
	const wchar_t* m_filepath;
	bool m_isInstancing = false;
	int m_maxInstance = 0;
	bool m_isSetInstance = false;
};

