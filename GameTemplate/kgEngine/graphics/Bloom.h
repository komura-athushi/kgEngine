#pragma once
#include "RenderTarget.h"
#include "Shader.h"
#include "GaussianBlur.h"

class PostEffect;
class Bloom
{
public:
	//コンストラクタ
	Bloom();
	//デストラクタ
	~Bloom();
	//ドロー
	void Draw(PostEffect& postEffect);
private:
	//アルファブレンディングステートを初期化する
	void InitAlphaBlendState();
	//シェーダーを初期化
	void InitShader();
	//レンダリングターゲットの初期化
	void InitRenderTarget();
	//サンプラステートの初期化
	void InitSamplerState();
private:
	static const int NUM_DOWN_SAMPLE = 4;				//ダウンサンプリングの回数。
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	ID3D11BlendState* m_disableBlendState = nullptr;	//アルファブレンディングを無効にするブレンディングステート。
	ID3D11BlendState* m_finalBlendState = nullptr;		//最終合成用のブレンディングステート。
	RenderTarget m_luminanceRT;			//輝度を抽出するレンダリングターゲット
	RenderTarget m_blurCombineRT;		//ブラーの画像を合成するレンダリングターゲット。
	Shader m_vs;						//何もしない頂点シェーダー。
	Shader m_psLuminance;				//輝度抽出用のピクセルシェーダー。
	Shader m_psCombine;					//ぼかし画像合成用のピクセルシェーダー。
	Shader m_psFinal;					//最終合成用のピクセルシェーダー。
	GaussianBlur m_gausianBlur[NUM_DOWN_SAMPLE];	//輝度をぼかすためのガウシアンブラー。
													//1/2→1/4→1/8→1/16の解像度までダウンサンプリングされていきます。
	ID3D11RasterizerState* m_rasterizerState = NULL;	//ラスタライザステート。
};

