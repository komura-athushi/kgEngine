#pragma once

class Fade:public IGameObject
{
	//シングルトン、そのクラスのインスタンスが一つしか存在しえないことを示すデザインパターンの一つです
private:
	//コンストラクタとデストラクタをprivateに宣言することで外部から生成出来なくします
	Fade();
	~Fade();
public:
	//この関数を使ってGameDataクラスのインスタンスを生成します
	//この関数内のローカルな静的変数は最初に関数が呼ばれるときには初期化されますが、以降呼ばれる時は初期化されません
	static Fade& GetInstance()
	{
		//privateなコンストラクタを呼び出す
		static Fade inst;
		return inst;
	}
	bool Start() override;
	void Update() override;
	void PostRender() override;
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	void StartFadeSlowIn()
	{
		m_state = enState_FadeSlowIn;
	}
	void StartFadeSlowOut()
	{
		m_state = enState_FadeSlowOut;
	}
	bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}

private:
	enum EnState {
		enState_FadeIn,			//!<フェードイン中。
		enState_FadeSlowIn,     //フェードイン中(スロウ)
		enState_FadeOut,		//!<フェードアウト中。
		enState_FadeSlowOut,    //フェードアウト中(スロウ)
		enState_Idle,		//!<アイドル中。
	};
	CSprite m_sprite;
	EnState m_state = enState_Idle;	//!<状態。
	float m_currentAlpha = 1.0f;		//!<現在のα値。
};

