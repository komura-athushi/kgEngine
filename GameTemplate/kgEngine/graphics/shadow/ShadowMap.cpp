#include "KGstdafx.h"
#include "ShadowMap.h"

ShadowMap::ShadowMap()
{

	//シャドウマップ生成用のレンダリングターゲットを作成。
	//解像度は2048×2048。
	//テクスチャのフォーマットはR成分のみの32bit浮動小数点型。
	m_shadowMapRT.Create(
		2048,
		2048,
		DXGI_FORMAT_R32_FLOAT
	);
}

ShadowMap::~ShadowMap()
{

}

void ShadowMap::UpdateFromLightDirection(const CVector3& lightCameraPos,const CVector3& lightDir)
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
	m_lightViewMatrix.MakeLookAt(
		m_lightCameraPosition,
		m_lightCameraTarget,
		lightCameraUpAxis
	);

	//ライトプロジェクション行列を作成する。
	//太陽光からの影を落としたいなら、平行投影行列を作成する。
	m_lightProjMatrix.MakeOrthoProjectionMatrix(
		3000,
		3000,
		0.1f,
		5000.0f
	);
}

void ShadowMap::UpdateFromLightTarget(const CVector3& lightCameraPos, CVector3 lightCameraTarget)
{
	m_lightCameraTarget = lightCameraTarget;
	m_lightCameraPosition = lightCameraPos;
	//ライトの方向を計算する。
	auto lightDir = m_lightCameraTarget - m_lightCameraPosition;
	if (lightDir.Length() < 0.0001f) {
		//ライトカメラの注視点と視点が近すぎる。
		//恐らくバグなので、クラッシュさせる。
		std::abort();
	}
	//正規化して、方向ベクトルに変換する。
	lightDir.Normalize();
	//
	UpdateFromLightDirection(lightCameraPos, lightDir);
}

void ShadowMap::RenderToShadowMap()
{
	auto d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//レンダリングターゲットを切り替える。
	ID3D11RenderTargetView* rts[] = {
		m_shadowMapRT.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_shadowMapRT.GetDepthStensilView());
	//ビューポートを設定。
	d3dDeviceContext->RSSetViewports(1, m_shadowMapRT.GetViewport());
	//シャドウマップをクリア。
	//一番奥のZは1.0なので、1.0で塗りつぶす。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
	m_shadowMapRT.ClearRenderTarget(clearColor);

	//シャドウキャスターをシャドウマップにレンダリング。
	for (auto& caster : m_shadowCasters) {
		caster->Draw(
			enRenderMode_CreateShadowMap
		);
	}
	//キャスターをクリア。
	m_shadowCasters.clear();
}