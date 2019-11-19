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
	//Color *= mulColor;
	return Color;
}

