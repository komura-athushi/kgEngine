/*****************************************************************//**
 * \file   MovePath.h
 * \brief MovePathクラス  
 * 
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "IMove.h"
#include "Path.h"
/**
 * \brief オブジェクトをパス移動させるクラス.
 */
class MovePath:public IMove
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	MovePath();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~MovePath() override final;
	/**
	 * オブジェクトの移動処理.
	 * 
	 * \return 移動した後の座標
	 */
	const CVector3& Move() override final;
	/**
	 * \brief 移動ステートを設定.
	 * 
	 */
	void SetMoveState() override final
	{
		IMove::SetMoveStateBasic(enMove_Path);
	}
	/**
	 * \brief 初期化処理.
	 *
	 * \param pos 座標
	 * \param move 移動速度
	 * \param movelimit 移動範囲
	 * \param rot 回転
	 */
	void Init(const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot = CQuaternion::Identity()) override final;
	/**
	 * \brief パスデータを読み込む.
	 * 
	 * \param filePath ファイルパス
	 */
	void ReadPath(const wchar_t* filePath);
	/**
	 * \brief 移動ベクトルを取得.
	 * 
	 * \return 移動ベクトル
	 */
	const CVector3& GetMoveVector() const override final
	{
		return m_moveVector;
	}
private:
	Path m_path;											//パス
	Point* m_point = nullptr;								//今向かってるポイント
	bool m_isStart = true;									//スタート
	bool m_isStop = false;									//ストップ？
	bool m_isAddDegree = true;								//方向転換
	float m_time = 0.0f;									//方向転換の時間
	float m_timer = 0.0f;									//タイマー
};

