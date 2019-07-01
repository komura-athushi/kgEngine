#include "stdafx.h"
#include "Moji.h"

const wchar_t* MOJI::MOVELIMIT = L"L";
const wchar_t* MOJI::PATHNUMBER = L"N";
MOJI::MOJI()
{
	//左右に移動
	MOVE.push_back(L"R");
	MOVESTATE.push_back(enMove_Lr);
	//上下に移動
	MOVE.push_back(L"U");
	MOVESTATE.push_back(enMove_Up);
	//回転
	MOVE.push_back(L"S");
	MOVESTATE.push_back(enMove_Rot);
	//パス移動
	MOVE.push_back(L"P");
	MOVESTATE.push_back(enMove_Path);

	//自転
	ROT.push_back(L"M");
	ROTSTATE.push_back(enRot_Rot);
	//移動方向に回転
	ROT.push_back(L"D");
	ROTSTATE.push_back(enRot_DirectionRot);
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
			if (State == enMove_Path) {
				const wchar_t* M = moji;
				const wchar_t* dest = wcsstr(M, PATHNUMBER);
				int r = (int)(dest - M + 1);
				M += r;
				int Number = wcstof(M, &mo);
				return MOVESTATUS{ State, Move, MoveLimit,Number };
			}
			return MOVESTATUS{ State, Move, MoveLimit };
		}
	}
	return MOVESTATUS{ enMove_No,0,0};
}

ROTSTATUS MOJI::FindRot(const wchar_t* moji)
{
	const wchar_t* Moji = moji;
	for (int i = 0; i + 1 < ROT.size(); i++) {
		const wchar_t* pdest = wcsstr(Moji, ROT[i]);
		if (pdest != NULL) {
			//文字列の中でROTの情報が書かれてる文字列の最初の添え字
			int result = (int)(pdest - Moji + 1);
			Moji += result;
			wchar_t* mo;
			//回転速度を取得
			float Speed = wcstof(Moji, &mo);
			//ステートを設定
			EnRotate State = ROTSTATE[i];
			return ROTSTATUS{ State, Speed};
		}
	}
	return ROTSTATUS{ enRot_No,0.0f };
}