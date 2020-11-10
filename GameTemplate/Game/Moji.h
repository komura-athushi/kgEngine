/*****************************************************************//**
 * \file   Moji.h
 * \brief  Moji�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include <wchar.h>
#include "Object/Move/IMove.h"
#include "Object/Rotation/IRot.h"
struct MoveStatus {
	EnMove s_state;					//MOVE�̎��
	int s_move;						//�ړ����x
	int s_limit;					//�ړ��͈�
	int s_pathnumber = 0;			//�p�X�̐�
};

struct RotStatus {
	EnRotate s_state;				//ROT�̎��
	float s_speed;					//��]�̑���
};
/**
 * \brief �I�u�W�F�N�g�̖��O����ړ���]�Ȃǂ̏����擾����.
 */
class Moji {
private:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Moji();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Moji();
public:
	/**
	 * \brief �C���X�^���X���擾.
	 * 
	 * \return �C���X�^���X
	 */
	static Moji& GetInstance() {
		static Moji instance;
		return instance;
	}
	/**
	 * \brief �I�u�W�F�N�g�̖��O����ړ��̏����擾.
	 * 
	 * \param moji �I�u�W�F�N�g�̖��O
	 * \return �ړ����
	 */
	 const MoveStatus FindMove(const wchar_t* moji);
	/**
	 * \brief �I�u�W�F�N�g�̖��O�����]�̏����擾.
	 * 
	 * \param moji �I�u�W�F�N�g�̖��O
	 * \return ��]���
	 */
	const RotStatus FindRot(const wchar_t* moji);
private:
	//��`��cpp�t�@�C����
    std::vector <const wchar_t*> m_move;
	std::vector <EnMove> m_moveState;
	static const wchar_t* m_moveLimit;
	static const wchar_t* m_pathNumer;
	//��`��cpp�t�@�C����
	std::vector <const wchar_t*> m_rot;
	std::vector <EnRotate> m_rotState;
};

/**
 * Moji�C���X�^���X���擾.
 * 
 * \return Moji�C���X�^���X 
 */
static inline Moji& GetMoji()
{
	return Moji::GetInstance();
}

/**
 * \brief �I�u�W�F�N�g�̖��O����ړ��̏����擾.
 * 
 * \param moji �I�u�W�F�N�g�̖��O
 * \return �ړ����
 */
static inline const MoveStatus FindMove(const wchar_t* moji)
{
	return GetMoji().FindMove(moji);
}

/**
 * \brief �I�u�W�F�N�g�̖��O�����]�̏����擾.
 * 
 * \param moji �I�u�W�F�N�g�̖��O
 * \return �ړ����
 */
static inline const RotStatus FindRot(const wchar_t* moji)
{
	return GetMoji().FindRot(moji);
}