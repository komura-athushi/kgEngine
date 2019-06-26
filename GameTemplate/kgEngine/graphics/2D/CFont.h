#pragma once

//SpriteのFactory
class FontFactory {
public:
	DirectX::SpriteFont* Load(const wchar_t* path);
private:
	//unique_ptr  スマートポインタ、newしたメモリを指すポインタが存在しなければ自動的に
	//deleteされる
	std::unordered_map<int, std::unique_ptr<DirectX::SpriteFont>> m_fontmap;
};

class CFont
{
public:
	CFont();
	~CFont();
	//フォントをロードする
	void LoadFont(const wchar_t* path) {
		m_spriteFont = m_fontfactory.Load(path);
	}
	//初期設定のシステムフォントを使用する
	void UseSystemFont()
	{
		m_spriteFont = Engine().GetGraphicsEngine().GetSpriteFont();
	}

	//描画する
		//※layerDepthの第三小数点以下は使わないほうがいいと思う
		//const CVector2& pos は0.0f～1.0fの範囲。{0.0f,0.0f}で画面左上
	void Draw(
		wchar_t const* text,
		const CVector2& pos,
		const CVector4& color = CVector4::White(),
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f
	);
	//const CVector2& pos をスクリーン座標で指定する版。右下の座標=画面解像度　
	void DrawScreenPos(
		wchar_t const* text,
		const CVector2& pos,
		const CVector4& color = CVector4::White(),
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f
	);
private:
	DirectX::SpriteBatch* m_spriteBatch = nullptr;
	DirectX::SpriteFont* m_spriteFont = nullptr;
	CVector2 m_screenSize;

	static FontFactory m_fontfactory;

};

