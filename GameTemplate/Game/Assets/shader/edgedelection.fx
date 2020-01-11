

/*!
  *@brief	頂点シェーダーの入力。
  */
struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};
/*!
 *@brief	ピクセルシェーダーへの入力。
 */
struct PS_EdgeInput {
	float4 pos : SV_Position;
	float2 tex0	: TEXCOORD0;
	float4 tex1 : TEXCOORD1;
	float4 tex2 : TEXCOORD2;
	float4 tex3 : TEXCOORD3;
	float4 tex4 : TEXCOORD4;
	float4 tex5 : TEXCOORD5;
	float4 tex6 : TEXCOORD6;
	float4 tex7 : TEXCOORD7;
	float4 tex8 : TEXCOORD8;
};

Texture2D<float4> normalTexture : register(t0);	//シーンテクスチャ。
Texture2D<float4> depthValueTexture : register(t1); //深度値テクスチャ

sampler Sampler : register(s0);

PS_EdgeInput VSXEdge(VSInput In)
{
	float2 texSize;
	float level;
	normalTexture.GetDimensions(0, texSize.x, texSize.y, level);

	PS_EdgeInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;

	float offset = 0.2f;
	//真ん中
	Out.tex0 = tex;

	//右上
	Out.tex1.xy = tex + float2(offset / texSize.x, -offset / texSize.y);
	
	//上
	Out.tex2.xy = tex + float2(0.0f, -offset / texSize.y);
	
	//左上
	Out.tex3.xy = tex + float2(-offset / texSize.x, -offset / texSize.y);

	//右
	Out.tex4.xy = tex + float2(offset / texSize.x, 0.0f);

	//左
	Out.tex5.xy = tex + float2(-offset / texSize.x, 0.0f);

	//右下
	Out.tex6.xy = tex + float2(offset / texSize.x, offset / texSize.y);

	//下	
	Out.tex7.xy = tex + float2(0.0f, offset / texSize.y);

	//左下
	Out.tex8.xy = tex + float2(-offset / texSize.x, offset / texSize.y);

	{
		//深度値を取り出すときに使うUV座標
		offset = 1.0f;
		//右上
		Out.tex1.zw = tex + float2(offset / texSize.x, -offset / texSize.y);

		//上
		Out.tex2.zw = tex + float2(0.0f, -offset / texSize.y);

		//左上
		Out.tex3.zw = tex + float2(-offset / texSize.x, -offset / texSize.y);

		//右
		Out.tex4.zw = tex + float2(offset / texSize.x, 0.0f);

		//左
		Out.tex5.zw = tex + float2(-offset / texSize.x, 0.0f);

		//右下
		Out.tex6.zw = tex + float2(offset / texSize.x, offset / texSize.y);

		//下	
		Out.tex7.zw = tex + float2(0.0f, offset / texSize.y);

		//左下
		Out.tex8.zw = tex + float2(-offset / texSize.x, offset / texSize.y);

	}
	return Out;
}

float4 PSEdge(PS_EdgeInput In) : SV_Target0
{
	float depth = depthValueTexture.Sample(Sampler,In.tex0);
	//float depth = In.pos.z / In.pos.w;
	if (depth < 0.0025f) {
		clip(-1);
	}

	float3 Normal;
	Normal = normalTexture.Sample(Sampler, In.tex0).xyz * -8.0f;
	Normal += normalTexture.Sample(Sampler, In.tex1.xy).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex2.xy).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex3.xy).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex4.xy).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex5.xy).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex6.xy).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex7.xy).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex8.xy).xyz;

	//周囲のピクセルの深度値の平均を計算する。
	float depth2 = depthValueTexture.Sample(Sampler, In.tex1).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex2.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex3.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex4.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex5.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex6.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex7.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex8.zw).x;
	depth2 /= 8.0f;
	//Normal = normalTexture.Sample(Sampler, In.tex0).xyz * -4.0f;
	////Normal += normalTexture.Sample(Sampler, In.tex1).xyz;
	//Normal += normalTexture.Sample(Sampler, In.tex2).xyz;
	////Normal += normalTexture.Sample(Sampler, In.tex3).xyz;
	//Normal += normalTexture.Sample(Sampler, In.tex4).xyz;
	//Normal += normalTexture.Sample(Sampler, In.tex5).xyz;
	////Normal += normalTexture.Sample(Sampler, In.tex6).xyz;
	//Normal += normalTexture.Sample(Sampler, In.tex7).xyz;
	////Normal += normalTexture.Sample(Sampler, In.tex8).xyz;

	float4 Color;
	if (length(Normal) >= 0.2f || abs(depth2-depth) > 0.001f ) {
		Color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	return Color;
}