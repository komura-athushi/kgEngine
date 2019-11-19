#include "KGstdafx.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"
#include "SkinModelEffect.h"
#include "shadow/kgShadowMap.h"

SkinModel::SkinModel()
{
	m_dirLight.direction[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
	m_dirLight.color[0] = { 0.5f, 0.5f, 0.5f, 1.0f };

	m_dirLight.direction[1] = { -0.707f, -0.707f, 0.0f, 0.0f };
	m_dirLight.color[1] = { 0.3f, 0.3f, 0.3f, 1.0f };

	m_dirLight.direction[2] = { 0.0f, -0.707f, 0.707f, 0.0f };
	m_dirLight.color[2] = { 0.3f, 0.3f, 0.3f, 1.0f };

	m_dirLight.direction[3] = { 1.0f, 0.0f, -1.0f, 0.0f };
	m_dirLight.color[3] = { 0.3f, 0.3f, 0.3f, 1.0f };
}

SkinModel::~SkinModel()
{
	if (m_cb != nullptr) {
		//�萔�o�b�t�@������B
		m_cb->Release();
	}
	//���C�g�p�̒萔�o�b�t�@�̉���B
	if (m_lightCb != nullptr) {
		m_lightCb->Release();
	}
	if (m_samplerState != nullptr) {
		//�T���v���X�e�[�g������B
		m_samplerState->Release();
	}
	if (m_instancingDataSB != nullptr) {
		//�C���X�^���V���O�p�̃X�g���N�`���[�o�b�t�@�����
		m_instancingDataSB->Release();
	}
	if (m_srv != nullptr) {
		//
		m_srv->Release();
	}
}
void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
{
	//�X�P���g���̃f�[�^��ǂݍ��ށB
	InitSkeleton(filePath);

	//�萔�o�b�t�@�̍쐬�B
	InitConstantBuffer();

	//�T���v���X�e�[�g�̏������B
	InitSamplerState();

	//SkinModelDataManager���g�p����CMO�t�@�C���̃��[�h�B
	m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

	m_enFbxUpAxis = enFbxUpAxis;

	m_toonMap = &ToonMap::GetIntance();
}
void SkinModel::InitInstancingData()
{
	if (m_maxInstance >= 1) {
		//�C���X�^���V���O�p�̃f�[�^���쐬�B
		m_instancingData.reset(new CMatrix[m_maxInstance]);
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRV�Ƃ��ăo�C���h�\�B
		desc.ByteWidth = sizeof(CMatrix) * m_maxInstance;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(CMatrix);
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = m_instancingData.get();
		Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&desc, &InitData, &m_instancingDataSB);
		{
			ID3D11Buffer* pBuf = m_instancingDataSB;
			if (pBuf != nullptr) {
				D3D11_BUFFER_DESC descBuf;
				ZeroMemory(&descBuf, sizeof(descBuf));
				pBuf->GetDesc(&descBuf);

				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				desc.BufferEx.FirstElement = 0;

				desc.Format = DXGI_FORMAT_UNKNOWN;
				desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

				Engine().GetGraphicsEngine().GetD3DDevice()->CreateShaderResourceView(pBuf, &desc, &m_srv);
			}
		}
		m_isInstancing = true;
	}
}
void SkinModel::InitSkeleton(const wchar_t* filePath)
{
	//�X�P���g���̃f�[�^��ǂݍ��ށB
	//cmo�t�@�C���̊g���q��tks�ɕύX����B
	std::wstring skeletonFilePath = filePath;
	//�����񂩂�.cmo�t�@�C���n�܂�ꏊ�������B
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmo�t�@�C����.tks�ɒu��������B
	skeletonFilePath.replace(pos, 4, L".tks");
	//tks�t�@�C�������[�h����B
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if ( result == false ) {
		//�X�P���g�����ǂݍ��݂Ɏ��s�����B
		//�A�j���[�V�������Ȃ����f���́A�X�P���g�����s�v�Ȃ̂�
		//�ǂݍ��݂Ɏ��s���邱�Ƃ͂���̂ŁA���O�o�͂����ɂ��Ă����B
#ifdef _DEBUG
		char message[256];
		sprintf(message, "tks�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
}
void SkinModel::InitConstantBuffer()
{
	//�쐬����o�b�t�@�̃T�C�Y��sizeof���Z�q�ŋ��߂�B
	int bufferSize = sizeof(SVSConstantBuffer);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC bufferDesc;
	{ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//�O�ŃN���A�B
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
	bufferDesc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
																//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
	}
	//�쐬�B
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);

	//�����āA���C�g�p�̒萔�o�b�t�@���쐬�B
	//�쐬����o�b�t�@�̃T�C�Y��ύX���邾���B
	bufferDesc.ByteWidth = sizeof(SDirectionLight);				//SDirectionLight��16byte�̔{���ɂȂ��Ă���̂ŁA�؂�グ�͂��Ȃ��B
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);
	bufferDesc.ByteWidth = ((sizeof(int) - 1) / 16 + 1) * 16;
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_isuvscroll);
	bufferDesc.ByteWidth = ((sizeof(float) - 1) / 16 + 1) * 16;
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_uvscroll);
}
void SkinModel::InitSamplerState()
{
	//�e�N�X�`���̃T���v�����O���@���w�肷�邽�߂̃T���v���X�e�[�g���쐬�B
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}
void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale)
{
	//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//���s�ړ��s����쐬����B
	transMatrix.MakeTranslation( position );
	//��]�s����쐬����B
	rotMatrix.MakeRotationFromQuaternion( rotation );
	rotMatrix.Mul(mBias, rotMatrix);
	//�g��s����쐬����B
	scaleMatrix.MakeScaling(scale);
	//���[���h�s����쐬����B
	//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
	//���Ԃ��ԈႦ���猋�ʂ��ς���B
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//�X�P���g���̍X�V�B
	if (m_isInstancing) {
		m_skeleton.Update(CMatrix::Identity());
	}
	else {
		m_skeleton.Update(m_worldMatrix);
	}
}

