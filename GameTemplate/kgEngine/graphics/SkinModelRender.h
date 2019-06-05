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
	*/
	void Init(const wchar_t* filePath,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ
		);
	//このクラスはこれ以上継承させないからオーバーライドはここで終わり
	bool Start() override final;
	void Update() override final;
	void PreUpdate() override final;
private:
	//アニメーションの初期化
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips);
public:
	//スキンモデルを取得
	SkinModel& GetSkinModel()
	{
		return m_skinModel;
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
	* @brief	アニメーションを再生。
	*@param[in]	clipNo	アニメーションクリップの番号。コンストラクタに渡したanimClipListの並びとなる。
	*@param[in]	interpolateTime		補完時間(単位：秒)
	*/
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_animation.Play(animNo, interpolateTime);
	}
	//アニメーションが再生中かどうかを取得
	bool IsPlayingAnimation() const
	{
		return m_animation.IsPlaying();
	}
	//シャドウキャスターを設定
	void SetShadowCaster(bool caster)
	{
		m_skinModel.SetShadowCaster(caster);
	}
	//シャドウレシーバーを設定
	void SetShadowReceiver(bool receiver)
	{
		m_skinModel.SetShadowReceiver(receiver);
	}
private:
	SkinModel m_skinModel;										//スキンモデル
	AnimationClip* m_animationClip;								//アニメーションクリップの数
	int	m_numAnimationClips = 0;								//!<アニメーションクリップの数。
	Animation m_animation;										//アニメーション
	EnFbxUpAxis	m_enFbxUpAxis = enFbxUpAxisZ;					//!<FBXの上方向。
	CVector3 m_position = CVector3::Zero();						//座標
	CVector3 m_scale = CVector3::One();							//大きさ
	CQuaternion m_rotation = CQuaternion::Identity();			//回転
	bool m_isActive = false;									//アクティブかどうか
	bool m_update = false;										//座標とか大きさが更新されたかどうか、初回は必ずtrueにして処理させる
};

