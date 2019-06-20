#pragma once
#include "Move/IMove.h"
#include "Rotation/IRot.h"
#include "ObjectData.h"
#include "physics/PhysicsStaticObject.h"
class Player;
struct  Vertex
{
	CVector3 s_list[22];
};

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
	//�����蔻��p�̒��_�f�[�^�𐶐�
	void ClcVertex();
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
	//�v���C���[�ɂ������Ă��邩�ǂ������擾
	bool GetisStickPlayer()
	{
		return m_movestate == enMove_MoveHit;
	}
	//�I�u�W�F�N�g�f�[�^��ݒ�
	void SetObjData(StructObjectData* objdata)
	{
		m_objdata = objdata;
	}
	//�I�u�W�F�N�g�f�[�^���擾
	StructObjectData& GetObjData()
	{
		return *m_objdata;
	}
	//�����蔻��p�̒��_�z��̑傫�����擾
	int GetVertexSize() const
	{
		int i = sizeof(m_bufferList) / sizeof(m_bufferList[0]);
		return i;
	}
	//�Y���̔ԍ��̓����蔻��p�̒��_�f�[�^���擾
	CVector3 GetBuffer(const int& number) 
	{
		return m_bufferList[number];
	}
	//���̂��ǂ���
	bool GetisSphere() const
	{
		return m_issphere;
	}
	//�t�@�C���p�X��ݒ�Acmo�t�@�C����ǂݍ���
	void SetFilePath(const wchar_t* path);
private:
	CSkinModelRender m_skin;
	CVector3 m_position = CVector3::Zero();
	CQuaternion m_rotation = CQuaternion::Identity();
	CMatrix m_localMatrix;
	CMatrix m_worldMatrix;
	EnMove m_movestate = enMove_No;
	EnRotate m_rotstate = enRot_No;
	IMove*  m_move = nullptr;
	IRotate* m_rot = nullptr;
	Player* m_player = nullptr;
	float m_size = 0.0f;							//�I�u�W�F�N�g�̔��a
	const wchar_t* m_filepath = nullptr;
	StructObjectData* m_objdata;
	PhysicsStaticObject m_staticobject;
	CVector3 m_bufferList[22];					//�����蔻��p�̒��_
	//CVector3 m_vertexList[14];					//�����蔻��p�̃x�N�g��(���W����̃x�N�g��)
	bool m_issphere = false;
};

class VertexFactory
{
private:
	VertexFactory() {};
	~VertexFactory() {};
public:
	static VertexFactory& GetInstance()
	{
		static VertexFactory instance;
		return instance;
	}
	friend Obj;
private:
	std::unordered_map<const wchar_t*, Vertex> m_vertexList;
};