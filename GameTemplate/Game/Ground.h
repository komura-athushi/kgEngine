#pragma once
class Ground:public IGameObject
{
public:
	Ground();
	~Ground();
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	CSkinModelRender* m_skinModelRender = nullptr;
};

