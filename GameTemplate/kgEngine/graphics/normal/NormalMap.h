#pragma once

#include "graphics/RenderTarget.h"

class NormalMap
{
public:
	NormalMap();
	~NormalMap();
	//シャドウキャスターを追加
	void RegistSkinModel(SkinModel* model)
	{
		m_modelList.push_back(model);
	}
	//シャドウマップのSRVを取得
	ID3D11ShaderResourceView* GetNormalMapSRV()
	{
		return m_normalMapRT.GetRenderTargetSRV();
	}
	void RenderNormalMap();
private:
	RenderTarget m_normalMapRT;								//法線を描画するレンダリングターゲット
	std::vector<SkinModel*> m_modelList;				//モデルの配列

};

