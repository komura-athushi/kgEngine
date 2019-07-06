#include "KGstdafx.h"
#include "Sprite.h"

CSprite::CSprite()
{
	m_screenSize.x = FRAME_BUFFER_W;
	m_screenSize.y = FRAME_BUFFER_H;
}

CSprite::~CSprite()
{

}

void CSprite::Init(const wchar_t* fileName)
{
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
}

void CSprite::DrawScreenPos(
	const CVector2& pos,
	const CVector2& scale,
	const CVector2& pivot,
	float rotation,
	const CVector4& color,
	DirectX::SpriteEffects effects,
	float layerDepth
)
{
	if (!m_srv) { return; }
	layerDepth *= 0.999f; layerDepth += 0.001f;
	layerDepth -= Engine().GetGraphicsEngine().AddAndGetLayerDepthCnt();
	m_spriteBatch->Draw(m_srv, pos.vec, nullptr, color, rotation, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(scale.x, scale.y), effects, layerDepth);
}

