#include "KGstdafx.h"
#include "NormalMap.h"


NormalMap::NormalMap()
{
	//シャドウマップ生成用のレンダリングターゲットを作成。
//解像度は2048×2048。
//テクスチャのフォーマットはR成分のみの32bit浮動小数点型。
	m_normalMapRT.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);
}

NormalMap::~NormalMap()
{

}

void NormalMap::RenderNormalMap(Camera* camera, SkinModel* skinModel)
{

	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//レンダリングターゲットを切り替える。
	ID3D11RenderTargetView* rts[] = {
		m_normalMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_normalMapRT.GetDepthStensilView());
	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_normalMapRT.GetViewport());
	//シャドウマップをクリア。
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_normalMapRT.ClearRenderTarget(clearColor);

	if (skinModel != nullptr) {
		skinModel->Draw(
			camera->GetViewMatrix(),
			camera->GetProjectionMatrix(),
			enRenderMode_NormalMap
		);
	}
	else if (m_modelList.size() >= 1) {
		//シャドウキャスターをシャドウマップにレンダリング。
		for (auto& caster : m_modelList) {
			caster->Draw(
				MainCamera().GetViewMatrix(),
				MainCamera().GetProjectionMatrix(),
				enRenderMode_NormalMap
			);
		}
	}
	//キャスターをクリア。
	m_modelList.clear();
}