#pragma once
#include "Move/IMove.h"
#include "Rotation/IRot.h"
class Player;
class Obj : public IGameObject{
public:
	Obj();
	~Obj();
	bool Start() override;
	void Update() override;
	//�ړ��X�e�[�^�X��ݒ�
	void InitMove(EnMove state ,const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity());
	//��]�X�e�[�^�X��ݒ�
	void InitRot(EnRotate state, const float& speed);
	//�v���C���[����Ƃ������[�J���s����v�Z
	void ClcLocalMatrix(const CMatrix& worldMatrix);
	//�v���C���[�Ɋ������܂ꂽ�Ƃ��̈ړ����]�̏���
	void ClcMatrix();
	//�傫�����擾
	float GetSize() const
	{
		return m_size;
	}
	//���W���擾
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//�v���C���[�Ɋ������܂ꂽ�I
	void SetHit()
	{
		m_movestate = enMove_MoveHit;
	}
private:
	CSkinModelRender* m_skin = nullptr;
	CVector3 m_position = CVector3::Zero();
	CQuaternion m_rotation = CQuaternion::Identity();
	CMatrix m_localMatrix;
	CMatrix m_worldMatrix;
	const wchar_t* m_filepath = nullptr;
	EnMove m_movestate;
	EnRotate m_rotstate;
	IMove*  m_move = nullptr;
	IRotate* m_rot = nullptr;
	Player* m_player = nullptr;
	float m_size = 10.0f;							//�I�u�W�F�N�g�̔��a
};
