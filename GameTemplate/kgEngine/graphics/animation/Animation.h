/*!
 * @brief	アニメーション
 */

#pragma once

#include "graphics/animation/AnimationClip.h"
#include "graphics/animation/AnimationPlayController.h"



class Skeleton;
class SkinModel;


/*!
* @brief	アニメーションクラス。
*/
class Animation {
public:
	Animation();
	~Animation();
	/*!
		*@brief	初期化。
		*@param[in]	skinModel		アニメーションさせるスキンモデル。
		*@param[in]	animeClipList	アニメーションクリップの配列。
		*@param[in]	numAnimClip		アニメーションクリップの数。
		*/
	void Init(SkinModel& skinModel, AnimationClip animClipList[], int numAnimClip);

		
	/*!
	*@brief	アニメーションの再生。
	*@param[in]	clipNo	アニメーションクリップの番号。Init関数に渡したanimClipListの並びとなる。
	*@param[in]	interpolateTime		補完時間(単位：秒)
	*/
	void Play(int clipNo, float interpolateTime = 0.0f)
	{
		PlayCommon(m_animationClips[clipNo], interpolateTime);
	}
	/*!
	* @brief	アニメーションの再生中？
	*/
	bool IsPlaying() const
	{
		int lastIndex = GetLastAnimationControllerIndex();
		return m_animationPlayController[lastIndex].IsPlaying();
	}
	
	/*!
	* @brief	アニメーションを進める。
	*@details
	* エンジン内部から呼ばれます。
	* ユーザーは使用しないでください。
	*@param[in]	deltaTime		アニメーションを進める時間(単位：秒)。
	*/
	void Update(float deltaTime);
	
private:
	void PlayCommon(AnimationClip* nextClip, float interpolateTime)
	{
		int index = GetLastAnimationControllerIndex();
		if (m_animationPlayController[index].GetAnimClip() == nextClip) {
			return;
		}
		if (interpolateTime == 0.0f) {
			//補完なし。
			m_numAnimationPlayController = 1;
		}
		else {
			//補完あり。
			m_numAnimationPlayController++;
		}
		index = GetLastAnimationControllerIndex();
		m_animationPlayController[index].ChangeAnimationClip(nextClip);
		m_animationPlayController[index].SetInterpolateTime(interpolateTime);
		m_interpolateTime = 0.0f;
		m_interpolateTimeEnd = interpolateTime;
	}
	/*!
		* @brief	ローカルポーズの更新。
		*/
	void UpdateLocalPose(float deltaTime);
	/*!
		* @brief	グローバルポーズの更新。
		*/
	void UpdateGlobalPose();
		
private:
		
	/*!
		*@brief	最終ポーズになるアニメーションのリングバッファ上でのインデックスを取得。
		*/
	int GetLastAnimationControllerIndex() const
	{
		return GetAnimationControllerIndex(m_startAnimationPlayController, m_numAnimationPlayController - 1);
	}
	/*!
	*@brief	アニメーションコントローラのリングバッファ上でのインデックスを取得。
	*@param[in]	startIndex		開始インデックス。
	*@param[in]	localIndex		ローカルインデックス。
	*/
	int GetAnimationControllerIndex(int startIndex, int localIndex) const
	{
		return (startIndex + localIndex) % ANIMATION_PLAY_CONTROLLER_NUM;
	}
private:
	static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;			//!<アニメーションコントローラの数。
	std::vector<AnimationClip*>	m_animationClips;					//!<アニメーションクリップの配列。
	Skeleton* m_skeleton = nullptr;	//!<アニメーションを適用するスケルトン。
	AnimationPlayController	m_animationPlayController[ANIMATION_PLAY_CONTROLLER_NUM];	//!<アニメーションコントローラ。リングバッファ。
	int m_numAnimationPlayController = 0;		//!<現在使用中のアニメーション再生コントローラの数。
	int m_startAnimationPlayController = 0;		//!<アニメーションコントローラの開始インデックス。
	float m_interpolateTime = 0.0f;
	float m_interpolateTimeEnd = 0.0f;
	bool m_isInterpolate = false;				//!<補間中？

};
