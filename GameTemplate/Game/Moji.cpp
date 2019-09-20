#include "stdafx.h"
#include "Moji.h"

const wchar_t* MOJI::MOVELIMIT = L"L";
const wchar_t* MOJI::PATHNUMBER = L"N";
MOJI::MOJI()
{
	//左右に移動、移動速度
	MOVE.push_back(L"R");
	MOVESTATE.push_back(enMove_Lr);
	//上下に移動、移動速度
	MOVE.push_back(L"U");
	MOVESTATE.push_back(enMove_Up);
	//回転
	MOVE.push_back(L"S");
	MOVESTATE.push_back(enMove_Rot);
	//パス移動、移動速度
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
	int Move = 0, MoveLimit = 0, Number = 0;
	EnMove State = enMove_No;

	const wchar_t* Moji = moji;
	for (int i = 0; i < MOVE.size(); i++) {
		const wchar_t* pdest = wcsstr(Moji,MOVE[i]);
		if (pdest != NULL) {
			//文字列の中でMOVEの情報が書かれてる文字列の最初の添え字
			int result = (int)(pdest - Moji + 1);
			Moji += result;
			wchar_t* mo;
			//移動速度を取得
			Move = wcstof(Moji, &mo);
			//ステートを設定
			State = MOVESTATE[i];
			//ステートがパスだったら番号読み込む、移動範囲の取得は必要無し	
			if (State == enMove_Path) {
				const wchar_t* M = moji;
				const wchar_t* dest = wcsstr(M, PATHNUMBER);
				int r = (int)(dest - M + 1);
				M += r;
				Number = wcstof(M, &mo);
				return MOVESTATUS{ State, Move, MoveLimit,Number };
			}
			const wchar_t* dest = wcsstr(Moji, MOVELIMIT);
			int re = (int)(dest - Moji + 1);
			Moji += re;
			//移動範囲を取得
			MoveLimit = wcstof(Moji, &mo);
			return MOVESTATUS{ State, Move, MoveLimit };
		}
		else {
			int p = 0;
		}
	}
	return MOVESTATUS{ enMove_No,0,0};
}

ROTSTATUS MOJI::FindRot(const wchar_t* moji)
{
	const wchar_t* Moji = moji;
	for (int i = 0; i < ROT.size(); i++) {
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

