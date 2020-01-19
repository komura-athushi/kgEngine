/*!
 * @brief	モデルシェーダー。
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);	
Texture2D<float4> shadowMap : register(t2);		//todo シャドウマップ。
Texture2D<float4> toonMap : register(t4);		//toonシェーダー用のテクスチャー
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);
StructuredBuffer<float4x4> instanceMatrix : register(t3);
//UVスクロール関係
//int isuvscroll : register(t3);
//float uvscroll : register(t4);
/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);
sampler ToonSampler : register(s1);
/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	//todo ライトビュー行列を追加。
	float4x4 mLightView;	//ライトビュー行列。
	float4x4 mLightProj;	//ライトプロジェクション行列。
	int isShadowReciever;	//シャドウレシーバーフラグ。
	int isDithering;
	
	float3 katamariVector;
};
static const int NUM_DIRECTION_LIG = 4;
/// <summary>
/// ライト用の定数バッファ。
/// </summary>
cbuffer LightCb : register(b1) {
	float3 dligDirection[NUM_DIRECTION_LIG];
	float4 dligColor[NUM_DIRECTION_LIG];
	float4 ambientlight;
	float3 eyePos;				//カメラの視点。
	float specPow;			//スペキュラライトの絞り。
	float3 eyeDir;
	int isToomShader;
	float4 color;
};

/// <summary>
/// シャドウマップ用の定数バッファ。
/// </summary>
cbuffer ShadowMapCb : register(b1) {
	float4x4 lightViewProjMatrix;	//ライトビュープロジェクション行列。
}

/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float3 Tangent  : TANGENT;				//接ベクトル。
    float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float2 TexCoord	: TEXCOORD0;			//UV座標。
    float3 Tangent	: TANGENT;				//接ベクトル。
    uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
    float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};

/// <summary>
/// シャドウマップ用のピクセルシェーダへの入力構造体。
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//座標。
};

//法線マップ用のピクセルシェーダーへの入力構造体
struct PSInput_NormalMap {
	float4 Position				: SV_POSITION;
	float3 Normal				: NORMAL;
};

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float4 posInLVP		: TEXCOORD1;	//ライトビュープロジェクション空間での座標。
	float3 worldPos		: TEXCOORD2;	//ワールド座標
};

//ピクセルシェーダーの出力構造体
struct PSOutPut {
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float depthView : SV_Target2;
};

//ディザパターン
static const int pattern[] = {
	0, 32,  8, 40,  2, 34, 10, 42,   /* 8x8 Bayer ordered dithering  */
	48, 16, 56, 24, 50, 18, 58, 26,  /* pattern.  Each input pixel   */
	12, 44,  4, 36, 14, 46,  6, 38,  /* is scaled to the 0..63 range */
	60, 28, 52, 20, 62, 30, 54, 22,  /* before looking in this table */
	3, 35, 11, 43,  1, 33,  9, 41,   /* to determine the action.     */
	51, 19, 59, 27, 49, 17, 57, 25,
	15, 47,  7, 39, 13, 45,  5, 37,
	63, 31, 55, 23, 61, 29, 53, 21
};

