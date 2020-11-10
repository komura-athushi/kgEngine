/*****************************************************************//**
 * \file   Moji.h
 * \brief  Mojiクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include <wchar.h>
#include "Object/Move/IMove.h"
#include "Object/Rotation/IRot.h"
struct MoveStatus {
	EnMove s_state;					//MOVEの種類
	int s_move;						//移動速度
	int s_limit;					//移動範囲
	int s_pathnumber = 0;			//パスの数
};

struct RotStatus {
	EnRotate s_state;				//ROTの種類
	float s_speed;					//回転の速さ
};
/**
 * \brief オブジェクトの名前から移動回転などの情報を取得する.
 */
class Moji {
private:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	Moji();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~Moji();
public:
	/**
	 * \brief インスタンスを取得.
	 * 
	 * \return インスタンス
	 */
	static Moji& GetInstance() {
		static Moji instance;
		return instance;
	}
	/**
	 * \brief オブジェクトの名前から移動の情報を取得.
	 * 
	 * \param moji オブジェクトの名前
	 * \return 移動情報
	 */
	 const MoveStatus FindMove(const wchar_t* moji);
	/**
	 * \brief オブジェクトの名前から回転の情報を取得.
	 * 
	 * \param moji オブジェクトの名前
	 * \return 回転情報
	 */
	const RotStatus FindRot(const wchar_t* moji);
private:
	//定義はcppファイルに
    std::vector <const wchar_t*> m_move;
	std::vector <EnMove> m_moveState;
	static const wchar_t* m_moveLimit;
	static const wchar_t* m_pathNumer;
	//定義はcppファイルに
	std::vector <const wchar_t*> m_rot;
	std::vector <EnRotate> m_rotState;
};

/**
 * Mojiインスタンスを取得.
 * 
 * \return Mojiインスタンス 
 */
static inline Moji& GetMoji()
{
	return Moji::GetInstance();
}

/**
 * \brief オブジェクトの名前から移動の情報を取得.
 * 
 * \param moji オブジェクトの名前
 * \return 移動情報
 */
static inline const MoveStatus FindMove(const wchar_t* moji)
{
	return GetMoji().FindMove(moji);
}

/**
 * \brief オブジェクトの名前から回転の情報を取得.
 * 
 * \param moji オブジェクトの名前
 * \return 移動情報
 */
static inline const RotStatus FindRot(const wchar_t* moji)
{
	return GetMoji().FindRot(moji);
}