#include "KGstdafx.h"
#include "SkinModelRender.h"
#include "SkinModelDataManager.h"
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
	if (m_update || m_animationClip != nullptr) {
		m_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		m_update = false;
	}
	if (m_animationClip != nullptr) {
		m_animation.Update(GameTime().GetFrameDeltaTime());
	}
	if (m_isActive) {
		m_skinModel.Draw();
	}
	
}