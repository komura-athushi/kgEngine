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
class GameData;
struct ObjModelData {
	CSkinModelRender s_skinmodel;
	int s_maxInstance = 0;
	int s_hashKey = 0;
};

class ObjModelDataFactory : public IGameObject{
private:
	ObjModelDataFactory();
	~ObjModelDataFactory() {}
public:
	static ObjModelDataFactory& GetInstance()
	{
		static ObjModelDataFactory instance;
		return instance;
	}
	bool Start() override;
	void PreUpdate() override;
	//�eSkinModel�̃C���X�^���X�f�[�^������������
	void InitInstancingData();
	void BeginUpdateInstancingData();
	//���f���f�[�^��S�č폜����
	void DeleteAllData();
	//�X�L�����f���擾
	SkinModel* GetSkinModel(int key)
	{
		return m_skinModelmap[key].get();
	}
public:
	ObjModelData* Load(const wchar_t* path);
private:
	//unique_ptr  �X�}�[�g�|�C���^�Anew�������������w���|�C���^�����݂��Ȃ���Ύ����I��
	//delete�����
	std::unordered_map<int, std::unique_ptr<ObjModelData>> m_modelmap;			//�Q�[���Ŏg�p���郂�f���f�[�^�̃}�b�v
	std::unordered_map<int, std::unique_ptr<SkinModel>> m_skinModelmap;							//�򍰏����p�̃��f��
};

static inline ObjModelDataFactory& GetObjModelDataFactory()
{
	return ObjModelDataFactory::GetInstance();
}

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
	//�v���C���[�ɂ��������I�u�W�F�N�g���H
	bool GetisHitPlayer()
	{
		return m_movestate == enMove_MoveHit;
	}
	//���U���g��ʂŕ`�悵�Ȃ��悤�ɂ���
	void SetNoDraw()
	{
		m_draw = false;
	}
	//�t�@�C���p�X��ݒ�Acmo�t�@�C����ǂݍ���
	void SetFilePath(const wchar_t* path);
	//�p�X�ړ��p�̃t�@�C���p�X��ǂ݂���
	void ReadMovePath(const int& number);
	//�������ݔ���p�̒������擾
	float GetRadius() const
	{
		return m_radius;
	}
private:
	//�v���C���[�Ɋ������܂ꂽ�Ƃ��̈ړ����]�̏���
	void ClcMatrix();
	//�����蔻��p�̒��_�f�[�^�𐶐�
	void ClcVertex();
private:
	ObjModelData* m_modeldata;												//�X�L�����f��
	CVector3 m_position = CVector3::Zero();									//���W								
	CQuaternion m_rotation = CQuaternion::Identity();						//��]
	CMatrix m_localMatrix;													//���[�J���s��
	CMatrix m_worldMatrix;													//���[���h�s��
	EnMove m_movestate = enMove_No;											//�ړ��X�e�[�g
	EnRotate m_rotstate = enRot_No;											//��]�X�e�[�g
	IMovePtr  m_move;														//�ړ�����������N���X
	IRotatePtr m_rot;														//��]����������N���X
	Anim m_anim;															//�A�j���[�V�����Đ�������N���X
	Player* m_player = nullptr;												//�v���C���[
	float m_size = 0.0f;													//�I�u�W�F�N�g�̔��a
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
	bool m_draw = true;														//�h���[���邩�ǂ���
	GameData* m_gamedata = nullptr;											//�Q�[���f�[�^
	float m_radius = 0.0f;													//���a(�傫��)
	bool m_isHit = false;													//�������܂ꂽ�H
	
};

