/*****************************************************************//**
 * \file   CascadeShadowMap.h
 * \brief  CascadeShadowMapクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/RenderTarget.h"


class SkinModel;
/**
 * \brief カスケードシャドウマップを生成するクラス.
 */
class CascadeShadowMap


{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	CascadeShadowMap();
	/**
	 * \brief 更新.
	 * 
	 */
	void Update();
	/**
	 * \brief シャドウマップを生成.
	 * 
	 */
	void RenderToShadowMap();
	/**
	 * \brief シャドウキャスターを追加.
	 * 
	 * \param shadowCaster スキンモデル
	 */
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCaters.push_back(shadowCaster);
	}

	/// <summary>
	/// レンダリングターゲットを取得
	/// </summary>
	/// <returns></returns>
	/**
	 * \brief レンダリングターゲットを取得.
	 * 
	 * \param splitNumber 画面分割の番号
	 * \param number シャドウマップの番号
	 * \return レンダリングターゲット
	 */
	RenderTarget* GetRenderTarget(const int splitNumber,const int number)
	{
		return &m_shadowMapRT[splitNumber][number];
	}
	/// <summary>
	/// ライトビュープロジェクション行列を取得
	/// </summary>
	/// <returns></returns>
	/**
	 * \brief ライトビュープロジェクション行列を取得.
	 * 
	 * \param splitNumber 画面分割の番号
	 * \return ライトビュープロジェクション行列
	 */
	const CMatrix& GetLightViewProjMatrix(const int splitNumber) const
	{
		return m_lightVieProjMatrix[splitNumber][m_shadowMapNumber];
	}
	/**
	 * \brief ライトビュープロジェクション行列を取得.
	 * 
	 * \param splitNumber 画面分割の番号
	 * \param number シャドウマップの番号
	 * \return ライトビュープロジェクション行列
	 */
	const CMatrix& GetLightViewProjMatrix(const int splitNumber,const int number) const
	{
		return m_lightVieProjMatrix[splitNumber][number];
	}
	/**
	 * \brief シャドウマップの番号を取得(1番目が0).
	 * 
	 * \return シャドウマップの番号
	 */
	const int GetShadowMapNumber() const
	{
		return m_shadowMapNumber;
	}
	/**
	 * \brief シャドウマップの深度値を取得.
	 * 
	 * \param splitNumber 画面分割の番号
	 * \param number シャドウマップの番号
	 * \return 深度値
	 */
	const float GetFar(const int splitNumber,const int number) const
	{
		return m_farList[splitNumber][number];
	}
	static const int SHADOWMAP_NUM = 3;
private:
	/**
	 * \brief ライトビュープロジェクション行列を計算.
	 * 
	 * \param splitNumber 画面分割の番号
	 */
	void UpdateLightViewProjMatrix(const int splitNumber);
private:
	CVector3 m_lightCameraPosition = CVector3(300.0f, 300.0f, -300.0f);		//ライトカメラの座標	
	CVector3 m_lightCamerataraget = CVector3(0.0f, 0.0f, 0.0f);		//ライトカメラの注視点
	CMatrix m_lightViewMatrix;			//ライトビュー行列
	CMatrix m_lightProjMatrix;			//ライトプロジェクション行列
	RenderTarget m_shadowMapRT[2][SHADOWMAP_NUM];			//シャドウマップのレンダリングターゲット
	CMatrix m_lightVieProjMatrix[2][SHADOWMAP_NUM];			//各シャドウマップのビュープロジェクション行列
	std::vector<SkinModel*> m_shadowCaters;		//シャドウキャスターのリスト
	CVector3 m_lightDir = CVector3(-0.577f,-0.577f,-0.577f);
	float m_lightHeight = 400.0f;
	int m_shadowMapNumber = 0;
	float m_farList[2][SHADOWMAP_NUM] = {1.0f,1.0f,1.0f,1.0f};
	CMatrix m_lightViewMatrixInv[2][SHADOWMAP_NUM];
};


