#pragma once
#include <wchar.h>
#include "IMove.h"
struct MOVESTATUS {
	EnMove s_state;					//MOVE�̎��
	int s_move;						//�ړ����x
	int s_limit;					//�ړ��͈�
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
	//�ړ��ɂ��Ă̕����񂪂�����Ă��钼�O�̕����̔z��̓Y������Ԃ�
	MOVESTATUS FindMove(const wchar_t* moji);
public:
	//��`��cpp�t�@�C����
    std::vector <const wchar_t*> MOVE;
	std::vector <EnMove> MOVESTATE;
	static const wchar_t* MOVELIMIT;				//�����͈�
};

static inline MOJI& Moji()
{
	return MOJI::GetInstance();
}

//�ړ��̎�ނƈړ����x��Ԃ�
static MOVESTATUS FindMove(const wchar_t* moji)
{
	return Moji().FindMove(moji);
}

static int FindMoveRange(const wchar_t* moji)
{

}



