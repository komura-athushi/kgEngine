#include "KGstdafx.h"
#include "Sprite.h"

CSprite::CSprite()
{
	m_screenSize.x = FRAME_BUFFER_W;
	m_screenSize.y = FRAME_BUFFER_H;
}

CSprite::~CSprite()
{
	if (m_srv != nullptr) {
		m_srv->Release();
	}
	if (m_cb != nullptr) {
		m_cb->Release();
	}
	if (m_tex != nullptr) {
		m_tex->Release();
	}
	if (m_dg != nullptr) {
		m_dg->Release();
	}
}

void CSprite::Init(ID3D11ShaderResourceView* srv)
{
	m_isNormal = true;
	m_srv = srv;
	if (m_srv != nullptr) {
		m_srv->AddRef();	//参照カウンタを増やす。
	}
	m_spriteBatch = Engine().GetGraphicsEngine().GetSpriteBatch();
}

void CSprite::Init(const wchar_t* fileName, bool isCircleGauge)
{
	m_isCircleGauge = isCircleGauge;
	HRESULT hr;
	hr = DirectX::CreateDDSTextureFromFile(Engine().GetGraphicsEngine().GetD3DDevice(), fileName, &m_tex, &m_srv);
	m_spriteBatch = Engine().GetGraphicsEngine().GetSpriteBatch();
	if (FAILED(hr)) {
#ifdef BUILD_LEVEL_MASTER
		char message[256];
		sprintf_s(message, "画像読み込みに失敗。\nファイルパスあってますか？\n%ls\n", fileName);
		MessageBox(NULL, message, "Error", MB_OK);
		std::abort();
#endif
	}
	//画像サイズの取得
	D3D11_RESOURCE_DIMENSION resType = D3D11_RESOURCE_DIMENSION_UNKNOWN;
	m_tex->GetType(&resType);
	switch (resType) {
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		ID3D11Texture2D* tex2d = static_cast<ID3D11Texture2D*>(m_tex);
		D3D11_TEXTURE2D_DESC desc;
		tex2d->GetDesc(&desc);

		//画像サイズの取得
		m_width = desc.Width;
		m_height = desc.Height;
		m_sourceRectangle.top = 0;
		m_sourceRectangle.left = 0;
		m_sourceRectangle.bottom = desc.Height;
		m_sourceRectangle.right = desc.Width;
	}
	break;
	default:
		break;
	}
	//作成するバッファのサイズをsizeof演算子で求める。
	int bufferSize = sizeof(ConstantBuffer);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc;
	{
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
		bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																	//アライメントって→バッファのサイズが16の倍数ということです。
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																	//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
		bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
		Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
	}											//CPUアクセスが不要な場合は0。
	int bufferSize2 = sizeof(float);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc2;
	{
		ZeroMemory(&bufferDesc2, sizeof(bufferDesc2));				//０でクリア。
		bufferDesc2.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
		bufferDesc2.ByteWidth = (((bufferSize2 - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																		//アライメントって→バッファのサイズが16の倍数ということです。
		bufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																		//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
		bufferDesc2.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
	}
	m_ps.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);
	//作成。
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc2, NULL, &m_dg);
}

void CSprite::DrawScreenPos(
	const CVector2& pos,
	const CVector2& scale,
	const CVector2& pivot,
	float rotation,
	const CVector4& color,
	DirectX::SpriteEffects effects,
	float layerDepth,
	float degree
)
{
	if (m_srv == nullptr) { return; }
	ConstantBuffer cb;
	cb.mulColor = color;
	layerDepth *= 0.999f; layerDepth += 0.001f;
	layerDepth -= Engine().GetGraphicsEngine().AddAndGetLayerDepthCnt();
	auto device = Engine().GetGraphicsEngine().GetD3DDeviceContext();

	Engine().GetGraphicsEngine().GetSpriteBatch()->Begin(DirectX::SpriteSortMode_BackToFront, nullptr, nullptr, nullptr, nullptr, [=]
		{
			device->UpdateSubresource(m_cb, 0, nullptr, &cb, 0, 0);
			device->PSSetConstantBuffers(0, 0, &m_cb);
			float Angle;
			if (m_isCircleGauge) {
				Angle = degree * CMath::PI / 180.0f;
			}
			else {
				Angle = CMath::PI * 2;
			}
			device->UpdateSubresource(m_dg, 0, nullptr, &Angle, 0, 0);
			device->PSSetConstantBuffers(1, 1, &m_dg);
			device->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), nullptr, 0);
		}

	);
	//Engine().GetGraphicsEngine().GetSpriteBatch()->Begin();
	m_spriteBatch->Draw(m_srv, pos.vec, &m_sourceRectangle, color, rotation, DirectX::XMFLOAT2(pivot.x * m_width, pivot.y * m_height), DirectX::XMFLOAT2(scale.x, scale.y), effects, layerDepth);
	//m_spriteBatch->Draw(m_srv, pos.vec);
	//m_spriteBatch->Draw(m_srv, DirectX::XMFLOAT2(0.0f,0.0f));
	Engine().GetGraphicsEngine().GetSpriteBatch()->End();
}

void CSprite::Draw()
{
	Engine().GetGraphicsEngine().GetSpriteBatch()->Begin();
	m_spriteBatch->Draw(m_srv, DirectX::XMFLOAT2(0.0f, 0.0f));
	m_spriteBatch->End();
}

