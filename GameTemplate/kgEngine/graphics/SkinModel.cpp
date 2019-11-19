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
		//定数バッファを解放。
		m_cb->Release();
	}
	//ライト用の定数バッファの解放。
	if (m_lightCb != nullptr) {
		m_lightCb->Release();
	}
	if (m_samplerState != nullptr) {
		//サンプラステートを解放。
		m_samplerState->Release();
	}
	if (m_instancingDataSB != nullptr) {
		//インスタンシング用のストラクチャーバッファを解放
		m_instancingDataSB->Release();
	}
	if (m_srv != nullptr) {
		//
		m_srv->Release();
	}
}
void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
{
	//スケルトンのデータを読み込む。
	InitSkeleton(filePath);

	//定数バッファの作成。
	InitConstantBuffer();

	//サンプラステートの初期化。
	InitSamplerState();

	//SkinModelDataManagerを使用してCMOファイルのロード。
	m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

	m_enFbxUpAxis = enFbxUpAxis;

	m_toonMap = &ToonMap::GetIntance();
}
void SkinModel::InitInstancingData()
{
	if (m_maxInstance >= 1) {
		//インスタンシング用のデータを作成。
		m_instancingData.reset(new CMatrix[m_maxInstance]);
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRVとしてバインド可能。
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
	//スケルトンのデータを読み込む。
	//cmoファイルの拡張子をtksに変更する。
	std::wstring skeletonFilePath = filePath;
	//文字列から.cmoファイル始まる場所を検索。
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmoファイルを.tksに置き換える。
	skeletonFilePath.replace(pos, 4, L".tks");
	//tksファイルをロードする。
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if ( result == false ) {
		//スケルトンが読み込みに失敗した。
		//アニメーションしないモデルは、スケルトンが不要なので
		//読み込みに失敗することはあるので、ログ出力だけにしておく。
#ifdef _DEBUG
		char message[256];
		sprintf(message, "tksファイルの読み込みに失敗しました。%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
}
void SkinModel::InitConstantBuffer()
{
	//作成するバッファのサイズをsizeof演算子で求める。
	int bufferSize = sizeof(SVSConstantBuffer);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc;
	{ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																//アライメントって→バッファのサイズが16の倍数ということです。
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
	bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
																//CPUアクセスが不要な場合は0。
	}
	//作成。
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);

	//続いて、ライト用の定数バッファを作成。
	//作成するバッファのサイズを変更するだけ。
	bufferDesc.ByteWidth = sizeof(SDirectionLight);				//SDirectionLightは16byteの倍数になっているので、切り上げはやらない。
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);
	bufferDesc.ByteWidth = ((sizeof(int) - 1) / 16 + 1) * 16;
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_isuvscroll);
	bufferDesc.ByteWidth = ((sizeof(float) - 1) / 16 + 1) * 16;
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_uvscroll);
}
void SkinModel::InitSamplerState()
{
	//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
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
	//3dsMaxと軸を合わせるためのバイアス。
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation( position );
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion( rotation );
	rotMatrix.Mul(mBias, rotMatrix);
	//拡大行列を作成する。
	scaleMatrix.MakeScaling(scale);
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算するように！
	//順番を間違えたら結果が変わるよ。
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//スケルトンの更新。
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
	//スケルトンの更新。
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
		//インスタンシング用のデータを更新
		d3dDeviceContext->UpdateSubresource(m_instancingDataSB, 0 ,NULL, m_instancingData.get(), 0, 0);
		//d3dDeviceContext->VSSetConstantBuffers(3, 1, &m_instancingDataSB);
		d3dDeviceContext->VSSetShaderResources(3, 1, &m_srv);
	}

	//定数バッファの内容を更新。
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_worldMatrix;
	if (renderMode == enRenderMode_Normal) {
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
	}
	//シャドウマップを作るときはらいとカメラの行列を使う
	else if (renderMode == enRenderMode_CreateShadowMap) {
		vsCb.mProj = shadowMap->GetLightProjMatrix();
		vsCb.mView = shadowMap->GetLightViewMatrix();
	}
	//todo ライトカメラのビュー、プロジェクション行列を送る。
	
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
	//引数がポインタのポインタ、t2なので引数を2、1にしてる
	d3dDeviceContext->PSSetShaderResources(2, 1, srvArray);
	//定数バッファをGPUに転送。
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//ライト用の定数バッファを更新
	d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
	m_dirLight.eyePos = MainCamera().GetTarget();
	d3dDeviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_dirLight, 0, 0);
	//サンプラステートを設定。
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//ボーン行列をGPUに転送。
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
	//UV関係
	//d3dDeviceContext->PSSetConstantBuffers
	//エフェクトにクエリを行う。
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

	//描画。
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