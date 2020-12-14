/*****************************************************************//**
 * \file   ToonMap.h
 * \brief  ToomMapクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
/**
 * \brief トゥーンマップを読み込む.
 */
class ToonMap
{
private:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	ToonMap() {
		Init();
	}
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~ToonMap() {}
public:
	/**
	 * \brief インスタンスを取得.
	 * 
	 * \return インスタンス
	 */
	static ToonMap& GetIntance()
	{
		static ToonMap instance;
		return instance;
	}
	/**
	 * \brief 初期化処理.
	 * 
	 */
	void Init();
	/**
	 * \brief サンプラステートの初期化.
	 * 
	 */
	void InitSamplerState();
	/**
	 * \brief トゥーンシェーダー用のテクスチャを読み込む.
	 * 
	 */
	void InitTextuer();
	/**
	 * \brief シェーダーリソースビューを取得.
	 * 
	 * \return シェーダーリソースビュー
	 */
	ID3D11ShaderResourceView* GetSRV()
	{
		return m_srv;
	}
	/**
	 * \brief サンプラステートを取得.
	 * 
	 * \return サンプラステート
	 */
	ID3D11SamplerState* GetSamplerState()
	{
		return m_samplerState;
	}
private:
	ID3D11ShaderResourceView* m_srv = nullptr;				//SRV
	ID3D11SamplerState* m_samplerState = nullptr;		//トゥーンシェーダー用のサンプラステート
};

