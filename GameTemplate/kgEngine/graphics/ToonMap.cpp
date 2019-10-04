#include "KGstdafx.h"
#include "ToonMap.h"

void ToonMap::Init()
{
	InitSamplerState();
	InitTextuer();
}

void ToonMap::InitSamplerState()
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

void ToonMap::InitTextuer()
{
	HRESULT hr;
	hr = DirectX::CreateDDSTextureFromFile(Engine().GetGraphicsEngine().GetD3DDevice(), L"Assets/shader/toonmap.dds", nullptr, &m_srv);
	if (FAILED(hr)) {
#ifdef BUILD_LEVEL_MASTER
		char message[256];
		sprintf_s(message, "画像読み込みに失敗。\nファイルパスあってますか？\n%ls\n", L"Assets/shader/toonmap.dds");
		MessageBox(NULL, message, "Error", MB_OK);
		std::abort();
#endif
	}
}