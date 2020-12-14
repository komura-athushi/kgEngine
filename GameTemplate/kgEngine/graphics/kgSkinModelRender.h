/*****************************************************************//**
 * \file   kgSkinModelRender.h
 * \brief  スキンモデルレンダー
 * 
 * \author akuro
 * \date   November 2020
 *********************************************************************/
#pragma once
class SkinModelDataManager;
/**
 * \brief モデルを描画する.
 */
class CSkinModelRender:public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	CSkinModelRender();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~CSkinModelRender();
	/**
	 * \brief 初期化処理.
	 * 
	 * \param filePath ロードするファイルパス
	 * \param animationClips アニメーションクリップの配列の先頭のポインタ
	 * \param numAnimationClips アニメーションクリップの数
	 * \param fbxUpAxis fbxの上方向
	 * \param isInstancing インスタンシング描画をするかどうか
	 */
	void Init(const wchar_t* filePath,
		AnimationClip* animationClips = nullptr,
		const int numAnimationClips = 0,
		EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ,
		bool isInstancing = false
		);
	/**
	 * \brief インスタンシング描画の際の最大描画数を設定.
	 * 
	 * \param maxInstance 最大描画数
	 */
	void SetInstanceNumber(const int maxInstance);
	/**
	 * \brief インスタンシング描画のためのデータを初期化.
	 * 
	 */
	void InitInstancing()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.InitInstancingData();
		}
	}
	//このクラスはこれ以上継承させないからオーバーライドはここで終わり
	/**
	 * \brief 初期化処理.
	 * 
	 * \return trueで初期化完了
	 */
	bool Start() override final;
	/**
	 * \brief 更新処理.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief 描画処理.
	 * 
	 */
	void Draw() override final;
	/**
	 * \brief 描画前の処理.
	 * 
	 */
	void PreUpdate() override final;
	/**
	 * \brief ワールド行列を計算.
	 * 
	 */
	void UpdateWorldMatrix();
	/**
	 * \brief アニメーションの初期化.
	 * 
	 * \param animationClips アニメーションクリップの配列の先頭のポインタ
	 * \param numAnimationClips アニメーションクリップの数
	 * \param filePath モデルのファイルパス
	 */
	void InitAnimation(AnimationClip* animationClips,const int numAnimationClips, const wchar_t* filePath = nullptr);
	/**
	 * \brief ノーマルマップなどのテクスチャーデータをロード.
	 * 
	 * \param textureData テクスチャーデータ
	 */
	void InitTexture(TextureData* textureData)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.InitTexture(textureData);
		}
	}
public:
	/**
	 * \brief スキンモデルを取得.
	 * 
	 * \return スキンモデル
	 */
	SkinModel& GetSkinModel()
	{
		return m_skinModelList[0]->s_skinModel;
	}
	/**
	 * \brief 座標を設定.
	 * 
	 * \param pos 座標
	 */
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
		m_update = true;
	}
	/**
	 * \brief 座標を取得.
	 *  
	 * \return 座標 
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/**
	 * \brief 大きさを設定.
	 * 
	 * \param scale 大きさ
	 */
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
		m_update = true;
	}
	/**
	 * \brief 大きさを取得.
	 * 
	 * \return 大きさ
	 */
	const CVector3& GetScale() const
	{
		return m_scale;
	}
	/**
	 * \brief 回転を設定.
	 * 
	 * \param rot 回転
	 */
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
		m_update = true;
	}
	/**
	 * \brief 回転を取得.
	 * 
	 * \return 回転
	 */
	const CQuaternion& GetRotation() const
	{
		return m_rotation;
	}
	/**
	 * \brief アクティブフラグを設定.
	 * 
	 * \param flag アクティブフラグ
	 */
	void SetisActive(const bool& flag)
	{
		m_isActive = flag;
	}
	/**
	 * \brief アクティブフラグを取得.
	 * 
	 * \return trueならアクティブ
	 */
	const bool isActive() const
	{
		return m_isActive;
	}
	/**
	 * \brief アニメーションを再生.
	 * 
	 * \param animNo アニメーションクリップの番号
	 * \param interpolateTime 補完時間
	 */
	void PlayAnimation(const int animNo,const float interpolateTime = 0.0f)
	{
		m_skinModelList[0]->s_animation.Play(animNo, interpolateTime);
	}
	/**
	 * \brief シャドウキャスターフラグを設定.
	 * 
	 * \param caster シャドウキャスターフラグ
	 */
	void SetShadowCaster(const bool caster)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetShadowCaster(caster);
		}
	}
	/**
	 * \brief シャドウレシーバーフラグを設定.
	 * 
	 * \param receiver シャドウレシーバーフラグ
	 */
	void SetShadowReceiver(const bool receiver)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetShadowReceiver(receiver);
		}
	}
	/**
	 * \brief スキンモデルを更新.
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
	 * \brief インスタンシング描画の前に呼び出す.
	 * 
	 */
	void BeginUpdateInstancingData()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.BeginUpdateInstancingData();
		}
	}
	/**
	 * \brief インスタンシング描画用のデータを更新.
	 * 
	 * \param pos 座標
	 * \param rot 回転
	 * \param scale 大きさ
	 * \param animNo アニメーションの番号
	 * \param interpolateTime アニメーションの補完時間
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
	 * \brief インスタンシング描画用のデータを更新.
	 * 
	 * \param worldMatrix ワールド行列
	 * \param animNo アニメーションの番号
	 * \param interpolateTime 補完時間
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
	 * \brief ワールド行列を設定.
	 * 
	 * \param worldmatrix ワールド行列
	 */
	void SetWorldMatrix(const CMatrix& worldmatrix);
	/**
	 * \brief トゥーンシェーダーをオフ.
	 * 
	 */
	void SetOffToonShader()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetOffToonShader();
		}
	}
	/**
	 * \brief 乗算カラーを設定.
	 * 
	 * \param color 乗算カラー
	 */
	void SetColor(const CVector4& color)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetColor(color);
		}
	}
private:
	//SkinModel m_skinModel;										//スキンモデル
	AnimationClip* m_animationClip;								//アニメーションクリップの数
	int	m_numAnimationClips = 0;								//!<アニメーションクリップの数。
	//Animation m_animation;										//アニメーション
	EnFbxUpAxis	m_enFbxUpAxis = enFbxUpAxisZ;					//!<FBXの上方向。
	CVector3 m_position = CVector3::Zero();						//座標
	CVector3 m_scale = CVector3::One();							//大きさ
	CQuaternion m_rotation = CQuaternion::Identity();			//回転
	CMatrix m_worldMatrix;										//ワールド行列
	bool m_isActive = false;									//アクティブかどうか
	bool m_update = false;										//座標とか大きさが更新されたかどうか、初回は必ずtrueにして処理させる
	bool m_isInitAnimation = false;								//アニメーションを初期化したかどうか
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

