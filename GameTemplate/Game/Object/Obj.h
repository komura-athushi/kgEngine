/*****************************************************************//**
 * \file   Obj.h
 * \brief  �I�u�W�F�N�g�֘A
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
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
	CSkinModelRender s_skinModel;	//���f��
	int s_maxInstance = 0;	//���f���̍ő吔
	int s_hashKey = 0;	//���O���琶�������n�b�V���l
};

/**
 * \brief �I�u�W�F�N�g�̃f�[�^���Ǘ�.
 */
class ObjModelDataFactory : public IGameObject{
private:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	ObjModelDataFactory();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~ObjModelDataFactory() {}
public:
	/**
	 * \brief �C���X�^���X���擾.
	 * 
	 * \return �C���X�^���X
	 */
	static ObjModelDataFactory& GetInstance()
	{
		static ObjModelDataFactory instance;
		return instance;
	}
	/**
	 * \brief ����������.
	 * 
	 * \return true�ŏ���������
	 */
	bool Start() override final;
	/**
	 * \brief �X�V�����̑O����.
	 * 
	 */
	void PreUpdate() override final;
	/**
	 * \brief �e�X�L�����f���̃C���X�^���V���O�p�̃��f����������.
	 * 
	 */
	void InitInstancingData();
	/**
	 * \brief �C���X�^�V���O�`��n�߂�O�̑O����.
	 * 
	 */
	void BeginUpdateInstancingData();
	/**
	 * \brief �I�u�W�F�N�g�p�̃f�[�^���폜����	.
	 * 
	 */
	void DeleteAllData();
	/**
	 * \brief �Q�[�����A�������񂾃I�u�W�F�N�g���E���ɕ\�����邽�߂̃��f�����擾.
	 * 
	 * \param key �I�u�W�F�N�g�̃n�b�V���l
	 * \return �X�L�����f��
	 */
	SkinModel* GetSkinModel(int key)
	{
		return m_skinModelMap[key].get();
	}
public:
	/**
	 * \brief �I�u�W�F�N�g�p�̃��f�������[�h.
	 * 
	 * \param path �I�u�W�F�N�g�̖��O
	 * \return ���f���f�[�^
	 */
	ObjModelData* Load(const wchar_t* path);
private:
	//unique_ptr  �X�}�[�g�|�C���^�Anew�������������w���|�C���^�����݂��Ȃ���Ύ����I��
	//delete�����
	std::unordered_map<int, std::unique_ptr<ObjModelData>> m_modelMap;			//�Q�[���Ŏg�p���郂�f���f�[�^�̃}�b�v
	std::unordered_map<int, std::unique_ptr<SkinModel>> m_skinModelMap;							//�򍰏����p�̃��f��
};

/**
 * \brief �C���X�^���X���擾.
 * 
 * \return �C���X�^���X
 */
static inline ObjModelDataFactory& GetObjModelDataFactory()
{
	return ObjModelDataFactory::GetInstance();
}

/**
 * \brief �I�u�W�F�N�g.
 */
