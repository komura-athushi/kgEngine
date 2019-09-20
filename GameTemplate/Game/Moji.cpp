#include "stdafx.h"
#include "Moji.h"

const wchar_t* MOJI::MOVELIMIT = L"L";
const wchar_t* MOJI::PATHNUMBER = L"N";
MOJI::MOJI()
{
	//���E�Ɉړ��A�ړ����x
	MOVE.push_back(L"R");
	MOVESTATE.push_back(enMove_Lr);
	//�㉺�Ɉړ��A�ړ����x
	MOVE.push_back(L"U");
	MOVESTATE.push_back(enMove_Up);
	//��]
	MOVE.push_back(L"S");
	MOVESTATE.push_back(enMove_Rot);
	//�p�X�ړ��A�ړ����x
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
	int Move = 0, MoveLimit = 0, Number = 0;
	EnMove State = enMove_No;

	const wchar_t* Moji = moji;
	for (int i = 0; i < MOVE.size(); i++) {
		const wchar_t* pdest = wcsstr(Moji,MOVE[i]);
		if (pdest != NULL) {
			//������̒���MOVE�̏�񂪏�����Ă镶����̍ŏ��̓Y����
			int result = (int)(pdest - Moji + 1);
			Moji += result;
			wchar_t* mo;
			//�ړ����x���擾
			Move = wcstof(Moji, &mo);
			//�X�e�[�g��ݒ�
			State = MOVESTATE[i];
			//�X�e�[�g���p�X��������ԍ��ǂݍ��ށA�ړ��͈͂̎擾�͕K�v����	
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
			//�ړ��͈͂��擾
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

