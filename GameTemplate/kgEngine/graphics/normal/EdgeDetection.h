#pragma once

#include "graphics/RenderTarget.h"
#include "graphics/Shader.h"
#include "graphics/PostEffect.h"

class PostEffect;
class NormalMap;
//エッジ検出をばっこりしていくぅ〜
class EdgeDetection
{
public:
	EdgeDetection();
	~EdgeDetection();
	//エッジ検出
	void EdgeRender(PostEffect& postEffect);
	//合成していく
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

