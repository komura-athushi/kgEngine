/*****************************************************************//**
 * \file   kgFont.h
 * \brief  CFontクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
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

/**
 * \brief フォントを描画.
 */
class CFont
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	CFont();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~CFont();
	/**
	 * \brief 描画する.
	 * 
	 * \param text 文字
	 * \param pos 座標、{0.0f,0.0f}～{1.0f,1.0f}の間
	 * \param color 色
	 * \param scale 大きさ
	 * \param pivot ピボット
	 * \param rotation 回転
	 * \param effects デフォルトはDirectX::SpriteEffects_None
	 * \param layerDepth 描画する順番(今は機能してません。。。)
	 */
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
	/**
	 * \brief 描画、座標をスクリーン座標で指定 .
	 * 
	 * \param text 文字
	 * \param pos スクリーン座標
	 * \param color 色
	 * \param scale 大きさ
	 * \param pivot ピボット
	 * \param rotation 回転
	 * \param effects デフォルトはDirectX::SpriteEffects_None
	 * \param layerDepth 描画する順番
	 * \param isIgnoreSplit 画面分割無視して画像描画
	 */
	void DrawScreenPos(
		wchar_t const* text,
		const CVector2& pos,
		const CVector4& color = CVector4::White(),
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f,
		bool isIgnoreSplit = false
	);
private:
	DirectX::SpriteBatch* m_spriteBatch = nullptr;
	DirectX::SpriteFont* m_spriteFont = nullptr;
	CVector2 m_screenSize;
	Shader m_ps;
	static FontFactory m_fontfactory;
	ID3D11Buffer* m_cb = nullptr;							//!<定数バッファ。
};

