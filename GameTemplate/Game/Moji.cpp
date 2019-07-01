#include "stdafx.h"
#include "Moji.h"

const wchar_t* MOJI::MOVELIMIT = L"L";
const wchar_t* MOJI::PATHNUMBER = L"N";
MOJI::MOJI()
{
	//���E�Ɉړ�
	MOVE.push_back(L"R");
	MOVESTATE.push_back(enMove_Lr);
	//�㉺�Ɉړ�
	MOVE.push_back(L"U");
	MOVESTATE.push_back(enMove_Up);
	//��]
	MOVE.push_back(L"S");
	MOVESTATE.push_back(enMove_Rot);
	//�p�X�ړ�
	MOVE.push_back(L"P");
	MOVESTATE.push_back(enMove_Path);

	//���]
	ROT.push_back(L"M");
	ROTSTATE.push_back(enRot_Rot);
	//�ړ������ɉ�]
	ROT.push_back(L"D");
	ROTSTATE.push_back(enRot_DirectionRot);
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
			//������̒���ROT�̏�񂪏�����Ă镶����̍ŏ��̓Y����
			int result = (int)(pdest - Moji + 1);
			Moji += result;
			wchar_t* mo;
			//��]���x���擾
			float Speed = wcstof(Moji, &mo);
			//�X�e�[�g��ݒ�
			EnRotate State = ROTSTATE[i];
			return ROTSTATUS{ State, Speed};
		}
	}
	return ROTSTATUS{ enRot_No,0.0f };
}