/*****************************************************************//**
 * \file   kgSkinModelRender.h
 * \brief  �X�L�����f�������_�[
 * 
 * \author akuro
 * \date   November 2020
 *********************************************************************/
#pragma once
class SkinModelDataManager;
/**
 * \brief ���f����`�悷��.
 */
class CSkinModelRender:public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	CSkinModelRender();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~CSkinModelRender();
	/**
	 * \brief ����������.
	 * 
	 * \param filePath ���[�h����t�@�C���p�X
	 * \param animationClips �A�j���[�V�����N���b�v�̔z��̐擪�̃|�C���^
	 * \param numAnimationClips �A�j���[�V�����N���b�v�̐�
	 * \param fbxUpAxis fbx�̏����
	 * \param isInstancing �C���X�^���V���O�`������邩�ǂ���
	 */
	void Init(const wchar_t* filePath,
		AnimationClip* animationClips = nullptr,
		const int numAnimationClips = 0,
		EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ,
		bool isInstancing = false
		);
	/**
	 * \brief �C���X�^���V���O�`��̍ۂ̍ő�`�搔��ݒ�.
	 * 
	 * \param maxInstance �ő�`�搔
	 */
	void SetInstanceNumber(const int maxInstance);
	/**
	 * \brief �C���X�^���V���O�`��̂��߂̃f�[�^��������.
	 * 
	 */
	void InitInstancing()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.InitInstancingData();
		}
	}
	//���̃N���X�͂���ȏ�p�������Ȃ�����I�[�o�[���C�h�͂����ŏI���
	/**
	 * \brief ����������.
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
	void Draw() override final;
	/**
	 * \brief �`��O�̏���.
	 * 
	 */
	void PreUpdate() override final;
	/**
	 * \brief ���[���h�s����v�Z.
	 * 
	 */
	void UpdateWorldMatrix();
	/**
	 * \brief �A�j���[�V�����̏�����.
	 * 
	 * \param animationClips �A�j���[�V�����N���b�v�̔z��̐擪�̃|�C���^
	 * \param numAnimationClips �A�j���[�V�����N���b�v�̐�
	 * \param filePath ���f���̃t�@�C���p�X
	 */
	void InitAnimation(AnimationClip* animationClips,const int numAnimationClips, const wchar_t* filePath = nullptr);
	/**
	 * \brief �m�[�}���}�b�v�Ȃǂ̃e�N�X�`���[�f�[�^�����[�h.
	 * 
	 * \param textureData �e�N�X�`���[�f�[�^
	 */
	void InitTexture(TextureData* textureData)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.InitTexture(textureData);
		}
	}
public:
	/**
	 * \brief �X�L�����f�����擾.
	 * 
	 * \return �X�L�����f��
	 */
	SkinModel& GetSkinModel()
	{
		return m_skinModelList[0]->s_skinModel;
	}
	/**
	 * \brief ���W��ݒ�.
	 * 
	 * \param pos ���W
	 */
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
		m_update = true;
	}
	/**
	 * \brief ���W���擾.
	 *  
	 * \return ���W 
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/**
	 * \brief �傫����ݒ�.
	 * 
	 * \param scale �傫��
	 */
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
		m_update = true;
	}
	/**
	 * \brief �傫�����擾.
	 * 
	 * \return �傫��
	 */
	const CVector3& GetScale() const
	{
		return m_scale;
	}
	/**
	 * \brief ��]��ݒ�.
	 * 
	 * \param rot ��]
	 */
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
		m_update = true;
	}
	/**
	 * \brief ��]���擾.
	 * 
	 * \return ��]
	 */
	const CQuaternion& GetRotation() const
	{
		return m_rotation;
	}
	/**
	 * \brief �A�N�e�B�u�t���O��ݒ�.
	 * 
	 * \param flag �A�N�e�B�u�t���O
	 */
	void SetisActive(const bool& flag)
	{
		m_isActive = flag;
	}
	/**
	 * \brief �A�N�e�B�u�t���O���擾.
	 * 
	 * \return true�Ȃ�A�N�e�B�u
	 */
	const bool isActive() const
	{
		return m_isActive;
	}
	/**
	 * \brief �A�j���[�V�������Đ�.
	 * 
	 * \param animNo �A�j���[�V�����N���b�v�̔ԍ�
	 * \param interpolateTime �⊮����
	 */
	void PlayAnimation(const int animNo,const float interpolateTime = 0.0f)
	{
		m_skinModelList[0]->s_animation.Play(animNo, interpolateTime);
	}
	/**
	 * \brief �V���h�E�L���X�^�[�t���O��ݒ�.
	 * 
	 * \param caster �V���h�E�L���X�^�[�t���O
	 */
	void SetShadowCaster(const bool caster)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetShadowCaster(caster);
		}
	}
	/**
	 * \brief �V���h�E���V�[�o�[�t���O��ݒ�.
	 * 
	 * \param receiver �V���h�E���V�[�o�[�t���O
	 */
	void SetShadowReceiver(const bool receiver)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetShadowReceiver(receiver);
		}
	}
	/**
	 * \brief �X�L�����f�����X�V.
	 * 
	 */
	void UpdateMatrix()
	{
		if (m_update) {
			m_skinModelList[0]->s_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
			m_update = false;
		}
	}
	/**
	 * \brief �C���X�^���V���O�`��̑O�ɌĂяo��.
	 * 
	 */
	void BeginUpdateInstancingData()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.BeginUpdateInstancingData();
		}
	}
	/**
	 * \brief �C���X�^���V���O�`��p�̃f�[�^���X�V.
	 * 
	 * \param pos ���W
	 * \param rot ��]
	 * \param scale �傫��
	 * \param animNo �A�j���[�V�����̔ԍ�
	 * \param interpolateTime �A�j���[�V�����̕⊮����
	 */
	void UpdateInstancingData(const CVector3& pos, const CQuaternion& rot, const CVector3& scale = CVector3::One(),const int animNo = 0,const float interpolateTime = 0.0f)
	{
		if (m_animationClip == nullptr) {
			m_skinModelList[0]->s_skinModel.UpdateInstancingData(pos, rot, scale);
		}
		else {
			m_skinModelList[animNo + 1]->s_skinModel.UpdateInstancingData(pos, rot, scale);
		}
	}
	/**
	 * \brief �C���X�^���V���O�`��p�̃f�[�^���X�V.
	 * 
	 * \param worldMatrix ���[���h�s��
	 * \param animNo �A�j���[�V�����̔ԍ�
	 * \param interpolateTime �⊮����
	 */
	void UpdateInstancingData(const CMatrix& worldMatrix,const int animNo = 0,const float interpolateTime = 0.0f)
	{
		if (m_animationClip == nullptr) {
			m_skinModelList[0]->s_skinModel.UpdateInstancingData(worldMatrix);
		}
		else {
			m_skinModelList[animNo + 1]->s_skinModel.UpdateInstancingData(worldMatrix);
		}
	}
	/**
	 * \brief ���[���h�s���ݒ�.
	 * 
	 * \param worldmatrix ���[���h�s��
	 */
	void SetWorldMatrix(const CMatrix& worldmatrix);
	/**
	 * \brief �g�D�[���V�F�[�_�[���I�t.
	 * 
	 */
	void SetOffToonShader()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetOffToonShader();
		}
	}
	/**
	 * \brief ��Z�J���[��ݒ�.
	 * 
	 * \param color ��Z�J���[
	 */
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

