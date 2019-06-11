#include "stdafx.h"
#include "Moji.h"

const wchar_t* MOJI::MOVELIMIT = L"L";
MOJI::MOJI()
{
	MOVE.push_back(L"R");
	MOVESTATE.push_back(enMove_Lr);
	MOVE.push_back(L"U");
	MOVESTATE.push_back(enMove_Up);
	MOVE.push_back(L"S");
	MOVESTATE.push_back(enMoce_Rot);

}

MOJI::~MOJI()
{

}

//移動についての文字列がかかれている直前の文字の配列の添え字を返す
MOVESTATUS MOJI::FindMove(const wchar_t* moji)
{
	const wchar_t* Moji = moji;
	for (int i = 0; i + 1 < MOVE.size(); i++) {
		const wchar_t* pdest = wcsstr(Moji,MOVE[i]);
		if (pdest != NULL) {
			//文字列の中でMOVEの情報が書かれてる文字列の最初の添え字
			int result = (int)(pdest - Moji + 1);
			Moji += result;
			wchar_t* mo;
			//移動速度を取得
			int Move = wcstof(Moji, &mo);
			const wchar_t* dest = wcsstr(Moji, MOVELIMIT);
			int re = (int)(dest - Moji + 1);
			Moji += re;
			//移動範囲を取得
			int MoveLimit = wcstof(Moji, &mo);
			//ステートを設定
			EnMove State = MOVESTATE[i];
			return MOVESTATUS{ State, Move, MoveLimit };
		}
	}
	return MOVESTATUS{ enMove_No,0,0};
}