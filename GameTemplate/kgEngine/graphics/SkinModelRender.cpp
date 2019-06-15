#include "KGstdafx.h"
#include "SkinModelRender.h"
#include "SkinModelDataManager.h"
#include "shadow/ShadowMap.h"
CSkinModelRender::CSkinModelRender()
{

}

CSkinModelRender::~CSkinModelRender()
{

}

bool CSkinModelRender::Start()
{
	return true;
}

void CSkinModelRender::Init(	
		const wchar_t* filePath,
		AnimationClip* animationClip,
		int numAnimationClips,
		EnFbxUpAxis fbxUpAxis
		)
{
	m_enFbxUpAxis = fbxUpAxis;
	m_skinModel.Init(filePath, fbxUpAxis);
	InitAnimation(animationClip, numAnimationClips);
	m_isActive = true;
	m_update = true;
}

void CSkinModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips)
{
	m_animationClip = animationClips;
	m_numAnimationClips = numAnimationClips;
	if (m_animationClip != nullptr) {
		m_animation.Init(m_skinModel, m_animationClip, m_numAnimationClips);
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
	m_skinModel.GetSkeleton().Update(m_worldMatrix);

	//�X�L�����f���Ƀ��[���h�s���ݒ�
	m_skinModel.SetWorldMatrix(m_worldMatrix);
}

//���[���h�s���ݒ�
void CSkinModelRender::SetWorldMatrix(const CMatrix& worldmatrix)
{
	m_worldMatrix = worldmatrix;
	//�X�P���g���̍X�V�B
	m_skinModel.GetSkeleton().Update(m_worldMatrix);

	//�X�L�����f���Ƀ��[���h�s���ݒ�
	m_skinModel.SetWorldMatrix(m_worldMatrix);
	m_update = false;
}

void CSkinModelRender::Update()
{
	//�X�V��true�ł���΃��f���̃A�b�v�f�[�g���s���AInit�֐����Ă΂ꂽ��K�����͍s����
	if (m_update ) {
		UpdateWorldMatrix();
		m_update = false;
	}
	//�A�j���[�V�����̍Đ����ł���΃A�j���[�V�����̃A�b�v�f�[�g���s��
	if (m_animation.IsPlaying()) {
		m_animation.Update(GameTime().GetFrameDeltaTime());
	}
	//�A�N�e�B�u�Ȃ烂�f�����h���[����
	if (m_isActive) {
		m_skinModel.Draw();
	}
	
}

void CSkinModelRender::PreUpdate()
{
	if (m_skinModel.isShadowCaster()) {
		Engine().GetGraphicsEngine().GetShadowMap()->RegistShadowCaster(&m_skinModel);
	}
}