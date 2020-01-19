/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

cbuffer cb : register(b0){
	float4 mulColor;	//��Z�J���[�B
};
//float4 mulColor : register(b0);
Texture2D<float4> Texture : register(t0);	//�J���[�e�N�X�`���B
sampler TextureSampler : register(s0);
//�p�x    
float Angle : register(t1);
//�t�H���g�̑傫��
float2 Scale : register(t2);
//�T�[�N���Q�[�W�p���ǂ���
//bool isCircleGauge : register(t5);

float4 PSMain(
	float4 color : COLOR0,
	float2 texCoord : TEXCOORD0) : SV_Target0
{
	float PI = 3.14159f;
	if (Angle - PI * 2 <= 0.1f) {
		float2 Center = {0.5f,0.5f};
		float2 Up = { 0.5f,1.0f };
		float2 Vector1 = {0.0f,-1.0f};
		float2 Vector2 = texCoord - Center;
		Vector1 = normalize(Vector1);
		Vector2 = normalize(Vector2);
		float Deg = acos(dot(Vector1, Vector2));
		Deg = abs(Deg);
		//���S����s�N�Z���̍��W�ւ̃x�N�g���ƒ��S���������ւ̃x�N�g���̓��ς�����
		//�t�R�T�C�����Ƃ�A�ݒ肵���p�x��菬������Ε\���A�傫����΃s�N�Z����j������
		if (Vector2.x < Vector1.x) {
			Deg = PI + (PI - Deg);
		}
		if (Deg >= Angle) {
			clip(-1);
		}
	}
	float4 Color = Texture.Sample(TextureSampler, texCoord);
	Color *= color;
	//Color *= mulColor;
	return Color;
}

float4 PSMain_Font(
	float4 color : COLOR0,
	float2 texCoord : TEXCOORD0) : SV_Target0
{
	float2 texSize;
	float level;
	//�e�N�X�`���[�̃T�C�Y���擾����
	Texture.GetDimensions(0, texSize.x, texSize.y, level);

	float2 tex = texCoord;
	float4 Color;
	//if (Scale.x >= 1.1f) {
		//tex /= Scale;

		float offset = 0.1f;

		float2 base = float2(tex.x - offset, tex.y - offset);

		float2 tex1;
		float2 tex2;
		float2 tex3;
		float2 tex4;
		{
			tex1 = base;
			tex2 = float2(base.x + offset, base.y);
			tex3 = float2(base.x, base.y + offset);
			tex4 = float2(base.x + offset, base.y + offset);


		}
		float4 Color1;
		float4 Color2;
		float4 Color3;
		float4 Color4;
		{
			Color1 = Texture.Sample(TextureSampler, tex1);
			Color2 = Texture.Sample(TextureSampler, tex2);
			Color3 = Texture.Sample(TextureSampler, tex3);
			Color4 = Texture.Sample(TextureSampler, tex4);

			Color = (1 - offset )* (1 - offset) * Color1 +
				offset * (1 - offset) * Color2 +
				(1 - offset) * offset * Color3 +
				offset * offset * Color4;
		}
		//Color = float4(1.0f,1.0f,1.0f,1.0f);
	//}
	//else {
		//Color = Texture.Sample(TextureSampler, texCoord);
	//}

	//Color = Color / 4;*/


	//float4 Color = Texture.Sample(TextureSampler, texCoord);
	Color *= color;
	//Color *= mulColor;
	return Color;
}

/*float offset = 1.0f;

float2 tex = texCoord;

float2 tex1;
float2 tex2;
float2 tex3;
float2 tex4;
{
	//��̃s�N�Z��
	tex1.xy = tex + float2(0.0f, -offset / texSize.y);

	//���̃s�N�Z��
	tex2.xy = tex + float2(0.0f, offset / texSize.y);

	//���̃s�N�Z��
	tex3.xy = tex + float2(-offset / texSize.x, 0.0f);

	//�E�̃s�N�Z��
	tex4.xy = tex + float2(offset / texSize.x, 0.0f);
}

float4 Color = 0.0f;

{
	float2 Color1 = Texture.Sample(TextureSampler, tex1);
	float2 Color2 = Texture.Sample(TextureSampler, tex2);
	float2 Color3 = Texture.Sample(TextureSampler, tex3);
	float2 Color4 = Texture.Sample(TextureSampler, tex4);

	float Color5 = Color1 * (offset)+Color2 * (1 - offset);
	float Color6 = Color3 * (offset)+Color4 * (1 - offset);

	Color
}
*/
