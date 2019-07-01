#pragma once
#include "IMove.h"
#include "Path.h"
class MovePath:public IMove
{
public:
	MovePath();
	~MovePath() override;
	//�ړ���̍��W��Ԃ�
	CVector3 Move() override;
	//�X�e�[�g��ݒ�A�h���N���X�Őݒ肷��p
	void SetMoveState() override
	{
		IMove::SetMoveStateBasic(enMove_Path);
	}
	/*!
	* @brief	�������B
	*@param[in] pos				���W
	*@param[in]	move			�ړ����x
	*@param[in]	movelimit		�ړ��͈�
	*@param[in]	rot				��]
	*/
	void Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity()) override;
	//�p�X�̃f�[�^�̃t�@�C����ǂݍ���
	void ReadPath(const wchar_t* filePath);
	//�ړ��x�N�g�����擾
	virtual CVector3 GetMoveVector() const override
	{
		return m_movevector;
	}
private:
	CVector3 m_position = CVector3::Zero();					//���W
	float m_movespeed = 0.0f;								//�ړ����x
	CVector3 m_movevector = CVector3::Zero();				//�ړ��x�N�g��
	bool m_isaccessup = true;								//�㑤�̍��W�ɋ߂Â����ǂ���
	bool m_isculcmovevector = false;						//�ړ��x�N�g�����v�Z�������ǂ���
	bool m_isstart = true;
	Path m_path;
	Point* m_point = nullptr;
};

