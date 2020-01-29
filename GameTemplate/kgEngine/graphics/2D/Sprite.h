#pragma once
#include "graphics/Shader.h"
//スプライト
class CSprite {
public:
	//コンストラクタ
	CSprite();
	//デストラクタ
	~CSprite();
	//初期化
	void Init(const wchar_t* fileName, bool isCircleGauge  = false);
	//初期化
	void Init(ID3D11ShaderResourceView* srv);

	/// <summary>
	/// スクリーン座標指定で描画
	/// </summary>
	/// <param name="pos">座標、(0.0f,0.0f)で左上、デフォルトは中央</param>
	/// <param name="scale">大きさ、(1.0f,1.0f)で1倍</param>
	/// <param name="pivot">ユニティ基準のピボット</param>
	/// <param name="rotation">回転</param>
	/// <param name="color">透明度(z)</param>
	/// <param name="effects">デフォルトはDirectX::SpriteEffects_None</param>
	/// <param name="layerDepth">描画する順番(今は機能してません。。。)</param>
	/// <param name="degree">ピクセルを表示する角度</param>
	/// <param name="isIgnoreSplit">画面分割無視して画像描画</param>
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