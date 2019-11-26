#include "KGstdafx.h"
#include "PostEffect.h"

PostEffect::PostEffect()
{

}


PostEffect::~PostEffect()
{
	if (m_vertexBuffer != nullptr) {
		m_vertexBuffer->Release();
	}
}

void PostEffect::Update()
{
}

void PostEffect::Draw()
{
	m_bloom.Draw(*this);
}

struct SVertex {
	float position[4];		//頂点座標。
	float uv[2];			//UV座標。これがテクスチャ座標
};
void PostEffect::InitFullScreenQuadPrimitive()
{
	if (m_isInit) {
		return;
	}
	//座標を適当に弄る
	//頂点バッファを初期化。
	SVertex vertex[4] = {
		//頂点１
		{
			//座標　position[4]
			-1.0f,  -1.0f, 0.0f, 1.0f,
			//UV座標 uv[2]
			0.0f, 1.0f
		},
		//頂点２
		{
			//座標　position[4]
			1.0f, -1.0f, 0.0f, 1.0f,
			//UV座標 uv[2]
			1.0f, 1.0f
		},
		//頂点３
		{
			//座標　position[4]
			-1.0f,  1.0f, 0.0f, 1.0f,
			//UV座標 uv[2]
			0.0f, 0.0f
		},
		//頂点４
		{
			//座標　position[4]
			1.0f,  1.0f, 0.0f, 1.0f,
			//UV座標 uv[2]
			1.0f, 0.0f
		},
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//構造体を0で初期化する。
	bd.Usage = D3D11_USAGE_DEFAULT;				//バッファーで想定されている読み込みおよび書き込みの方法。
												//取りあえずはD3D11_USAGE_DEFAULTでよい。
	bd.ByteWidth = sizeof(vertex);				//頂点バッファのサイズ。頂点のサイズ×頂点数となる。
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//これから作成するバッファが頂点バッファであることを指定する。

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	//頂点バッファの作成。
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
	m_isInit = true;
}

void PostEffect::InitScreenQuadPrimitive(CVector2 pos1, const CVector2& pos2, const CVector2& pos3, const CVector2& pos4)
{
	if (m_isInit) {
		return;
	}
	//座標を適当に弄る
	//頂点バッファを初期化。
	SVertex vertex[4] = {
		//頂点１
		{
			//座標　position[4]
			pos1.x,  pos1.y, 0.0f, 1.0f,
			//UV座標 uv[2]
			0.0f, 1.0f
		},
		//頂点２
		{
			//座標　position[4]
			pos2.x, pos2.y, 0.0f, 1.0f,
			//UV座標 uv[2]
			1.0f, 1.0f
		},
		//頂点３
		{
			//座標　position[4]
			pos3.x,  pos3.y, 0.0f, 1.0f,
			//UV座標 uv[2]
			0.0f, 0.0f
		},
		//頂点４
		{
			//座標　position[4]
			pos4.x,  pos4.y, 0.0f, 1.0f,
			//UV座標 uv[2]
			1.0f, 0.0f
		},
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//構造体を0で初期化する。
	bd.Usage = D3D11_USAGE_DEFAULT;				//バッファーで想定されている読み込みおよび書き込みの方法。
												//取りあえずはD3D11_USAGE_DEFAULTでよい。
	bd.ByteWidth = sizeof(vertex);				//頂点バッファのサイズ。頂点のサイズ×頂点数となる。
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//これから作成するバッファが頂点バッファであることを指定する。

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	//頂点バッファの作成。
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
	m_isInit = true;
}

void PostEffect::DrawFullScreenQuadPrimitive(ID3D11DeviceContext* deviceContext, Shader& vsShader, Shader& psShader)
{
	//プリミティブのトポロジーとして、トライアングルストリップを設定する。
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	unsigned int vertexSize = sizeof(SVertex);
	unsigned int offset = 0;
	//輝度抽出用のシェーダーを設定する。
	deviceContext->VSSetShader(
		(ID3D11VertexShader*)vsShader.GetBody(), nullptr, 0
	);
	deviceContext->PSSetShader(
		(ID3D11PixelShader*)psShader.GetBody(), nullptr, 0
	);
	deviceContext->IASetInputLayout(vsShader.GetInputLayout());

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->Draw(4, 0);
}