/*!
 *@brief	スキン行列を計算。
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	//ローカル座標系からワールド座標系に変換する
	float4 worldPos = mul(mWorld, In.Position);
	//解説７　鏡面反射の計算のために、ワールド座標をピクセルシェーダーに渡す。
	psInput.worldPos = worldPos;
	//ワールド座標系からカメラ座標系に変換する
	psInput.Position = mul(mView, worldPos);
	//カメラ座標系からスクリーン座標系に変換する
	psInput.Position = mul(mProj, psInput.Position);
	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
		//ワールド座標系からライトビュー座標系に変換
		psInput.posInLVP = mul(mLightView, worldPos);
		//ライトビュー座標系からライトプロジェクション行列に変換
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));
	return psInput;
}
//インスタンシング用のスキンなしモデル用頂点シェーダー
PSInput VSMainInstancing(VSInputNmTxVcTangent In,uint instanceID : SV_InstanceID)
{
	PSInput psInput = (PSInput)0;
	//ローカル座標系からワールド座標系に変換する
	float4 worldPos = mul(instanceMatrix[instanceID], In.Position);
	//解説７　鏡面反射の計算のために、ワールド座標をピクセルシェーダーに渡す。
	psInput.worldPos = worldPos;
	//float4 worldPos = mul(mWorld, In.Position);
	//ワールド座標系からカメラ座標系に変換する
	psInput.Position = mul(mView, worldPos);
	//カメラ座標系からスクリーン座標系に変換する
	psInput.Position = mul(mProj, psInput.Position);
	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
		//ワールド座標系からライトビュー座標系に変換
		psInput.posInLVP = mul(mLightView, worldPos);
		//ライトビュー座標系からライトプロジェクション行列に変換
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(instanceMatrix[instanceID], In.Normal));
	psInput.Tangent = normalize(mul(instanceMatrix[instanceID], In.Tangent));
	return psInput;
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 worldPos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //最後のボーンを計算する。
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
	    worldPos = mul(skinning, In.Position);
		//解説７　鏡面反射の計算のために、ワールド座標をピクセルシェーダーに渡す。
		psInput.worldPos = worldPos;
	}
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	//ワールド座標系からカメラ座標系に変換する
	psInput.Position = mul(mView, worldPos);
	//カメラ座標系からスクリーン座標系に変換する
	psInput.Position = mul(mProj, psInput.Position);
	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
		//ワールド座標系からライトビュー座標系に変換
		psInput.posInLVP = mul(mLightView, worldPos);
		//ライトビュー座標系からライトプロジェクション行列に変換
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//インスタンシング用のスキンありモデル用の頂点シェーダー
PSInput VSMainSkinInstancing(VSInputNmTxWeights In, uint instanceID : SV_InstanceID)
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;
	//ローカル座標系からワールド座標系に変換する
	float4 worldPos = 0; 
	{

		float w = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
			skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
			w += In.Weights[i];
		}
		//最後のボーンを計算する。
		skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
		skinning = mul(instanceMatrix[instanceID], skinning);
		//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
		worldPos = mul(skinning, In.Position);
		//解説７　鏡面反射の計算のために、ワールド座標をピクセルシェーダーに渡す。
		psInput.worldPos = worldPos;
	}
	psInput.Normal = normalize(mul(skinning, In.Normal));
	psInput.Tangent = normalize(mul(skinning, In.Tangent));

	//ワールド座標系からカメラ座標系に変換する
	psInput.Position = mul(mView, worldPos);
	//カメラ座標系からスクリーン座標系に変換する
	psInput.Position = mul(mProj, psInput.Position);
	if (isShadowReciever == 1) {
		//続いて、ライトビュープロジェクション空間に変換。
		//ワールド座標系からライトビュー座標系に変換
		psInput.posInLVP = mul(mLightView, worldPos);
		//ライトビュー座標系からライトプロジェクション行列に変換
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
	return psInput;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
PSOutPut PSMain( PSInput In ) : SV_Target0
{
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);

	float3 lig = 0.0f;

	float rim_power = 2.0f;
	float p = 0.0f;
	//if (isDithering == 1) {
	//	if (katamariVector.z >= In.Position.z ) {
	//		//ディザリングを試す
	//		float2 pos = In.Position.xy / In.Position.w;
	//		pos *= 0.5f;
	//		pos += 0.5f;
	//		float2 uv = fmod(pos * 1000.0f, 8.0f);
	//		int t = 0;
	//		int x = (int)clamp(uv.x, 0.0f, 7.0f);
	//		int y = (int)clamp(uv.y, 0.0f, 7.0f);
	//		int index = y * 8 + x;
	//		t = pattern[index];
	//		if (t >= 30) {
	//			clip(-1);
	//		}
	//	}
	//}


	//リムライト...?
	/*if (isToomShader == 1) {
		float3 eyeVector = eyeDir;
		eyeVector = normalize(eyeVector);
		p = dot(In.Normal, eyeVector);
		p = p * 0.5f + 0.5f;
		if (p <= 0.7f) {
			lig += float3(1.0f, 1.0f, 1.0f);
		}
		else {
			lig += float3(0.5f, 0.5f, 0.5f);
		}
	}
	else {
		lig += float3(0.5f, 0.5f, 0.5f);
	}*/

	//トゥーンシェーダー
	if (isToomShader == 1) {
		p = dot(In.Normal * -1.0f, dligDirection[0].xzy);
		p = p * 0.5f + 0.5f;
		p = p * p;
		float2 pos = float2(p, 0.0f);
		float4 Col = toonMap.Sample(ToonSampler, pos);
		lig += Col.xyz * 1.0f;
		//リムライトの計算
		/*float3 eye = normalize(eyeDir);
		float rim = saturate(1.0f - dot(-eye, In.Normal));
		if (rim >= 0.75f) {
			rim = pow(rim, 3.0f);
			lig += float3(2.0f, 2.0f, 2.0f) * rim;
		}*/
		//rim = pow(rim, 3.0f);
		//lig += float3(1.0f, 1.0f, 1.0f) * rim;
	}
	else {
		lig += float3(0.5f, 0.5f, 0.5f);
	}


	//ディレクションライトの拡散反射光を計算する。
	/*for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		lig += max(0.0f, dot(In.Normal * -1.0f, dligDirection[i])) * dligColor[i];
	}
	//ディレクションライトの鏡面反射光を計算する。
	{
		//実習　鏡面反射を計算しなさい。
		//① ライトを当てる面から視点に伸びるベクトルtoEyeDirを求める。
		//	 視点の座標は定数バッファで渡されている。LightCbを参照するように。
		float3 toEyeDir = normalize(eyePos - In.worldPos);

		//② １で求めたtoEyeDirの反射ベクトルを求める。
		float3 reflectEyeDir = -toEyeDir + 2 * dot(In.Normal, toEyeDir) * In.Normal;

		//③ ２で求めた反射ベクトルとディレクションライトの方向との内積を取って、スペキュラの強さを計算する。
		float t = 0.0f;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
			t += max(0.0f, dot(-dligDirection[i], reflectEyeDir));
		}
		t /= 4;
		//④ pow関数を使って、スペキュラを絞る。絞りの強さは定数バッファで渡されている。
		//	 LightCbを参照するように。
		float3 specLig = 0.0f;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
			specLig += pow(t, specPow) * dligColor[i].xyz;
		}
		specLig /= 4;
		//⑤ スペキュラ反射が求まったら、ligに加算する。
		//鏡面反射を反射光に加算する。
		lig += specLig;
	}*/
	lig.xyz += ambientlight.xyz;
	if (isShadowReciever == 1) {	//シャドウレシーバー。
		//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
		//プロジェクション行列をシャドウマップのUV座標に変換している
		float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
	//シャドウマップのUV座標範囲内かどうかを判定する。
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {

			///LVP空間での深度値を計算。
			float zInLVP = In.posInLVP.z / In.posInLVP.w;
			//シャドウマップに書き込まれている深度値を取得。
			float zInShadowMap = shadowMap.Sample(Sampler, shadowMapUV);

			if (zInLVP > zInShadowMap + 0.001f) {
				//影が落ちているので、光を弱くする
				lig *= 0.5f;
			}
		}
	}
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;
	finalColor *= color;
	PSOutPut output;
	output.color = finalColor;
	In.Normal = In.Normal / 2.0f + 1.0f / 2.0f;
	output.normal = float4(In.Normal.x, In.Normal.y, In.Normal.z, 1.0f);
	output.depthView = In.Position.z / In.Position.w;
	return output;
	//return finalColor;
}

