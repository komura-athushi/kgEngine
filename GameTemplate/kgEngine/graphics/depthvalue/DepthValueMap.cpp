#include "KGstdafx.h"
#include "DepthValueMap.h"


DepthValueMap::DepthValueMap()
{
	//シャドウマップ生成用のレンダリングターゲットを作成。
//解像度は2048×2048。
//テクスチャのフォーマットはR成分のみの32bit浮動小数点型。
	m_depthValueMapRT.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R32_FLOAT
	);
}

DepthValueMap::~DepthValueMap()
{

}

void  DepthValueMap::RenderDepthValueMap()
{

	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//レンダリングターゲットを切り替える。
	ID3D11RenderTargetView* rts[] = {
		m_depthValueMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_depthValueMapRT.GetDepthStensilView());
	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_depthValueMapRT.GetViewport());
	//シャドウマップをクリア。
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_depthValueMapRT.ClearRenderTarget(clearColor);

	if (m_modelList.size() >= 1) {
		//シャドウキャスターをシャドウマップにレンダリング。
		for (auto& caster : m_modelList) {
			caster->Draw(
				MainCamera().GetViewMatrix(),
				MainCamera().GetProjectionMatrix(),
				enRenderMode_DepthValueMap
			);
		}
	}
	//キャスターをクリア。
	m_modelList.clear();
}