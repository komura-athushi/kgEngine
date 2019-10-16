/*!
 * @brief	���f���V�F�[�_�[�B
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//�A���x�h�e�N�X�`���B
Texture2D<float4> albedoTexture : register(t0);	
Texture2D<float4> shadowMap : register(t2);		//todo �V���h�E�}�b�v�B
Texture2D<float4> toonMap : register(t4);		//toon�V�F�[�_�[�p�̃e�N�X�`���[
//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);
StructuredBuffer<float4x4> instanceMatrix : register(t3);
//UV�X�N���[���֌W
//int isuvscroll : register(t3);
//float uvscroll : register(t4);
/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);
sampler ToonSampler : register(s1);
/////////////////////////////////////////////////////////////
// �萔�o�b�t�@�B
/////////////////////////////////////////////////////////////
/*!
 * @brief	���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	//todo ���C�g�r���[�s���ǉ��B
	float4x4 mLightView;	//���C�g�r���[�s��B
	float4x4 mLightProj;	//���C�g�v���W�F�N�V�����s��B
	int isShadowReciever;	//�V���h�E���V�[�o�[�t���O�B
};
static const int NUM_DIRECTION_LIG = 4;
/// <summary>
/// ���C�g�p�̒萔�o�b�t�@�B
/// </summary>
cbuffer LightCb : register(b1) {
	float3 dligDirection[NUM_DIRECTION_LIG];
	float4 dligColor[NUM_DIRECTION_LIG];
	float4 ambientlight;
	float3 eyePos;				//�J�����̎��_�B
	float specPow;			//�X�y�L�������C�g�̍i��B
};

/// <summary>
/// �V���h�E�}�b�v�p�̒萔�o�b�t�@�B
/// </summary>
cbuffer ShadowMapCb : register(b1) {
	float4x4 lightViewProjMatrix;	//���C�g�r���[�v���W�F�N�V�����s��B
}

/////////////////////////////////////////////////////////////
//�e��\����
/////////////////////////////////////////////////////////////
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float3 Tangent  : TANGENT;				//�ڃx�N�g���B
    float2 TexCoord : TEXCOORD0;			//UV���W�B
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float2 TexCoord	: TEXCOORD0;			//UV���W�B
    float3 Tangent	: TANGENT;				//�ڃx�N�g���B
    uint4  Indices  : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
    float4 Weights  : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};

/// <summary>
/// �V���h�E�}�b�v�p�̃s�N�Z���V�F�[�_�ւ̓��͍\���́B
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//���W�B
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float4 posInLVP		: TEXCOORD1;	//���C�g�r���[�v���W�F�N�V������Ԃł̍��W�B
	float3 worldPos		: TEXCOORD2;	//���[���h���W
};
/*!
 *@brief	�X�L���s����v�Z�B
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
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	//���[�J�����W�n���烏�[���h���W�n�ɕϊ�����
	float4 worldPos = mul(mWorld, In.Position);
	//����V�@���ʔ��˂̌v�Z�̂��߂ɁA���[���h���W���s�N�Z���V�F�[�_�[�ɓn���B
	psInput.worldPos = worldPos;
	//���[���h���W�n����J�������W�n�ɕϊ�����
	psInput.Position = mul(mView, worldPos);
	//�J�������W�n����X�N���[�����W�n�ɕϊ�����
	psInput.Position = mul(mProj, psInput.Position);
	if (isShadowReciever == 1) {
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
		//���[���h���W�n���烉�C�g�r���[���W�n�ɕϊ�
		psInput.posInLVP = mul(mLightView, worldPos);
		//���C�g�r���[���W�n���烉�C�g�v���W�F�N�V�����s��ɕϊ�
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));
	return psInput;
}
//�C���X�^���V���O�p�̃X�L���Ȃ����f���p���_�V�F�[�_�[
PSInput VSMainInstancing(VSInputNmTxVcTangent In,uint instanceID : SV_InstanceID)
{
	PSInput psInput = (PSInput)0;
	//���[�J�����W�n���烏�[���h���W�n�ɕϊ�����
	float4 worldPos = mul(instanceMatrix[instanceID], In.Position);
	//����V�@���ʔ��˂̌v�Z�̂��߂ɁA���[���h���W���s�N�Z���V�F�[�_�[�ɓn���B
	psInput.worldPos = worldPos;
	//float4 worldPos = mul(mWorld, In.Position);
	//���[���h���W�n����J�������W�n�ɕϊ�����
	psInput.Position = mul(mView, worldPos);
	//�J�������W�n����X�N���[�����W�n�ɕϊ�����
	psInput.Position = mul(mProj, psInput.Position);
	if (isShadowReciever == 1) {
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
		//���[���h���W�n���烉�C�g�r���[���W�n�ɕϊ�
		psInput.posInLVP = mul(mLightView, worldPos);
		//���C�g�r���[���W�n���烉�C�g�v���W�F�N�V�����s��ɕϊ�
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(instanceMatrix[instanceID], In.Normal));
	psInput.Tangent = normalize(mul(instanceMatrix[instanceID], In.Tangent));
	return psInput;
}
/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 worldPos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //�Ō�̃{�[�����v�Z����B
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
	    worldPos = mul(skinning, In.Position);
		//����V�@���ʔ��˂̌v�Z�̂��߂ɁA���[���h���W���s�N�Z���V�F�[�_�[�ɓn���B
		psInput.worldPos = worldPos;
	}
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	//���[���h���W�n����J�������W�n�ɕϊ�����
	psInput.Position = mul(mView, worldPos);
	//�J�������W�n����X�N���[�����W�n�ɕϊ�����
	psInput.Position = mul(mProj, psInput.Position);
	if (isShadowReciever == 1) {
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
		//���[���h���W�n���烉�C�g�r���[���W�n�ɕϊ�
		psInput.posInLVP = mul(mLightView, worldPos);
		//���C�g�r���[���W�n���烉�C�g�v���W�F�N�V�����s��ɕϊ�
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//�C���X�^���V���O�p�̃X�L�����胂�f���p�̒��_�V�F�[�_�[
PSInput VSMainSkinInstancing(VSInputNmTxWeights In, uint instanceID : SV_InstanceID)
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;
	//���[�J�����W�n���烏�[���h���W�n�ɕϊ�����
	float4 worldPos = 0; 
	{

		float w = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
			skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
			w += In.Weights[i];
		}
		//�Ō�̃{�[�����v�Z����B
		skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
		skinning = mul(instanceMatrix[instanceID], skinning);
		//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
		worldPos = mul(skinning, In.Position);
		//����V�@���ʔ��˂̌v�Z�̂��߂ɁA���[���h���W���s�N�Z���V�F�[�_�[�ɓn���B
		psInput.worldPos = worldPos;
	}
	psInput.Normal = normalize(mul(skinning, In.Normal));
	psInput.Tangent = normalize(mul(skinning, In.Tangent));

	//���[���h���W�n����J�������W�n�ɕϊ�����
	psInput.Position = mul(mView, worldPos);
	//�J�������W�n����X�N���[�����W�n�ɕϊ�����
	psInput.Position = mul(mProj, psInput.Position);
	if (isShadowReciever == 1) {
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
		//���[���h���W�n���烉�C�g�r���[���W�n�ɕϊ�
		psInput.posInLVP = mul(mLightView, worldPos);
		//���C�g�r���[���W�n���烉�C�g�v���W�F�N�V�����s��ɕϊ�
		psInput.posInLVP = mul(mLightProj, psInput.posInLVP);
	}
	psInput.TexCoord = In.TexCoord;
	return psInput;
}
//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);

	float3 lig = 0.0f;

	float p = 0.0f;
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		p += max(0.0f,dot(In.Normal * -1.0f, dligDirection[i].xyz));
	}
	//p = dot(In.Normal * -1.0f, dligDirection.xzy);
	p = p * 0.5f + 0.5f;
	p = p * p;
	float2 pos = float2(p, 0.0f);
	float4 Col = toonMap.Sample(ToonSampler,pos);
	lig += Col.xyz;

	//�f�B���N�V�������C�g�̊g�U���ˌ����v�Z����B
	/*for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		lig += max(0.0f, dot(In.Normal * -1.0f, dligDirection[i])) * dligColor[i];
	}
	//�f�B���N�V�������C�g�̋��ʔ��ˌ����v�Z����B
	{
		//���K�@���ʔ��˂��v�Z���Ȃ����B
		//�@ ���C�g�𓖂Ă�ʂ��王�_�ɐL�т�x�N�g��toEyeDir�����߂�B
		//	 ���_�̍��W�͒萔�o�b�t�@�œn����Ă���BLightCb���Q�Ƃ���悤�ɁB
		float3 toEyeDir = normalize(eyePos - In.worldPos);

		//�A �P�ŋ��߂�toEyeDir�̔��˃x�N�g�������߂�B
		float3 reflectEyeDir = -toEyeDir + 2 * dot(In.Normal, toEyeDir) * In.Normal;

		//�B �Q�ŋ��߂����˃x�N�g���ƃf�B���N�V�������C�g�̕����Ƃ̓��ς�����āA�X�y�L�����̋������v�Z����B
		float t = 0.0f;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
			t += max(0.0f, dot(-dligDirection[i], reflectEyeDir));
		}
		t /= 4;
		//�C pow�֐����g���āA�X�y�L�������i��B�i��̋����͒萔�o�b�t�@�œn����Ă���B
		//	 LightCb���Q�Ƃ���悤�ɁB
		float3 specLig = 0.0f;
		for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
			specLig += pow(t, specPow) * dligColor[i].xyz;
		}
		specLig /= 4;
		//�D �X�y�L�������˂����܂�����Alig�ɉ��Z����B
		//���ʔ��˂𔽎ˌ��ɉ��Z����B
		lig += specLig;
	}*/
	lig.xyz += ambientlight.xyz;
	if (isShadowReciever == 1) {	//�V���h�E���V�[�o�[�B
		//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
		//�v���W�F�N�V�����s����V���h�E�}�b�v��UV���W�ɕϊ����Ă���
		float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
	//�V���h�E�}�b�v��UV���W�͈͓����ǂ����𔻒肷��B
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {

			///LVP��Ԃł̐[�x�l���v�Z�B
			float zInLVP = In.posInLVP.z / In.posInLVP.w;
			//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
			float zInShadowMap = shadowMap.Sample(Sampler, shadowMapUV);

			if (zInLVP > zInShadowMap + 0.01f) {
				//�e�������Ă���̂ŁA�����キ����
				lig *= 0.5f;
			}
		}
	}
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}

// <summary>
/// �X�L�������V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
/// </summary>
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//���C�g�v���W�F�N�V�����s��ɕϊ����Ă���
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
// <summary>
/// �X�L�������V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�A�C���X�^���V���O�p
/// </summary>
PSInput_ShadowMap VSMainInstancing_ShadowMap(VSInputNmTxVcTangent In, uint instanceID : SV_InstanceID)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//���C�g�v���W�F�N�V�����s��ɕϊ����Ă���
	float4 pos = mul(instanceMatrix[instanceID], In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
// <summary>
/// �X�L������V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
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
/// �X�L������V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�A�C���X�^���V���O�p
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
/// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//�ˉe��Ԃł�Z�l��Ԃ��B
	return In.Position.z / In.Position.w;
}