class Obj : public IGameObject{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Obj();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Obj() override final;
	/**
	 * \brief ����������.
	 * 
	 * \return true�ŏ���������
	 */
	bool Start() override final;
	/**
	 * \brief �X�V����.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief �ړ��Ɋւ���X�e�[�^�X��ݒ�.
	 * 
	 * \param state �ړ��X�e�[�g
	 * \param pos ���W
	 * \param move �ړ����x
	 * \param movelimit �ړ��͈�
	 * \param rot ��]
	 */
	void InitMove(const EnMove state ,const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot = CQuaternion::Identity());
	/**
	 * \brief ��]�Ɋւ���X�e�[�^�X��ݒ�.
	 * 
	 * \param state ��]�X�e�[�g
	 * \param speed ��]���x
	 */
	void InitRot(const EnRotate state, const float speed);
	/**
	 * \brief �v���C���[�𒆐S�Ƃ������[�J���s����v�Z.
	 * 
	 * \param worldMatrix �v���C���[�̃��[���h�s��
	 */
	void ClcLocalMatrix(const CMatrix& worldMatrix);
	/**
	 * \brief �傫�����擾.
	 * 
	 * \return �傫��
	 */
	const float GetSize() const
	{
		return m_size;
	}
	/**
	 * \brief ���W���擾.
	 * 
	 * \return ���W
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/**
	 * \brief �v���C���[�Ɋ������܂ꂽ�Ƃ����t���O���Z�b�g.
	 * 
	 */
	void SetHit()
	{
		m_moveState = enMove_MoveHit;
	}
	/**
	 * \brief �I�u�W�F�N�g�f�[�^��ݒ�.
	 * 
	 * \param objdata �I�u�W�F�N�g�f�[�^
	 */
	void SetObjData(StructObjectData* objdata)
	{
		m_objData = objdata;
	}
	/**
	 * \brief �I�u�W�F�N�g�f�[�^���擾.
	 * 
	 * \return �I�u�W�F�N�g�f�[�^
	 */
	const StructObjectData& GetObjData() const
	{
		return *m_objData;
	}
	/**
	 * \brief �v���C���[�Ɋ������܂�Ă��邩�ǂ�������.
	 * 
	 * \return �v���C���[�Ɋ������܂�Ă�����true
	 */
	bool GetisHitPlayer()
	{
		return m_moveState == enMove_MoveHit;
	}
	/**
	 * \brief ���f����`�悵�Ȃ��悤�ɐݒ肷��.
	 * 
	 */
	void SetNoDraw()
	{
		m_draw = false;
	}
	/**
	 * \brief ���f����ǂݍ���.
	 * 
	 * \param path ���f���̖��O
	 */
	void SetFilePath(const wchar_t* path);
	/**
	 * \brief �p�X�f�[�^��ǂݍ���.
	 * 
	 * \param number �p�X�̖��O
	 */
	void ReadMovePath(const int& number);
private:
	/**
	 * \brief ���[���h�s����v�Z.
	 * 
	 */
	void ClcMatrix();
	/**
	 * \brief �o�E���f�B���O�{�b�N�X���v�Z.
	 * 
	 */
	void ClcVertex();
private:
	ObjModelData* m_modelData;												//�X�L�����f��
	CVector3 m_position = CVector3::Zero();									//���W								
	CQuaternion m_rotation = CQuaternion::Identity();						//��]
	CMatrix m_localMatrix;													//���[�J���s��
	CMatrix m_worldMatrix;													//���[���h�s��
	EnMove m_moveState = enMove_No;											//�ړ��X�e�[�g
	EnRotate m_rotState = enRot_No;											//��]�X�e�[�g
	IMovePtr  m_move;														//�ړ�����������N���X
	IRotatePtr m_rot;														//��]����������N���X
	Anim m_anim;															//�A�j���[�V�����Đ�������N���X
	Player* m_player = nullptr;												//�v���C���[
	float m_size = 0.0f;													//�I�u�W�F�N�g�̔��a
	StructObjectData* m_objData;											//�I�u�W�F�N�g�̃f�[�^
	PhysicsStaticObject m_staticObject;										//�X�^�e�B�b�N�I�u�W�F�N�g
	bool m_isSphere = false;												//���̂��ǂ���
	bool m_isLineSegment = false;											//�����������邩�ǂ���
	float m_lenght = 0.0f;													
	LineSegment m_lineSegment;												//�����N���X
	CVector3 m_lineVector;													//�����x�N�g��
	bool m_isClcLineSegment = false;										//�������v�Z���邩�ǂ���
	CBox m_box;																//�o�E���f�B���O�{�b�N�X
	CFont m_font;															//����
	bool m_draw = true;														//�h���[���邩�ǂ���
	GameData* m_gameData = nullptr;											//�Q�[���f�[�^
	float m_radius = 0.0f;													//���a(�傫��)
	bool m_isHit = false;													//�������܂ꂽ�H
	
};

