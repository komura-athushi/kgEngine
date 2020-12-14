/*****************************************************************//**
 * \file   EdgeDetection.h
 * \brief  EdgeDitectionクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

#include "graphics/RenderTarget.h"
#include "graphics/Shader.h"
#include "graphics/PostEffect.h"

class PostEffect;
class NormalMap;
/**
 * \brief エッジ検出とエッジ合成をするクラス.
 */
class EdgeDetection
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	EdgeDetection();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~EdgeDetection();
	/**
	 * \brief エッジ検出.
	 * 
	 * \param postEffect ポストエフェクト
	 */
	void EdgeRender(PostEffect& postEffect);
	/**
	 * \brief エッジ合成.
	 * 
	 * \param postEffect ポストエフェクト
	 * \param renderTarget 合成先のレンダリングターゲット
	 */
	void Draw(PostEffect& postEffect ,RenderTarget* renderTarget = nullptr);
	//ガウスフィルターの初期化だけど今は必要ないね
	void InitGaussian(NormalMap* normalMap);
private:
	RenderTarget m_edgeMapRT;							//法線を描画するレンダリングターゲット
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	Shader m_vsShader;									//頂点検出用の頂点シェーダー	
	Shader m_psShader;									//頂点検出用のピクセルシェーダー
	Shader m_vs;										//何もしない頂点シェーダー。
	Shader m_psFinal;									//最終合成用のピクセルシェーダー。
	ID3D11BlendState* m_finalBlendState = nullptr;		//最終合成用のブレンディングステート。
	ID3D11BlendState* m_disableBlendState = nullptr;	//アルファブレンディングを無効にするブレンディングステート。
	GaussianBlur m_gaussianBlur;						//ガウスフィルター、今は使ってない
};

