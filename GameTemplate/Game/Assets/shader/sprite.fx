/*!
 * @brief	スプライト用のシェーダー。
 */

cbuffer cb : register(b0){
	float4 mulColor;	//乗算カラー。
};
//float4 mulColor : register(b0);
Texture2D<float4> Texture : register(t0);	//カラーテクスチャ。
sampler TextureSampler : register(s0);
//角度
float Angle : register(t1);
//サークルゲージ用かどうか
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
		//中心からピクセルの座標へのベクトルと中心から上方向へのベクトルの内積を求め
		//逆コサインをとり、設定した角度より小さければ表示、大きければピクセルを破棄する
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

