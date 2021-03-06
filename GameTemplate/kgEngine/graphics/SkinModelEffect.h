#pragma once

#include "graphics/Shader.h"

/*!
*@brief	モデルエフェクト。
*/
class ModelEffect : public DirectX::IEffect {
protected:
	std::wstring m_materialName;	//!<マテリアル名。
	Shader* m_pVSShader = nullptr;
	Shader* m_pPSShader = nullptr;
	Shader m_vsShader;
	Shader m_psShader;
	Shader m_vsShaderInstancing;
	Shader m_vsShadowMap;			//シャドウマップ生成用の頂点シェーダー。
	Shader m_vsShadowMapInstancing;			//シャドウマップ生成用の頂点シェーダー、インスタンシング用
	Shader m_vsCascadeShadowMap;			//カスケードシャドウマップ生成用の頂点シェーダー
	Shader m_vsCascadeShadowMapInstancing;     //カスケードシャドウマップ生成用の頂点シェーダー、インスタンシング用
	Shader m_psShadowMap;		//シャドウマップ生成用のピクセルシェーダー。
	Shader m_vsNormalMap;			//シャドウマップ生成用の頂点シェーダー。
	Shader m_vsNormalMapInstancing;			//シャドウマップ生成用の頂点シェーダー、インスタンシング用
	Shader m_psNormalMap;		//シャドウマップ生成用のピクセルシェーダー。
	bool isSkining;
	ID3D11ShaderResourceView* m_albedoTex = nullptr;
	EnRenderMode m_renderMode = enRenderMode_Invalid;	//レンダリングモード。
	int m_numInstance = 1;
public:
	ModelEffect()
	{
		m_psShader.Load("Assets/shader/model.fx", "PSMain", Shader::EnType::PS);
		
		m_pPSShader = &m_psShader;
		//todo シャドウマップ用のシェーダーをロード。
		m_psShadowMap.Load("Assets/shader/model.fx", "PSMain_ShadowMap", Shader::EnType::PS);
		m_psNormalMap.Load("Assets/shader/model.fx", "PSMain_NormalMap", Shader::EnType::PS);
	}
	virtual ~ModelEffect()
	{
		if (m_albedoTex) {
			m_albedoTex->Release();
		}
	}
	void __cdecl Apply(ID3D11DeviceContext* deviceContext) override;

	void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override
	{
		*pShaderByteCode = m_vsShader.GetByteCode();
		*pByteCodeLength = m_vsShader.GetByteCodeSize();
	}
	void SetAlbedoTexture(ID3D11ShaderResourceView* tex)
	{
		m_albedoTex = tex;
	}
	void SetMatrialName(const wchar_t* matName)
	{
		m_materialName = matName;
	}
	
	bool EqualMaterialName(const wchar_t* name) const
	{
		return wcscmp(name, m_materialName.c_str()) == 0;
	}
	void SetRenderMode(EnRenderMode renderMode)
	{
		m_renderMode = renderMode;
	}
	void SetNumInstance(int numInstance)
	{
		m_numInstance = numInstance;
	}
};
/*!
*@brief
*  スキンなしモデルエフェクト。
*/
class NonSkinModelEffect : public ModelEffect {
public:
	NonSkinModelEffect()
	{
		m_vsShader.Load("Assets/shader/model.fx", "VSMain", Shader::EnType::VS);
		//インスタンシング描画用の頂点シェーダーをロード
		m_vsShaderInstancing.Load("Assets/shader/model.fx", "VSMainInstancing", Shader::EnType::VS);
		m_pVSShader = &m_vsShader;
		//スキン無しシャドウマップ
		m_vsShadowMap.Load("Assets/shader/model.fx", "VSMain_ShadowMap", Shader::EnType::VS);
		m_vsShadowMapInstancing.Load("Assets/shader/model.fx", "VSMainInstancing_ShadowMap", Shader::EnType::VS);

		m_vsCascadeShadowMap.Load("Assets/shader/model.fx", "VSMain_CascadeShadowMap", Shader::EnType::VS);
		m_vsCascadeShadowMapInstancing.Load("Assets/shader/model.fx", "VSMainInstancing_CascadeShadowMap", Shader::EnType::VS);

		m_vsNormalMap.Load("Assets/shader/model.fx", "VSMain_NormalMap", Shader::EnType::VS);
		m_vsNormalMapInstancing.Load("Assets/shader/model.fx", "VSMainInstancing_NormalMap", Shader::EnType::VS);
		isSkining = false;
	}
};
/*!
*@brief
*  スキンモデルエフェクト。
*/
class SkinModelEffect : public ModelEffect {
public:
	SkinModelEffect()
	{
		wchar_t hoge[256];
		GetCurrentDirectoryW(256, hoge);
		m_vsShader.Load("Assets/shader/model.fx", "VSMainSkin", Shader::EnType::VS);
		m_vsShaderInstancing.Load("Assets/shader/model.fx", "VSMainSkinInstancing", Shader::EnType::VS);
		//スキンありシャドウマップ
		m_vsShadowMap.Load("Assets/shader/model.fx", "VSMainSkin_ShadowMap", Shader::EnType::VS);
		m_vsShadowMapInstancing.Load("Assets/shader/model.fx", "VSMainSkinInstancing_ShadowMap", Shader::EnType::VS);

		m_vsCascadeShadowMap.Load("Assets/shader/model.fx", "VSMainSkin_CascadeShadowMap", Shader::EnType::VS);
		m_vsCascadeShadowMapInstancing.Load("Assets/shader/model.fx", "VSMainSkinInstancing_CascadeShadowMap", Shader::EnType::VS);

		m_vsNormalMap.Load("Assets/shader/model.fx", "VSMainSkin_NormalMap", Shader::EnType::VS);
		m_vsNormalMapInstancing.Load("Assets/shader/model.fx", "VSMainSkinInstancing_NormalMap", Shader::EnType::VS);

		m_pVSShader = &m_vsShader;
		isSkining = true;
	}
};

/*!
*@brief
*  エフェクトファクトリ。
*/
class SkinModelEffectFactory : public DirectX::EffectFactory {
public:
	SkinModelEffectFactory(ID3D11Device* device) :
		EffectFactory(device) {}
	std::shared_ptr<DirectX::IEffect> __cdecl CreateEffect(const EffectInfo& info, ID3D11DeviceContext* deviceContext)override
	{
		std::shared_ptr<ModelEffect> effect;
		if (info.enableSkinning) {
			//スキニングあり。
			effect = std::make_shared<SkinModelEffect>();
		}
		else {
			//スキニングなし。
			effect = std::make_shared<NonSkinModelEffect>();
		}
		effect->SetMatrialName(info.name);
		if (info.diffuseTexture && *info.diffuseTexture)
		{
			ID3D11ShaderResourceView* texSRV;
			DirectX::EffectFactory::CreateTexture(info.diffuseTexture, deviceContext, &texSRV);
			effect->SetAlbedoTexture(texSRV);
		}
		return effect;
	}

	void __cdecl CreateTexture(const wchar_t* name, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView** textureView) override
	{
		return DirectX::EffectFactory::CreateTexture(name, deviceContext, textureView);
	}
};
