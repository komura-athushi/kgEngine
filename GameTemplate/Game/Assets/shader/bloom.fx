/*!
 *@brief	ブルーム用のシェーダー
 */

 /*!
  *@brief	頂点シェーダーの入力。
  */
struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};
/*!
 *@brief	ピクセルシェーダーの入力。
 */
struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

/*!
 * @brief	頂点シェーダー。
 */
PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

Texture2D<float4> sceneTexture : register(t0);	//シーンテクスチャ。
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////
// 輝度抽出用
/////////////////////////////////////////////////////////
/*!
 * @brief	輝度抽出用のピクセルシェーダー。
 */
float4 PSSamplingLuminance(PSInput In) : SV_Target0
{
	float4 color = sceneTexture.Sample(Sampler, In.uv);
	float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
	clip(t - 1.01f);			//輝度が1.0以下ならピクセルキル
	color.xyz *= (t - 1.0f);
	color.a = 1.0f;
	return color;
}

float4 PSMain(PSInput In) : SV_Target0
{
	float4 color = sceneTexture.Sample(Sampler, In.uv);
	return color;
}
/////////////////////////////////////////////////////////
// ボケ画像合成用
/////////////////////////////////////////////////////////

Texture2D<float4> bokeTexture00 : register(t0);
Texture2D<float4> bokeTexture01 : register(t1);
Texture2D<float4> bokeTexture02 : register(t2);
Texture2D<float4> bokeTexture03 : register(t3);

float4 PSCombine(PSInput In) : SV_Target0
{
	float2 uv = In.uv;
	float4 finalColor = bokeTexture00.Sample(Sampler, uv);
	finalColor += bokeTexture01.Sample(Sampler, uv);
	finalColor += bokeTexture02.Sample(Sampler, uv);
	finalColor += bokeTexture03.Sample(Sampler, uv);
	finalColor /= 4.0f;
	finalColor.a = 1.0f;
	return finalColor;
}
/////////////////////////////////////////////////////////
// ブラー
/////////////////////////////////////////////////////////

Texture2D<float4> finalBlurTexture : register(t0);	//最終ブラーテクスチャ。


/*!
 *@brief	最終合成用のピクセルシェーダー。
 */
float4 PSFinal(PSInput In) : SV_Target0
{
	return finalBlurTexture.Sample(Sampler, In.uv);
}