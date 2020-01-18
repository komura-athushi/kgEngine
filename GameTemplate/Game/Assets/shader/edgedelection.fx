

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
	float4 tex1 : TEXCOORD1;
	float4 tex2 : TEXCOORD2;
	float4 tex3 : TEXCOORD3;
	float4 tex4 : TEXCOORD4;
	float4 tex5 : TEXCOORD5;
	float4 tex6 : TEXCOORD6;
	float4 tex7 : TEXCOORD7;
	float4 tex8 : TEXCOORD8;
};

Texture2D<float4> normalTexture : register(t0);	//�V�[���e�N�X�`���B
Texture2D<float4> depthValueTexture : register(t1); //�[�x�l�e�N�X�`��

sampler Sampler : register(s0);		//�T���v���[

PS_EdgeInput VSXEdge(VSInput In)
{
	float2 texSize;
	float level;
	//�e�N�X�`���[�̃T�C�Y���擾����
	normalTexture.GetDimensions(0, texSize.x, texSize.y, level);

	PS_EdgeInput Out;
	Out.pos = In.pos;
	float2 tex = In.uv;

	float offset = 0.2f;
	//�@��
	{
		//�^�񒆂̃s�N�Z��
		Out.tex0 = tex;

		//�E��̃s�N�Z��
		Out.tex1.xy = tex + float2(offset / texSize.x, -offset / texSize.y);

		//��̃s�N�Z��
		Out.tex2.xy = tex + float2(0.0f, -offset / texSize.y);

		//����̃s�N�Z��
		Out.tex3.xy = tex + float2(-offset / texSize.x, -offset / texSize.y);
		
		//�E�̃s�N�Z��
		Out.tex4.xy = tex + float2(offset / texSize.x, 0.0f);

		//���̃s�N�Z��
		Out.tex5.xy = tex + float2(-offset / texSize.x, 0.0f);

		//�E���̃s�N�Z��
		Out.tex6.xy = tex + float2(offset / texSize.x, offset / texSize.y);

		//���̃s�N�Z��
		Out.tex7.xy = tex + float2(0.0f, offset / texSize.y);

		//�����̃s�N�Z��
		Out.tex8.xy = tex + float2(-offset / texSize.x, offset / texSize.y);
	}

	{
		//�[�x�l�����o���Ƃ��Ɏg��UV���W
		offset = 1.0f;
		//�E��̃s�N�Z��
		Out.tex1.zw = tex + float2(offset / texSize.x, -offset / texSize.y);

		//��̃s�N�Z��
		Out.tex2.zw = tex + float2(0.0f, -offset / texSize.y);

		//����̃s�N�Z��
		Out.tex3.zw = tex + float2(-offset / texSize.x, -offset / texSize.y);

		//�E�̃s�N�Z��
		Out.tex4.zw = tex + float2(offset / texSize.x, 0.0f);

		//���̃s�N�Z��
		Out.tex5.zw = tex + float2(-offset / texSize.x, 0.0f);

		//�E���̃s�N�Z��
		Out.tex6.zw = tex + float2(offset / texSize.x, offset / texSize.y);

		//���̃s�N�Z��
		Out.tex7.zw = tex + float2(0.0f, offset / texSize.y);

		//�����̃s�N�Z��
		Out.tex8.zw = tex + float2(-offset / texSize.x, offset / texSize.y);

	}
	return Out;
}

float4 PSEdge(PS_EdgeInput In) : SV_Target0
{
	float depth = depthValueTexture.Sample(Sampler,In.tex0);

	if (depth < 0.002f) {
		clip(-1);
	}

	//���͂̃s�N�Z���̖@���̒l�̕��ς��v�Z����B
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

	//���͂̃s�N�Z���̐[�x�l�̕��ς��v�Z����B
	float depth2 = depthValueTexture.Sample(Sampler, In.tex1).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex2.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex3.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex4.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex5.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex6.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex7.zw).x;
	depth2 += depthValueTexture.Sample(Sampler, In.tex8.zw).x;
	depth2 /= 8.0f;

	float4 Color;
	//�@���̌v�Z���ʁA���邢�͐[�x�l�̌v�Z���ʂ����ȏ�Ȃ�G�b�W�Ƃ݂Ȃ��B
	if (length(Normal) >= 0.2f || abs(depth2-depth) > 0.0006f ) {
		Color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	return Color;
}