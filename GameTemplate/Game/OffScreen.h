#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"

class OffScreen : public IGameObject
{
public:
	OffScreen();
	~OffScreen();
	bool Start() override;
	void Draw() override;
	void SetSkinModel(SkinModel* skinModel)
	{
		m_skinModel = skinModel;
	}
	void InitSamplerState();
private:
	RenderTarget m_offRenderTarget;
	OffScreenCamera m_offScreenCamera;
	SkinModel* m_skinModel = nullptr;
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rot = CQuaternion::Identity();
	Shader m_vs;
	Shader m_ps;
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	PostEffect m_postEffect;
	ID3D11RasterizerState* m_rasterizerState = NULL;	//ラスタライザステート。
	float m_degree = 0.0f;
};

