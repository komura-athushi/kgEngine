#include "stdafx.h"
#include "Moji.h"

const wchar_t* Moji::m_moveLimit = L"L";
const wchar_t* Moji::m_pathNumer = L"N";
Moji::Moji()
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

Moji::~Moji()
{

}

//�ړ��ɂ��Ă̕����񂪂�����Ă��钼�O�̕����̔z��̓Y������Ԃ�
const MoveStatus Moji::FindMove(const wchar_t* moji)
{
	int move = 0, moveLimit = 0, number = 0;
	EnMove state = enMove_No;

	const wchar_t* name = moji;
	for (int i = 0; i < m_move.size(); i++) {
		const wchar_t* pdest = wcsstr(name,m_move[i]);
		if (pdest != NULL) {
			//������̒���MOVE�̏�񂪏�����Ă镶����̍ŏ��̓Y����
			int result = (int)(pdest - name + 1);
			name += result;
			wchar_t* mo;
			//�ړ����x���擾
			move = wcstof(name, &mo);
			//�X�e�[�g��ݒ�
			state = m_moveState[i];
			//�X�e�[�g���p�X��������ԍ��ǂݍ��ށA�ړ��͈͂̎擾�͕K�v����	
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
			//�ړ��͈͂��擾
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
			//������̒���ROT�̏�񂪏�����Ă镶����̍ŏ��̓Y����
			int result = (int)(pdest - name + 1);
			name += result;
			wchar_t* mo;
			//��]���x���擾
			float speed = wcstof(name, &mo);
			//�X�e�[�g��ݒ�
			EnRotate state = m_rotState[i];
			return RotStatus{ state, speed};
		}
	}
	return RotStatus{ enRot_No,0.0f };
}

