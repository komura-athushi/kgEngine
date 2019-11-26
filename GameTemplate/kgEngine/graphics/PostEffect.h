#pragma once
#include "Bloom.h"
//ポストエフェクトクラス
class PostEffect
{
public:
	//コンストラクタ
	PostEffect();
	//デストラクタ
	~PostEffect();
	//更新
	void Update();
	//ドロー
 	void Draw();
	//フルスクリーン描画
	void DrawFullScreenQuadPrimitive(ID3D11DeviceContext* deviceContext, Shader& vsShader, Shader& psShader);
	//フルスクリーン描画用の四角形プリミティブの初期化
	void InitFullScreenQuadPrimitive();
	//描画用の四角形プリミティブの初期化
	void InitScreenQuadPrimitive(CVector2 pos1, const CVector2& pos2, const CVector2& pos3, const CVector2& pos4);
private:
	Bloom	m_bloom;  //ブルーム
	ID3D11Buffer* m_vertexBuffer = nullptr;		//頂点バッファ。
	ID3D11InputLayout* m_inputLayout = nullptr;
	bool m_isInit = false;
};

