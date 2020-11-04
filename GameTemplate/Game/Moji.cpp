#include "stdafx.h"
#include "Moji.h"

const wchar_t* MOJI::m_moveLimit = L"L";
const wchar_t* MOJI::m_pathNumer = L"N";
MOJI::MOJI()
{
	//���E�Ɉړ��A�ړ����x
	m_move.push_back(L"R");
	m_moveState.push_back(enMove_Lr);
	//�O��Ɉړ��A�ړ����x
	m_move.push_back(L"F");
	m_moveState.push_back(enMove_Fb);
	//�㉺�Ɉړ��A�ړ����x
	m_move.push_back(L"U");
	m_moveState.push_back(enMove_Up);
	//��]
	m_move.push_back(L"S");
	m_moveState.push_back(enMove_Rot);
	//�p�X�ړ��A�ړ����x
	m_move.push_back(L"P");
	m_moveState.push_back(enMove_Path);

	//���]
	m_rot.push_back(L"M");
	m_rotState.push_back(enRot_Rot);
	//�ړ������ɉ�]
	m_rot.push_back(L"D");
	m_rotState.push_back(enRot_DirectionRot);
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
	for (int i = 0; i < m_move.size(); i++) {
		const wchar_t* pdest = wcsstr(Moji,m_move[i]);
		if (pdest != NULL) {
			//������̒���MOVE�̏�񂪏�����Ă镶����̍ŏ��̓Y����
			int result = (int)(pdest - Moji + 1);
			Moji += result;
			wchar_t* mo;
			//�ړ����x���擾
			Move = wcstof(Moji, &mo);
			//�X�e�[�g��ݒ�
			State = m_moveState[i];
			//�X�e�[�g���p�X��������ԍ��ǂݍ��ށA�ړ��͈͂̎擾�͕K�v����	
			if (State == enMove_Path) {
				const wchar_t* M = moji;
				const wchar_t* dest = wcsstr(M, m_pathNumer);
				int r = (int)(dest - M + 1);
				M += r;
				Number = wcstof(M, &mo);
				return MOVESTATUS{ State, Move, MoveLimit,Number };
			}
			const wchar_t* dest = wcsstr(Moji, m_moveLimit);
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
	for (int i = 0; i < m_rot.size(); i++) {
		const wchar_t* pdest = wcsstr(Moji, m_rot[i]);
		if (pdest != NULL) {
			//������̒���ROT�̏�񂪏�����Ă镶����̍ŏ��̓Y����
			int result = (int)(pdest - Moji + 1);
			Moji += result;
			wchar_t* mo;
			//��]���x���擾
			float Speed = wcstof(Moji, &mo);
			//�X�e�[�g��ݒ�
			EnRotate State = m_rotState[i];
			return ROTSTATUS{ State, Speed};
		}
	}
	return ROTSTATUS{ enRot_No,0.0f };
}

