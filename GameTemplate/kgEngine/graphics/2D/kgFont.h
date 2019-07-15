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
	void LoadFont(const wchar_t* path) 
	{
		m_spriteFont = m_fontfactory.Load(path);
	}
	/// <summary>
	/// 描画する
	/// </summary>
	/// <param name="text">文字</param>
	/// <param name="pos">座標、{0.0f,0.0f}～{1.0f,1.0f}の間</param>
	/// <param name="color">色</param>
	/// <param name="scale">大きさ</param>
	/// <param name="pivot">ピボット</param>
	/// <param name="rotation">回転</param>
	/// <param name="effects">デフォルトはDirectX::SpriteEffects_None</param>
	/// <param name="layerDepth">描画する順番</param>
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
	/// <summary>
	///描画、座標をスクリーン座標で指定 
	/// </summary>
	/// <param name="text">文字</param>
	/// <param name="pos">スクリーン座標</param>
	/// <param name="color">色</param>
	/// <param name="scale">大きさ</param>
	/// <param name="pivot">ピボット</param>
	/// <param name="rotation">回転</param>
	/// <param name="effects">デフォルトはDirectX::SpriteEffects_None</param>
	/// <param name="layerDepth">描画する順番</param>
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

