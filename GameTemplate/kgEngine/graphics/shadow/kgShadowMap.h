#pragma once
#include "Noncopyable.h"
#include "graphics/RenderTarget.h"
class SkinModel;
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	//ライトビュー行列を取得
	CMatrix GetLightViewMatrix() const
	{
		return m_lightViewMatrix;
	}
	//ライトプロジェクション行列を取得
	CMatrix GetLightProjMatrix() const
	{
		return m_lightProjMatrix;
	}
	//ライトビュー行列やライトプロジェクション行列を更新
	void UpdateFromLightTarget(const CVector3& lightCameraPos, CVector3 lightCameraTarget);
	//更新(ライトカメラの向きを指定するバージョン)
	void UpdateFromLightDirection(const CVector3& lightCameraPos,const CVector3& lightDir);
	//シャドウマップにシャドウキャスターをレンダリング
	void RenderToShadowMap();
	//シャドウキャスターを追加
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCasters.push_back(shadowCaster);
	}
	//シャドウマップのSRVを取得
	ID3D11ShaderResourceView* GetShadowMapSRV()
	{
		return m_shadowMapRT.GetRenderTargetSRV();
	}
private:
	CVector3 m_lightCameraPosition = {0.0f,1000.0f,500.0f};			//ライトカメラの視点
	CVector3 m_lightCameraTarget = {0.0f,200.0f,0.0f};			//ライトカメラの注視点
	CMatrix m_lightViewMatrix = CMatrix::Identity();			//ライトビュー行列
	CMatrix m_lightProjMatrix = CMatrix::Identity();			//ライトプロジェクション行列
	RenderTarget m_shadowMapRT;								//シャドウマップを描画するレンダリングターゲット
	std::vector<SkinModel*> m_shadowCasters;				//シャドウキャスターの配列
};

