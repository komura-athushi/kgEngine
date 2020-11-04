#pragma once
class SkinModelDataManager;
class CSkinModelRender:public IGameObject
{
public:
	CSkinModelRender();
	~CSkinModelRender();
	/*!
	* @brief	初期化。
	*@param[in]	filePath			ロードしたいcmoファイルのファイルパス、L"Resource/"から必要です
	*@param[in]	animationClips		アニメーションクリップの配列の先頭アドレス
	*@param[in]	numAnimClip			アニメーションクリップの数
	*@param[in]	enUpdateAxis		fbxの上方向。
	*@param[in]	isInstancing		インスタンシングするかどうか
	*/
	void Init(const wchar_t* filePath,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ,
		bool isInstancing = false
		);
	//インスタンシング描画のインスタンスの数を初期化
	void SetInstanceNumber(const int& maxInstance);
	//インスタンシング描画のデータを初期化
	void InitInstancing()
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.InitInstancingData();
		}
	}
	//このクラスはこれ以上継承させないからオーバーライドはここで終わり
	bool Start() override final;
	void Update() override final;
	void Draw() override final;
	void PreUpdate() override final;
	//ワールド行列を更新
	void UpdateWorldMatrix();
	//アニメーションの初期化
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips, const wchar_t* filePath = nullptr);
	void InitTexture(TextureData* textureData)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.InitTexture(textureData);
		}
	}
public:
	//スキンモデルを取得
	SkinModel& GetSkinModel()
	{
		return m_skinModelList[0]->s_skinModel;
	}
	//座標を設定
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
		m_update = true;
	}
	//座標を取得
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//大きさを設定
	void SetScale(const CVector3 scale)
	{
		m_scale = scale;
		m_update = true;
	}
	//大きさを取得
	CVector3 GetScale() const
	{
		return m_scale;
	}
	//回転を設定
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
		m_update = true;
	}
	//回転を取得
	CQuaternion GetRotation() const
	{
		return m_rotation;
	}
	//アクティブフラグを設定
	void SetisActive(const bool& flag)
	{
		m_isActive = flag;
	}
	//アクティブかどうかを取得
	bool isActive() const
	{
		return m_isActive;
	}
	/*!
	*@brief	アニメーションを再生。
	*@param[in]	clipNo	アニメーションクリップの番号。コンストラクタに渡したanimClipListの並びとなる。
	*@param[in]	interpolateTime		補完時間(単位：秒)
	*/
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_skinModelList[0]->s_animation.Play(animNo, interpolateTime);
	}
	//アニメーションが再生中かどうかを取得
	/*bool IsPlayingAnimation() const
	{
		return m_skinModelList[0]->s_animation.IsPlaying();
	}*/
	//シャドウキャスターを設定
	void SetShadowCaster(bool caster)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetShadowCaster(caster);
		}
	}
	//シャドウレシーバーを設定
	void SetShadowReceiver(bool receiver)
	{
		for (auto itr = m_skinModelList.begin(); itr != m_skinModelList.end(); ++itr) {
			itr->second->s_skinModel.SetShadowReceiver(receiver);
		}
	}
	//スキンモデルの色々を更新
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
	/// iインスタンシング描画のデータを更新します
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大</param>
	void UpdateInstancingData(const CVector3& pos, const CQuaternion& rot, const CVector3& scale = CVector3::One(),int animNo = 0, float interpolateTime = 0.0f)
	{
		if (m_animationClip == nullptr) {
			m_skinModelList[0]->s_skinModel.UpdateInstancingData(pos, rot, scale);
		}
		else {
			m_skinModelList[animNo + 1]->s_skinModel.UpdateInstancingData(pos, rot, scale);
		}
	}
	//インスタンシング用のデータを更新、ワールド行列を直接設定する用
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
	/// 乗算カラーを設定
	/// </summary>
	/// <param name="color"></param>
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

