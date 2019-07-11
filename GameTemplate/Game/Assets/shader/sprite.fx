/*!
 * @brief	スプライト用のシェーダー。
 */

cbuffer cb : register(b0){
	float4 mulColor;	//乗算カラー。
};

struct VSInput{
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> Texture : register(t0);	//カラーテクスチャ。
sampler TextureSampler : register(s0);
//角度
float Degree : register(t1);
PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos =In.pos;
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain(float4 color : COLOR0,
	float2 texCoord : TEXCOORD0) : SV_Target0
{
	float2 Center = {0.5f,0.5f};
    float2 Up = { 0.5f,1.0f };
	float2 Vector1 = {0.0f,-1.0f};
	float2 Vector2 = texCoord - Center;
	Vector1 = normalize(Vector1);
	Vector2 = normalize(Vector2);
	float Deg = acos(dot(Vector1, Vector2));
	Deg = abs(Deg);
	if (Vector2.x < Vector1.x) {
		Deg = 3.1415f + (3.1415 - Deg);
	}
	if (Deg >= Degree) {
		clip(-1);
	}
	float4 Color = Texture.Sample(TextureSampler, texCoord);
	Color.x *= mulColor.x;
	Color.y *= mulColor.y;
	Color.z *= mulColor.z;
	Color.w *= mulColor.w;
	return Color;
}