#include "KGstdafx.h"
#include "GaussianBlur.h"
#include "PostEffect.h"

void GaussianBlur::Init(ID3D11ShaderResourceView* srcTextureSRV, float blurIntensity)
{
	//ソーステクスチャのSRVをコピー。
	m_srcTextureSRV = srcTextureSRV;
	//UpdateWeight関数で使用するので、ブラーの強さをメンバ変数に記憶しておく。
	m_blurIntensity = blurIntensity;

	//ソーステクスチャの情報を取得。
	ID3D11Texture2D* tex;
	//まず、SRVに関連付けされているテクスチャを取得する。
	srcTextureSRV->GetResource((ID3D11Resource**)&tex);
	//取得したテクスチャから、テクスチャ情報を取得する。
	D3D11_TEXTURE2D_DESC texDesc;
	tex->GetDesc(&texDesc);
	//テクスチャ情報を取得できたので、テクスチャの参照カウンタをおろす。
	tex->Release();
	//ソーステクスチャの幅と高さを記憶しておく。
	m_srcTextureWidth = texDesc.Width;
	m_srcTextureHeight = texDesc.Height;

	//Xブラー用のレンダリングターゲットを作成。
	m_renderTarget[enRenderTarget_XBlur].Create(
		texDesc.Width / 2,		//横の解像度を半分にする。
		texDesc.Height,
		texDesc.Format);

	//Yブラー用のレンダリングターゲットを作成。
	m_renderTarget[enRenderTarget_YBlur].Create(
		texDesc.Width / 2,		//横の解像度を半分にする。
		texDesc.Height / 2,		//縦の解像度を半分にする。
		texDesc.Format
	);

	//シェーダーをロード。
	m_vsXBlur.Load("Assets/shader/GaussianBlur.fx", "VSXBlur", Shader::EnType::VS);
	m_vsYBlur.Load("Assets/shader/GaussianBlur.fx", "VSYBlur", Shader::EnType::VS);
	m_psBlur.Load("Assets/shader/GaussianBlur.fx", "PSBlur", Shader::EnType::PS);


	{
		//作成する定数バッファの情報を設定する。
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = (((sizeof(SBlurParam)) / 16) + 1) * 16;	//16バイトアライメントに切りあげる。
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, nullptr, &m_blurCbGpu);
	}
	

	//初期化済みの印。
	m_isInited = true;
}

void GaussianBlur::UpdateWeights()
{
	//ガウスフィルタの重みを更新する。
	float total = 0;
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_blurParam.weights[i] = expf(-0.5f * (float)(i * i) / m_blurIntensity);
		total += 2.0f * m_blurParam.weights[i];

	}
	// 規格化。重みのトータルが1.0になるように、全体の重みで除算している。
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_blurParam.weights[i] /= total;
	}
}
void GaussianBlur::Execute(PostEffect& postEffect)
{
	if (m_isInited == false) {
		//初期化できてないぞ。
		return;
	}
	auto deviceContext = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	//ガウシアンフィルターの重みテーブルを更新する。
	UpdateWeights();

	//重みテーブルを更新したので、VRAM上の定数バッファも更新する。
	//メインメモリの内容をVRAMにコピー。
	deviceContext->UpdateSubresource(
		m_blurCbGpu, 0, nullptr, &m_blurParam, 0, 0);
	//レジスタb0にm_blurCbGpuのアドレスを設定する。
	deviceContext->PSSetConstantBuffers(
		0, 1, &m_blurCbGpu);

	//Xブラー。
	{
		//Xブラー用のレンダリングターゲットに変更する。
		Engine().GetGraphicsEngine().ChangeRenderTarget(
			&m_renderTarget[enRenderTarget_XBlur],
			m_renderTarget[enRenderTarget_XBlur].GetViewport());
		//ソーステクスチャのアドレスをt0レジスタに設定する。
		deviceContext->VSSetShaderResources(0, 1, &m_srcTextureSRV);
		deviceContext->PSSetShaderResources(0, 1, &m_srcTextureSRV);

		//フルスクリーン描画を行ってブラーを実行。
		postEffect.DrawFullScreenQuadPrimitive(
			deviceContext, m_vsXBlur, m_psBlur);
	}
	//続いてYブラー
	{
		//Yブラー用のレンダリングターゲットに変更する。
		Engine().GetGraphicsEngine().ChangeRenderTarget(
			&m_renderTarget[enRenderTarget_YBlur],
			m_renderTarget[enRenderTarget_YBlur].GetViewport()
		);
		//Yブラーをかけるソーステクスチャのアドレスをt0に設定する。
		//YブラーをかけるのはXブラーをかけたテクスチャになる。
		auto srcTextureSrv = m_renderTarget[enRenderTarget_XBlur].GetRenderTargetSRV();
		deviceContext->VSSetShaderResources(0, 1, &srcTextureSrv);
		deviceContext->PSSetShaderResources(0, 1, &srcTextureSrv);

		//フルスクリーン描画を行ってブラーを実行。
		postEffect.DrawFullScreenQuadPrimitive(
			deviceContext, m_vsYBlur, m_psBlur
		);

	}
}
