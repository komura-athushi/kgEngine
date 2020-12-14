/*****************************************************************//**
 * \file   Sprite.h
 * \brief  Spriteクラス
 * 
 * \author komurra
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/Shader.h"
/**
 * \brief 画像描画.
 */
class CSprite {
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	CSprite();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~CSprite();
	/**
	 * \brief 初期化処理.
	 * 
	 * \param fileName 画像のファイルパス
	 * \param isCircleGauge trueなら円形ゲージにする
	 */
	void Init(const wchar_t* fileName, bool isCircleGauge  = false);
	/**
	 * \brief 初期化処理.
	 * 
	 * \param srv シェーダーリソースビュー
	 */
	void Init(ID3D11ShaderResourceView* srv);
	/**
	 * \brief スクリーン座標指定で描画.
	 * 
	 * \param pos 座標、(0.0f,0.0f)で左上、デフォルトは中央
	 * \param scale 大きさ、(1.0f,1.0f)で1倍
	 * \param pivot ユニティ基準のピボット
	 * \param rotation 回転
	 * \param color 透明度
	 * \param effects デフォルトはDirectX::SpriteEffects_None
	 * \param layerDepth 描画する順番(今は機能してません。。。)
	 * \param degree ピクセルを表示する角度
	 * \param isIgnoreSplit 画面分割無視して画像描画
	 */
	void DrawScreenPos(const CVector2& pos = { 1280.0f / 2, 720.0f / 2},
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2(0.5f,0.5f),
		float rotation = 0.0f,
		const CVector4 & color = CVector4::White(),
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f,
		float degree = 0.0f,
		bool isIgnoreSplit = false
	);
	/**
	 * \brief 描画処理.
	 * 
	 */
	void Draw();
private:
	struct ConstantBuffer {
		CVector4 mulColor;
	};
	CVector2 m_screenSize = CVector2::Zero();
	DirectX::SpriteBatch* m_spriteBatch = nullptr;
	ID3D11ShaderResourceView* m_srv = nullptr;
	ID3D11Resource* m_tex = nullptr;
	unsigned int m_width, m_height;
	ID3D11Buffer* m_cb = nullptr;							//!<定数バッファ。
	ID3D11Buffer* m_dg = nullptr;							//定数バッファ(角度)
	Shader m_ps;
	float m_degree = 0.0f;
	bool m_isCircleGauge = false;							//円形ゲージにするかどうか
	RECT m_sourceRectangle;									//画像サイズ
	bool m_isNormal = false;
};