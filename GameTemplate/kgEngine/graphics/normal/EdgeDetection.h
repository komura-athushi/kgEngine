#pragma once

#include "graphics/RenderTarget.h"

//エッジ検出をばっこりしていくぅ～
class EdgeDetection
{
public:
	EdgeDetection();
	~EdgeDetection();

private:
	RenderTarget m_edgeMapRT;								//法線を描画するレンダリングターゲット
};

