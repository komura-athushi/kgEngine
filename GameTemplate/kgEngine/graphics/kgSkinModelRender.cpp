#include "KGstdafx.h"
#include "kgSkinModelRender.h"
#include "SkinModelDataManager.h"
#include "shadow/kgShadowMap.h"
#include "shadow\CascadeShadowMap.h"
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
		EnFbxUpAxis fbxUpAxis,
		bool isInstancing
		)
{
	m_enFbxUpAxis = fbxUpAxis;
	m_filepath = filePath;
	m_isInstancing = isInstancing;
	m_skinModelList.emplace(0, std::make_unique<AnimModel>());
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
				m_skinModelList.emplace(i, std::make_unique<AnimModel>());
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
	m_skinModelList[0]->s_skinModel.GetSkeleton().Update(m_worldMatrix);

	//スキンモデルにワールド行列を設定
	m_skinModelList[0]->s_skinModel.SetWorldMatrix(m_worldMatrix);
	m_update = false;
}

//ワールド行列を設定
void CSkinModelRender::SetWorldMatrix(const CMatrix& worldmatrix)
{
	m_worldMatrix = worldmatrix;
	//スケルトンの更新。
	m_skinModelList[0]->s_skinModel.GetSkeleton().Update(m_worldMatrix);

	//スキンモデルにワールド行列を設定
	m_skinModelList[0]->s_skinModel.SetWorldMatrix(m_worldMatrix);
	m_update = false;
}

void CSkinModelRender::Update()
{
	
	
}

void CSkinModelRender::Draw()
{
	//更新がtrueであればモデルのアップデートを行う、Init関数が呼ばれたら必ず一回は行われる
	if (m_update) {
		UpdateWorldMatrix();
		m_update = false;
	}
	//アニメーションの再生中であればアニメーションのアップデートを行う
	/*if (m_skinModelList[0].get()->s_animation.IsPlaying()) {
		m_skinModelList[0].get()->s_animation.Update(GameTime().GetFrameDeltaTime());
	}
	//アクティブならモデルをドローする
	if (m_isActive) {
		m_skinModel.Draw();
	}*/
	for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
		//アニメーションの再生中であればアニメーションのアップデートを行う
		if (itr->second->s_skinModel.GetInstanceNumber() != 0 || !itr->second->s_skinModel.GetisInstancing()) {
			if (itr->second->s_animation.IsPlaying()) {
				itr->second->s_animation.Update(GameTime().GetFrameDeltaTime());
			}
			//インスタンシング描画しないならスキンモデルに計算したワールド行列を設定する
			/*if (!itr->second->s_skinModel.GetisInstancing()) {
				itr->second->s_skinModel.SetWorldMatrix(m_worldMatrix);
			}*/
			//アクティブならモデルをドローする
			if (m_isActive) {
				for (int i = 0; i < Engine().GetGraphicsEngine().GetSplitNumber(); i++) {
					if (Engine().GetGraphicsEngine().GetisSplit()) {
						Engine().GetGraphicsEngine().SetSplitViewPort(i);
					}
					else {
						Engine().GetGraphicsEngine().SetNormalViewPort();
					}
					itr->second->s_skinModel.Draw(&MainCamera(i),
						enRenderMode_Normal,
						true,
						i);
				}
			}
		}
	}
}

void CSkinModelRender::PreUpdate()
{
	for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
		if (itr->second->s_skinModel.isShadowCaster()) {
			//Engine().GetGraphicsEngine().GetShadowMap()->RegistShadowCaster(&itr->second->s_skinModel);
			Engine().GetGraphicsEngine().GetCascadeShadowMap()->RegistShadowCaster(&itr->second->s_skinModel);
		}
	}
}