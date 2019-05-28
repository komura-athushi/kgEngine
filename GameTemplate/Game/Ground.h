#pragma once
class Ground:public IGameObject
{
public:
	Ground();
	~Ground();
	bool Start();
	void Update();
private:
	CSkinModelRender* m_skinModelRender = nullptr;
};

