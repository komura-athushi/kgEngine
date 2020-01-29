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
	//0で一番目
	CMatrix GetLightViewMatrix(const int number = 0) const
	{
		return m_lightViewMatrix[number];
	}
	//ライトプロジェクション行列を取得
	//0で一番目
	CMatrix GetLightProjMatrix(const int number = 0) const
	{
		return m_lightProjMatrix[number];
	}
	//ライトビュー行列やライトプロジェクション行列を更新
	//0で一番目
	void UpdateFromLightTarget(const CVector3& lightCameraPos, CVector3 lightCameraTarget , const int number = 0);
	//更新(ライトカメラの向きを指定するバージョン)
	//0で一番目
	void UpdateFromLightDirection(const CVector3& lightCameraPos,const CVector3& lightDir , const int number = 0);
	//シャドウマップにシャドウキャスターをレンダリング
	void RenderToShadowMap();
	//シャドウキャスターを追加
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCasters.push_back(shadowCaster);
	}
	//シャドウマップのSRVを取得
	//0で一番目
	ID3D11ShaderResourceView* GetShadowMapSRV(const int number = 0)
	{
		return m_shadowMapRT[number].GetRenderTargetSRV();
	}
private:
	CVector3 m_lightCameraPosition[2] = { {0.0f,1000.0f,500.0f} ,{0.0f,1000.0f,500.0f} };			//ライトカメラの視点
	CVector3 m_lightCameraTarget[2] = { {0.0f,200.0f,0.0f} ,{0.0f,200.0f,0.0f} };			//ライトカメラの注視点
	CMatrix m_lightViewMatrix[2] = { CMatrix::Identity() ,CMatrix::Identity()};			//ライトビュー行列
	CMatrix m_lightProjMatrix[2] = { CMatrix::Identity() , CMatrix::Identity()};			//ライトプロジェクション行列
	RenderTarget m_shadowMapRT[2];								//シャドウマップを描画するレンダリングターゲット
	std::vector<SkinModel*> m_shadowCasters;				//シャドウキャスターの配列
};

