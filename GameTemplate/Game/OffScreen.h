#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
const float FRAME_H = 200.0f;
const float FRAME_W = 200.0f;
class OffScreen : public IGameObject
{
public:
	OffScreen();
	~OffScreen();
	bool Start() override;
	void Draw() override;
	void Model();
	void InitSamplerState();
private:
	RenderTarget m_offRenderTarget;
	OffScreenCamera m_offScreenCamera;
	SkinModel m_skinModel;
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rot = CQuaternion::Identity();
	Shader m_vs;
	Shader m_ps;
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	PostEffect m_postEffect;
	ID3D11RasterizerState* m_rasterizerState = NULL;	//ラスタライザステート。
};

