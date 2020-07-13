#include "KGstdafx.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/SkinModelShaderConst.h"

void __cdecl ModelEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	switch (m_renderMode) {
	case enRenderMode_Normal:
		if (m_numInstance == 0) {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
		}
		else {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShaderInstancing.GetBody(), NULL, 0);
		}
		deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
		deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &m_albedoTex);
		break;
	case enRenderMode_CreateCascadeShadowMap:
		if (m_numInstance == 0) {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsCascadeShadowMap.GetBody(), NULL, 0);
		}
		else {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsCascadeShadowMapInstancing.GetBody(), NULL, 0);
		}
		//シャドウマップ生成。
		/*if (m_numInstance == 0) {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMap.GetBody(), NULL, 0);
		}
		else {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMapInstancing.GetBody(), NULL, 0);
		}*/
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psShadowMap.GetBody(), NULL, 0);
		break;
	case enRenderMode_CreateShadowMap:
		//シャドウマップ生成。
		if (m_numInstance == 0) {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMap.GetBody(), NULL, 0);
		}
		else {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMapInstancing.GetBody(), NULL, 0);
		}
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psShadowMap.GetBody(), NULL, 0);
		break;
	case enRenderMode_NormalMap:
		if (m_numInstance == 0) {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsNormalMap.GetBody(), NULL, 0);
		}
		else {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsNormalMapInstancing.GetBody(), NULL, 0);
		}
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psNormalMap.GetBody(), NULL, 0);
		break;
	case enRenderMode_DepthValueMap:
		if (m_numInstance == 0) {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMap.GetBody(), NULL, 0);
		}
		else {
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMapInstancing.GetBody(), NULL, 0);
		}
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psShadowMap.GetBody(), NULL, 0);
		break;
	}
}