/*****************************************************************//**
 * \file   Anim.h
 * \brief Animクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "Object/Move/IMove.h"
/**
 * \brief オブジェクトのアニメーションの制御する.
 */
class Anim
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Anim();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Anim();
	/**
	 * \brief 初期化処理.
	 * 
	 * \param filepath アニメーションのファイルパス
	 * \param skin スキンモデルレンダラー
	 */
	void Init(const wchar_t* filepath,CSkinModelRender* skin);
	/**
	 * \brief アニメーションを再生.
	 * 
	 * \param state 移動状態を表すステート
	 */
	void PlayAnimation(EnMove state);
	/**
	 * \brief 現在再生中のアニメーションを取得.
	 * 
	 * \return 再生中のアニメーションの番号
	 */
	int GetPlayAnimationType()
	{
		return m_animationState;
	}
private:
	CSkinModelRender* m_skinModelRender = nullptr;				//モデル
	//アニメーション
	enum EnAnimationClip {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_num
	};
	AnimationClip m_animClip[enAnimationClip_num];				//アニメーションクリップ
	EnAnimationClip m_animationState = enAnimationClip_idle;	//再生中のアニメーションの番号
	bool m_isInit = false;										//初期化した？
};

