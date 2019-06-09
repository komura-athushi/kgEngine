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

void CSkinModelRender::Update()
{
	//�X�V��true�ł���΃��f���̃A�b�v�f�[�g���s���AInit�֐����Ă΂ꂽ��K�����͍s����
	if (m_update ) {
		m_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
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