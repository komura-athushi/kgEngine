#include "stdafx.h"
#include "Moji.h"

const wchar_t* Moji::m_moveLimit = L"L";
const wchar_t* Moji::m_pathNumer = L"N";
Moji::Moji()
{
	//左右に移動、移動速度
	m_move.push_back(L"R");
	m_moveState.push_back(enMove_Lr);
	//前後に移動、移動速度
	m_move.push_back(L"F");
	m_moveState.push_back(enMove_Fb);
	//上下に移動、移動速度
	m_move.push_back(L"U");
	m_moveState.push_back(enMove_Up);
	//回転
	m_move.push_back(L"S");
	m_moveState.push_back(enMove_Rot);
	//パス移動、移動速度
	m_move.push_back(L"P");
	m_moveState.push_back(enMove_Path);

	//自転
	m_rot.push_back(L"M");
	m_rotState.push_back(enRot_Rot);
	//移動方向に回転
	m_rot.push_back(L"D");
	m_rotState.push_back(enRot_DirectionRot);
}

Moji::~Moji()
{

}

//移動についての文字列がかかれている直前の文字の配列の添え字を返す
const MoveStatus Moji::FindMove(const wchar_t* moji)
{
	int move = 0, moveLimit = 0, number = 0;
	EnMove state = enMove_No;

	const wchar_t* name = moji;
	for (int i = 0; i < m_move.size(); i++) {
		const wchar_t* pdest = wcsstr(name,m_move[i]);
		if (pdest != NULL) {
			//文字列の中でMOVEの情報が書かれてる文字列の最初の添え字
			int result = (int)(pdest - name + 1);
			name += result;
			wchar_t* mo;
			//移動速度を取得
			move = wcstof(name, &mo);
			//ステートを設定
			state = m_moveState[i];
			//ステートがパスだったら番号読み込む、移動範囲の取得は必要無し	
			if (state == enMove_Path) {
				const wchar_t* M = moji;
				const wchar_t* dest = wcsstr(M, m_pathNumer);
				int r = (int)(dest - M + 1);
				M += r;
				number = wcstof(M, &mo);
				return MoveStatus{ state, move, moveLimit,number };
			}
			const wchar_t* dest = wcsstr(name, m_moveLimit);
			int re = (int)(dest - name + 1);
			name += re;
			//移動範囲を取得
			moveLimit = wcstof(name, &mo);
			return MoveStatus{ state, move, moveLimit };
		}
		else {
			int p = 0;
		}
	}
	return MoveStatus{ enMove_No,0,0};
}
 
const RotStatus Moji::FindRot(const wchar_t* moji)
{
	const wchar_t* name = moji;
	for (int i = 0; i < m_rot.size(); i++) {
		const wchar_t* pdest = wcsstr(name, m_rot[i]);
		if (pdest != NULL) {
			//文字列の中でROTの情報が書かれてる文字列の最初の添え字
			int result = (int)(pdest - name + 1);
			name += result;
			wchar_t* mo;
			//回転速度を取得
			float speed = wcstof(name, &mo);
			//ステートを設定
			EnRotate state = m_rotState[i];
			return RotStatus{ state, speed};
		}
	}
	return RotStatus{ enRot_No,0.0f };
}