// <summary>
/// スキン無しシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//ライトプロジェクション行列に変換している
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
// <summary>
/// スキン無しシャドウマップ生成用の頂点シェーダー、インスタンシング用
/// </summary>
PSInput_ShadowMap VSMainInstancing_ShadowMap(VSInputNmTxVcTangent In, uint instanceID : SV_InstanceID)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//ライトプロジェクション行列に変換している
	float4 pos = mul(instanceMatrix[instanceID], In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
// <summary>
/// スキンありシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMainSkin_ShadowMap(VSInputNmTxWeights In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	float4 pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
// <summary>
/// スキンありシャドウマップ生成用の頂点シェーダー、インスタンシング用
/// </summary>
PSInput_ShadowMap VSMainSkinInstancing_ShadowMap(VSInputNmTxWeights In, uint instanceID : SV_InstanceID)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	skinning = mul(instanceMatrix[instanceID], skinning);
	float4 pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
/// <summary>
/// ピクセルシェーダーのエントリ関数。
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//射影空間でのZ値を返す。
	return In.Position.z / In.Position.w;
}




// <summary>
/// スキン無しシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_NormalMap VSMain_NormalMap(VSInputNmTxVcTangent In)
{
	PSInput_NormalMap psInput = (PSInput_NormalMap)0;
	//ライトプロジェクション行列に変換している
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.Normal = In.Normal / 2.0f + 1.0f / 2.0f;
	return psInput;
}
// <summary>
/// スキン無しシャドウマップ生成用の頂点シェーダー、インスタンシング用
/// </summary>
PSInput_NormalMap VSMainInstancing_NormalMap(VSInputNmTxVcTangent In, uint instanceID : SV_InstanceID)
{
	PSInput_NormalMap psInput = (PSInput_NormalMap)0;
	//ライトプロジェクション行列に変換している
	float4 pos = mul(instanceMatrix[instanceID], In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.Normal = In.Normal / 2.0f + 1.0f / 2.0f;
	return psInput;
}
// <summary>
/// スキンありシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_NormalMap VSMainSkin_NormalMap(VSInputNmTxWeights In)
{
	PSInput_NormalMap psInput = (PSInput_NormalMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	float4 pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.Normal = In.Normal / 2.0f + 1.0f / 2.0f;
	return psInput;
}
// <summary>
/// スキンありシャドウマップ生成用の頂点シェーダー、インスタンシング用
/// </summary>
PSInput_NormalMap VSMainSkinInstancing_NormalMap(VSInputNmTxWeights In, uint instanceID : SV_InstanceID)
{
	PSInput_NormalMap psInput = (PSInput_NormalMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	skinning = mul(instanceMatrix[instanceID], skinning);
	float4 pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.Normal = In.Normal / 2.0f + 1.0f / 2.0f;
	return psInput;
}

/// <summary>
/// ピクセルシェーダーのエントリ関数。
/// </summary>
float4 PSMain_NormalMap(PSInput_NormalMap In) : SV_Target0
{
	//射影空間でのZ値を返す。
	return float4(In.Normal.x,In.Normal.y,In.Normal.z,1.0f);
}