void SkinModel::UpdateInstancingData(
	const CVector3& pos,
	const CQuaternion& rot,
	const CVector3& scale,
	EnFbxUpAxis enUpdateAxis)
{
	UpdateWorldMatrix(pos, rot, scale);
	if (m_numInstance < m_maxInstance) {
		m_instancingData[m_numInstance] = m_worldMatrix;
		m_numInstance++;
	}
}

void SkinModel::UpdateInstancingData(const CMatrix& worldMatrix)
{
	m_worldMatrix = worldMatrix;
	//�X�P���g���̍X�V�B
	if (m_isInstancing) {
		m_skeleton.Update(CMatrix::Identity());
	}
	else {
		m_skeleton.Update(m_worldMatrix);
	}
	if (m_numInstance < m_maxInstance) {
		m_instancingData[m_numInstance] = m_worldMatrix;
		m_numInstance++;
	}
}

void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix,EnRenderMode renderMode)
{
	if (m_isInstancing && m_numInstance == 0) {
		return;
	}
	DirectX::CommonStates state(Engine().GetGraphicsEngine().GetD3DDevice());

	ID3D11DeviceContext* d3dDeviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();

	auto shadowMap = Engine().GetGraphicsEngine().GetShadowMap();

	if (m_numInstance >= 1) {
		//�C���X�^���V���O�p�̃f�[�^���X�V
		d3dDeviceContext->UpdateSubresource(m_instancingDataSB, 0 ,NULL, m_instancingData.get(), 0, 0);
		//d3dDeviceContext->VSSetConstantBuffers(3, 1, &m_instancingDataSB);
		d3dDeviceContext->VSSetShaderResources(3, 1, &m_srv);
	}

	//�萔�o�b�t�@�̓��e���X�V�B
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_worldMatrix;
	if (renderMode == enRenderMode_Normal) {
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
	}
	//�V���h�E�}�b�v�����Ƃ��͂炢�ƃJ�����̍s����g��
	else if (renderMode == enRenderMode_CreateShadowMap) {
		vsCb.mProj = shadowMap->GetLightProjMatrix();
		vsCb.mView = shadowMap->GetLightViewMatrix();
	}
	//todo ���C�g�J�����̃r���[�A�v���W�F�N�V�����s��𑗂�B
	
	vsCb.mLightProj = shadowMap->GetLightProjMatrix();
	vsCb.mLightView = shadowMap->GetLightViewMatrix();
	if (m_isShadowReceiver == true) {
		vsCb.isShadowReciever = 1;
	}
	else {
		vsCb.isShadowReciever = 0;
	}
	ID3D11ShaderResourceView* srvArray[]{
		shadowMap->GetShadowMapSRV()
	};
	//�������|�C���^�̃|�C���^�At2�Ȃ̂ň�����2�A1�ɂ��Ă�
	d3dDeviceContext->PSSetShaderResources(2, 1, srvArray);
	//�萔�o�b�t�@��GPU�ɓ]���B
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//���C�g�p�̒萔�o�b�t�@���X�V
	d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
	m_dirLight.eyePos = MainCamera().GetTarget();
	d3dDeviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_dirLight, 0, 0);
	//�T���v���X�e�[�g��ݒ�B
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//�{�[���s���GPU�ɓ]���B
	m_skeleton.SendBoneMatrixArrayToGPU();
	if (renderMode == enRenderMode_Normal) {
		ID3D11ShaderResourceView* srvArray[]{
			m_toonMap->GetSRV()
		};
		d3dDeviceContext->PSSetShaderResources(4, 1, srvArray);
		/*ID3D11SamplerState* sampArray[]{
			m_toonMap->GetSamplerState()
		};
		d3dDeviceContext->PSSetSamplers(1, 1, sampArray);*/
	}
	//UV�֌W
	//d3dDeviceContext->PSSetConstantBuffers
	//�G�t�F�N�g�ɃN�G�����s���B
	m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
		auto modelMaterial = reinterpret_cast<ModelEffect*>(material);
		modelMaterial->SetRenderMode(renderMode);
		//if (renderMode == enRenderMode_Normal) {
			if (m_isInstancing) {
				modelMaterial->SetNumInstance(m_numInstance);
			}
			else {
				modelMaterial->SetNumInstance(0);
			}
		//}
	});

	//�`��B
	if (m_numInstance == 0) {
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_worldMatrix,
			viewMatrix,
			projMatrix
		);
	}
	else {
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_worldMatrix,
			viewMatrix,
			projMatrix,
			false,
			m_numInstance
		);
	}
}