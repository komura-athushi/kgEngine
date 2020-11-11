/*****************************************************************//**
 * \file   OffScreen.h
 * \brief  OffScreenクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
#include "Object/ObjectData.h"
#include "graphics/2D/kgFont.h"

/**
 * \brief 右下の小窓に巻き込んだオブジェクトを表示するクラス.
 */
class OffScreen : public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	OffScreen();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~OffScreen() override final;
	/**
	 * \brief 初期化処理.
	 *
	 * \return trueで初期化完了
	 */
	bool Start() override final;
	/**
	 * \brief 描画処理.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief 右下の小窓に表示するモデルを設定.
	 * 
	 * \param skinModel モデル
	 */
	void SetSkinModel(SkinModel* skinModel)
	{
		m_skinModel = skinModel;
	}
	/**
	 * \brief オブジェクトのデータを設定.
	 * 
	 * \param objData オブジェクトデータ
	 */
	void SetObjData(StructObjectData* objData)
	{
		m_objData = objData;
	}
private:
	/**
	 * \brief サンプルステート初期化.
	 * 
	 */
	void InitSamplerState();
	/**
	 * \brief カメラ更新処理.
	 * 
	 */
	void UpdateCamera();
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

