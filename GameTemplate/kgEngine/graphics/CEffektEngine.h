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
	//�G�t�F�N�g�̏������B
	void InitEffekseer();
	//�J�����s��
	void CameraEffekseer(const int cameraNumber = 0);
	//�G�t�F�N�g�̃h���[
	void Draw(const int cameraNumber = 0);
	//�A�b�v�f�[�g
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



