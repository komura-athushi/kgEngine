#pragma once
#include <wchar.h>
#include "Object/Move/IMove.h"
#include "Object/Rotation/IRot.h"
struct MOVESTATUS {
	EnMove s_state;					//MOVEの種類
	int s_move;						//移動速度
	int s_limit;					//移動範囲
	int s_pathnumber = 0;
};

struct ROTSTATUS {
	EnRotate s_state;				//ROTの種類
	float s_speed;					//回転の速さ
};
//外部ファイルからオブジェクトの情報を取得する
class MOJI {
private:
	MOJI();
	~MOJI();
public:
	static MOJI& GetInstance() {
		static MOJI instance;
		return instance;
	}
	//移動について情報を文字列から書き出す
	MOVESTATUS FindMove(const wchar_t* moji);
	//回転について情報を文字列から書き出す
	ROTSTATUS FindRot(const wchar_t* moji);
public:
	//定義はcppファイルに
    std::vector <const wchar_t*> m_move;
	std::vector <EnMove> m_moveState;
	static const wchar_t* m_moveLimit;
	static const wchar_t* m_pathNumer;
	//定義はcppファイルに
	std::vector <const wchar_t*> m_rot;
	std::vector <EnRotate> m_rotState;
};

static inline MOJI& Moji()
{
	return MOJI::GetInstance();
}

//移動の種類と移動速度と移動範囲を返す
static MOVESTATUS FindMove(const wchar_t* moji)
{
	return Moji().FindMove(moji);
}

//回転の種類と回転速度を返す
static ROTSTATUS FindRot(const wchar_t* moji)
{
	return Moji().FindRot(moji);
}