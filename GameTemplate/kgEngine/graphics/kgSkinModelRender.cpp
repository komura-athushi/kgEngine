#include "KGstdafx.h"
#include "kgSkinModelRender.h"
#include "SkinModelDataManager.h"
#include "shadow/kgShadowMap.h"
CSkinModelRender::CSkinModelRender()
{

}

CSkinModelRender::~CSkinModelRender()
{
	for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
		delete itr->second;
	}
}

bool CSkinModelRender::Start()
{
	return true;
}

void CSkinModelRender::Init(	
		const wchar_t* filePath,
		AnimationClip* animationClip,
		int numAnimationClips,
		EnFbxUpAxis fbxUpAxis,
		bool isInstancing
		)
{
	m_enFbxUpAxis = fbxUpAxis;
	m_filepath = filePath;
	m_isInstancing = isInstancing;
	m_skinModelList.emplace(0, new AnimModel());
	m_skinModelList[0]->s_skinModel.Init(filePath, fbxUpAxis);
	InitAnimation(animationClip, numAnimationClips);
	m_isActive = true;
	m_update = true;
}

void CSkinModelRender::SetInstanceNumber(const int& maxInstance)
{
	m_maxInstance = maxInstance;
	for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
		itr->second->s_skinModel.SetInstancingNumber(maxInstance);
	}
	m_isSetInstance = true;
}

void CSkinModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, const wchar_t* filePath)
{
	if (m_isInitAnimation) {
		return;
	}
	m_animationClip = animationClips;
	m_numAnimationClips = numAnimationClips;
	if (m_animationClip != nullptr) {
		if (m_isInstancing) {
			for (int i = 1; i <= m_numAnimationClips; i++) {
				m_skinModelList.emplace(i, new AnimModel());
				m_skinModelList[i]->s_skinModel.Init(filePath, m_enFbxUpAxis);
				m_skinModelList[i]->s_animation.Init(m_skinModelList[i]->s_skinModel, m_animationClip, m_numAnimationClips);
				m_skinModelList[i]->s_animation.Play(i - 1, 0.0f);
				m_skinModelList[i]->s_skinModel.SetInstancingNumber(m_maxInstance);
			}
		}
		else {
			m_skinModelList[0]->s_animation.Init(m_skinModelList[0]->s_skinModel, m_animationClip, m_numAnimationClips);
		}
		m_isInitAnimation = true;
	}
}

void CSkinModelRender::UpdateWorldMatrix()
{
	//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//���s�ړ��s����쐬����B
	transMatrix.MakeTranslation(m_position);
	//��]�s����쐬����B
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	//�g��s����쐬����B
	scaleMatrix.MakeScaling(m_scale);
	//���[���h�s����쐬����B
	//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
	//���Ԃ��ԈႦ���猋�ʂ��ς���B
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//�X�P���g���̍X�V�B
	m_skinModelList[0]->s_skinModel.GetSkeleton().Update(m_worldMatrix);

	//�X�L�����f���Ƀ��[���h�s���ݒ�
	m_skinModelList[0]->s_skinModel.SetWorldMatrix(m_worldMatrix);
}

//���[���h�s���ݒ�
void CSkinModelRender::SetWorldMatrix(const CMatrix& worldmatrix)
{
	m_worldMatrix = worldmatrix;
	//�X�P���g���̍X�V�B
	m_skinModelList[0]->s_skinModel.GetSkeleton().Update(m_worldMatrix);

	//�X�L�����f���Ƀ��[���h�s���ݒ�
	m_skinModelList[0]->s_skinModel.SetWorldMatrix(m_worldMatrix);
	m_update = false;
}

void CSkinModelRender::Update()
{
	
	
}

void CSkinModelRender::Draw()
{
	//�X�V��true�ł���΃��f���̃A�b�v�f�[�g���s���AInit�֐����Ă΂ꂽ��K�����͍s����
	if (m_update) {
		UpdateWorldMatrix();
		m_update = false;
	}
	//�A�j���[�V�����̍Đ����ł���΃A�j���[�V�����̃A�b�v�f�[�g���s��
	/*if (m_skinModelList[0].get()->s_animation.IsPlaying()) {
		m_skinModelList[0].get()->s_animation.Update(GameTime().GetFrameDeltaTime());
	}
	//�A�N�e�B�u�Ȃ烂�f�����h���[����
	if (m_isActive) {
		m_skinModel.Draw();
	}*/
	for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
		//�A�j���[�V�����̍Đ����ł���΃A�j���[�V�����̃A�b�v�f�[�g���s��
		if (itr->second->s_skinModel.GetInstanceNumber() != 0 || !itr->second->s_skinModel.GetisInstancing()) {
			if (itr->second->s_animation.IsPlaying()) {
				itr->second->s_animation.Update(GameTime().GetFrameDeltaTime());
			}
			//�A�N�e�B�u�Ȃ烂�f�����h���[����
			if (m_isActive) {
				itr->second->s_skinModel.Draw();
			}
		}
	}
}

void CSkinModelRender::PreUpdate()
{
	for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
		if (itr->second->s_skinModel.isShadowCaster()) {
			Engine().GetGraphicsEngine().GetShadowMap()->RegistShadowCaster(&itr->second->s_skinModel);
		}
	}
}