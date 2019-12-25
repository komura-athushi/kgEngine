

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
struct PS_EdgeInput {
	float4 pos : SV_Position;
	float2 tex0	: TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
	float2 tex4 : TEXCOORD4;
	float2 tex5 : TEXCOORD5;
	float2 tex6 : TEXCOORD6;
	float2 tex7 : TEXCOORD7;
	float2 tex8 : TEXCOORD8;
};

Texture2D<float4> normalTexture : register(t0);	//�V�[���e�N�X�`���B
sampler Sampler : register(s0);

PS_EdgeInput VSXEdge(VSInput In)
{
	float2 texSize;
	float level;
	normalTexture.GetDimensions(0, texSize.x, texSize.y, level);

	PS_EdgeInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;

	//�^��
	Out.tex0 = tex;

	//�E��
	Out.tex1 = tex + float2(1.0f / texSize.x, -1.0f / texSize.y);	
	
	//��
	Out.tex2 = tex + float2(0.0f, -1.0f / texSize.y);
	
	//����
	Out.tex3 = tex + float2(-1.0f / texSize.x, -1.0f / texSize.y);

	//�E
	Out.tex4 = tex + float2(1.0f / texSize.x, 0.0f);

	//��
	Out.tex5 = tex + float2(-1.0f / texSize.x, 0.0f);

	//�E��
	Out.tex6 = tex + float2(1.0f / texSize.x, 1.0f / texSize.y);

	//��	
	Out.tex7 = tex + float2(0.0f, 1.0f / texSize.y);

	//����
	Out.tex8 = tex + float2(-1.0f / texSize.x, 1.0f / texSize.y);

	return Out;
}

float4 PSEdge(PS_EdgeInput In) : SV_Target0
{
	float3 Normal;
	Normal = normalTexture.Sample(Sampler, In.tex0).xyz * -8.0f;
	Normal += normalTexture.Sample(Sampler, In.tex1).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex2).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex3).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex4).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex5).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex6).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex7).xyz;
	Normal += normalTexture.Sample(Sampler, In.tex8).xyz;

	float4 Color;
	if (length(Normal) >= 3.0f) {
		Color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	return Color;
}