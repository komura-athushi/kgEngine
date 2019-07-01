#pragma once
#include <wchar.h>
#include "Object/Move/IMove.h"
#include "Object/Rotation/IRot.h"
struct MOVESTATUS {
	EnMove s_state;					//MOVE�̎��
	int s_move;						//�ړ����x
	int s_limit;					//�ړ��͈�
	int s_pathnumber = 0;
};

struct ROTSTATUS {
	EnRotate s_state;				//ROT�̎��
	float s_speed;					//��]�̑���
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
	//�ړ��ɂ��ď��𕶎��񂩂珑���o��
	MOVESTATUS FindMove(const wchar_t* moji);
	//��]�ɂ��ď��𕶎��񂩂珑���o��
	ROTSTATUS FindRot(const wchar_t* moji);
public:
	//��`��cpp�t�@�C����
    std::vector <const wchar_t*> MOVE;
	std::vector <EnMove> MOVESTATE;
	static const wchar_t* MOVELIMIT;
	static const wchar_t* PATHNUMBER;
	//��`��cpp�t�@�C����
	std::vector <const wchar_t*> ROT;
	std::vector <EnRotate> ROTSTATE;
};

static inline MOJI& Moji()
{
	return MOJI::GetInstance();
}

//�ړ��̎�ނƈړ����x�ƈړ��͈͂�Ԃ�
static MOVESTATUS FindMove(const wchar_t* moji)
{
	return Moji().FindMove(moji);
}

//��]�̎�ނƉ�]���x��Ԃ�
static ROTSTATUS FindRot(const wchar_t* moji)
{
	return Moji().FindRot(moji);
}



