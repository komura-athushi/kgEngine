#pragma once

#include "graphics/RenderTarget.h"
#include "graphics/Shader.h"
#include "graphics/PostEffect.h"

class PostEffect;
//エッジ検出をばっこりしていくぅ～
class EdgeDetection
{
public:
	EdgeDetection();
	~EdgeDetection();
	void EdgeRender(PostEffect& postEffect);
	
private:
	RenderTarget m_edgeMapRT;								//法線を描画するレンダリングターゲット
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	Shader m_vsShader;
	Shader m_psShader;

};

