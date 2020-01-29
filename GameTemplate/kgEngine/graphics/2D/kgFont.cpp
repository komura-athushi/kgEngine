#include "KGstdafx.h"
#include "kgFont.h"

DirectX::SpriteFont* FontFactory::Load(const wchar_t* path)
{
	int key = Util::MakeHash(path);
	if (m_fontmap.count(key) == 0) {
		//std::make_unique  スマートポインタ生成のヘルパー関数
		//newが不要になる、()に初期値
		m_fontmap.emplace(key, std::make_unique<DirectX::SpriteFont>(Engine().GetGraphicsEngine().GetD3DDevice(),path));
	}
	//getでマッピングされている値
	return m_fontmap[key].get();
}

FontFactory CFont::m_fontfactory;

CFont::CFont()
{
	m_spriteBatch = Engine().GetGraphicsEngine().GetSpriteBatch();
	m_spriteFont = Engine().GetGraphicsEngine().GetSpriteFont();
	m_screenSize.x = FRAME_BUFFER_W;
	m_screenSize.y = FRAME_BUFFER_H;
}

CFont::~CFont()
{

}

void CFont::Draw(
	wchar_t const* text,
	const CVector2& pos,
	const CVector4& color,
	const CVector2& scale,
	const CVector2& pivot,
	float rotation,
	DirectX::SpriteEffects effects,
	float layerDepth
) {
	DrawScreenPos(text, { pos.x * m_screenSize.x, pos.y * m_screenSize.y }, color, scale, pivot, rotation, effects, layerDepth);
}

void CFont::DrawScreenPos(
	wchar_t const* text,
	const CVector2& pos,
	const CVector4& color,
	const CVector2& scale,
	const CVector2& pivot,
	float rotation,
	DirectX::SpriteEffects effects,
	float layerDepth,
	bool isIgnoreSplit
)
{
	if (text == nullptr) {
		return;
	}

	layerDepth *= 0.999f; layerDepth += 0.001f;
	layerDepth -= Engine().GetGraphicsEngine().AddAndGetLayerDepthCnt();
	CVector2 position = pos;
	CVector2 size = scale;

	if (Engine().GetGraphicsEngine().GetSplitNumber() != 1 && !isIgnoreSplit) {
		//position.y /= Engine().GetGraphicsEngine().GetSplitNumber();
		//size.y /= Engine().GetGraphicsEngine().GetSplitNumber();
	}
	Engine().GetGraphicsEngine().GetSpriteBatch()->Begin(DirectX::SpriteSortMode_BackToFront);
	m_spriteFont->DrawString(
		m_spriteBatch,
		text,
		position.vec,
		color,
		rotation,
		DirectX::XMFLOAT2(pivot.x, pivot.y),
		DirectX::XMFLOAT2(size.x, size.y),
		effects,
		layerDepth
	);
	Engine().GetGraphicsEngine().GetSpriteBatch()->End();
}

