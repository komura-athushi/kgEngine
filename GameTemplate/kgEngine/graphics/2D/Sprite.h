#pragma once
#include "graphics/Shader.h"
//スプライト
class CSprite {
public:
	//デフォルトのUV座標
	static const CVector2 DEFAULT_PIVOT;
	//コンストラクタ
	CSprite();
	//デストラクタ
	~CSprite();
	//初期化
	void Init(const wchar_t* fileName);
	//スクリーン座標指定で画像を描画
	void DrawScreenPos(const CVector2& pos,
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		const CVector4 & color = CVector4::White(),
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f);
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
	Shader m_ps;
	Shader m_vs;
	CVector4 m_mulColor = CVector4::Blue();				//乗算カラー
};