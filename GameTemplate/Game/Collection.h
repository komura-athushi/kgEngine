#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
#include "Object/ObjectData.h"

class Collection : public IGameObject
{
public:
	Collection();
	~Collection();
	bool Start() override;
	void Draw() override;
	
	void InitSamplerState();
	void OffScreenRender();
private:
	const static int W_NUMBER = 4;
	const static int H_NUMBER = 4;
	RenderTarget m_offRenderTarget;
	OffScreenCamera m_offScreenCamera;
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rot = CQuaternion::Identity();
	Shader m_vs;
	Shader m_ps;
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	PostEffect m_postEffect[W_NUMBER * H_NUMBER];
	ID3D11RasterizerState* m_rasterizerState = NULL;	//ラスタライザステート。
	std::map<int, SkinModel_ObjData*> m_modelList;
	float m_degree = 0.0f;
	int m_page = 1;
	int m_listSize = 0;
	CSprite m_sprite;
	CSprite m_haikei;
	CSprite m_collectionCursor;
	int m_cursorNumber = 1;
	PostEffect m_mainPostEffect;
	int m_maximumPage = 1;
	int m_mimmumPage = 1;
	int m_finalPageNumber = 0;
	CFont m_font;
	int m_hitNumber = 0;
};

