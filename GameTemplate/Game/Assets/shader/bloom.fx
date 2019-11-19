/*!
 *@brief	�u���[���p�̃V�F�[�_�[
 */

 /*!
  *@brief	���_�V�F�[�_�[�̓��́B
  */
struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};
/*!
 *@brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

/*!
 * @brief	���_�V�F�[�_�[�B
 */
PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = In.pos;
	psIn.uv = In.uv;
	return psIn;
}

Texture2D<float4> sceneTexture : register(t0);	//�V�[���e�N�X�`���B
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////
// �P�x���o�p
/////////////////////////////////////////////////////////
/*!
 * @brief	�P�x���o�p�̃s�N�Z���V�F�[�_�[�B
 */
float4 PSSamplingLuminance(PSInput In) : SV_Target0
{
	float4 color = sceneTexture.Sample(Sampler, In.uv);
	float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
	clip(t - 1.01f);			//�P�x��1.0�ȉ��Ȃ�s�N�Z���L��
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
// �{�P�摜�����p
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
// �u���[
/////////////////////////////////////////////////////////

Texture2D<float4> finalBlurTexture : register(t0);	//�ŏI�u���[�e�N�X�`���B


/*!
 *@brief	�ŏI�����p�̃s�N�Z���V�F�[�_�[�B
 */
float4 PSFinal(PSInput In) : SV_Target0
{
	return finalBlurTexture.Sample(Sampler, In.uv);
}