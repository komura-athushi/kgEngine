#pragma once
class CEffektEngine
{
public:
	static CEffektEngine& GetInstance()
	{
		static CEffektEngine instance;
		return instance;
	}
	~CEffektEngine();
private:
	CEffektEngine();
	
public:
	bool GetEffektManeger()
	{
		return  m_effekseerManager;
	}
	//エフェクトの初期化。
	void InitEffekseer();
	//カメラ行列
	void CameraEffekseer(const int cameraNumber = 0);
	//エフェクトのドロー
	void Draw(const int cameraNumber = 0);
	//アップデート
	void Update();


	void SetPosition(Effekseer::Handle effect ,CVector3 pos)
	{
		m_effekseerManager->SetLocation(effect, pos.x , pos.y , pos.z);
	}

	void SetScale(Effekseer::Handle effect, CVector3 scale)
	{
		m_effekseerManager->SetScale(effect, scale.x, scale.y, scale.z);
	}

	void SetRotation(Effekseer::Handle effect, float rotX, float rotY, float rotZ)
	{
		m_effekseerManager->SetRotation(effect, rotX, rotY, rotZ);
	}
	void Stop(Effekseer::Handle handle)
	{
		m_effekseerManager->StopEffect(handle);
	}

	Effekseer::Effect* CreateEffekseerEffect(const wchar_t* filePath);
	Effekseer::Handle Play(Effekseer::Effect* effect);

private:
	Effekseer::Manager* m_effekseerManager;
	EffekseerRenderer::Renderer*	m_effekseerRenderer = nullptr;
	Effekseer::Effect* m_attackEffect = nullptr;
	Effekseer::Handle m_playEffectHandle = -1;
};



