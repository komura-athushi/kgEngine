/*****************************************************************//**
 * \file   Collection.h
 * \brief  コレクション
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
#include "Object/ObjectData.h"

class Fade;
/**
 * \brief 図鑑を表示するクラス。
 */
class Collection : public IGameObject
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Collection();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Collection() override final;
	/**
	 * \brief 更新前の初期化処理.
	 * 
	 * \return trueで初期化が終わった.
	 */
	bool Start() override final;
	/**
	 * \brief 更新処理.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief 描画処理.
	 * 
	 */
	void Draw() override final;
private:
	/**
	 * \brief サンプルステート初期化.
	 * 
	 */
	void InitSamplerState();
	/**
	 * \brief 各種モデルの描画処理.
	 * 
	 */
	void OffScreenRender();
	/**
	 * \brief カーソルの更新処理.
	 * 
	 */
	void UpdateCursor();
	/**
	 * \brief カメラの更新処理.
	 * 
	 * \param skinModel　スキンモデル
	 */
	void UpdateCamera(SkinModel_ObjData* data);
private:
	const static int W_NUMBER = 4;						//横幅
	const static int H_NUMBER = 4;						//縦幅
	RenderTarget m_offRenderTarget;						//オフスクリーンのレンダーターゲット
	OffScreenCamera m_offScreenCamera;					//オフスクリーン用のカメラ
	CVector3 m_position = CVector3::Zero();				//モデルの座標
	CVector3 m_scale = CVector3::One();					//大きさ
	CQuaternion m_rot = CQuaternion::Identity();		//回転
	Shader m_vs;										//何もしないシェーダー
	Shader m_ps;										//何もしないシェーダー
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	PostEffect m_postEffect[W_NUMBER * H_NUMBER];		//ポストエフェクト、1ページのモデル分
	ID3D11RasterizerState* m_rasterizerState = NULL;	//ラスタライザステート。
	const std::map<float, SkinModel_ObjDataPtr>* m_modelList = nullptr;	//モデルのマップ
	float m_degree = 0.0f;								//モデルの回転の角度
	int m_page = 1;										//何ページ目か
	int m_listSize = 0;									//モデルの数
	CSprite m_sprite;									//画像
	CSprite m_haikei;									//画像
	CSprite m_collectionCursor;							//カーソル
	int m_cursorNumber = 1;								//カーソルが何番目か
	PostEffect m_mainPostEffect;						//左の大きいやつ用のポストエフェクト
	int m_maximumPage = 1;								//ページの最大数
	int m_mimmumPage = 1;								//ページの最小数
	int m_finalPageNumber = 0;							//最後のページ
	CFont m_font;										//文字
	int m_hitNumber = 0;								//巻き込んだものの数
	SkinModel m_model;									//左端用のモデル
	bool m_isWaitFadeout = false;						
	Fade* m_fade = nullptr;				
};

