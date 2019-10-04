#pragma once
//トゥーンシェーダー用のテクスチャを読み込むぜ
class ToonMap
{
public: 
	ToonMap() {}
	~ToonMap() {}

	//初期化
	void Init();
	/*!
	*@brief	サンプラステートの初期化。
	*/
	void InitSamplerState();
	//トゥーンシェーダー用のテクスチャを読み込む
	void InitTextuer();
	//SRVを取得
	ID3D11ShaderResourceView* GetSRV()
	{
		return m_srv;
	}
	//サンプラステートを取得
	ID3D11SamplerState* GetSamplerState()
	{
		return m_samplerState;
	}
	ID3D11ShaderResourceView* m_srv = nullptr;				//SRV
	ID3D11SamplerState* m_samplerState = nullptr;		//トゥーンシェーダー用のサンプラステート
};

