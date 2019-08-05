#pragma once
#include "Move/IMove.h"
#include "Rotation/IRot.h"
#include "Anim/Anim.h"
#include "ObjectData.h"
#include "physics/PhysicsStaticObject.h"
#include "LineSegment.h"
#include "math/kgBox.h"
#include "graphics/2D/kgFont.h"
class Player;

class ObjModelDataFactory {
private:
	ObjModelDataFactory() {}
	~ObjModelDataFactory() {}
public:
	ObjModelDataFactory& GetInstance()
	{
		static ObjModelDataFactory instance;
		return instance;
	}
public:
	CSkinModelRender* Load(const wchar_t* filepath);
private:
	//unique_ptr  �X�}�[�g�|�C���^�Anew�������������w���|�C���^�����݂��Ȃ���Ύ����I��
	//delete�����
	std::unordered_map<int, std::unique_ptr<CSkinModelRender>> m_modelmap;
};

class Obj : public IGameObject{
public:
	Obj();
	~Obj();
	bool Start() override;
	void Update() override;
	void PostRender() override;
	/// <summary>
	/// �ړ��Ɋւ���X�e�[�^�X��ݒ�
	/// </summary>
	/// <param name="state">�X�e�[�g</param>
	/// <param name="pos">���W</param>
	/// <param name="move">�ړ����x</param>
	/// <param name="movelimit">�ړ��͈�</param>
	/// <param name="rot">��]</param>
	void InitMove(EnMove state ,const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity());
	/// <summary>
	/// ��]�Ɋւ���X�e�[�^�X��ݒ�
	/// </summary>
	/// <param name="state">�X�e�[�g</param>
	/// <param name="speed">��]���x</param>
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
	//�����蔻��p�̃I�u�W�F�N�g�̒������擾
	int GetLenght() const
	{
		return m_lenght;
	}
	//���̂��ǂ���
	bool GetisSphere() const
	{
		return m_issphere;
	}
	//�o�E���f�B���O�{�b�N�X���擾
	CBox* GetBox()
	{
		return &m_box;
	}
	//�t�@�C���p�X��ݒ�Acmo�t�@�C����ǂݍ���
	void SetFilePath(const wchar_t* path);
	//�p�X�ړ��p�̃t�@�C���p�X��ǂ݂���
	void ReadMovePath(const int& number);
private:
	CSkinModelRender m_skin;												//�X�L�����f��
	CVector3 m_position = CVector3::Zero();									//���W								
	CQuaternion m_rotation = CQuaternion::Identity();						//��]
	CMatrix m_localMatrix;													//���[�J���s��
	CMatrix m_worldMatrix;													//���[���h�s��
	EnMove m_movestate = enMove_No;											//�ړ��X�e�[�g
	EnRotate m_rotstate = enRot_No;											//��]�X�e�[�g
	IMove*  m_move = nullptr;												//�ړ�����������N���X
	IRotate* m_rot = nullptr;												//��]����������N���X
	Anim m_anim;															//�A�j���[�V�����Đ�������N���X
	Player* m_player = nullptr;												//�v���C���[
	float m_size = 0.0f;							//�I�u�W�F�N�g�̔��a
	StructObjectData* m_objdata;											//�I�u�W�F�N�g�̃f�[�^
	PhysicsStaticObject m_staticobject;										//�X�^�e�B�b�N�I�u�W�F�N�g
	bool m_issphere = false;												//���̂��ǂ���
	bool m_islinesegment = false;											//�����������邩�ǂ���
	float m_lenght = 0.0f;													
	LineSegment m_linesegment;												//�����N���X
	CVector3 m_linevector;													//�����x�N�g��
	bool m_isclclinesegment = false;										//�������v�Z���邩�ǂ���
	CBox m_box;																//�o�E���f�B���O�{�b�N�X
	CFont m_font;															//����
	
};

