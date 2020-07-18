#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
#include "Object/ObjectData.h"
#include "graphics/2D/kgFont.h"

class OffScreen : public IGameObject
{
public:
	OffScreen();
	~OffScreen();
	bool Start() override;
	void PostRender() override;
	//小窓に表示するようのモデルをセット
	void SetSkinModel(SkinModel* skinModel)
	{
		m_skinModel = skinModel;
	}
	//オブジェクトのデータをセット
	void SetObjData(StructObjectData* objData)
	{
		m_objData = objData;
	}
private:
	//サンプルステート初期化
	void InitSamplerState();
private:
	RenderTarget m_offRenderTarget;						//小窓のレンダーターゲット
	OffScreenCamera m_offScreenCamera;					//小窓用のカメラ
	SkinModel* m_skinModel = nullptr;					//小窓に表示する用のモデル
	CVector3 m_position = CVector3::Zero();				//座標
	CVector3 m_scale = CVector3::One();					//大きさ
	CQuaternion m_rot = CQuaternion::Identity();		//回転
	Shader m_vs;										//何もない頂点シェーダー
	Shader m_ps;										//何もない頂点シェーダー
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	PostEffect m_postEffect;							//ポストエフェクト
	ID3D11RasterizerState* m_rasterizerState = NULL;	//ラスタライザステート。
	float m_degree = 0.0f;								//回転の角度
	StructObjectData* m_objData = nullptr;				//オブジェクトデータ
	CSprite m_sprite;
	CFont m_font;																				//文字
};

