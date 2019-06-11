#pragma once
#include <wchar.h>
#include "IMove.h"
struct MOVESTATUS {
	EnMove s_state;					//MOVEの種類
	int s_move;						//移動速度
	int s_limit;					//移動範囲
};

class MOJI {
private:
	MOJI();
	~MOJI();
public:
	static MOJI& GetInstance() {
		static MOJI instance;
		return instance;
	}
	//移動についての文字列がかかれている直前の文字の配列の添え字を返す
	MOVESTATUS FindMove(const wchar_t* moji);
public:
	//定義はcppファイルに
    std::vector <const wchar_t*> MOVE;
	std::vector <EnMove> MOVESTATE;
	static const wchar_t* MOVELIMIT;				//動く範囲
};

static inline MOJI& Moji()
{
	return MOJI::GetInstance();
}

//移動の種類と移動速度を返す
static MOVESTATUS FindMove(const wchar_t* moji)
{
	return Moji().FindMove(moji);
}

static int FindMoveRange(const wchar_t* moji)
{

}



