#pragma once

class DepthValueMap
{
public:
	DepthValueMap();
	~DepthValueMap();
	//シャドウキャスターを追加
	void RegistSkinModel(SkinModel* model)
	{
		m_modelList.push_back(model);
	}
	//シャドウマップのSRVを取得
	ID3D11ShaderResourceView* GetDepthValueMapSRV()
	{
		return m_depthValueMapRT.GetRenderTargetSRV();
	}
	void RenderDepthValueMap();
	//レンダーターゲットを取得
	RenderTarget* GetRenderTarget()
	{
		return &m_depthValueMapRT;
	}
private:
	RenderTarget m_depthValueMapRT;								//法線を描画するレンダリングターゲット
	std::vector<SkinModel*> m_modelList;				//モデルの配列
};

