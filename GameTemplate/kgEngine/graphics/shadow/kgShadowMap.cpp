#include "KGstdafx.h"
#include "kgShadowMap.h"

ShadowMap::ShadowMap()
{

	//シャドウマップ生成用のレンダリングターゲットを作成。
	//解像度は2048×2048。
	//テクスチャのフォーマットはR成分のみの32bit浮動小数点型。
	for (int i = 0; i < m_splitMaximumNumber; i++) {
		m_shadowMapRT[i].Create(
			4096,
			4096,
			DXGI_FORMAT_R32_FLOAT
		);
	}
}

ShadowMap::~ShadowMap()
{

}

void ShadowMap::UpdateFromLightDirection(const CVector3& lightCameraPos,const CVector3& lightDir, const int number)
{
	//ライトの方向によって、ライトカメラの上方向を決める。
	CVector3 lightCameraUpAxis;
	if (fabsf(lightDir.y) > 0.99998f) {
		//ほぼ真上or真下を向いているので、1,0,0を上方向とする。
		lightCameraUpAxis = CVector3::AxisX();
	}
	else {
		//真上を向いていないときは、0,1,0を上方向とする。
		lightCameraUpAxis = CVector3::AxisY();
	}
	//カメラの上方向が決まったので、ライトビュー行列を計算する。
	m_lightViewMatrix[number].MakeLookAt(
		m_lightCameraPosition[number],
		m_lightCameraTarget[number],
		lightCameraUpAxis
	);

	//ライトプロジェクション行列を作成する。
	//太陽光からの影を落としたいなら、平行投影行列を作成する。
	m_lightProjMatrix[number].MakeOrthoProjectionMatrix(
		500,
		500,
		10.0f,
		1500.0f
	);
}

void ShadowMap::UpdateFromLightTarget(const CVector3& lightCameraPos, CVector3 lightCameraTarget, const int number)
{
	m_lightCameraTarget[number] = lightCameraTarget;
	m_lightCameraPosition[number] = lightCameraPos;
	//ライトの方向を計算する。
	auto lightDir = m_lightCameraTarget[number] - m_lightCameraPosition[number];
	if (lightDir.Length() < 0.0001f) {
		//ライトカメラの注視点と視点が近すぎる。
		//恐らくバグなので、クラッシュさせる。
		std::abort();
	}
	//正規化して、方向ベクトルに変換する。
	lightDir.Normalize();
	//
	UpdateFromLightDirection(lightCameraPos, lightDir, number);
}

void ShadowMap::RenderToShadowMap()
{
	for (int i = 0; i < Engine().GetGraphicsEngine().GetSplitNumber(); i++) {
		auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
		//レンダリングターゲットを切り替える。
		ID3D11RenderTargetView* rts[] = {
			m_shadowMapRT[i].GetRenderTargetView()
		};
		d3dDeviceContext->OMSetRenderTargets(1, rts, m_shadowMapRT[i].GetDepthStensilView());
		//ビューポートを設定。
		d3dDeviceContext->RSSetViewports(1, m_shadowMapRT[i].GetViewport());
		//シャドウマップをクリア。
		//一番奥のZは1.0なので、1.0で塗りつぶす。
		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
		m_shadowMapRT[i].ClearRenderTarget(clearColor);
	if (m_shadowCasters.size() >= 1) {
			//シャドウキャスターをシャドウマップにレンダリング。
			for (auto& caster : m_shadowCasters) {
				caster->Draw(
					&MainCamera(i),
					enRenderMode_CreateShadowMap
				);
			}
		}
	}
	//キャスターをクリア。
	m_shadowCasters.clear();
}