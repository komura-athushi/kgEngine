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

//�ړ��ɂ��Ă̕����񂪂�����Ă��钼�O�̕����̔z��̓Y������Ԃ�
MOVESTATUS MOJI::FindMove(const wchar_t* moji)
{
	const wchar_t* Moji = moji;
	for (int i = 0; i + 1 < MOVE.size(); i++) {
		const wchar_t* pdest = wcsstr(Moji,MOVE[i]);
		if (pdest != NULL) {
			//������̒���MOVE�̏�񂪏�����Ă镶����̍ŏ��̓Y����
			int result = (int)(pdest - Moji + 1);
			Moji += result;
			wchar_t* mo;
			//�ړ����x���擾
			int Move = wcstof(Moji, &mo);
			const wchar_t* dest = wcsstr(Moji, MOVELIMIT);
			int re = (int)(dest - Moji + 1);
			Moji += re;
			//�ړ��͈͂��擾
			int MoveLimit = wcstof(Moji, &mo);
			//�X�e�[�g��ݒ�
			EnMove State = MOVESTATE[i];
			return MOVESTATUS{ State, Move, MoveLimit };
		}
	}
	return MOVESTATUS{ enMove_No,0,0};
}