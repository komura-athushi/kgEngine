/*!
 *@brief	�K�E�V�A���u���[�B
 */

 /*!
  *@brief	���_�V�F�[�_�[�̓��́B
  */
struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};
/*!
 *@brief	�s�N�Z���V�F�[�_�[�ւ̓��́B
 */
struct PS_BlurInput {
	float4 pos	: SV_POSITION;
	float4 tex0	: TEXCOORD0;
	float4 tex1 : TEXCOORD1;
	float4 tex2 : TEXCOORD2;
	float4 tex3 : TEXCOORD3;
	float4 tex4 : TEXCOORD4;
	float4 tex5 : TEXCOORD5;
	float4 tex6 : TEXCOORD6;
	float4 tex7 : TEXCOORD7;
};

Texture2D<float4> srcTexture : register(t0);	//�\�[�X�e�N�X�`���B
sampler Sampler : register(s0);


/*!
 * @brief	�u���[�p�̒萔�o�b�t�@�B
 */
cbuffer CBBlur : register(b0)
{
	float4 weight[2];	//!<�d�݁B
}
/*!
 * @brief	X�u���[���_�V�F�[�_�[�B
 */
PS_BlurInput VSXBlur(VSInput In)
{
	float2 texSize;
	float level;
	//�P�x�e�N�X�`���̃T�C�Y���擾����B
	srcTexture.GetDimensions(0, texSize.x, texSize.y, level);

	PS_BlurInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;

	Out.tex0.xy = tex + float2(1.0f / texSize.x, 0.0f);	//XY�Ɂ{�̕����AZW�Ɂ|�̕�����UV�l��ݒ肷��B
	Out.tex0.zw = tex - float2(1.0f / texSize.x, 0.0f);

	Out.tex1.xy = tex + float2(3.0f / texSize.x, 0.0f);
	Out.tex1.zw = tex - float2(3.0f / texSize.x, 0.0f);

	Out.tex2.xy = tex + float2(5.0f / texSize.x, 0.0f);
	Out.tex2.zw = tex - float2(5.0f / texSize.x, 0.0f);

	Out.tex3.xy = tex + float2(7.0f / texSize.x, 0.0f);
	Out.tex3.zw = tex - float2(7.0f / texSize.x, 0.0f);

	Out.tex4.xy = tex + float2(9.0f / texSize.x, 0.0f);
	Out.tex4.zw = tex - float2(9.0f / texSize.x, 0.0f);

	Out.tex5.xy = tex + float2(11.0f / texSize.x, 0.0f);
	Out.tex5.zw = tex - float2(11.0f / texSize.x, 0.0f);

	Out.tex6.xy = tex + float2(13.0f / texSize.x, 0.0f);
	Out.tex6.zw = tex - float2(13.0f / texSize.x, 0.0f);

	Out.tex7.xy = tex + float2(15.0f / texSize.x, 0.0f);
	Out.tex7.zw = tex - float2(15.0f / texSize.x, 0.0f);

	return Out;
}
/*!
 * @brief	Y�u���[���_�V�F�[�_�[�B
 */
PS_BlurInput VSYBlur(VSInput In)
{
	//�P�x�e�N�X�`���̃T�C�Y���擾����B
	float2 texSize;
	float level;
	srcTexture.GetDimensions(0, texSize.x, texSize.y, level);
	PS_BlurInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;

	//XY�Ɂ{�̕����AZW�Ɂ|�̕�����UV�l��ݒ肷��B
	Out.tex0.xy = tex + float2(0.0f, 1.0f / texSize.y);
	Out.tex0.zw = tex - float2(0.0f, 1.0f / texSize.y);

	Out.tex1.xy = tex + float2(0.0f, 3.0f / texSize.y);
	Out.tex1.zw = tex - float2(0.0f, 3.0f / texSize.y);

	Out.tex2.xy = tex + float2(0.0f, 5.0f / texSize.y);
	Out.tex2.zw = tex - float2(0.0f, 5.0f / texSize.y);

	Out.tex3.xy = tex + float2(0.0f, 7.0f / texSize.y);
	Out.tex3.zw = tex - float2(0.0f, 7.0f / texSize.y);

	Out.tex4.xy = tex + float2(0.0f, 9.0f / texSize.y);
	Out.tex4.zw = tex - float2(0.0f, 9.0f / texSize.y);

	Out.tex5.xy = tex + float2(0.0f, 11.0f / texSize.y);
	Out.tex5.zw = tex - float2(0.0f, 11.0f / texSize.y);

	Out.tex6.xy = tex + float2(0.0f, 13.0f / texSize.y);
	Out.tex6.zw = tex - float2(0.0f, 13.0f / texSize.y);

	Out.tex7.xy = tex + float2(0.0f, 15.0f / texSize.y);
	Out.tex7.zw = tex - float2(0.0f, 15.0f / texSize.y);
	return Out;
}
/*!
 * @brief	�u���[�s�N�Z���V�F�[�_�[�B
 */
float4 PSBlur(PS_BlurInput In) : SV_Target0
{
	float4 Color;
	Color = weight[0].x * (srcTexture.Sample(Sampler, In.tex0.xy)
				   + srcTexture.Sample(Sampler, In.tex0.zw));

	Color += weight[0].y * (srcTexture.Sample(Sampler, In.tex1.xy)
				   + srcTexture.Sample(Sampler, In.tex1.zw));

	Color += weight[0].z * (srcTexture.Sample(Sampler, In.tex2.xy)
			  + srcTexture.Sample(Sampler, In.tex2.zw));

	Color += weight[0].w * (srcTexture.Sample(Sampler, In.tex3.xy)
				   + srcTexture.Sample(Sampler, In.tex3.zw));

	Color += weight[1].x * (srcTexture.Sample(Sampler, In.tex4.xy)
				   + srcTexture.Sample(Sampler, In.tex4.zw));

	Color += weight[1].y * (srcTexture.Sample(Sampler, In.tex5.xy)
				   + srcTexture.Sample(Sampler, In.tex4.zw));

	Color += weight[1].z * (srcTexture.Sample(Sampler, In.tex6.xy)
				   + srcTexture.Sample(Sampler, In.tex6.zw));

	Color += weight[1].w * (srcTexture.Sample(Sampler, In.tex7.xy)
					 + srcTexture.Sample(Sampler, In.tex7.zw));

	return float4(Color.xyz, 1.0f);
}
