/*****************************************************************//**
 * \file   Fade.h
 * \brief  Fadeクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

/**
 * \brief フェードを管理.
 */
class Fade:public IGameObject
{
private:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Fade();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Fade();
public:
	/**
	 * \brief インスタンスを取得.
	 * 
	 * \return 
	 */
	static Fade& GetInstance()
	{
		//privateなコンストラクタを呼び出す
		static Fade inst;
		return inst;
	}
	/**
	 * \brief 初期化.
	 * 
	 * \return trueで初期化完了
	 */
	bool Start() override final;
	/**
	 * \brief 更新処理.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief 描画処理.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief スタートフェードイン.
	 * 
	 */
	void StartFadeIn()
	{
		m_state = enState_FadeOutLoading;
	}
	/**
	 * \brief スタートフェードアウト.
	 * 
	 */
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	/**
	 * \brief スタートスローフェードイン.
	 * 
	 */
	void StartFadeSlowIn()
	{
		m_state = enState_FadeSlowIn;
	}
	/**
	 * \brief スタートスローフェードアウト.
	 * 
	 */
	void StartFadeSlowOut()
	{
		m_state = enState_FadeSlowOut;
	}
	/**
	 * \brief フェード中？.
	 * 
	 * \return フェードしてたらtrue
	 */
	bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	/**
	 * \brief アルファ値を取得.
	 * 
	 * \return アルファ値
	 */
	float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}
	/**
	 * \brief フェードイン中？.
	 * 
	 * \return フェードイン中ならtrue
	 */
	bool IsFadeIn() const
	{
		return m_state == enState_FadeIn;
	}
	/**
	 * \brief 待機中？.
	 * 
	 * \return 待機中ならtrue
	 */
	bool IsIdle() const
	{
		return m_state == enState_Idle;
	}
private:
	enum EnState {
		enState_FadeIn,			//!<フェードイン中。
		enState_FadeSlowIn,     //フェードイン中(スロウ)
		enState_FadeOut,		//!<フェードアウト中。
		enState_FadeSlowOut,    //フェードアウト中(スロウ)
		enState_FadeInLoading,  //文字をフェードイン
		enState_FadeOutLoading, //文字をフェードアウト
		enState_Idle,		//!<アイドル中。

	};
	CSprite m_sprite;						//スプライト
	CSprite m_loading;
	EnState m_state = enState_Idle;	//!<状態。
	float m_currentAlpha = 0.0f;		//!<現在のα値。
	CVector2 m_position = CVector2(FRAME_BUFFER_W / 2, FRAME_BUFFER_H / 2);
};

