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
	//3dsMaxと軸を合わせるためのバイアス。
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation(m_position);
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	//拡大行列を作成する。
	scaleMatrix.MakeScaling(m_scale);
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算するように！
	//順番を間違えたら結果が変わるよ。
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//スケルトンの更新。
	m_skinModel.GetSkeleton().Update(m_worldMatrix);

	//スキンモデルにワールド行列を設定
	m_skinModel.SetWorldMatrix(m_worldMatrix);
}

//ワールド行列を設定
void CSkinModelRender::SetWorldMatrix(const CMatrix& worldmatrix)
{
	m_worldMatrix = worldmatrix;
	//スケルトンの更新。
	m_skinModel.GetSkeleton().Update(m_worldMatrix);

	//スキンモデルにワールド行列を設定
	m_skinModel.SetWorldMatrix(m_worldMatrix);
	m_update = false;
}

void CSkinModelRender::Update()
{
	//更新がtrueであればモデルのアップデートを行う、Init関数が呼ばれたら必ず一回は行われる
	if (m_update ) {
		UpdateWorldMatrix();
		m_update = false;
	}
	//アニメーションの再生中であればアニメーションのアップデートを行う
	if (m_animation.IsPlaying()) {
		m_animation.Update(GameTime().GetFrameDeltaTime());
	}
	//アクティブならモデルをドローする
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